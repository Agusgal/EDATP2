#include "graficos.h"
#include "../logica/funcionamiento.h"

void showMainMenu(simulation_t* sim, int* mode) {

    //El usuario ingresa un alto de tablero en [1;67]
    do{
    printf("%s", "Ingresar alto: ");   
    scanf("%d", &sim->h);
    printf("%s", "\n");
    } while (sim->h < 1 && 68 < sim->h);

    //El usuario ingresa un ancho de tablero en [1;120]
    do{
    printf("%s", "Ingresar ancho: ");   
    scanf("%d", &sim->w);
    printf("%s", "\n");
    } while (sim->w < 1 && 120 < sim->w);

    //El usuario ingresa una cant de robots entre [1; casillas del tablero]
    do{
    printf("%s", "Ingresar cant de robots: ");
    scanf("%d", &sim->numRobots);       //MAX = 67 * 120
    printf("%s","\n");
    } while (sim->numRobots < 1 && (sim->w * sim->h) < sim->numRobots);

    //El usuario ingresa el modo en cuestion.
    do {
        printf("%s\n", "Ingresar el modo: \n");
        printf("%s", "-> 1 Tiempo Real \n");
        printf("%s", "-> 2 Grafico de tiempo medio de limpieza \n");
        scanf("%d", mode);
    } while (*mode < 1 && 2 < *mode);

}

int init_allegro()
{
    //Iniciamos allegro y todos sus addons. 
    // En caso de que uno falle retornamos 1, sino 0.

    if (!al_init()) { 
        fprintf(stderr, "failed to initialize allegro!\n");
        return 1;
    }

    if (!al_init_primitives_addon()) {
        fprintf(stderr, "failed to initialize primitives addon!\n");
        return 1;
    }

    if (!al_init_image_addon()) {
        fprintf(stderr, "failed to initialize image addon!\n");
        return 1;
    }

    if (!al_init_font_addon()) {
        fprintf(stderr, "failed to initialize fonts addon!\n");
        return 1;
    }

    if (!al_init_ttf_addon()) {
        fprintf(stderr, "failed to initialize ttf addon!\n");
        return 1;
    }

    return 0;
}

/*
NO SE USA

int drawUI(simulation_t* sim) {

    char buffer[30] = "";
    ALLEGRO_FONT* font = al_load_ttf_font("../resources/fonts/Ranchers-Regular.ttf", FONT_SIZE*3, 0);

    if (!font) {
        fprintf(stderr, "Could not load font.\n");
        return -1;
    }

    sprintf(buffer, "Gen %d", sim->tickCount);
    al_draw_text(font, WHITE, SCREENWIDTH * 7 / 8, SCREENHEIGHT * 7 / 8, ALLEGRO_ALIGN_CENTRE, buffer);

    return 0;

}*/

void drawFloor(simulation_t* sim, ALLEGRO_BITMAP** textures)
{
    // Definimos una escala para que el tablero siempre este en pantalla completa.
    float scale = getScale(sim->w, sim->h);

    // Variables para centrar la simulación.
    int offsetX = 0, offsetY = 0;
    offsetX = SCREENWIDTH / 2 - sim->w * scale * PIXELSPERUNIT/2;
    offsetY = SCREENHEIGHT / 2 - sim->h * scale * PIXELSPERUNIT/2; //TODO: puede fallar, si falla agregar lo que esta en escala


    for (int i = 0; i < sim->h; i++) {
        for (int j = 0; j < sim->w; j++) {

            // Calculo de variables que ayudan al posicionamiento de las baldozas.
            float posX = sim->floor[i][j].xPos * scale;
            float posY = sim->floor[i][j].yPos * scale;
            int imgWidth = al_get_bitmap_width(sim->floor[i][j].texture);
            int imgHeight = al_get_bitmap_height(sim->floor[i][j].texture);


            if(sim->floor[i][j].state == CLEAN){
                //Si el piso esta limpio, colocamos la imagen de la baldozas limpia.
                al_draw_scaled_bitmap(textures[GREENTILEIMG], 0, 0, imgWidth, imgHeight, posX + offsetX, posY + offsetY,
                                                                        imgWidth * scale, imgHeight * scale, 0);
            }
            else{
                //Si el piso esta sucio, colocamos la imagen de la baldozas sucia.
                al_draw_scaled_bitmap(textures[REDTILEIMG], 0, 0, imgWidth, imgHeight, posX + offsetX, posY + offsetY,
                                                                        imgWidth * scale, imgHeight * scale, 0);
            }


        }
    }

}


