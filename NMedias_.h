/* 
 * File:   NMedias_.h
 * Author: jsa
 *
 * Created on 29 de noviembre de 2015, 20:17
 */

#ifndef NMEDIAS__H
#define	NMEDIAS__H




class  NMedias_{
    
private:



public:
    
    NMedias_();    
    
    NMedias_(cv::Vec3b cols[3]);
    
    void procesar(cv::Mat imagen);


};

#endif	/* NMEDIAS__H */

