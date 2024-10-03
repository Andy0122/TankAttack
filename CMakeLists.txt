cmake_minimum_required(VERSION 3.29)
project(TankAttack)

set(CMAKE_CXX_STANDARD 20)

# Buscar las librerías de GTK
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)

# Incluir los directorios de GTK
include_directories(${GTK_INCLUDE_DIRS})

# Linkear las librerías de GTK
link_directories(${GTK_LIBRARY_DIRS})

# Evitar que se usen -Wl,-rpath
add_definitions(${GTK_CFLAGS_OTHER})

include_directories(include)


add_executable(TankAttack src/main.cpp
        src/entities/Tank.cpp
        include/entities/Tank.h
        include/systems/GridGraph.h
        src/systems/GridGraph.cpp
        src/Model.cpp
        src/View.cpp
        src/Controller.cpp
        include/Position.h)

# Linkear las librerías de GTK con tu ejecutable
target_link_libraries(TankAttack ${GTK_LIBRARIES})
