#include <gtk/gtk.h>

GtkWidget      *window_main;
GtkWidget      *btn_quit;
GtkBuilder     *builder;

int 
main(int argc, char *argv[]) 
{ 
    gtk_init(&argc, &argv); 

    builder = gtk_builder_new_from_file("glade/window_main.glade");
    
    window_main = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    btn_quit = GTK_WIDGET(gtk_builder_get_object(builder, "btn_quit"));

    g_signal_connect(btn_quit, "activate", G_CALLBACK(gtk_main_quit), NULL);

    gtk_builder_connect_signals(builder, NULL);

    g_object_unref(builder);

    gtk_widget_show(window_main);

    gtk_main();

    return EXIT_SUCCESS;
}

