/*		gproblem
 *      Author: carmen
 */

#include "aproblem_gui.h"
#include "aproblem.h"
#include "PD/a_problem_PD.h"
#include "../PD/PD_algorithm.h"

static void resolve_aPD(PAproblem);

void get_data(GtkWidget *calculate, gpointer data) {
    int numt = atoi((char *)gtk_entry_get_text(GTK_ENTRY(tasks_number)));
    int numr = atoi((char *)gtk_entry_get_text(GTK_ENTRY(resources_number)));
    const gchar *text;
    text=gtk_entry_get_text(GTK_ENTRY(url));

    Aproblem *ap=data;
    char buffer[20];

	Cadena cadena_url;
	strcpy(cadena_url,text);

    read_aproblem_file(&ap, numt, numr, cadena_url);
    //gtk_label_set_text(GTK_LABEL(values), buffer);
    //printf("\n %s", text);
    resolve_aPD(&ap);

}

void resolve_aPD(PAproblem pap)
{
    show_aproblem(pap);
	AproblemPD appd;
	testInitAProblemPD(&appd, *pap);
	AlgorithmPD alg;
	init_algorithmPD(&alg, appd);
	exec_algorithm(&alg);
	Solution sol;
	get_PDsolution(&alg, &sol);
	for(int i=0;i<alg.ppd.solution.lengthArrays;i++){
		printf("\nResources: \n*%s\n",alg.ppd.solution.resources[i].name);
	}
	printf("Solution value: ");
	printf("%f", alg.ppd.solution.acum);
	delete_algorithmPD(&alg);
}

void create_aproblem_window(GtkWidget *window)
{
	GtkWidget *grid, *done;


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

	    Aproblem ap;
	    g_signal_connect(done, "clicked", G_CALLBACK(get_data), &ap);
	    gtk_grid_attach(GTK_GRID(grid), done, 0, 4, 1, 1);

	    gtk_widget_show_all(window);

}



