cmake_minimum_required(VERSION 3.30)
project(Snake)

# Especificar el estándar de C++ (ej. C++17)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Indicar dónde está el ejecutable y sus archivos fuente
add_executable(snake src/main.cpp)
set_property(TARGET snake PROPERTY CXX_MODULE_STD ON)