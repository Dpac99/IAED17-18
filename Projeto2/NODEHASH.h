/*NODEHASH.H*/

#ifndef HASH
#define HASH

#include <stdlib.h>
#include "NODELIST.h"

/*Hashtable, hashed by id. Default hash size is 1023;
  External chaining method chosen to deal with collisions;
  Also contains bad_path, to know if path is clean or not;
  Also contains total, aka project duration*/

typedef struct nodehash
{
    Nodelist *heads;
    int size;
    int bad_path;
    unsigned long total;
} *Nodehash;

int NODEHASH(unsigned long id, Nodehash hash);
/*Hash function*/
Nodehash NODEHASHinit(int size);
/*Default: bad_path = 1, total = 0, heads point NULL*/
void NODEHASHadd(Task task, Nodehash hash);
/*Appends at head of chain*/
void NODEHASHremove(unsigned long id, Nodehash hash);
/*Search and remove by id*/
void NODEHASHfree(Nodehash hash);
/*Frees everything in hash */
link NODEHASHfind(unsigned long id, Nodehash hash); 
/*Returns NULL if no task with given id is in hash*/

#endif
