#include "graficos.h"
#include "../logica/funcionamiento.h"

/*int main()
{
    ALLEGRO_BITMAP* textures[NUMOFTEXTURES];
    ALLEGRO_DISPLAY* display = NULL;
    simulation_t sim;
    int mode;

    showMainMenu(&sim, &mode);

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

    initSimulation(&sim, textures);

    for (int i = 0; i < 100; i++) {

        drawFloor(&sim);
        drawRobot(&sim);
        //drawUI(&sim);

        al_flip_display();

        //al_rest(1);
    }

    destroy_all(&sim, textures, display);

    //float means[] = { 7.5, 6, 4.5, 4, 3.25, 1.99, 0.5 };
    //draw_histogram(means, 7);

    return 0;
}*/

void showMainMenu(simulation_t* sim, int* mode) {

    printf("%s", "Ingresar cant de robots: ");
    scanf("%d", &sim->numRobots);       //MAX = 67 * 120
    printf("%s","\n");

    printf("%s", "Ingresar alto: ");    //max 67
    scanf("%d", &sim->h);
    printf("%s", "\n");

    printf("%s", "Ingresar ancho: ");   //max 120
    scanf("%d", &sim->w);
    printf("%s", "\n");

    do {
        printf("%s\n", "Ingresar el modo: \n");
        printf("%s", "-> 1 Tiempo Real \n");
        printf("%s", "-> 2 Grafico de tiempo medio de limpieza \n");
        scanf("%d", mode);
    } while (*mode < 1 && 2 < *mode);

}

