//
// Created by Agustin on 14/03/2021.
//

#ifndef EDATP2_FUNCIONAMIENTO_H
#define EDATP2_FUNCIONAMIENTO_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../estructuras/estructuras.h"
#include "../graficos/config.h"


#include <allegro5/allegro5.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_color.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_primitives.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>


#define PI          acos(-1)
#define SIZE(v) (sizeof(v)/sizeof(v[0]))
#define CLEAN 0
#define DIRTY 1
#define ANG2RAD(x) (((x) * (PI))/180)  // buscar un pi mas preciso en libreria
#define TIME 1 // a cuantos segundos equivale un tick
#define TICK2TIME(x)  ( ( (x) * (TIME) ) / (1) )    // Todo: medio aldope el "1" XD


int createFloor(simulation_t *sim, ALLEGRO_BITMAP** textura);

int createrobots(simulation_t* sim, ALLEGRO_BITMAP** textura); // listo
void deleterobots ( robot_t* robots);
void moveRobots(simulation_t *sim);
void cleanFloor(simulation_t* sim);


bool checkclean(simulation_t* sim);


void runsimulation(simulation_t *sim, ALLEGRO_BITMAP** textures, int mode);
int createsim(simulation_t *sim, ALLEGRO_BITMAP** textura); //listo
float thousandsimulaciones(simulation_t *sim, int x);


#endif //EDATP2_FUNCIONAMIENTO_H
