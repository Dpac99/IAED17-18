/*IDLIST.h*/

#ifndef IDLIST
#include <stdlib.h>
#include <stdio.h>
#define IDLIST

/*Simple list of id's and nodes to store them*/

typedef struct id_node{
    unsigned long id;
    struct id_node *next;
    struct id_node *prev;
}* Id_node;

typedef struct idlist{
    Id_node head, tail;
}* Idlist;

Id_node IDNODEinit(unsigned long id);
/*Creates new ID node*/
Idlist IDLISTinit();
/*Creates new list (head and tail)*/
void IDLISTadd(Idlist list, unsigned long id);  
/*Appends at tail*/
void IDLISTpop(Idlist list);                    
/*Pops head*/
void IDLISTfree(Idlist list);
/*Frees list*/
int IDLISTfind(Idlist list, unsigned long id);
/*Returns True(!0) or False(0) if given ID is in list or not*/

#endif

