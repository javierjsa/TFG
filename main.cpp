
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

#include <Segmentador_.cpp>

#define ARMA_DONT_PRINT_ERRORS 	
#define ARMA_NO_DEBUG

using namespace cv;
using namespace std;
using namespace mlpack;


int main(int argc, char** argv) {
        
    int count[]={129,1886,123};// numero de imagenes de muestra por directorio
    std::vector<string> dirs;
    dirs.push_back("./flecha/");
    dirs.push_back("./lineav/");
    dirs.push_back("./carril/");
    
    int k;
    int c;
    
    int db;
    int dr;
    int dg;
    
    string sal;   
   
    switch(argc){
        
        case 2:
            
            k=50;
            c=3;
            db=50;
            dr=50;
            dg=50;
            sal="salida";
            break;       
        
        case 3:
            
            k=50;
            c=3;
            db=50;
            dr=50;
            dg=50;
            sal=argv[2];
            break;
        
        case 5:
        
            k=atoi(argv[3]);
            c=atoi(argv[4]);
            db=50;
            dr=50;
            dg=50;
            sal=argv[2];
            break;
            
        case 6:
        
            k=atoi(argv[3]);
            c=atoi(argv[4]);
            db=atoi(argv[5]);
            dr=atoi(argv[5]);
            dg=atoi(argv[5]);
            sal=argv[2];
            break;
            
        case 8:
            
            k=atoi(argv[3]);
            c=atoi(argv[4]);
            db=atoi(argv[7]);
            dr=atoi(argv[5]);
            dg=atoi(argv[6]);
            sal=argv[2];
            break;
            
        default:
            
            cerr<<"Uso:\n";
            cerr<<argv[0]<<" "<<"arc_fuente\n"; 
            cerr<<argv[0]<<" "<<"arc_fuente arch_destino\n"; 
            cerr<<argv[0]<<" "<<"arc_fuente arch_destino n_iteraciones n_clusters\n"; 
            cerr<<argv[0]<<" "<<"arc_fuente arch_destino n_iteraciones n_clusters deltR deltG deltB\n";  
            return 1;            
    }            
        
    cv::VideoCapture cap(argv[1]); 
     
    const string NAME = sal;   
   
    int ex = static_cast<int>(cap.get(CV_CAP_PROP_FOURCC));       
    
    Size S = Size((int) cap.get(CV_CAP_PROP_FRAME_WIDTH),    
                  (int) cap.get(CV_CAP_PROP_FRAME_HEIGHT));
 
    VideoWriter outputVideo;                                           
    outputVideo.open(NAME, ex, 30, S, true);    
    
    if (!outputVideo.isOpened())
    {
        cout  << "Could not open the output video for write: " << argv[1] << endl;
        return -1;
    }            
    
    if(!cap.isOpened()) 
        return -1;
    
    Segmentador s= Segmentador(cap,outputVideo,dirs,count,k,c);
    s.Procesar();
        
    return 0;
    
    }
    




