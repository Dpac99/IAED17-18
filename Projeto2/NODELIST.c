/*NODELIST.c*/

#include "NODELIST.h"

Nodelist NODELISTinit(){
    Nodelist x = (Nodelist)malloc(sizeof(struct list));
    x->head = NULL;
    x->tail = NULL;
    return x;
}

void NODELISTadd(Nodelist list, Task task){
    link node = NODEinit(task);
    if(list->head == NULL){
        list->tail = node;
        list->head = node;
    }
    else{
        node->prev = list->tail;
        list->tail->next = node;
        list->tail = node;
    }

}

void NODELISTremove(Nodelist list, unsigned long id, int safe){
    link t;
    for(t=list->tail; t!=NULL; t=t->prev){
        if(t->task->id == id){
            if(t == list->head){
                list->head = list->head->next;
                if(list->tail == t)
                    list->tail = list->tail->prev;
                NODEfree(t, safe);
                return;
            }
            else{
                t->prev->next = t->next;
                if(list->tail == t)
                    list->tail = list->tail->prev;
                else
                    t->next->prev = t->prev;
                NODEfree(t,safe);
                return;
            }
        }
    }
}

void NODELISTpop(Nodelist list, int safe){
    link aux = list->head;
    if(list->head == NULL)
        return;
    list->head = list->head->next;
    NODEfree(aux, safe);
}

void NODELISTfree(Nodelist list, int safe){
    /*Note: was recursive, broke stack...*/
    link aux;
    while((aux=list->head)!=NULL)
        NODELISTpop(list, safe);
    free(list);
}

link NODELISTfind(Nodelist list, unsigned long id){
    link aux;
    if(list->head == NULL)
        return NULL;
    for(aux = list->head; aux !=NULL; aux=aux->next)
        if(aux->task->id == id)
            return aux;
    return NULL;
}

void NODELISTprint(Nodelist list, int bad_path, int critical){
    link aux;
    for(aux=list->head; aux!=NULL; aux = aux->next)
        NODEprint(aux,bad_path,critical);
}


int NODELISTsize(Nodelist list){
    int cnt = 0;
    link aux;
    for(aux=list->head; aux!=NULL; aux=aux->next,cnt++);
    return cnt;
}

