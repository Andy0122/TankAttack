#include <gtk/gtk.h>
#include "Controller.h"
#include "Model.h"
#include "View.h"


static void activate(GtkApplication* app, gpointer user_data) {
    auto* model = static_cast<Model*>(user_data);

    GtkWidget* window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Tank Attack!");
    gtk_window_set_default_size(GTK_WINDOW(window), 1440, 900);
    gtk_widget_set_vexpand(window, TRUE);
    gtk_widget_set_hexpand(window, TRUE);

    // Create view
    auto* view = new View(window);

    // Create controller
    auto* controller = new Controller(view, model);

    gtk_widget_show_all(window);
}

int main(const int argc, char *argv[]) {
    auto* model = new Model();

    GtkApplication* app = gtk_application_new("com.example.tankAttack", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), model);
    const int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}