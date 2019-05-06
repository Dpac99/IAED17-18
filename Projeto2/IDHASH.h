#include "IDLIST.h"

typedef struct idhash{
    int size;       
    Idlist *heads;
} *Idhash;

int IDHASH(unsigned long id, Idhash hash);
Idhash IDHASHinit(int size);
void IDHASHadd(Idhash hash, unsigned long id);
void IDHASHfree(Idhash hash);
int IDHASHfind(Idhash hash, unsigned long id);