int init_allegro()
{
    if (!al_init()) { //Primera funcion a llamar antes de empezar a usar allegro.
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

}

void drawFloor(simulation_t* sim, ALLEGRO_BITMAP** textures)
{

    float scale = getScale(sim->w, sim->h);
    int offsetX = 0, offsetY = 0;


    offsetX = SCREENWIDTH / 2 - sim->w * scale * PIXELSPERUNIT/2;
    offsetY = SCREENHEIGHT / 2 - sim->h * scale * PIXELSPERUNIT/2; //TODO: puede fallar, si falla agregar lo que esta en escala


    for (int i = 0; i < sim->h; i++) {
        for (int j = 0; j < sim->w; j++) {

            float posX = sim->floor[i][j].xPos * scale;
            float posY = sim->floor[i][j].yPos * scale;
            int imgWidth = al_get_bitmap_width(sim->floor[i][j].texture);
            int imgHeight = al_get_bitmap_height(sim->floor[i][j].texture);


            if(sim->floor[i][j].state == CLEAN){
                al_draw_scaled_bitmap(textures[GREENTILEIMG], 0, 0, imgWidth, imgHeight, posX + offsetX, posY + offsetY,
                                                                        imgWidth * scale, imgHeight * scale, 0);
            }
            else{
                al_draw_scaled_bitmap(textures[REDTILEIMG], 0, 0, imgWidth, imgHeight, posX + offsetX, posY + offsetY,
                                                                        imgWidth * scale, imgHeight * scale, 0);
            }


        }
    }

}


void drawRobot(simulation_t* sim)
{

    float scale = getScale(sim->w, sim->h);
    int offsetX = 0, offsetY = 0;

    int tileW = al_get_bitmap_width(sim->floor[0][0].texture);
    int tileH = al_get_bitmap_height(sim->floor[0][0].texture);

    offsetX = SCREENWIDTH / 2 - sim->w * scale * PIXELSPERUNIT/2;
    offsetY = SCREENHEIGHT / 2 - sim->h * scale * PIXELSPERUNIT/2;

    for (int i = 0; i < sim->numRobots; i++) {

        int tilex = floor(sim->robots[i].xPos);
        int tiley = floor(sim->robots[i].yPos);
        int imgW = al_get_bitmap_width(sim->robots[i].texture);
        int imgH = al_get_bitmap_height(sim->robots[i].texture);
        float angle = sim->robots[i].angle;


        al_draw_scaled_bitmap(sim->robots[i].texture, 0, 0, imgW, imgH, (tilex * tileW + (tileW - imgW)/2) * scale + offsetX , (tiley * tileH + (tileH - imgH) / 2) * scale + offsetY,
                                                            imgW * scale, imgH * scale , 0);

        imgW = al_get_bitmap_width(sim->robots[i].arrow);
        imgH = al_get_bitmap_height(sim->robots[i].arrow);

        al_draw_scaled_rotated_bitmap(sim->robots[i].arrow, imgW/2, imgH, (tilex * tileW + imgH / 2)*scale + offsetX, (tiley * tileH + imgH / 2) * scale + offsetY, scale, scale, ANG2RAD(angle+90), 0);
    }

}

float getScale(int w, int h) {
    if ((SCREENHEIGHT / h) <= (SCREENWIDTH / w)) {
        return (float)SCREENHEIGHT / (float)h / (float)PIXELSPERUNIT;
    }
    else {
        return (float)SCREENWIDTH / (float)w / (float)PIXELSPERUNIT;
    }

}

// 0 si cargo tood bien, sino 1
int loadTextures(ALLEGRO_BITMAP** textura) {

    char rutaEfectiva[100];
    int error = 0;

    for (int i = 0; !error && i < NUMOFTEXTURES; i++) {

        int size = sprintf(rutaEfectiva, "../graficos/resources/textures/%d.png", i+1);
        textura[i] = al_load_bitmap(rutaEfectiva);

        if (textura[i] == NULL)
        {
            printf("couldn't load %s\n", rutaEfectiva);
            error = 1;
        }
    }

    return error;
}

int initSimulation(simulation_t* sim, ALLEGRO_BITMAP** textura) {

    sim->robots = (robot_t*) malloc(sizeof(robot_t) * sim->numRobots);

    if (sim->robots == NULL) {
        fprintf(stderr, "failed reserving memory for robots!\n");
        return 1;
    }

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

    for (int i = 0; i < sim->numRobots; i++) {
        sim->robots[i].texture = textura[CLEANERIMG];
        sim->robots[i].arrow = textura[ARROWIMG];
        sim->robots[i].xPos = i * PIXELSPERUNIT;
        sim->robots[i].yPos = i * PIXELSPERUNIT;
        sim->robots[i].angle = PIXELSPERUNIT * i;
    }

    for (int i = 0; i < sim->h; i++) {
        for (int j = 0; j < sim->w; j++) {

            sim->floor[i][j].texture = textura[REDTILEIMG];
            sim->floor[i][j].state = CLEAN;
            sim->floor[i][j].yPos = i * PIXELSPERUNIT;
            sim->floor[i][j].xPos = j * PIXELSPERUNIT;
        }
    }

    return 0;
}


int draw_histogram(float* mean, int quant)
{
    char arr[MAX_CANT + 1] = "";

    ALLEGRO_FONT* font = al_load_ttf_font("../graficos/resources/fonts/Ranchers-Regular.ttf", FONT_SIZE, 0);

    if (!font) {
        fprintf(stderr, "Could not load font.\n");
        return -1;
    }

    for (int i = 0; i < quant; i++)
    {
        // Dibujamos los ejes
        al_clear_to_color(BLACK);
        al_draw_line(X_MARGIN_LEFT - 15, HIST_Y - Y_MARGIN_INF, HIST_X - X_MARGIN_RIGHT + 5, HIST_Y - Y_MARGIN_INF, WHITE, 2);
        al_draw_line(X_MARGIN_LEFT, HIST_Y - Y_MARGIN_INF + 15, X_MARGIN_LEFT, Y_MARGIN_SUP - 5, WHITE, 2);
        
        // Escribimos que es cada eje.
        al_draw_text(font, WHITE, HIST_X - X_MARGIN_LEFT + 10, HIST_Y - Y_MARGIN_INF * 3 / 4, ALLEGRO_ALIGN_CENTRE, "Robots");
        al_draw_text(font, WHITE, X_MARGIN_LEFT / 2, 5, ALLEGRO_ALIGN_CENTRE, "Tiempo");

        for (int j = 0; j <= i; j++)
        {
            // Dibujamos el rectangulo.
            al_draw_filled_rectangle(X_INIT(j, quant), HIST_Y - Y_MARGIN_INF, X_FIN(j, quant), RECT_HEIGHT(j), RED);

            // Dibujamos los X ticks.
            sprintf(arr, "%d", j + 1);
            al_draw_text(font, WHITE, (X_FIN(j, quant) + X_INIT(j, quant)) / 2, HIST_Y - Y_MARGIN_INF * 3 / 4, ALLEGRO_ALIGN_CENTRE, arr);

            // Dibujamos los Y ticks.
            sprintf(arr, "%.2f", mean[j]);
            al_draw_text(font, WHITE, X_MARGIN_LEFT / 2, RECT_HEIGHT(j) - FONT_SIZE / 2, ALLEGRO_ALIGN_CENTRE, arr);
        }
        al_flip_display();
        al_rest(ANIMATION_TIME);
    }
    al_rest(GRAPH_TIME); 

    al_destroy_font(font);
    return 0;
}


void destroy_all(simulation_t* sim, ALLEGRO_BITMAP** textures, ALLEGRO_DISPLAY* display)
{
    for (int i = 0; i < NUMOFTEXTURES; i++) {
        al_destroy_bitmap(textures[i]);
    }

    free(sim->robots);

    for (int i = 0; i < sim->h; i++) {
        free(sim->floor[i]);
    }
    free(sim->floor);

    al_destroy_display(display);
    al_shutdown_font_addon();
    al_shutdown_primitives_addon();
    al_shutdown_image_addon();
    al_shutdown_ttf_addon();
}


