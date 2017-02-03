/* 
 * File:   Segmentador.cpp
 * Author: jsa
 * 
 * Created on 9 de diciembre de 2015, 11:22
 */
#ifndef INCLUDES
#define INCLUDES

#include <mlpack/core.hpp>
#include <mlpack/methods/kmeans/kmeans.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <mlpack/methods/kmeans/random_partition.hpp>
#include <math.h>
#include <Constants.h>
#endif

#ifndef CLUSTER
#define CLUSTER 2
#endif

#ifndef SEGMENTADOR_H
#define	SEGMENTADOR_H
#endif

#include "Segmentador.h"


#include <KMedias_.cpp>
#include <NMedias_.cpp>
#include <Conexos_.cpp>

#define ARMA_DONT_PRINT_ERRORS 	
#define ARMA_NO_DEBUG

using namespace cv;
using namespace std;
using namespace mlpack;
using namespace arma;


class  Segmentador{
    
private:

std::vector<std::list<Muestra> >muestras ;
    
VideoCapture in;
VideoWriter out;

VideoWriter cnout;

KMedias_ kr;
KMedias_ kg;
KMedias_ kb;

NMedias_ Nrgb;
    
Conexos  conn;
cv::Mat frame;
cv::Mat channel[3];
cv::Mat select;
cv::Mat comp;

std::vector<float> datosr;
std::vector<float> datosg;
std::vector<float> datosb;

//Directorios de las muestras

std::vector<string> dirs;


//Numero de archivos
int* count;

int ex;
    
const string NAME= "salida.avi";

int k;

int fr=0;
double frcount=0;

cv::Point p1;
cv::Point p2;
cv::Point p3;
cv::Point p4; 

vector< vector<Point> >  co_ordinates;

arma::fmat r; 
arma::fmat g;        
arma::fmat b;

cv::Vec3b col=Vec3b VERDE;

float x,y;

int xorig,yorig,xfin,yfin,x1,y1,x2,y2,xr,yr; //valoresROI;
 
Size S;

std::list<cv::Mat> lista;

public:
    
Segmentador(){
    
}    
    
Segmentador(VideoCapture& in, VideoWriter& out,std::vector<string> dirs, int count[] ,int k,int clust) {
    
   
        
    this->in=in;
    this->out=out;
    this->cnout=cnout;
    this->k=k;
    
    for(int i=0; i<8;i++)
        this->muestras.push_back(std::list<Muestra>());
  
    this->count=count;
    
    this->dirs=dirs;
    
    datosr = std::vector<float>(clust);
    datosg = std::vector<float>(clust);
    datosb = std::vector<float>(clust);
    
    kr=KMedias_(k,clust,&datosr);
    kg=KMedias_(k,clust,&datosg); 
    kb=KMedias_(k,clust,&datosb);
    
    
    this->Nrgb=NMedias_(&datosr,&datosg,&datosb,col,clust);
    
    this->S = Size((int) in.get(CV_CAP_PROP_FRAME_WIDTH),   
                  (int) in.get(CV_CAP_PROP_FRAME_HEIGHT));
    
    x=(float)S.width;
    y=(float)S.height;
    
    this->x1=(int)round((x/100)* IZQUIERDA);
    this->x2=(int)round((x/100)* DERECHA);
    
    this->y1=(int)round((y/100)* ARRIBA);
    this->y2=(int)round((y/100)* ABAJO);
    
    this->xr=(x2-x1);
    this->yr=(y2-y1);
    
    p1= Point(x1,y1);
    p2= Point(x2,y1);
    p3= Point(x2,y2);
    p4= Point(x1,y2);
    co_ordinates.push_back(vector<Point>());
    co_ordinates[0].push_back(p1);
    co_ordinates[0].push_back(p2);
    co_ordinates[0].push_back(p3);
    co_ordinates[0].push_back(p4);
        
    conn=Conexos(p1,S,muestras,this->count);
    
    cout<<p1<<","<<p2<<","<<p3<<","<<p4<<"\n";
    
    this->ex = static_cast<int>(in.get(CV_CAP_PROP_FOURCC));           
               
}

void Procesar(){
    
    this->Carga();
    cout<<"Tamaño en segmentador de vector "<<this->muestras.size()<<"\n";
    this->conn.setMuestras(this->muestras);
    for(int i=0;i<8;i++)
        cout<<this->muestras[i].size()<<",";
    cout<<"\n";               
    
    this->cnout.open(NAME,this->ex, 30, S, true);    
    namedWindow("segmentacion",1);       
    cv::Mat frame;    
      
    for(;;)
    {        
        if(fr==NFRAMES)
           fr=0;         
        
        in>> frame; 
       
        if (frame.empty()) break;           
        
        if(fr==0){    
            Mascara(frame);                      
            Transformar();
            
            cerr<<"Frame: "<<frcount<<"\n\n";
            
            kr.procesar(r);
            kg.procesar(g);
            kb.procesar(b);                
                       
        }
         
        
      cv::Mat temp=frame(Rect(x1,y1,xr,yr));  
      
      cv::Mat labels=cv::Mat(temp.size(),CV_16U); 
      
      comp=cv::Mat(temp.size(),CV_8UC1,Scalar(0));
        
      Nrgb.procesar(temp,comp);  
      
      temp=frame(Rect(x1,y1,xr+1,yr+1)); 
      
      int lab=conn.procesar(temp,comp,labels);    
      
     cv::rectangle(frame, Rect(x1-1,y1-1,xr+3,yr+3),Scalar BLANCO, 1, 4, 0);
      
     out << frame;     
      cv::Mat aux;   
     
      cnout<<aux;
      
      imshow("segmentacion", frame);
     
      waitKey(1);
       
       fr++;
       frcount++;
    }    
                
}

void Mascara(cv::Mat frame){
    //importante generar la mascar sobre el frame completo y no un roi,
    //se producen efectos extraños
           
    //Se crea la base de la mascara
    cv::Mat mask(frame.rows, frame.cols, CV_8UC1, cv::Scalar(0));   
    //Parte visible
    drawContours( mask,co_ordinates,0, Scalar(255),CV_FILLED, 8 );   
    //Imagen para almacenar el resultado
    cv::Mat resultado = cv::Mat(frame.rows, frame.cols, CV_8UC3,Scalar(0,0,0));   
    //Se copia el resultado
    frame.copyTo(resultado, mask);     
    cv::Mat roi = resultado( Rect(x1,y1,x2-x1,y2-y1));    
    //Selección de los pixeles del recorte, ya que la máscara es un triángulo
    cv::Mat column = cv::Mat(roi.rows*roi.cols,1, CV_8UC3); 
    cv::MatIterator_<Vec3b> it, end,vec;    
    vec=column.begin<Vec3b>();
        
    //Vec3b cero=Vec3b(0,0,0); //el fondo de la máscara es negro        
    int cont=0;
    
    for( it = roi.begin<Vec3b>(), end = roi.end<Vec3b>(); it != end; ++it,++vec)
    {
       Vec3b a=Vec3b(*it);
       if((a[0]!=0)||(a[1]!=0)||(a[2]!=0)){
           (*vec)=a;
           cont++;
       }    
    }
       
    //Se recorta la parte no usada de 
    cv::Mat seleccion = column(Range(0,cont),Range::all()).clone();
    
    //Pixeles útiles separados por canal
    cv::split(seleccion,channel);    
}

//transforma la matriz de opencv a matriz mlpack
void Transformar(){
    
    arma::Mat<unsigned char> auxb=arma::Mat<unsigned char>( const_cast<unsigned char*>(channel[0].data), channel[0].cols, channel[0].rows );
    arma::Mat<unsigned char> auxg=arma::Mat<unsigned char>( const_cast<unsigned char*>(channel[1].data), channel[1].cols, channel[1].rows );
    arma::Mat<unsigned char> auxr=arma::Mat<unsigned char>( const_cast<unsigned char*>(channel[2].data), channel[2].cols, channel[2].rows );
    
    this->r=conv_to<fmat>::from(auxr);
   
    this->g=conv_to<fmat>::from(auxg);
    this->b=conv_to<fmat>::from(auxb);
  
}

/**
 * Cada imagen se coloca en una lista en función de su tamaño
 * el parametro cat indica su categoria
 * count lee el número de imagenes indicado  
 */
void Carga(){
        
    Size s;
    int w;
    int h;
    int lim;
    for(int j=0;j<this->dirs.size();j++){
    
        lim=*((this->count)+j);
        for(int i=0;i<lim;i++){

            stringstream ss;
            ss<<this->dirs.at(j)<<i<<".jpg";
            string nom=ss.str();
            cout<<nom<<"\n";

            float ar=(float)w/(float)h;
            cv::Mat temp=imread(nom,CV_8UC1);

            s=temp.size();
            w=s.width;
            h=s.height;

            Muestra mr= Muestra(j,temp);        


            if(ar<LA){
                (this->muestras[0]).push_back(mr);
            }
            else if (ar>=LA && ar<LB){
                 (this->muestras[1]).push_back(mr);
            }
            else if (ar>=LB && ar<LC){
                 (this->muestras[2]).push_back(mr);
            }     
            else if (ar>=LC && ar<LD){
                 (this->muestras[3]).push_back(mr);
            }
            else if (ar>=LD && ar<LE){
                 (this->muestras[4]).push_back(mr);
            }     
            else if (ar>=LE && ar<LF){
                 (this->muestras[5]).push_back(mr);
            }     
            else if (ar>=LF && ar<LG){
                 (this->muestras[6]).push_back(mr);
            }     
            else if (ar>=LG){
                 (this->muestras[7]).push_back(mr);       
            }     
        }
    }
}

};