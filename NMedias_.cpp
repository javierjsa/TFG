/* 
 * File:   NMedias_.cpp
 * Author: jsa
 * 
 * Created on 29 de noviembre de 2015, 20:17
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
#endif

#ifndef NMEDIAS__H
#define	NMEDIAS__H
#endif

using namespace cv;
using namespace std;
using namespace mlpack;

class  NMedias_{
    
private:

    std::vector<float>* datosr;
    std::vector<float>* datosg;
    std::vector<float>* datosb; 
    
    Vec3b col;    
    int clust;

public:
    
    NMedias_(){
        
    }
    
    NMedias_(std::vector<float>* datosr,std::vector<float>* datosg,std::vector<float>* datosb,Vec3b& col, int clust){      
    
    this->clust=clust;        
    this->col=col;
    
    this->datosr=datosr;
    this->datosg=datosg;
    this->datosb=datosb;
    
    }
           
    void procesar(cv::Mat imagen,cv::Mat connected){
        
        MatIterator_<Vec3b> it, end;
        MatIterator_<uchar> cit;
        bool found;
        
        for( it = imagen.begin<Vec3b>(), end = imagen.end<Vec3b>(),
             cit=connected.begin<uchar>(); it != end; ++it,++cit)
            {
            found=1;
             Vec3b a((*it));   
             float bval=abs(a[0]-(*datosb)[clust-1]);             
             float gval=abs(a[1]-(*datosg)[clust-1]);    
             float rval=abs(a[2]-(*datosr)[clust-1]);         
             
             for(int i=clust-2;i>=0;i--){
                float bvaux=abs(a[0]-(*datosb)[i]);             
                float gvaux=abs(a[1]-(*datosg)[i]);    
                float rvaux=abs(a[2]-(*datosr)[i]);   
            
                if((bvaux<bval)||(gvaux<gval)||(rvaux<rval)){
                   found=0;
                   
                }
                        
             }   
           
             if(found==1){         
              
               (*cit)=255;
             }  
        }
   }  
};

