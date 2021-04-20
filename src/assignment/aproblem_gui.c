/*		gproblem
 *      Author: carmen
 */

#include "aproblem_gui.h"
#include "aproblem.h"
#include "PD/a_problem_PD.h"
#include "../PD/PD_algorithm.h"
#include "../MPI/problem_MPI.h"

static void resolve_aPD(PAproblem, int);
static void show_error(GtkWidget *, gpointer );
static const gchar *myCssFile = "/home/practica/eclipse-workspace/c/src/css/mystyle.css";
int prune=1;
int redistribution_rr=1;
int tuple_p=1;


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

    int i=read_aproblem_file(&pap_from_gui, numt, numr, cadena_url);
    if(print_all)
    {
    	printf("\naproblem_gui.c get_data()-> READED file  %d\n",i);
    }
    g_print ("\naproblem_gui.c 		get_data()		prune->%d",prune);
    g_print ("\naproblem_gui.c 		get_data()		rr->%d",redistribution_rr);
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
    {
    	printf("\naproblem_gui.c resolve_aPD()-> Resolving\n");
    }

	distribution(&final_alg,prune,redistribution_rr,tuple_p);
	init_slaves=1;
    if(print_all)
    {
    	printf("\naproblem_gui.c resolve_aPD()-> Resolving\n");
    }
	MPI_Bcast(&init_slaves,1,MPI_INT,0,MPI_COMM_WORLD);
    if(print_all)
    {
    	printf("\naproblem_gui.c resolve_aPD()-> Resolving\n");
    }
	MPI_Ibarrier(MPI_COMM_WORLD, &request);//the slaves join when they have finished

	  init_listening(&request_petition, &request_best);
	  while(!all_finished)
	  {
		  scan_petition(&request_petition, &request_best, &request_bcast);
		  MPI_Test(&request,&all_finished, MPI_STATUS_IGNORE);//test barrier
	  }
	  if(print_all)
	  {
		  printf("\naproblem_gui.c		resolve_aPD()	all finished ibarrier");
	  }
	  finish_work();
	  //////////////////////////// Solved window
	  GtkWidget  *window_solved, *grid_solved;
	  window_solved = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	    /* Sets the border width of the window. */
	  gtk_container_set_border_width (GTK_CONTAINER (window_solved), 20);
	  grid_solved = gtk_grid_new();
	  gtk_container_add(GTK_CONTAINER(window_solved), grid_solved);
	  gtk_window_set_title (GTK_WINDOW (window_solved), "Solved");
	  gtk_window_set_default_size (GTK_WINDOW (window_solved), 200, 200);

	  table = gtk_grid_new();
	  gtk_widget_set_hexpand  (GTK_WIDGET (table),          TRUE);
	  gtk_container_set_border_width (GTK_CONTAINER (table), 10);
	  g_object_set (table, "baseline-row", 1, NULL);
	  gtk_widget_set_name(table, "mytable");

	  GdkColor color;
	  gdk_color_parse ("yellow", &color);

	    for(int i=0;i<final_alg.ppd.aproblem.numTask;i++)
	    {
		    for(int j=0;j<final_alg.ppd.aproblem.numResources;j++)
		    {
		    	 double value_in_table=final_alg.ppd.aproblem.values[j*final_alg.ppd.aproblem.numTask+i];
		    	 gchar *str = g_strdup_printf("%.2f", value_in_table);
		    	 value = gtk_label_new(str);
		         g_object_set (value, "margin", 5, NULL);
		         if(j==final_sol[i])//TODO
		         {
		        	 gtk_widget_modify_bg ( GTK_WIDGET(value), GTK_STATE_NORMAL, &color);
		         }
		    	 gtk_grid_attach(GTK_GRID(table), value, i, j, 1, 1);
		    }
	    }
	  gtk_grid_attach(GTK_GRID(grid_solved), table, 0, 1, 1, 1);

	  double best_final_value=final_alg.best;
	  gchar *str = g_strdup_printf("\n Solution acum: %.2f \n", best_final_value);
	  value = gtk_label_new(str);
	  gtk_grid_attach(GTK_GRID(grid_solved), value, 0, 0, 1, 1);


	  for(int i=0;i<final_alg.ppd.aproblem.numTask;i++){
		  gchar *str = g_strdup_printf("%s->%s", (final_alg.ppd.aproblem.tasks[i].name),(final_alg.ppd.aproblem.resources[final_sol[i]].name));
		  gtk_grid_attach(GTK_GRID(grid_solved), gtk_label_new(str), 0, i+2, 1, 1);
	  }

	  GtkCssProvider *    cssProvider     = gtk_css_provider_new();

	  if( gtk_css_provider_load_from_path(cssProvider, myCssFile, NULL) )
		{
			 gtk_style_context_add_provider(gtk_widget_get_style_context(grid_solved),
												GTK_STYLE_PROVIDER(cssProvider),
												GTK_STYLE_PROVIDER_PRIORITY_USER);
			 gtk_style_context_add_provider(gtk_widget_get_style_context(table),
												GTK_STYLE_PROVIDER(cssProvider),
												GTK_STYLE_PROVIDER_PRIORITY_USER);
		}
	  gtk_widget_show_all(window_solved);

	  end_clock();

	//delete_algorithmPD(&alg);

}
void button_toggled_cb (GtkWidget *button, gpointer   user_data)
{
  char *b_state;
  const char *button_label;

  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (button)))
  {
	  b_state = "on";
  }
  else
  {
          b_state = "off";
          g_print ("\n");
  }
  button_label = gtk_button_get_label (GTK_BUTTON (button));
  if(strcmp(button_label,"Prune") && strcmp(b_state,"on"))
  {
	  prune=1;
	  g_print ("\naproblem_gui.c		button_toggled_cb()		prune");
  }
  else if(strcmp(button_label,"Prune") && strcmp(b_state,"off"))
  {
	  prune=0;
	  g_print ("\naproblem_gui.c		button_toggled_cb()		NO prune");
  }

   g_print ("\n%s was turned %s\n", button_label, b_state);
   g_print ("\naproblem_gui.c 		button_toggled_cb ()		prune->%d",prune);
 }
