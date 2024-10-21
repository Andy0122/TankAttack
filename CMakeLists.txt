cmake_minimum_required(VERSION 3.10)
project(TankAttack)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Buscar las librerías usando PkgConfig
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2MIXER REQUIRED SDL2_mixer)

# Verificar que las bibliotecas se encontraron
if(NOT SDL2MIXER_FOUND)
    message(FATAL_ERROR "SDL2_mixer no encontrado. Por favor, instala libsdl2-mixer-dev.")
endif()

# Incluir los directorios
include_directories(
        ${GTK_INCLUDE_DIRS}
        ${SDL2_INCLUDE_DIRS}
        ${SDL2MIXER_INCLUDE_DIRS}
        include
)

# Linkear los directorios
link_directories(
        ${GTK_LIBRARY_DIRS}
        ${SDL2_LIBRARY_DIRS}
        ${SDL2MIXER_LIBRARY_DIRS}
)

# Añadir definiciones
add_definitions(
        ${GTK_CFLAGS_OTHER}
        ${SDL2_CFLAGS_OTHER}
        ${SDL2MIXER_CFLAGS_OTHER}
)

# Añadir tu ejecutable y archivos fuente
add_executable(TankAttack
        src/main.cpp
        src/entities/Tank.cpp
        src/systems/GridGraph.cpp
        src/Model.cpp
        src/View.cpp
        src/Controller.cpp
        src/entities/Bullet.cpp
        src/systems/Pathfinder.cpp
        src/systems/SoundManager.cpp
        include/entities/Player.h
        src/entities/Player.cpp
        include/data_structures/Queue.h
        src/data_structures/Queue.cpp
        include/data_structures/LinkedList.h
        src/data_structures/LinkedList.cpp
        # Añade otros archivos fuente según sea necesario
)

# Linkear las bibliotecas
target_link_libraries(TankAttack
        ${GTK_LIBRARIES}
        ${SDL2_LIBRARIES}
        ${SDL2MIXER_LIBRARIES}
)
