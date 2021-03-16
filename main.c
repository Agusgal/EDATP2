#include <stdio.h>
#include "graficos/graficos.h"
#include "logica/funcionamiento.h"
#include "parser/parser.h"


int main(int argc, char* argv[])
{
    // Punteros de allegro.
    ALLEGRO_BITMAP* textures[NUMOFTEXTURES];
    ALLEGRO_DISPLAY* display = NULL;
    
    // Declaramos la simulación.
    simulation_t sim;
    
    /*
    // Declaramos el modo de la simulación.
    int mode;

    // Recoge datos del usuario
    showMainMenu(&sim, &mode);
    */

    if (parseCmdLine(argc, argv, &parseCallback, &sim)==-1)
    {
        return -1;
    }

    // Inicializaciones allegro
    if (init_allegro()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    // Creamos el display. Si no se puede crear, devolvemos error.
    display = al_create_display(SCREENWIDTH, SCREENHEIGHT);
    if (!display) {
        fprintf(stderr, "failed to display!\n");
        return -1;
    }

    // Cargamos las texturas, si falla devolvemos el error.
    if (loadTextures(textures)) {
        fprintf(stderr, "failed to load textures!\n");
        return -1;
    }

    // Inicializamos la simulación.
    createsim(&sim, textures);

    if (sim.modo == 2)
    {
    // Sólo si estamos en modo 2.

        //Reservamos memoria para el arreglo de promedios.
        float* mean = (float*)malloc(sizeof(float));

        // Declaramos e inicializamos la cant de robots en 1.
        int robots = 1;
        sim.numRobots = robots; // Guardamos en la estructura la cant de robots a correr.

        // Guardamos como primer elemento del arreglo promedios la traducción de la macro
        // del promedio de las 1000 simulaciones.
        mean[robots-1] = TICK2TIME(thousandsimulaciones(&sim, robots));

        do
        {
            // Incrementamos la cantidad de robots.
            robots++;

            // Guardamos en la estructura la cant de robots a correr.
            sim.numRobots = robots;

            // Agrandamos la memoria dinamica.
            mean = (float*)realloc(mean, sizeof(float) * robots);
            if (mean == NULL)
            {
                // Si falla el resize de la memoria dinamica retornamos error.
                return -1;                
            }

            // Guardamos como robot-esimo elemento del arreglo de promedios la traducción de la macro
            // del promedio de las 1000 simulaciones con cantidad "robots" de robots.
            mean[robots-1] = TICK2TIME(thousandsimulaciones(&sim, robots));

        // Repetimos el bucle hasta que el promedio converja.
        } while (fabs(mean[robots-2] - mean[robots-1]) >=  0.1); //Aca para simulaciones con muchas celdas conviene poner un limite porcentual

        // Una vez obtenido el arreglo de promedios, pedimos que se grafique el histograma.
        if (draw_histogram(mean, robots) == -1)
        {
            // Si no se pudo graficar el histograma, retornamos error.
            fprintf(stderr, "failed to draw histogram!\n");
            return -1;
        }

    }
    else if (sim.modo == 1)
    {   
    // Si estoy en el modo 1.

        // Corro la simulación mostrandola en pantalla.
        runsimulation(&sim, textures, sim.modo);

        //Destruimos los recursos de allegro.
        destroy_all(&sim, textures, display);

        // Aclaramos por consola, la cantidad de ticks que se tardó en limpiar el piso.
        printf(" el numero de ticks que tardo en completar la limpieza fue %d ", sim.tickCount);
    }

    return 0;
}

int parseCallback(char *key, char *value, void *userData)
{
    simulation_t * Data = userData;   // "Casteamos" el puntero al formato de nuestra
                                      // estructura.
    
    if (!strcmp(key, "height"))        
    {
        long int temp = atol(value);    // Si Key="height" ==> guardamos value en el campo altura.
        if (!temp)
        {
            return ERROR_CALLBACK;
        }
        Data->h = temp;
    }
    else if (!strcmp(key, "weight"))
    {
        long int temp = atol(value);    // Si Key="weight" ==> guardamos value en el campo ancho.
        if (!temp)
        {
            return ERROR_CALLBACK;
        }
        Data->w = temp;
    }
    else if (!strcmp(key, "robots"))
    {    
        long int temp = atol(value);    // Si Key="robots" ==> guardamos value en el campo robots.
        if (!temp)
        {
            return ERROR_CALLBACK;
        }
        Data->robots = temp;
    }
    else if (!strcmp(key, "modo"))
    {    
        long int temp = atol(value);    // Si Key="modo" ==> guardamos value en el campo modo.
        if (!temp)
        {
            return ERROR_CALLBACK;
        }
        Data->modo = temp;
    }
    else{
        return ERROR_CALLBACK;
    }
    return OK;
}