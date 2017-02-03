/* 
 * File:   Muestra.cpp
 * Author: jsa
 * 
 * Created on 2 de abril de 2016, 20:55
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
#endif

#define	MUESTRA_H

#include "Muestra.h"

class Muestra{

public:    

int cat;
cv::Mat mr;


    

Muestra(int cat, cv::Mat mr) {
    
    this->cat=cat;
    this->mr=mr;    
    
}


~Muestra() {
}

};