void button_toggled_rr (GtkWidget *button, gpointer   user_data)
{
  char *b_state;
  const char *button_label;

  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (button)))
  {
	  b_state = "on";
  }
  else
  {
          b_state = "off";
          g_print ("\n");
  }
  button_label = gtk_button_get_label (GTK_BUTTON (button));
  if(strcmp(button_label,"Redistribution Round Robin") && strcmp(b_state,"on"))
  {
	  redistribution_rr=1;
	  g_print ("\naproblem_gui.c		button_toggled_cb()		rr");
  }
  else if(strcmp(button_label,"Redistribution Round Robin") && strcmp(b_state,"off"))
  {
	  redistribution_rr=0;
	  g_print ("\naproblem_gui.c		button_toggled_cb()		NO rr");
  }

   g_print ("\n%s was turned %s\n", button_label, b_state);
   g_print ("\naproblem_gui.c 		button_toggled_cb ()		rr->%d", redistribution_rr);
 }
void button_toggled_tp (GtkWidget *button, gpointer   user_data)
{
  char *b_state;
  const char *button_label;

  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (button)))
  {
	  b_state = "on";
  }
  else
  {
          b_state = "off";
          g_print ("\n");
  }
  button_label = gtk_button_get_label (GTK_BUTTON (button));
  if(strcmp(button_label,"Tuple prune") && strcmp(b_state,"on"))
  {
	  tuple_p=1;
	  g_print ("\naproblem_gui.c		button_toggled_tp()		tp");
  }
  else if(strcmp(button_label,"Tuple prune") && strcmp(b_state,"off"))
  {
	  tuple_p=0;
	  g_print ("\naproblem_gui.c		button_toggled_tp()		NO tp");
  }

   g_print ("\n%s was turned %s\n", button_label, b_state);
   g_print ("\naproblem_gui.c 		button_toggled_tp ()		tp->%d", tuple_p);
 }

