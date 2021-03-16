#include <stdio.h>
#include "graficos/graficos.h"
#include "logica/funcionamiento.h"


int main(int argc, char* argv[])
{
    ALLEGRO_BITMAP* textures[NUMOFTEXTURES];
    ALLEGRO_DISPLAY* display = NULL;
    simulation_t sim;
    int mode;

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
        float* mean = (float*)malloc (sizeof(float));
        int x = 1;
        sim.numRobots = x;
        mean[x-1] = TICK2TIME(thousandsimulaciones(&sim, x)); 

        do
        {
            x++;
            sim.numRobots = x;
            mean = (float*) realloc(mean, sizeof(float) * x);
            if (mean == NULL)
            {
                return -1;                      // si falla y no sabemos pq es por esto
            }
            mean[x-1] = TICK2TIME(thousandsimulaciones(&sim, x));

        } while ((mean[x] - mean[x-1]) >=  0.1) ;

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
