//
// Created by Agustin on 14/03/2021.
//

#ifndef EDATP2_ESTRUCTURAS_H
#define EDATP2_ESTRUCTURAS_H

#include <stdbool.h>
#include <allegro5/bitmap.h>



enum textures {ARROWIMG, REDTILEIMG, GREENTILEIMG, CLEANERIMG};

typedef struct {
    ALLEGRO_BITMAP* texture;
    float xPos;
    float yPos;
    bool state;
}tile_t;

typedef struct {
    ALLEGRO_BITMAP* texture;
    ALLEGRO_BITMAP* arrow;
    float xPos;
    float yPos;
    float angle;
}robot_t;

typedef struct {
    int cantrob;
    float tiempomedio;
}funcion_t ;

typedef struct {
    int w;
    int h;
    int numRobots;
    int tickCount;
    robot_t* robots;
    tile_t** floor;
    int modo;
}simulation_t;


#endif //EDATP2_ESTRUCTURAS_H
