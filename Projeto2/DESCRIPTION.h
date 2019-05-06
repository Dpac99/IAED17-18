/*DESCRIPTION.h*/

#ifndef FUNCTIONS
#define FUNCTIONS

#define HASHSIZE 1023
#define BUFSIZE 20
#define eq(A,B) (!strcmp(A,B))
#define CHUNK 20

#include "IDHASH.h"
#include "NODEHASH.h"
#include "IDLIST.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "INPUT.h"

int add_task(Nodehash hash, Nodelist list);
/*Adds a task to project, calculating early start*/
void illegal_arguments();
/*Print of illegal_arguments*/
void clearbuffer();
/*Clears buffer*/
int depend(Nodehash hash);
/*Lists tasks dependent of read task*/
int duration(Nodehash hash, Nodelist list);
/*Prints all tasks with duration > than given. Defaults to 0*/
int _remove(Nodehash hash, Nodelist list);
/*Removes task from project*/
void early_start(Task task);
/*Calculates early start for a single task*/
void late_start(Nodelist all, unsigned long total);
/*Calculates late start for every task*/
int path(Nodelist all, Nodehash hash);
/*Calculates critical path*/
unsigned long _total(Nodelist list);
/*Calculates project duration*/
int first(Nodelist list);
/*Prints id of the task that depends on the most tasks*/
int second(Nodelist list, Nodehash hash);
/*Prints id of the task with latest completion time*/

#endif
