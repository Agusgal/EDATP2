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

    display = al_create_display(SCREENWEIGHT, SCREENHEIGHT);
    if (!display) {
        fprintf(stderr, "failed to display!\n");
        return -1;
    }

    if (loadTextures(textures)) {
        fprintf(stderr, "failed to load textures!\n");
        return -1;
    }


    createsim(&sim, textures);

    /*if (mode == 2)
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
    }*/

    if (mode == 1)
    {
        runsimulation(&sim);
        printf(" el numero de ticks que tardo en completar la limpieza fue %d ", sim.tickCount);
    }

    return 0;
}
