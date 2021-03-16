//
// Created by Agustin on 14/03/2021.
//

#ifndef TP2_GRAFICOS_H
#define TP2_GRAFICOS_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_color.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_primitives.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "config.h"

#include "../estructuras/estructuras.h"

#define BLACK al_map_rgb(0, 0 ,0)
#define RED al_map_rgb(255, 0, 0)
#define WHITE al_map_rgb(250, 250, 250)
#define BLUE  al_map_rgb(0, 63, 92)
#define CLEAN 0



void showMainMenu(simulation_t* sim, int* mode);
int init_allegro();
int loadTextures(ALLEGRO_BITMAP** textura);
//int initSimulation(simulation_t* sim, ALLEGRO_BITMAP** textura); NO SE USA
void drawFloor(simulation_t* sim, ALLEGRO_BITMAP** textures);
void drawRobot(simulation_t* sim);
void destroy_all(simulation_t* sim, ALLEGRO_BITMAP** textures, ALLEGRO_DISPLAY* display);
float getScale(int w, int h);
//int drawUI(simulation_t* sim); NO SE USA

int draw_histogram(float* mean, int quant);

#endif //TP2_GRAFICOS_H