void drawRobot(simulation_t* sim)
{
    // Definimos una escala para que el tablero siempre este en pantalla completa.
    float scale = getScale(sim->w, sim->h);

    // Variables para centrar la simulación. 
    int offsetX = 0, offsetY = 0;
    offsetX = SCREENWIDTH / 2 - sim->w * scale * PIXELSPERUNIT/2;
    offsetY = SCREENHEIGHT / 2 - sim->h * scale * PIXELSPERUNIT/2;
    
    // Calculamos el ancho y alto de las baldozas.
    int tileW = al_get_bitmap_width(sim->floor[0][0].texture);
    int tileH = al_get_bitmap_height(sim->floor[0][0].texture);

    for (int i = 0; i < sim->numRobots; i++) {

        // Calculamos la posición entera del robot.
        int tilex = floor(sim->robots[i].xPos);
        int tiley = floor(sim->robots[i].yPos);

        // Calculamos el ancho y alto de la imagen de los robots y su angulo.
        int imgW = al_get_bitmap_width(sim->robots[i].texture);
        int imgH = al_get_bitmap_height(sim->robots[i].texture);
        float angle = sim->robots[i].angle;

        // Dibujamos los robots escalados.
        al_draw_scaled_bitmap(sim->robots[i].texture, 0, 0, imgW, imgH, (tilex * tileW + (tileW - imgW)/2) * scale + offsetX , (tiley * tileH + (tileH - imgH) / 2) * scale + offsetY,
                                                            imgW * scale, imgH * scale , 0);

        // Calculamos el ancho y alto de la imagen de la flecha.
        imgW = al_get_bitmap_width(sim->robots[i].arrow);
        imgH = al_get_bitmap_height(sim->robots[i].arrow);

        // Dibujamos las flechas escaladas y con el ángulo debido.
        al_draw_scaled_rotated_bitmap(sim->robots[i].arrow, imgW/2, imgH, (tilex * tileW + imgH / 2)*scale + offsetX, (tiley * tileH + imgH / 2) * scale + offsetY, scale, scale, ANG2RAD(angle+90), 0);
    }

}

float getScale(int w, int h) {
    //TODO: comentar
    if ((SCREENHEIGHT / h) <= (SCREENWIDTH / w)) {
        return (float)SCREENHEIGHT / (float)h / (float)PIXELSPERUNIT;
    }
    else {
        return (float)SCREENWIDTH / (float)w / (float)PIXELSPERUNIT;
    }

}

int loadTextures(ALLEGRO_BITMAP** textura) {

    //Definimos un buffer y una variable auxiliar para error.
    char rutaEfectiva[100];
    int error = 0;

    for (int i = 0; !error && i < NUMOFTEXTURES; i++) {
        //Para cada textura:
        
        // cargamos su ruta.
        int size = sprintf(rutaEfectiva, "../graficos/resources/textures/%d.png", i+1);
        // intentamos cargarla.
        textura[i] = al_load_bitmap(rutaEfectiva);

        if (textura[i] == NULL)
        {
            // Si la carga falla, cargamos 1=TRUE en error.
            printf("couldn't load %s\n", rutaEfectiva);
            error = 1;
        }
    }

    return error; // Retornamos si hubo o no error.
}


