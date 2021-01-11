/*
 ============================================================================
 Name        : c.c
 Author      : cj

 ============================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

#include "common/logico.h"
#include "assignment/aproblem.h"
#include "../tests/test_resource.c"
#include "../tests/test_task.c"
#include "../tests/test_aproblem.c"
#include "../tests/test_aproblemPD.c"
#include "../tests/test_aPD_algorithm.c"
#include <gtk/gtk.h>
#include <glib.h>

//values
static GtkWidget *tasks_number;
static GtkWidget *resources_number;
static GtkWidget *url;
//tags
static GtkWidget *resources;
static GtkWidget *tasks;
static GtkWidget *values;
static GtkWidget *url_file;

void get_data(GtkWidget *calculate, gpointer data) {
    int numt = atoi((char *)gtk_entry_get_text(GTK_ENTRY(tasks_number)));
    int numr = atoi((char *)gtk_entry_get_text(GTK_ENTRY(resources_number)));
    const gchar *text;
    text=gtk_entry_get_text(GTK_ENTRY(url));


    char buffer[20];
    printf("Num values: %d", numt * numr);

    //gtk_label_set_text(GTK_LABEL(values), buffer);
    printf("\n %s", text);
}


int main(int argc, char **argv) {
    GtkWidget *window, *grid, *done;
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    resources = gtk_label_new("Num Resources");
    gtk_grid_attach(GTK_GRID(grid), resources, 1, 0, 1, 1);
    tasks = gtk_label_new("Num Tasks");
    gtk_grid_attach(GTK_GRID(grid), tasks, 0, 0, 1, 1);

    tasks_number = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), tasks_number, 0, 1, 1, 1);

    resources_number = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), resources_number, 1, 1, 1, 1);

    url_file = gtk_label_new("URL");
    gtk_grid_attach(GTK_GRID(grid), url_file, 0, 2, 1, 1);

    url = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), url, 0, 3, 2, 1);

    done = gtk_button_new_with_label("Done");
    g_signal_connect(done, "clicked", G_CALLBACK(get_data), NULL);
    gtk_grid_attach(GTK_GRID(grid), done, 0, 4, 1, 1);



    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
//int main(int argc, char* argv[]){
//	/*tests*//////////////
////	testInit1();
////	testInit2();
////	testInit3();
////	testInit4();
////	//aproblem
////	testInit5();
////	testInit6();
//////	testInit6b();
////
////	testInit7();
////	testInit8();
//	testInit9();
//
//	//num nodes
///*	int totalProcess=2;
//	//gui
//	//get data
//	int N=3;
//	int M=3;
//	double vector[]={1.,2.,3.,0.,0.,1.,9.,8.,7.};
//	//dinamic size
//	//vector de vectores. Cada fila se gestiona con puntero y le matriz es un array de esos punteros
//	//dinamic_matrix[0]=puntero al vector de enteros de la primera fila
//
//	double **dinamic_matrix;
//	dinamic_matrix=malloc(M*sizeof(double *));
//	for (int i=0; i<M;i++){
//		dinamic_matrix[i]=malloc(N*sizeof(double *));
//	}
//	/////
//	int k=0;
//	for (int i=0;i<M;i++){
//		for(int j=0;j<N;j++){
//			double aux=vector[k];
//			dinamic_matrix[i][j]=aux;
//			k++;
//		}
//
//	}
//
//	//crea problema inicio
//
//	Gproblem gp;
//	initGProblem(&gp, M, N, dinamic_matrix);
//	showGproblem(gp);*/
//
//	return 0;
//}
