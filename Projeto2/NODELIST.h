/*NODELIST.h*/

#ifndef NLIST
#define NLIST

#include "NODE.h"
#include "TASK.h"

/*Struct to keep multiple lists of tasks. Has a head and a tail*/

struct list
{
    link head;
    link tail;
};

Nodelist NODELISTinit();
void NODELISTadd(Nodelist list, Task task); 
/*Appended at tail*/
void NODELISTremove(Nodelist list, unsigned long id, int safe); 
/*Search and remove*/
void NODELISTpop(Nodelist list, int safe); 
/*Pops head, moves head forward*/
void NODELISTfree(Nodelist list, int safe); 
/*If signal, frees only pointers. Else, frees the nodes as well*/
link NODELISTfind(Nodelist list, unsigned long id); 
/*Finds a task with provided id. Returns NULL if none*/
void NODELISTprint(Nodelist list, int bad_path, int critical);
/*Critical to only print crtitcal path. bad_path to not print path vars*/
int NODELISTsize(Nodelist list);
/*Returns size of a list*/

#endif
