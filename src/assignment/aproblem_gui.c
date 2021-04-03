/*		gproblem
 *      Author: carmen
 */

#include "aproblem_gui.h"
#include "aproblem.h"
#include "PD/a_problem_PD.h"
#include "../PD/PD_algorithm.h"
#include "../MPI/problem_MPI.h"

static void resolve_aPD(PAproblem, int);

void get_data(GtkWidget *calculate, gpointer data) {
	init_clock();
    int numt = atoi((char *)gtk_entry_get_text(GTK_ENTRY(tasks_number)));
    int numr = atoi((char *)gtk_entry_get_text(GTK_ENTRY(resources_number)));
    const gchar *text;
    text=gtk_entry_get_text(GTK_ENTRY(url));
    int num_processes=(int)data;
    if(print_all)
    {
    	printf("\naproblem_gui.c get_data()-> NUM PROCESSES: %d\n",num_processes);
    }


	Cadena cadena_url;
	strcpy(cadena_url,text);

    read_aproblem_file(&pap_from_gui, numt, numr, cadena_url);
    if(print_all)
    {
    	printf("\naproblem_gui.c get_data()-> READED file\n");
    }
    resolve_aPD(&pap_from_gui, num_processes);
    //gtk_label_set_text(GTK_LABEL(values), buffer);
    //printf("\n %s", text);

}

void resolve_aPD(PAproblem pap, int num_processes)
{
	all_finished=0;
    AproblemPD appd;
    initAProblemPD(&appd, pap);
	init_algorithmPD(&final_alg, appd);
	distribution(&final_alg);
	init_slaves=1;
	MPI_Bcast(&init_slaves,1,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Ibarrier(MPI_COMM_WORLD, &request);//the slaves join when they have finished
    if(print_all)
    {
    	printf("\naproblem_gui.c resolve_aPD()-> Resolving\n");
    }
	  init_listening(&request_petition, &request_best);
	  while(!all_finished)
	  {
		  scan_petition(&request_petition, &request_best, &request_bcast);
		  MPI_Test(&request,&all_finished, MPI_STATUS_IGNORE);//test barrier
	  }
	  finish_work();
	  //////////////////////////// Solved window
	  GtkWidget  *window_solved, *grid_solved;
	  window_solved = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	  grid_solved = gtk_grid_new();
	  gtk_container_add(GTK_CONTAINER(window_solved), grid_solved);
	  gtk_window_set_title (GTK_WINDOW (window_solved), "Solved");
	  gtk_window_set_default_size (GTK_WINDOW (window_solved), 200, 200);

	  double best_final_value=final_alg.best;
	  gchar *str = g_strdup_printf("\n Solution acum: %f \n", best_final_value);
	  value = gtk_label_new(str);
	  gtk_grid_attach(GTK_GRID(grid_solved), value, 0, 0, 1, 1);


	  for(int i=0;i<final_alg.ppd.aproblem.numTask;i++){
		  gchar *str = g_strdup_printf("%s->", (final_alg.ppd.aproblem.tasks[i].name));
		  gtk_grid_attach(GTK_GRID(grid_solved), gtk_label_new(str), 0, i+1, 1, 1);
	  }

	  gtk_widget_show_all(window_solved);
	  //////////////////////////////
	  end_clock();
	//gtk_main_quit();
	//delete_algorithmPD(&alg);

}

void create_aproblem_window(GtkWidget *window,int num_processes)
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

	    int n=num_processes;
	    if(print_all)
	    {
	    	printf("\naproblem_gui.c create_aproblem_window()-> num process: %d\n",n);
	    }
	    g_signal_connect(done, "clicked", G_CALLBACK(get_data), n);
	    gtk_grid_attach(GTK_GRID(grid), done, 0, 4, 1, 1);

	    gtk_widget_show_all(window);

}






