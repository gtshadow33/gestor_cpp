cmake_minimum_required(VERSION 3.10)
project(gestor_tareas)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Buscar todos los .cpp dentro de src
file(GLOB SRC_FILES src/*.cpp)

add_executable(gestor_tareas ${SRC_FILES})