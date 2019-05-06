/*MAIN.c*/

/*Diogo Pacheco, No 89433, Alameda*/

#include "DESCRIPTION.h"

int main (){
    char buf[BUFSIZE];
    int clear  = 1;
    Nodehash nodehash = NODEHASHinit(HASHSIZE);
    Nodelist ordered = NODELISTinit();
    scanf("%s", buf);
    while (!eq(buf,"exit")){
        if(eq(buf,"add")){
            clear = add_task(nodehash, ordered);
        }
        else if(eq(buf,"depend")){
            clear = depend(nodehash);
        }
        else if(eq(buf,"duration")){
            clear = duration(nodehash, ordered);
        }
        else if(eq(buf,"remove")){
            clear = _remove(nodehash, ordered);
        }
        else if(eq(buf,"path")){
            clear = path(ordered, nodehash);
        }
        else if(eq(buf,"first")){
            clear = first(ordered);
        }
        else if(eq(buf,"second")){
            clear = second(ordered, nodehash);
        }
        else{
            illegal_arguments();
        }
        if(clear)
            clearbuffer();
        scanf("%s", buf);
        clear = 1;
    }
    NODEHASHfree(nodehash);
    NODELISTfree(ordered,0);
    return 0;
}
