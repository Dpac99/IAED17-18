#include "IDLIST.h"

Id_node IDNODEinit(unsigned long id){
    Id_node new = (Id_node)malloc(sizeof(struct id_node));
    new->id = id;
    new->next = NULL;
    new->prev = NULL;
    return new;
}

Idlist IDLISTinit(){
    Idlist new = (Idlist)malloc(sizeof(struct idlist));
    new->head=NULL;
    new->tail=NULL;
    return new;
}

void IDLISTadd(Idlist list, unsigned long id){
    Id_node new = IDNODEinit(id);
    if(list->head == NULL){
        list->head = new;
        list->tail = new;
    }
    else{
        new->prev = list->tail;
        list->tail->next = new;
        list->tail = new;
    }
}

void IDLISTpop(Idlist list){
    Id_node temp = list->head;
    if(list->head == NULL)
        return;
    list->head = list->head->next;
    free(temp);
}

void IDLISTfree(Idlist list){ 
    /*Note: was recursive, broke stack...*/
    Id_node aux;
    while((aux=list->head)!=NULL)
        IDLISTpop(list);
    free(list);
}

int IDLISTfind(Idlist list, unsigned long id){
    Id_node it;
    if(list->head == NULL)
        return 0;
    for(it = list->head; it != NULL; it = it->next)
        if(it->id == id)
            return 1;
    return 0;
}
