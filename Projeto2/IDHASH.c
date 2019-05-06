#include "IDHASH.h"

int IDHASH(unsigned long id, Idhash hash){
    return (id%(hash->size));
}

Idhash IDHASHinit(int size){
    Idhash new = (Idhash)malloc(sizeof(struct idhash));
    int i;
    new->size = size;
    new->heads = (Idlist*)malloc(size*sizeof(Idlist));
    for(i=0; i<size; i++)
        new->heads[i] = IDLISTinit();
    return new;
}

void IDHASHadd(Idhash hash, unsigned long id){
    int i = IDHASH(id, hash);
    IDLISTadd(hash->heads[i], id);
}

void IDHASHfree(Idhash hash){
    int i;
    for(i=0; i<hash->size; i++)
        IDLISTfree(hash->heads[i]);
    free(hash->heads);
    free(hash);
}

int IDHASHfind(Idhash hash, unsigned long id){
    int i = IDHASH(id, hash);
    if (hash->heads[i]->head == NULL)
        return 0;
    return IDLISTfind(hash->heads[i], id);
}


