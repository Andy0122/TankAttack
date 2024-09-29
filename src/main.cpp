#include <gtk/gtk.h>
#include "Controller.h"
#include "Model.h"
#include "View.h"


static void activate(GtkApplication* app, gpointer user_data) {
    // Crear la ventana
    GtkWidget* window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Tank Attack!");
    gtk_window_set_default_size(GTK_WINDOW(window), 1440, 900);
    gtk_widget_set_vexpand(window, TRUE);
    gtk_widget_set_hexpand(window, TRUE);

    // Crear el modelo a partir de los datos pasados como user_data
    auto* model = static_cast<Model*>(user_data);

    // Crear la vista
    auto* view = new View(window);

    // Crear el controlador
    auto* controller = new Controller(view, model);

    // Mostrar la ventana con todos sus elementos
    gtk_widget_show_all(window);
}

int main(const int argc, char *argv[]) {
    // Crear el modelo
    auto* model = new Model();

    // Inicializar GtkApplication
    GtkApplication* app = gtk_application_new("com.example.tankAttack", G_APPLICATION_DEFAULT_FLAGS);

    // Conectar la señal de activación y pasar el modelo
    g_signal_connect(app, "activate", G_CALLBACK(activate), model);

    // Ejecutar la aplicación
    const int status = g_application_run(G_APPLICATION(app), argc, argv);

    // Liberar la memoria del objeto app
    g_object_unref(app);

    return status;
}