void create_aproblem_window(GtkWidget *window,int num_processes)
{

	g_print ("\naproblem_gui.c 		create_aproblem_window()		prune->%d",prune);
	g_print ("\naproblem_gui.c 		create_aproblem_window()		rr->%d",redistribution_rr);



		GtkWidget *grid, *done;
		GtkWidget *radio_prune,*radio_no_prune,*radio_rr, *radio_no_rr, *radio_tp,*radio_no_tp;

	    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	    ////////////////////////////////
	    GtkWidget *error;
	    error = gtk_button_new_with_label("Error");
		g_signal_connect(G_OBJECT(error), "clicked",
		        G_CALLBACK(show_error), (gpointer) window);
		/////////////////////////////////7777
	    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	    /* Sets the border width of the window. */
	    gtk_container_set_border_width (GTK_CONTAINER (window), 20);
	    grid = gtk_grid_new();

	    gtk_widget_set_name(grid, "first_grid");
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

	    /*Create an initial radio button*/
	     radio_prune = gtk_radio_button_new_with_label (NULL, "Prune");
	     /*Create a second radio button, and add it to the same group as Button 1*/
	     radio_no_prune = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio_prune), "No prune");
	     gtk_grid_attach (GTK_GRID (grid), radio_prune, 0, 5, 1, 1);
	     gtk_grid_attach (GTK_GRID (grid), radio_no_prune, 1, 5, 1, 1);
	     /*set the initial state of each button*/
	     gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radio_prune), TRUE);
	     gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radio_no_prune), FALSE);
	     /*Connect the signal handlers (aka Callback functions) to the buttons*/
	     g_signal_connect (GTK_TOGGLE_BUTTON (radio_prune), "toggled",
	                       G_CALLBACK (button_toggled_cb), window);
	     g_signal_connect (GTK_TOGGLE_BUTTON (radio_no_prune), "toggled",
	                       G_CALLBACK (button_toggled_cb), window);


	     /*Create an initial radio button*/
		 radio_rr = gtk_radio_button_new_with_label (NULL, "Redistribution Round Robin");
		 /*Create a second radio button, and add it to the same group as Button 1*/
		 radio_no_rr = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio_rr), "No redistribution Round Robin");
		 gtk_grid_attach (GTK_GRID (grid), radio_rr, 0, 6, 1, 1);
		 gtk_grid_attach (GTK_GRID (grid), radio_no_rr, 1, 6, 1, 1);
		 /*set the initial state of each button*/
		 gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radio_rr), TRUE);
		 gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radio_no_rr), FALSE);
		 /*Connect the signal handlers (aka Callback functions) to the buttons*/
		 g_signal_connect (GTK_TOGGLE_BUTTON (radio_rr), "toggled",
						   G_CALLBACK (button_toggled_rr), window);
		 g_signal_connect (GTK_TOGGLE_BUTTON (radio_no_rr), "toggled",
						   G_CALLBACK (button_toggled_rr), window);

	     /*Create an initial radio button*/
		 radio_tp = gtk_radio_button_new_with_label (NULL, "Tuple prune");
		 /*Create a second radio button, and add it to the same group as Button 1*/
		 radio_no_tp = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio_tp), "No tuple prune");
		 gtk_grid_attach (GTK_GRID (grid), radio_tp, 0, 7, 1, 1);
		 gtk_grid_attach (GTK_GRID (grid), radio_no_tp, 1, 7, 1, 1);
		 gtk_grid_attach (GTK_GRID(grid), error, 1, 9, 1, 1);
		 /*set the initial state of each button*/
		 gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radio_rr), TRUE);
		 gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radio_no_rr), FALSE);
		 /*Connect the signal handlers (aka Callback functions) to the buttons*/
		 g_signal_connect (GTK_TOGGLE_BUTTON (radio_tp), "toggled",
						   G_CALLBACK (button_toggled_tp), window);
		 g_signal_connect (GTK_TOGGLE_BUTTON (radio_no_tp), "toggled",
						   G_CALLBACK (button_toggled_tp), window);


	    done = gtk_button_new_with_label("Done");

	    int n=num_processes;
	    if(print_all)
	    {
	    	printf("\naproblem_gui.c create_aproblem_window()-> num process: %d\n",n);
	    }
	    g_signal_connect(done, "clicked", G_CALLBACK(get_data), n);
	    gtk_grid_attach(GTK_GRID(grid), done, 0, 8, 1, 1);

	    /////////////////////// CSS

	    GtkCssProvider *    cssProvider     = gtk_css_provider_new();
//	    const gchar *myCssFile = "/home/practica/eclipse-workspace/c/src/css/mystyle.css";
		    if( gtk_css_provider_load_from_path(cssProvider, myCssFile, NULL) )
	    {
	         gtk_style_context_add_provider(gtk_widget_get_style_context(window),
	                                            GTK_STYLE_PROVIDER(cssProvider),
	                                            GTK_STYLE_PROVIDER_PRIORITY_USER);
	    }

	    gtk_widget_show_all(window);

}

void show_error(GtkWidget *widget, gpointer window) {

  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Error loading file");
  gtk_window_set_title(GTK_WINDOW(dialog), "Error");
//  const gchar *myCssFile = "/home/practica/eclipse-workspace/c/src/css/mystyle.css";
  	  	  GtkCssProvider *    cssProvider     = gtk_css_provider_new();
  		    if( gtk_css_provider_load_from_path(cssProvider, myCssFile, NULL) )
  	    {
  	         gtk_style_context_add_provider(gtk_widget_get_style_context(dialog),
  	                                            GTK_STYLE_PROVIDER(cssProvider),
  	                                            GTK_STYLE_PROVIDER_PRIORITY_USER);
  	    }
  gtk_widget_set_name(dialog, "mydialog");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}


