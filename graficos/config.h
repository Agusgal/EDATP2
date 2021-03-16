#ifndef _CONFIG_H_
#define _CONFIG_H_

#define PIXELSPERUNIT 16
#define NUMOFTEXTURES 4
#define SCREENWIDTH 1920
#define SCREENHEIGHT 1080

// Constantes y Macros del Histograma
#define HIST_X  850
#define HIST_Y  500
#define X_MARGIN_LEFT 40
#define X_MARGIN_RIGHT 15
#define Y_MARGIN_INF 25
#define Y_MARGIN_SUP 15
#define HIST_SEP 15
#define HIST_MARGIN 10
#define AXIS_MARGIN 5
#define RECT_WIDTH(quant)  ((HIST_X - X_MARGIN_LEFT - X_MARGIN_RIGHT - AXIS_MARGIN - (quant) * HIST_SEP) / (quant))
#define X_INIT(j, quant)   (AXIS_MARGIN + X_MARGIN_LEFT + HIST_SEP + (HIST_SEP + RECT_WIDTH(quant)) * (j))
#define X_FIN(j, quant)    (AXIS_MARGIN + X_MARGIN_LEFT + (HIST_SEP + RECT_WIDTH(quant)) * ((j) + 1))
#define REL(j)              (mean[(j)]/mean[0])
#define RECT_HEIGHT(j)      (HIST_Y - (Y_MARGIN_INF + (HIST_Y - Y_MARGIN_INF - Y_MARGIN_SUP - HIST_MARGIN) * REL(j)))
#define FONT_SIZE           Y_MARGIN_INF/2
#define MAX_CANT    10
#define GRAPH_TIME  5
#define ANIMATION_TIME  0.5
#endif //_CONFIG_H_