cmake_minimum_required(VERSION 3.17)
project(EDATP2)

set(CMAKE_CXX_STANDARD 20)

add_executable(EDATP2 main.c graficos/graficos.c graficos/graficos.h graficos/config.h logica/funcionamiento.c logica/funcionamiento.h estructuras/estructuras.h)


INCLUDE_DIRECTORIES( /usr/local/Cellar/allegro/5.2.7.0/include )
LINK_DIRECTORIES(/usr/local/Cellar/allegro/5.2.7.0/lib )

file(GLOB LIBRARIES "/usr/local/Cellar/allegro/5.2.7.0/lib/*.dylib")
message("LIBRARIES = ${LIBRARIES}")

TARGET_LINK_LIBRARIES(EDATP2  ${LIBRARIES})