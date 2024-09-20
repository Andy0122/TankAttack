#ifndef RENDERER_H
#define RENDERER_H

#include <gtk/gtk.h>
#include "../systems/GridGraph.h"  // Incluir GridGraph para acceder a los datos

/**
 * @brief Clase que se encarga de renderizar el GridGraph en un área de dibujo GTK.
 */
class Renderer {
public:
    /**
     * @brief Constructor de la clase Renderer.
     *
     * @param gridGraph Referencia al objeto GridGraph que contiene la cuadrícula a dibujar.
     */
    Renderer(GridGraph& gridGraph);

    /**
     * @brief Destructor de la clase Renderer.
     *
     * Libera los recursos asociados, como las imágenes cargadas.
     */
    ~Renderer();

    /**
     * @brief Metodo para dibujar la cuadrícula en un widget GTK.
     *
     * @param widget El widget de GTK donde se va a dibujar.
     * @param cr El contexto de Cairo utilizado para dibujar.
     */
    void draw(GtkWidget *widget, cairo_t *cr);

    /**
     * @brief Obtiene el tamaño de las celdas.
     *
     * @return El tamaño de una celda de la cuadrícula.
     */
    int getCellSize() const { return cellSize; }

    /**
     * @brief Obtiene el desplazamiento en el eje X.
     *
     * @return El desplazamiento horizontal para centrar la cuadrícula.
     */
    int getXOffset() const { return xOffset; }

    /**
     * @brief Obtiene el desplazamiento en el eje Y.
     *
     * @return El desplazamiento vertical para centrar la cuadrícula.
     */
    int getYOffset() const { return yOffset; }

    /**
     * @brief Devuelve el número de filas de la cuadrícula.
     *
     * @return El número de filas en la cuadrícula.
     */
    int getRows() const { return gridGraph.getRows(); }

    /**
     * @brief Devuelve el número de columnas de la cuadrícula.
     *
     * @return El número de columnas en la cuadrícula.
     */
    int getCols() const { return gridGraph.getCols(); }

private:
    GridGraph& gridGraph;             ///< Referencia al objeto GridGraph que se va a dibujar.
    int cellSize;                     ///< Tamaño de cada celda de la cuadrícula.
    int xOffset;                      ///< Desplazamiento en el eje X para centrar la cuadrícula.
    int yOffset;                      ///< Desplazamiento en el eje Y para centrar la cuadrícula.

    cairo_surface_t *accessibleImage;  ///< Imagen que representa un nodo accesible.
    cairo_surface_t *inaccessibleImage;  ///< Imagen que representa un nodo inaccesible.

    /**
     * @brief Calcula las dimensiones de las celdas y el desplazamiento en base al tamaño de la ventana.
     *
     * @param width Ancho del área de dibujo.
     * @param height Alto del área de dibujo.
     */
    void calculateDimensions(int width, int height);

    /**
     * @brief Carga las imágenes que representan los nodos accesibles e inaccesibles.
     */
    void loadImages();
};

#endif