int draw_histogram(float* mean, int quant)
{
    //Definimos arr para usarlo como buffer.
    char arr[MAX_CANT + 1] = "";

    //Cargamos la dir. de la font en un puntero
    ALLEGRO_FONT* font = al_load_ttf_font("../graficos/resources/fonts/Ranchers-Regular.ttf", FONT_SIZE, 0);

    if (!font) {
        // Si la carga de la font falla, retornamos error.
        fprintf(stderr, "Could not load font.\n");
        return -1;
    }

    for (int i = 0; i < quant; i++)
    {
        // Dibujamos los ejes
        al_clear_to_color(WHITE);
        al_draw_line(X_MARGIN_LEFT - 15, HIST_Y - Y_MARGIN_INF, HIST_X - X_MARGIN_RIGHT + 5, HIST_Y - Y_MARGIN_INF, BLACK, 2);
        al_draw_line(X_MARGIN_LEFT, HIST_Y - Y_MARGIN_INF + 15, X_MARGIN_LEFT, Y_MARGIN_SUP - 5, BLACK, 2);
        

        for(int j=1; j<=NUM_Y_TICKS; j++)
        {
            // Dibujamos las lineas auxiliares y los y_ticks
            //ESTE FOR DIBUJA LINEAS PUNTEADAS
            for (int k = 1; k <= LINE_RELATION; k+=2)
            {
                al_draw_line(X_MARGIN_LEFT + 5*(k-1), RECT_HEIGHT(0) + LINE_PARTITION*(j-1), X_MARGIN_LEFT + 5 + 5*(k-1), RECT_HEIGHT(0) + LINE_PARTITION*(j-1), BLACK, 1);
            }

            //el draw de abajo es para lineas rectas
            //al_draw_line(X_MARGIN_LEFT, RECT_HEIGHT(0) + LINE_PARTITION*(j-1), HIST_X - X_MARGIN_RIGHT, RECT_HEIGHT(0) + LINE_PARTITION*(j-1), WHITE, 1);
            sprintf(arr, "%.2f", mean[0]/j);
            al_draw_text(font, BLACK, X_MARGIN_LEFT / 2, RECT_HEIGHT(0) + LINE_PARTITION*(j-1) - FONT_SIZE / 2, ALLEGRO_ALIGN_CENTRE, arr);
        }

        // Escribimos que es cada eje.
        al_draw_text(font, BLACK, HIST_X - X_MARGIN_LEFT + 10, HIST_Y - Y_MARGIN_INF * 3 / 4, ALLEGRO_ALIGN_CENTRE, "Robots");
        al_draw_text(font, BLACK, X_MARGIN_LEFT / 2, 5, ALLEGRO_ALIGN_CENTRE, "Tiempo");

        for (int j = 0; j <= i; j++)
        {
            // Dibujamos el rectangulo.
            al_draw_filled_rectangle(X_INIT(j, quant), HIST_Y - Y_MARGIN_INF, X_FIN(j, quant), RECT_HEIGHT(j), BLUE);

            // Dibujamos los X ticks.
            sprintf(arr, "%d", j + 1);
            al_draw_text(font, BLACK, (X_FIN(j, quant) + X_INIT(j, quant)) / 2, HIST_Y - Y_MARGIN_INF * 3 / 4, ALLEGRO_ALIGN_CENTRE, arr);

        }

        //Actualizamos el display y esperamos un tiempo para que se aprecie la animación.
        al_flip_display();
        al_rest(ANIMATION_TIME);
    }
    // Una vez completo el gráfico dejamos un tiempo para que se muestre.
    al_rest(GRAPH_TIME); 

    // Destruimos la font.
    al_destroy_font(font);
    return 0;
}


void destroy_all(simulation_t* sim, ALLEGRO_BITMAP** textures, ALLEGRO_DISPLAY* display)
{
    //Destruimos los bitmaps con las texturas.
    for (int i = 0; i < NUMOFTEXTURES; i++) {
        al_destroy_bitmap(textures[i]);
    }

    // Destruimos display y addons de Allegro.
    al_shutdown_font_addon();
    al_shutdown_primitives_addon();
    al_shutdown_image_addon();
    al_shutdown_ttf_addon();
}


