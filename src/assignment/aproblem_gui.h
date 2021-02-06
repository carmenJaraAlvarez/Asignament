#ifndef INCLUDES_APROBLEM_GUI_H_
#define INCLUDES_APROBLEM_GUI_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"
#include "../common/logico.h"
#include "../common/cadena.h"
#include "../common/type.h"
#include "solution.h"
#include "task.h"
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
//the problem
PAproblem pap_from_gui;



  void get_data(GtkWidget *, gpointer);
  void create_aproblem_window(GtkWidget *,int );

  
  #endif /* INCLUDES_APROBLEM_GUI_H_ */
