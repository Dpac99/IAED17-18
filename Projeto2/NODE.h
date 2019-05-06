/*NODE.h*/

#ifndef NODE
#define NODE

typedef struct node *link;

#define HASHSIZE 1023
#include "NODELIST.h"
#include "TASK.h"
#include "IDLIST.h"
#include <stdio.h>

/*Tasklist node. Has task, previous and next*/

struct node
{
    Task task;
    struct node* next;
    struct node* prev;
};


link NODEinit(Task task);
/*Creates new node with given task*/
void NODEfree(link t, int safe);
/*Frees node, task too*/
void NODEadd_depend(Task origin, Task depend);
/*Adds depend to dependencies of origin and origin to is_depended of depend*/
void NODEprint(link node, int bad_path, int critical);
/*Critical means only the criticl tasks will be printed*/

#endif
