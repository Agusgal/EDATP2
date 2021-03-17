#include <stdio.h>
#include "graficos/graficos.h"
#include "logica/funcionamiento.h"
#include "parser/parser.h"

int parseCallback(char *key, char *value, void *userData);

int main(int argc, char* argv[])
{
    // Punteros de allegro.
    ALLEGRO_BITMAP* textures[NUMOFTEXTURES] = {NULL};
    ALLEGRO_DISPLAY* display = NULL;
    simulation_t sim = {0,0,0,0,NULL,NULL,0};     // Declaramos la simulación.

    //Leemos la entrada pasada por comando
    if (parseCmdLine(argc, argv, &parseCallback, &sim) == -1)
    {
        fprintf(stderr, "failed to parse command line!\n");
        return -1;
    }
    else if(sim.mode == 0){ //si no hay modo de ejecucion
        fprintf(stderr, "Enter the execution mode using option -mode [num]\n");
        return -1;
    }
    else if(sim.h == 0 || sim.w == 0){   //si no se ingreso la altura o el ancho
        fprintf(stderr, "There are not enought information to run simulation. Enter w or h using option -widht [num] or -height [num]\n");
        return -1;
    }
    else if(sim.mode == 1 && sim.numRobots == 0){ //Si el modo es 1 pero no hay robots
        fprintf(stderr, "There are no robots. Enter the number of robots to run the simulation using -robots [num]\n");
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

    if (sim.mode == 2)
    {
        // Sólo si estamos en mode 2.

        //Cargamos la pantalla de carga
        ALLEGRO_BITMAP* loadingScreen = al_load_bitmap("../graficos/resources/textures/loadingScreen.png");

        if(loadingScreen == NULL){
            fprintf(stderr, "Could not loading screen image.\n");
            return -1;
        }
        else{   //Si no hubo error al cargarla, la dibujamos
            int iW = al_get_bitmap_width(loadingScreen);
            int iH = al_get_bitmap_height(loadingScreen);
            al_draw_bitmap(loadingScreen, SCREENWIDTH/2 - iW/2,SCREENHEIGHT/2 - iH/2,0);
            al_flip_display();
        }

        //Reservamos memoria para el arreglo de promedios.
        float* mean = (float*)malloc(sizeof(float));

        // Declaramos e inicializamos la cant de robots en 1.
        int robots = 1;
        sim.numRobots = robots; // Guardamos en la estructura la cant de robots a correr.

        // Guardamos como primer elemento del arreglo promedios la traducción de la macro
        // del promedio de las 1000 simulaciones.
        mean[robots-1] = TICK2TIME(thousandsimulaciones(&sim));

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
            mean[robots-1] = TICK2TIME(thousandsimulaciones(&sim));

        // Repetimos el bucle hasta que el promedio converja.
        } while (fabs(mean[robots-2] - mean[robots-1]) >=  0.1); //Aca para simulaciones con muchas celdas conviene poner un limite porcentual

        // Una vez obtenido el arreglo de promedios, pedimos que se grafique el histograma.
        if (draw_histogram(mean, robots) == -1)
        {
            // Si no se pudo graficar el histograma, retornamos error.
            fprintf(stderr, "failed to draw histogram!\n");
            destroy_all(&sim, textures, display);
            return -1;
        }

        al_destroy_bitmap(loadingScreen);
        free(mean);
    }
    else if (sim.mode == 1)
    {   
    // Si estoy en el mode 1.

        // Inicializamos la simulación.
        createsim(&sim, textures);

        // Corro la simulación mostrandola en pantalla.
        runsimulation(&sim, textures);

        // Aclaramos por consola, la cantidad de ticks que se tardó en limpiar el piso.
        printf("Los robots tardaron %d ticks en completar la limpieza\n", sim.tickCount);

        //Destruimos los recursos de allegro.
        destroy_all(&sim, textures, display);
    }


    return 0;
}

//Interpretamos la entrada que se realizo por ventana de comandos
int parseCallback(char *key, char *value, void *userData)
{
    simulation_t* Data = (simulation_t*) userData;   // "Casteamos" el puntero al formato de nuestra
    // estructura.

    if (!strcmp(key, "height"))
    {
        int temp = atoi(value);    // Si Key="height" ==> guardamos value en el campo altura.
        if (temp <= 0 || 100 < temp)
        {
            return ERROR_CALLBACK;
        }
        Data->h = temp;
    }
    else if (!strcmp(key, "widht"))
    {
        int temp = atoi(value);    // Si Key="weight" ==> guardamos value en el campo ancho.
        if (temp <= 0 || 70 < temp)
        {
            return ERROR_CALLBACK;
        }
        Data->w = temp;
    }
    else if (!strcmp(key, "robots"))
    {
        int temp = atoi(value);    // Si Key="robots" ==> guardamos value en el campo robots.
        if (temp <= 0)
        {
            return ERROR_CALLBACK;
        }
        Data->numRobots = temp;
    }
    else if (!strcmp(key, "mode"))
    {
        int temp = atoi(value);    // Si Key="mode" ==> guardamos value en el campo mode.
        if ((temp != 1) && (temp != 2))
        {
            return ERROR_CALLBACK;
        }
        Data->mode = temp;
    }
    else{
        return ERROR_CALLBACK;
    }
    return OK;
}