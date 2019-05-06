/*NODE.c*/

#include "NODE.h"

link NODEinit(Task task){
    link x = (link)malloc(sizeof(struct node));
    x->task = task;
    x->next = NULL;
    x->prev = NULL;
    return x;
}

void NODEfree(link t, int safe){
    if(t == NULL)
        return;
    if(!safe)
        TASKfree(t->task);
    free(t);
}
        
void NODEadd_depend(Task origin, Task depend){
    NODELISTadd(origin->depends, depend);
    NODELISTadd(depend->is_depended, origin);
}

void NODEprint(link node, int bad_path, int critical){
    if(critical){
        if(node->task->early == node->task->late){
            TASKprint(node->task, bad_path);
            printf("\n");
        }
    }
    else{
        TASKprint(node->task, bad_path);
        printf("\n");
    }
}
