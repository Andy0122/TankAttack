#include "../include/systems/GridGraph.h"
#include "../include/systems/Renderer.h"
#include <gtk/gtk.h>

// Definir una función de callback de dibujo con la firma adecuada
static void on_draw_callback(GtkWidget *widget, cairo_t *cr, gpointer data) {
    // Convertir 'data' a Renderer*
    Renderer* renderer = static_cast<Renderer*>(data);
    renderer->draw(widget, cr);
}

// Función para manejar el evento de clic
static gboolean on_click_event(GtkWidget *widget, GdkEventButton *event, gpointer data) {
    Renderer* renderer = static_cast<Renderer*>(data);

    // Obtener el ID del cuadro clickeado
    int cellSize = renderer->getCellSize();
    int xOffset = renderer->getXOffset();
    int yOffset = renderer->getYOffset();

    // Verificar si el clic está dentro de la cuadrícula
    if (event->x >= xOffset && event->y >= yOffset) {
        int col = (event->x - xOffset) / cellSize;
        int row = (event->y - yOffset) / cellSize;

        if (col >= 0 && col < renderer->getCols() && row >= 0 && row < renderer->getRows()) {
            int id = row * renderer->getCols() + col;  // ID del cuadro clickeado
            g_print("Se hizo clic en el cuadro con ID: %d\n", id);
        }
    }

    return TRUE;  // Devolver TRUE para indicar que el evento fue manejado
}

int main(int argc, char *argv[]) {
    // Inicializar GTK
    gtk_init(&argc, &argv);

    // Crear el grafo
    GridGraph gridGraph;
    gridGraph.generateObstacles(2);
    gridGraph.connectNodes();
    gridGraph.printAccessibility();

    // Crear la ventana
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Tank Attack!");
    gtk_window_set_default_size(GTK_WINDOW(window), 1440, 900);

    // Conectar la señal de destruir
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Crear el área de dibujo
    GtkWidget *drawingArea = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), drawingArea);

    // Habilitar eventos de clic en el área de dibujo
    gtk_widget_add_events(drawingArea, GDK_BUTTON_PRESS_MASK);

    // Crear el objeto Renderer para manejar el dibujo
    Renderer renderer(gridGraph);

    // Conectar la señal de dibujo
    g_signal_connect(G_OBJECT(drawingArea), "draw", G_CALLBACK(on_draw_callback), &renderer);

    // Conectar la señal de clic
    g_signal_connect(G_OBJECT(drawingArea), "button-press-event", G_CALLBACK(on_click_event), &renderer);

    // Mostrar la ventana
    gtk_widget_show_all(window);

    // Correr el loop principal de GTK
    gtk_main();

    return 0;
}