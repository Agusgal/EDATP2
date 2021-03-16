#include <stdio.h>
#include "graficos/graficos.h"
#include "logica/funcionamiento.h"


int main(int argc, char* argv[])
{
    ALLEGRO_BITMAP* textures[NUMOFTEXTURES];
    ALLEGRO_DISPLAY* display = NULL;
    simulation_t sim;
    int mode;

    //recoge datos del usuario
    showMainMenu(&sim, &mode);

    //Inicializaciones allegro
    if (init_allegro()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    display = al_create_display(SCREENWIDTH, SCREENHEIGHT);
    if (!display) {
        fprintf(stderr, "failed to display!\n");
        return -1;
    }

    if (loadTextures(textures)) {
        fprintf(stderr, "failed to load textures!\n");
        return -1;
    }


    createsim(&sim, textures);

    if (mode == 2)
    {
        //Esta haciendo quilombo aca con mean, malloc y realloc por eso sale antes de la condicion del while.
        float* mean = (float*)malloc(sizeof(float));
        int x = 1;
        sim.numRobots = x;
        mean[x-1] = TICK2TIME(thousandsimulaciones(&sim, x)); //se pasa a tiempo medio con el macro

        do
        {
            x++;
            sim.numRobots = x;
            mean = (float*)realloc(mean, sizeof(float) * x);
            if (mean == NULL)
            {
                return -1;                      // si falla y no sabemos pq es por esto TODO: error control aca
            }
            mean[x-1] = TICK2TIME(thousandsimulaciones(&sim, x)); //x=2
            //printf("x-2: %f, x-1: %f\n", mean[x-2], mean[x-1]);

        } while (fabs(mean[x-2] - mean[x-1]) >=  0.1); //Aca para simulaciones con muchas celdas conviene poner un limite porcentual

        if (draw_histogram(mean, x) == -1)
        {
            fprintf(stderr, "failed to draw histogram!\n");
            return -1;
        }

    }
    else if (mode == 1)
    {
        runsimulation(&sim, textures, mode);
        printf(" el numero de ticks que tardo en completar la limpieza fue %d ", sim.tickCount);
    }

    return 0;
}
