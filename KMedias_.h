/* 
 * File:   KMedias_.h
 * Author: jsa
 *
 * Created on 24 de noviembre de 2015, 13:27
 */

#ifndef KMEDIAS__H
#define	KMEDIAS__H

class  KMedias_{

private:
    
    void ordenarIndices(arma::fmat* cent);
    
public:
    
    KMedias_();
     
    KMedias_(int k, float* media, float* desv);   
    
    void procesar(arma::fmat imagen);
    
};

#endif	/* KMEDIAS__H */

