#ifndef _CONFIG_H_
#define _CONFIG_H_

#define PIXELSPERUNIT 16
#define NUMOFTEXTURES 4
#define SCREENWIDTH 1366
#define SCREENHEIGHT 768

// Constantes y Macros del Histograma
#define HIST_X  SCREENWIDTH * 9/10
#define HIST_Y  SCREENHEIGHT * 17/20
#define X_MARGIN_LEFT 60
#define X_MARGIN_RIGHT 15
#define Y_MARGIN_INF 25
#define Y_MARGIN_SUP 15
#define NUM_Y_TICKS 9
#define HIST_SEP 5
#define HIST_MARGIN 10
#define AXIS_MARGIN 5
#define RECT_WIDTH(quant)  ((HIST_X - X_MARGIN_LEFT - X_MARGIN_RIGHT - AXIS_MARGIN - (quant) * HIST_SEP) / (quant))
#define X_INIT(j, quant)   (AXIS_MARGIN + X_MARGIN_LEFT + HIST_SEP + (HIST_SEP + RECT_WIDTH(quant)) * (j))
#define X_FIN(j, quant)    (AXIS_MARGIN + X_MARGIN_LEFT + (HIST_SEP + RECT_WIDTH(quant)) * ((j) + 1))
#define REL(j)              (mean[(j)]/mean[0])
#define RECT_HEIGHT(j)      (HIST_Y - (Y_MARGIN_INF + (HIST_Y - Y_MARGIN_INF - Y_MARGIN_SUP - HIST_MARGIN) * REL(j)))
#define FONT_SIZE           (Y_MARGIN_INF/2.5f)


#define LINE_PARTITION    ((HIST_Y - Y_MARGIN_INF - RECT_HEIGHT(0))/NUM_Y_TICKS) //defini esta particion como referencia para dibuajr lineas
#define LINE_WIDTH 5
#define EFFECTIVE_WIDTH     HIST_X - X_MARGIN_RIGHT - X_MARGIN_LEFT
#define LINE_RELATION      ((EFFECTIVE_WIDTH)/(LINE_WIDTH))



#define MAX_CANT    10
#define GRAPH_TIME  10
#define ANIMATION_TIME  0.5

#endif //_CONFIG_H_