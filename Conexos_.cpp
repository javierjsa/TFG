/* 
 * File:   Conexos.cpp
 * Author: jsa
 * 
 * Created on 28 de enero de 2016, 12:27
 */


#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <list>
#include <string>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>
#include <Constants.h>
#include <Muestra.cpp>
using namespace std;
using namespace cv;


class Conexos{

private:

int cn;    
int* label;
int* count;
cv::Point origen;
double area;
double top,bot;


std::vector<list<Muestra> > muestras;
int dim;

std::list<cv::Mat> lista;
    
public:
    
Conexos() {    

}    
    
    
Conexos(cv::Point origen,cv::Size s, std::vector<list<Muestra> > muestrasa, int count[] ) {
    
     this->origen=origen;//para imagen completa
     this->area=s.height*s.height;
     this->top=area* LIM_SUP;
     this->bot=area* LIM_INF;
     this->lista=lista;
     this->cn=0;
     this->muestras=muestrasa;
     this->count=count;
     cout<<"Tamaños de vector recibido en conexos"<<muestrasa.size()<<"\n";
     cout<<"Tamaño  de vector asignado en conexos"<<this->muestras.size()<<"\n";
    
}

void setMuestras(std::vector<list<Muestra> > muestrasa){
     this->muestras=muestrasa;
}


int procesar(cv::Mat frame, cv::Mat imagen,cv::Mat labels) {
    
    cv::Mat stats;    
    cv::Mat centroids;    
 
    int la = connectedComponentsWithStats(imagen, labels,stats,centroids, 8, CV_16U);
    cv::Scalar saux;
    
    double tam,height,width,bbox,roi;
    
    //Se recorren todas las componentes conexas excepto el fondo
    for(int i=1;i<la;i++){      
       
        tam= stats.at<int>(i,cv::CC_STAT_AREA);        
        height=stats.at<int>(i,cv::CC_STAT_HEIGHT);        
        width=stats.at<int>(i,cv::CC_STAT_WIDTH);
        bbox=height*width;        
        roi=imagen.size().height*imagen.size().width;
        
        
        /*el componente se considera relevante si su area esta dentro de
         un cierto tamaño o si, siendo mayor, no ocupa mas del 80% de su bounding box o
         su forma es rectangular*/        
        if(((tam>bot)&&(tam<top))||((tam>top)&&((bbox<0.45*roi)))){
             

            //extrae la componente de labels
            cv::Point aux=cv::Point(stats.at<int>(i,cv::CC_STAT_LEFT),stats.at<int>(i,cv::CC_STAT_TOP));    
            cv::Point aux_op=cv::Point((aux.x+stats.at<int>(i,cv::CC_STAT_WIDTH)),(aux.y+stats.at<int>(i,cv::CC_STAT_HEIGHT)));
            Mat conn=labels(Rect(aux.x,aux.y,width,height)); 
            
            //limpia componentes solapadas
            Mat temp=extraer(conn,i);   
            //compara la componente con cada una de las muestras
            int cat= comparar(temp);                        
            //segun categoría, pinta rectángulo
            if (cat==0)//flecha
                saux=Scalar ROJO;
            else if (cat==1)//linea
                saux=Scalar VERDE;
            else if (cat==2)    
                saux=Scalar AMARILLO;
            //Dibuja bounding box, vertice sup.izdo, inf dcho            
            if(cat!=-1)
                cv::rectangle(frame, aux, aux_op, saux, 1, 4, 0);        
            
        }   
            
      }  
     
    return la;
}

cv::Mat extraer (cv::Mat label, int n){
     cout<<"extraer\n";
     cv::Mat limpia(label.rows, label.cols, CV_8UC1, cv::Scalar(0));
     cv::MatIterator_<uint16_t> it, end;
     cv::MatIterator_<uchar> limp;
     limp=limpia.begin<uchar>();
    
     for( it = label.begin<uint16_t>(), end = label.end<uint16_t>(); it != end; ++it,++limp){
         if ((uint16_t)(*it)==n)
            (*limp)=255;
      }
          
     return limpia; 
}

/*
 * Recibe la componente conexa y escala al tamaño de la muestra
 * y devuelve la categoría a la que pertenece la componente
 */
int comparar(cv::Mat temp){
    cout<<"comparar\n";
    int last=0; //categoria asignada
    double votacion[3]={0};
    Size s=temp.size();
    
    float ar= (float)s.width/(float)s.height;    
    std::list<Muestra>* miLista;    
       
  if(ar<LA){
            
        miLista=&(this->muestras[0]);
         cout<<"seleccionada lista"<<"0";
        }
        else if (ar>=LA && ar<LB){
              miLista=&(this->muestras[1]);
               cout<<"seleccionada lista"<<"1";
        }
        else if (ar>=LB && ar<LC){
             miLista=&(this->muestras[2]);
              cout<<"seleccionada lista"<<"2";
        }     
        else if (ar>=LC && ar<LD){
             miLista=&(this->muestras[3]);
              cout<<"seleccionada lista"<<"3";
        }
        else if (ar>=LD && ar<LE){
              miLista=&(this->muestras[4]);
               cout<<"seleccionada lista"<<"4";
        }     
        else if (ar>=LE && ar<LF){
              miLista=&(this->muestras[5]);
               cout<<"seleccionada lista"<<"5";
        }     
        else if (ar>=LF && ar<LG){
              miLista=&(this->muestras[6]);
               cout<<"seleccionada lista"<<"6";
        }     
        else if (ar>=LG){
             miLista=&(this->muestras[7]); 
             cout<<"seleccionada lista"<<"7";
        }     
    cout<<"\n";
    cout<<"tamaño "<<(*miLista).size()<<"\n";
  for (std::list<Muestra>::iterator it = (*miLista).begin(); it != (*miLista).end(); it++){

                //escalamos la componente conexa al tamaño de la muestra            
                cv::Mat res;
                cv::Mat muestra = (*it).mr;               
                Size s= muestra.size();
                
                cv::resize(temp,res,s,0,0,INTER_NEAREST);
                //imagen de destino para el xor
                cv::Mat dst;               

                int pixels=cv::countNonZero(muestra);                               
                bitwise_xor(muestra,res,dst);               
                               
                if(cv::countNonZero(dst)<=(AJUSTE*pixels)){                   
                   votacion[(*it).cat]++;                                                     
                }  
               
            }          
   
    double vot=0;
    last=0;
    double aux;
    for(int i=0;i<CATEGORIAS;i++){           
        aux=votacion[i]/(double)(*((this->count)+i));
        cout<<"aux:"<<aux;
        if(aux>vot){
            vot=aux;
            last=i;
        }    
    }
    if (vot>0)
        return last;
    else 
        return -1;
    
}
~Conexos() {
}

};