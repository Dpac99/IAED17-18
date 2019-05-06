/*TASK.c*/

#include "TASK.h"

Task TASKnew(unsigned long id, char* info, unsigned long time){
    Task new = (Task)malloc(sizeof(struct task));
    new->id = id;
    new->time = time;
    new->info = info;
    new->depends = NODELISTinit();
    new->is_depended = NODELISTinit();
    new->early = 0;
    new->late = 0;
    return new;
}

void TASKfree(Task task){
    if(task == NULL)
        return;
    free(task->info);
    NODELISTfree(task->depends,1);
    NODELISTfree(task->is_depended,1);
    free(task);

}
    
int TASKcompare(Task task1, Task task2){
    return (task1->id == task2->id);
}

void TASKprint(Task task, int bad_path){
    link aux;
    printf("%lu %s %lu",task->id, task->info, task->time);
    if(!bad_path){
        printf(" [%lu ", task->early);
        if(task->early == task->late)
            printf("CRITICAL]");
        else
            printf("%lu]", task->late);
    }
    for(aux=task->depends->head; aux!=NULL; aux=aux->next)
        printf(" %lu", aux->task->id); 
}
