/* 
 * File:   Constants.h
 * Author: jsa
 *
 * Created on 22 de mayo de 2016, 13:23
 */

#ifndef CONSTANTS_H
#define	CONSTANTS_H

//Numero de categorias
#define CATEGORIAS 3

//Colores de la segmentacion
#define ROJO  (0,0,255)
#define VERDE  (0,255,0)
#define AMARILLO (0,255,255)
#define BLANCO (255,255,255)

//Porcentaje de ajuste a muestras
#define AJUSTE 0.75

// 1 de cada NFRAMES se procesa por Kmedias
#define NFRAMES 30

//Intervalos de las listas de muestras
#define LA 0.5
#define LB 1
#define LC 1.5
#define LD 2
#define LE 2.5
#define LF 3
#define LG 3.5

//Ajuste de la ROI de segmentacion en % del frame
#define IZQUIERDA 2.7
#define DERECHA 97
#define ARRIBA  46
#define ABAJO   96

//Ajuste de tamaño max y min comp. conexas como %del frame
#define LIM_SUP 0.0005
#define LIM_INF 0.0001

#endif	/* CONSTANTS_H */

