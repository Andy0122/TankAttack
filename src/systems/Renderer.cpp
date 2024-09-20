#include "../../include/systems/Renderer.h"

/**
 * @brief Constructor de la clase Renderer.
 *
 * Inicializa el Renderer y carga las imágenes de los nodos.
 *
 * @param gridGraph Referencia al objeto GridGraph que contiene la cuadrícula a dibujar.
 */
Renderer::Renderer(GridGraph& gridGraph) : gridGraph(gridGraph), cellSize(0), xOffset(0), yOffset(0) {
    loadImages();
}

/**
 * @brief Destructor de la clase Renderer.
 *
 * Libera los recursos asociados, como las imágenes cargadas.
 */
Renderer::~Renderer() {
    cairo_surface_destroy(accessibleImage);
    cairo_surface_destroy(inaccessibleImage);
}

/**
 * @brief Carga las imágenes que representan los nodos accesibles e inaccesibles.
 *
 * Carga las imágenes desde la ruta definida para usarlas en el dibujo de la cuadrícula.
 */
void Renderer::loadImages() {
    accessibleImage = cairo_image_surface_create_from_png("../assets/textures/accessible.png");
    if (cairo_surface_status(accessibleImage) != CAIRO_STATUS_SUCCESS) {
        g_print("Error: No se pudo cargar la imagen accesible\n");
    }

    inaccessibleImage = cairo_image_surface_create_from_png("../assets/textures/inaccessible.png");
    if (cairo_surface_status(inaccessibleImage) != CAIRO_STATUS_SUCCESS) {
        g_print("Error: No se pudo cargar la imagen inaccesible\n");
    }
}

/**
 * @brief Dibuja la cuadrícula en el área de dibujo de GTK.
 *
 * Recorre los nodos en la cuadrícula y dibuja las imágenes correspondientes
 * para los nodos accesibles e inaccesibles, ajustando el tamaño de las imágenes
 * al tamaño de las celdas.
 *
 * @param widget El widget de GTK donde se va a dibujar.
 * @param cr El contexto de Cairo utilizado para dibujar.
 */
void Renderer::draw(GtkWidget *widget, cairo_t *cr) {
    GtkAllocation allocation;
    gtk_widget_get_allocation(widget, &allocation);

    calculateDimensions(allocation.width, allocation.height);  // Calcular el tamaño de las celdas

    for (int row = 0; row < gridGraph.getRows(); ++row) {
        for (int col = 0; col < gridGraph.getCols(); ++col) {
            // Obtener el nodo específico
            const Node& node = gridGraph.getNode(row, col);

            // Seleccionar la imagen a dibujar
            cairo_surface_t *image = node.accessible ? accessibleImage : inaccessibleImage;

            // Calcular la escala en función del tamaño de la celda
            double imgWidth = cairo_image_surface_get_width(image);
            double imgHeight = cairo_image_surface_get_height(image);

            // Guardar el estado actual del contexto
            cairo_save(cr);

            // Ajustar la escala para que la imagen se ajuste al tamaño de la celda
            cairo_translate(cr, xOffset + col * cellSize, yOffset + row * cellSize);
            cairo_scale(cr, cellSize / imgWidth, cellSize / imgHeight);

            // Dibujar la imagen escalada
            cairo_set_source_surface(cr, image, 0, 0);
            cairo_paint(cr);

            // Restaurar el estado original del contexto
            cairo_restore(cr);

            // Dibujar bordes
            cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);  // Negro para los bordes
            cairo_rectangle(cr, xOffset + col * cellSize, yOffset + row * cellSize, cellSize, cellSize);
            cairo_stroke(cr);
        }
    }
}

/**
 * @brief Calcula el tamaño de las celdas y los offsets en función del tamaño de la ventana.
 *
 * Calcula el tamaño adecuado de las celdas de la cuadrícula, así como los desplazamientos
 * horizontales y verticales para centrar la cuadrícula en el área de dibujo.
 *
 * @param width Ancho del área de dibujo.
 * @param height Alto del área de dibujo.
 */
void Renderer::calculateDimensions(int width, int height) {
    int topMargin = 100;
    int bottomMargin = 200;
    cellSize = (height - topMargin - bottomMargin) / gridGraph.getRows();
    xOffset = (width - (gridGraph.getCols() * cellSize)) / 2;
    yOffset = topMargin;
}
