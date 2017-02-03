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

#ifndef KMEDIAS__H
#define	KMEDIAS__H
#endif

#define CLUSTERS 2


#include <vector>



#include "KMedias_.h"

using namespace cv;
using namespace std;
using namespace mlpack;

class  KMedias_{
    
private:      
    
    std::vector<float>* result;   
    
    size_t clusters;
    arma::fmat datos;
    arma::Col<size_t> assignments;
    arma::fmat centroids; 
   
    mlpack::kmeans::KMeans<> k;    
    
    std::vector<int> indices;
    
    void ordenarIndices(){ //ordena de menor a mayor
       
        this->indices[0]=0;
    
        for (int i=1;i<clusters;i++){        
            int aux=this->centroids.at(0,i);
            int j;
            for(j=i-1;j>=0 &&centroids[indices[j]]>aux; j--)
                indices[j+1]=indices[j];
            indices[j+1]=i;       
        }   
        
        for (int i=0;i<clusters;i++){        
           (*result).at(i)=centroids[indices[i]];
        }
        
    }
    
public:
    
    KMedias_(){
        
    }
    
    
    KMedias_(int k,int clust,std::vector<float>* datos)
    {
                          
        this->centroids=arma::fmat(1,clust);     
        
        this->k= mlpack::kmeans::KMeans<>(k,1.0);
                
        this->indices=std::vector<int>(clust);
        
        this->result=datos;
        
        this->clusters=clust;
        
        for(int i=0;i<clust;i++)
            indices[i]=i;
               
    }
    
    void procesar(arma::fmat& datos){        
       
        this->k.Cluster(datos,clusters,assignments,centroids,false,true);
               
        
        arma::Col<size_t>::iterator ita,end; 
        arma::fmat::iterator dat=datos.begin();
        
        std::vector<int> count=std::vector<int>(clusters);  
               
        this->ordenarIndices(); //esto hay que cambiarlo         
      
    }

};