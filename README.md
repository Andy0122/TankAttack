# Tank Attack

## Descripción

"Tank Attack" es un juego retro de estrategia por turnos diseñado en C++ utilizando un enfoque de programación orientada a objetos. Dos jugadores compiten en una cuadrícula con tanques de colores, donde el objetivo es destruir los tanques del oponente o, al finalizar el tiempo de 5 minutos, ser el jugador con más tanques intactos.

Objetivos del Proyecto

    General: Desarrollar un juego en C++ aplicando programación orientada a objetos.
    Específicos:
        Aplicar conceptos de orientación a objetos en un contexto de juego.
        Implementar algoritmos de pathfinding (BFS, Dijkstra y A*) y movimiento aleatorio.
        Aplicar patrones de diseño en la solución de problemas de movimiento, disparo y power-ups.

Reglas del Juego

    Cada jugador cuenta con 4 tanques (colores rojo/azul y amarillo/celeste) con comportamientos diferentes según su color.
    Los tanques azules/celestes se mueven usando BFS o movimiento aleatorio con una probabilidad del 50%.
    Los tanques rojos/amarillos se mueven usando Dijkstra o movimiento aleatorio con un 80% de probabilidad de aplicar Dijkstra.
    Cada turno, un jugador puede mover un tanque, disparar o activar un power-up.
    Las balas siguen un algoritmo de línea de visión y rebotan en las paredes.
## Instalación

### Prerrequisitos

- Ubuntu Linux
- Compilador C++ compatible con C++17
- CMake 3.10 o superior
- Bibliotecas de desarrollo de SDL2 y SDL2_mixer

### Instalación de Dependencias

Ejecuta los siguientes comandos para instalar las dependencias necesarias:

```bash
sudo apt update
sudo apt install -y build-essential cmake libsdl2-dev libsdl2-mixer-dev libgtk-3-dev
```
