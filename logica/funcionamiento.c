//
// Created by Agustin on 14/03/2021.
//

#include "funcionamiento.h"
#include "../graficos/graficos.h"

/*int main(void)
{
    int modo = 1;
    long ancho = 2;
    long alto = 3;
    int cantrob = 2;
    if (modo)
    {

        int x = 0;
        funcion_t* funcion = (funcion_t*)malloc(sizeof(funcion_t)); // falta correjir
        if (funcion != NULL)
        {

            (funcion )->tiempomedio = TICK2TIME(thousandsimulaciones(ancho, alto, 1));
            (funcion )->cantrob = 1;

            do
            {
                x++;

                funcion = (funcion_t*)realloc(funcion, sizeof(funcion_t) * (x + 1));

                if (funcion == NULL)
                {
                    return -1;                      // si falla y no sabemos pq es por esto
                }

                (funcion + (x))->tiempomedio = TICK2TIME(thousandsimulaciones(ancho, alto, x + 1));



                (funcion + (x))->cantrob = (x + 1);




            } while ( ( (*( funcion + (x ) )).tiempomedio) - (((*(funcion + (x - 1))).tiempomedio)) >=  0.1);

        }
        else
        {
            free(funcion);
        }
    }

    else if (modo == 1)
    {
        simulation_t* simu = runsimulation(ancho, alto, cantrob, modo);
        printf(" el numero de ticks que tardo en completar la limpieza fue %d ", simu->tickcount);

    }

    return 0;
}*/



int createrobots(simulation_t* sim, ALLEGRO_BITMAP** textura)
{

    srand(time(NULL));

    sim->robots = (robot_t*) malloc(sizeof(robot_t) * (sim->numRobots)) ;

    if (sim->robots == NULL) {
        fprintf(stderr, "failed reserving memory for robots!\n");
        return 1;
    }

    if (sim->robots != NULL)
    {

        for (int i = 0; i < sim->numRobots; i++)
        {
            if(textura != NULL) {
                sim->robots[i].texture = textura[CLEANERIMG];
                sim->robots[i].arrow = textura[ARROWIMG];
            }
            sim->robots[i].xPos = (rand() % (sim->w)) + ((rand() % (10)) * (0.1));//Todo hay casteo que se tiene que arreglar
            sim->robots[i].yPos = (rand() % (sim->h)) + ((rand() % (10)) * (0.1));
            sim->robots[i].angle = (rand() % 361);
        }
    }
    else
    {
        free(sim->robots);
    }

    return 0;
}

int createFloor(simulation_t *sim, ALLEGRO_BITMAP** textura)
{
    sim->floor = (tile_t**) malloc(sizeof(tile_t*) * sim->h);

    if (sim->floor != NULL) {
        for (int i = 0; i < sim->h; i++) {

            sim->floor[i] = (tile_t*) malloc(sizeof(tile_t) * sim->w);

            if (sim->floor[i] == NULL) {
                fprintf(stderr, "failed reserving memory for floor rows!\n");
                return 1;
            }
        }
    }
    else {
        fprintf(stderr, "failed reserving memory for floor columns!\n");
        return 1;
    }

    for (int i = 0; i < sim->h; i++) {
        for (int j = 0; j < sim->w; j++) {

            if(textura != NULL){
                sim->floor[i][j].texture = textura[REDTILEIMG];
            }
            sim->floor[i][j].state = DIRTY;
            sim->floor[i][j].yPos = i * PIXELSPERUNIT;
            sim->floor[i][j].xPos = j * PIXELSPERUNIT;
        }
    }

    return 0;
}


void deleterobots(robot_t* robots)
{
    if (robots)
    {
        free(robots);
    }
}
void deletefloor(simulation_t* sim)
{
    for (int i = 0; i < sim->h; i++) {
        free(sim->floor[i]);
    }
    free(sim->floor);
}

int createsim(simulation_t *sim, ALLEGRO_BITMAP** textura)
{
    sim->tickCount = 0;

    if(createrobots(sim, textura) || createFloor(sim, textura))
    {
        return 1;
    }
    else
    {
        return 0; //no hubo error
    }
}

void moveRobots(simulation_t *sim)
{
    int error = 0;

    for (int i = 0; i < sim->numRobots; i++)
    {
        float nextx = sim->robots[i].xPos + cos(ANG2RAD(sim->robots[i].angle));
        float nexty = sim->robots[i].yPos + sin(ANG2RAD(sim->robots[i].angle));

        if ((nextx >= 0) && (nextx < sim->w) && (nexty >= 0) && (nexty < sim->h))
        {
            //muevo ambos robots
            sim->robots[i].yPos = nexty;
            sim->robots[i].xPos = nextx;
        }
        else
        {
            sim->robots[i].angle = (rand() % 360);
        }
/*
        while (error == 1)
        {
            sim->robots[i].angle = (rand() % 360);
            nextx = sim->robots[i].xPos + cos(ANG2RAD(sim->robots[i].angle));
            nexty = sim->robots[i].yPos + sin(ANG2RAD(sim->robots[i].angle));

            error = 0;
            if ((nextx >= 0) && (nextx < sim->w) && (nexty >= 0) && (nexty < sim->h))
            {
                //muevo ambos robots
                sim->robots[i].yPos = nexty;
                sim->robots[i].xPos = nextx;
            }
            else
            {
                error = 1;
            }
        }*/
    }
}


void cleanFloor(simulation_t* sim)
{
    //int basura;
    for(int i = 0; i < sim->numRobots; i++)
    {
        int tilex = floor(sim->robots[i].xPos);
        int tiley = floor(sim->robots[i].yPos);

        sim->floor[tiley][tilex].state = CLEAN;
    }
}


bool checkclean(simulation_t*sim)
{
    bool itsclean = true;

    for (int i = 0; i < sim->h; i++)
    {
        for (int j = 0; j < sim->w; j++)
        {
            if(sim->floor[i][j].state == DIRTY)
            {
                return !itsclean;
            }
        }
    }
    return itsclean;
}

void runsimulation(simulation_t *sim, ALLEGRO_BITMAP** textures, int mode)
{

    while (!checkclean(sim))
    {
        cleanFloor(sim);

        if(mode == 1) {
            drawFloor(sim, textures);
            drawRobot(sim);
            //drawUI(&sim); TODO: porque esta esto aca??

            al_flip_display();

            al_rest(TICK2TIME(TIME));
            al_clear_to_color(BLACK);
        }

        moveRobots(sim);

        sim->tickCount++;

    }

    deleterobots(sim->robots);
    deletefloor(sim);

}


float thousandsimulaciones(simulation_t *sim, int x)
{
    int cont = 0;
    float ticks = 0;
    
    while (cont < SIMULATION_N)
    {
        createsim(sim,NULL);
        runsimulation(sim , NULL , 2);
        ticks += (((float)sim->tickCount) / SIMULATION_N);
        cont++;
    }
    printf("Se tardó %f ticks en correr 1000 simulaciones con %d robots.\n", ticks, sim->numRobots);
    return ticks;
}