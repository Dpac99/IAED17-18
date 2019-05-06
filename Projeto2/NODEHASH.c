/*NODEHASH.c*/

#include "NODEHASH.h"

int NODEHASH(unsigned long id, Nodehash hash)
{
    return (id%hash->size);
}

Nodehash NODEHASHinit(int size)
{
    Nodehash new = (Nodehash)malloc(sizeof(struct nodehash));
    int i;
    new->size = size;
    new->bad_path = 1;
    new->heads = (Nodelist*)malloc(size*sizeof(Nodelist));
    new->total = 0;
    for(i=0; i<size; i++)
        new->heads[i] = NODELISTinit();
    return new;
}

void NODEHASHadd(Task task, Nodehash hash)
{
    int i = NODEHASH(task->id, hash);
    NODELISTadd(hash->heads[i], task);
    return;
}

void NODEHASHremove(unsigned long id, Nodehash hash)
{
    int i = NODEHASH(id, hash);
    NODELISTremove(hash->heads[i], id,0);
    return;
}

void NODEHASHfree(Nodehash hash)
{
    int i;
    for(i=0; i<hash->size; i++)
        NODELISTfree(hash->heads[i],1);
    free(hash->heads);
    free(hash);
}

link NODEHASHfind(unsigned long id, Nodehash hash)
{
    int i = NODEHASH(id, hash);
    if(hash->heads[i]->head == NULL)
        return NULL;
    return NODELISTfind(hash->heads[i], id);
}

