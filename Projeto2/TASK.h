/*TASK.h*/

#ifndef TASK
#define TASK

typedef struct list *Nodelist;
typedef struct task *Task;

#include "NODE.h"
#include "NODELIST.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*This is the base struct. Contains ID, DURATION, DESCRIPTION
  a list of tasks that it depends on,a list of tasks
  that depende on it, as well as the early and late starts*/

struct task
{
    unsigned long id,time, early, late;
    char *info;
    Nodelist depends;
    Nodelist is_depended;
};

Task TASKnew(unsigned long id, char *info, unsigned long time);
/*Creates task*/
void TASKfree(Task task);
/*Frees task*/
int TASKcompare(Task task1, Task task2); 
/*Compares id's*/
void TASKprint(Task task, int bad_path);
/*Format: ID INFO TIME [EARLY LATE/CRITICAL]* DEPENDENCIES
 *depending on bad_path */

#endif
