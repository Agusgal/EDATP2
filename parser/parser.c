#include "parser.h"

typedef int(*pCallback) (char*, char*, void*);

/*****************************************************************************************
    PARSECMDLINE
    Recibe: argc: cant de elementos en arv.
            argv: arreglo de opciones y parametros.
            pCallback: puntero a la funcion que interpreta los datos.
            userData: estructura donde se guardan los datos.
    Retorna: Cantidad de opciones+parametros recibidos si todo es correcto. Si hay error, -1.
 ****************************************************************************************/

int parseCmdLine(int argc, char* argv[], pCallback p,void*userData) {
    int datos = 0;                  // var donde guardamos params+opciones
    
    for (int i = 1; i < argc; ++i) {    //la primer palabra (con indice 0) es el nombre del programa y  no se cuenta
        
        if (argv[i][0] == '-') {            //Si estoy leyendo una opción   
            
            if (i >= (argc - 1)) {         
                // Si hay clave pero no valor, devuelvo error. 
                return ERROR_PARSER;
            }    
            if ( argv[i][1] == '\0') {  //si es valor sin clave
                return ERROR_PARSER;           // devuelvo error.
            }
            
            if ( p(&(argv[i][1]), argv[i + 1], userData) == 1) {   
                //Si el callback no detecta errores:
                    ++datos;        //sumo cant de datos recibidos.
                    ++i;            //salteo el valor de la clave
            }
            else {      
                return ERROR_PARSER;   // Si callback da error, devuelvo error.
            }
            
        }

        else {                     //Se está recibiendo un parametro
            if (p(NULL, argv[i],userData) == 1) {   
                //Si el callback no detecta errores, incremento los datos recibidos.
                ++datos;
            }
            else {              // Si el Callback devuelve ERROR, devuelvemos error.
                return ERROR_PARSER;
            }
        }
    }

    // Si al final de todo el analisis no hubo errores, devuelvo la cant de 
    // elementos analizados.
    return datos;   
}

