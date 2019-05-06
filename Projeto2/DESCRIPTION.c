/*DESCRIPTION.c*/

#include "DESCRIPTION.h"

int add_task(Nodehash hash, Nodelist tlist){
    unsigned long id, time, aux;
    int test;
    link dp;
    Task task;
    char *info;
    Idhash depends1 = IDHASHinit(HASHSIZE);
    Idlist depends2 = IDLISTinit();
    Id_node it;
    getchar(); /*Clears space*/ 
    if((test = SCAN(&id)) != 1){ /*Reads ID*/
        illegal_arguments();
        IDHASHfree(depends1);
        IDLISTfree(depends2);
        return (test==0);
    }
    if((test = getInfo(&info)) <= 0 || getchar()!=' '){ /*Reads info*/
        illegal_arguments();
        IDHASHfree(depends1);
        IDLISTfree(depends2);
        if(test != -2)
            free(info);
        return (test==0 || test == -2);
    }
    if((test = SCAN(&time)) == 0){ /*Reads time*/
        illegal_arguments();
        free(info);
        IDHASHfree(depends1);
        IDLISTfree(depends2);
        return 1;
    }
    while(test != -1){ /*Reads every dependency, stores every id in a list */
        test = SCAN(&aux);
        if(test==0) {
            illegal_arguments();
            IDHASHfree(depends1);
            IDLISTfree(depends2);
            free(info);
            return 1;
        }
        if(!IDHASHfind(depends1,aux)){
            IDLISTadd(depends2, aux);
            IDHASHadd(depends1, aux);
        }
    }
    if((NODEHASHfind(id, hash))!=NULL){ /*Sees if ID already exists*/
        printf("id already exists\n");
        IDHASHfree(depends1);
        IDLISTfree(depends2);
        free(info);
        return 0;
    }
    for(it = depends2->head; it!=NULL; it=it->next){ /*Sees if every */
        if(NODEHASHfind(it->id, hash) == NULL){     /*Dependency exists*/
            printf("no such task\n");
            free(info);
            IDHASHfree(depends1);
            IDLISTfree(depends2);
            return 0;
        }
    }
    task = TASKnew(id, info, time);
    hash->bad_path = 1;
    for(it=depends2->head; it!= NULL; it=it->next){
        dp=NODEHASHfind(it->id, hash);
        NODEadd_depend(task, dp->task);
    }
    NODEHASHadd(task,hash);
    NODELISTadd(tlist, task);
    IDHASHfree(depends1);
    IDLISTfree(depends2);
    early_start(task);
    return 0;
}

int depend(Nodehash hash){
    char c;
    unsigned long id;
    int test;
    link node, aux;
    if((c=getchar())=='\n'){ /*Checks if ID was given*/
        illegal_arguments();
        return 0;
    }
    if((test = SCAN(&id))!=-1){ /*Reads ID*/
        illegal_arguments();
        return (test==0);
    }
    if((node=NODEHASHfind(id, hash))==NULL){ /*Checks if ID exists*/
        printf("no such task\n");
        return 0;
    }
    printf("%lu:", node->task->id);
    if(node->task->is_depended->head == NULL){
        printf(" no dependencies\n");
        return 0;
    }
    for(aux=node->task->is_depended->head; aux!=NULL; aux=aux->next)
        printf(" %lu", aux->task->id);
    printf("\n");
    return 0;
}

int duration(Nodehash hash, Nodelist list){
    char c;
    unsigned long time;
    link aux;
    if((c=getchar())=='\n') /*Checks if time was given. Defaults to 0*/
        time = 0;
    else{
        if((SCAN(&time))!=-1){ /*Reads time*/
            illegal_arguments();
            return 1;
        }
    }
    for(aux=list->head; aux!=NULL; aux=aux->next)
        if(aux->task->time >= time){
            NODEprint(aux, hash->bad_path, 0);
        }
    return 0;
}

int _remove(Nodehash hash, Nodelist list){
    char c;
    unsigned long id;
    link temp1, temp2;
    int test;
    if((c=getchar())=='\n'){ /*Checks if ID was given*/
        illegal_arguments();
        return 0;
    }
    if((test = SCAN(&id)) != -1){ /*Reads ID*/
        illegal_arguments();
        return (test==0);
    }
    if((temp1 = NODEHASHfind(id, hash)) == NULL){ /*Checks if ID exists*/
            printf("no such task\n");
            return 0;
    }
    if(temp1->task->is_depended->head == NULL){
        hash->bad_path = 1;
        for(temp2 = temp1->task->depends->head;
        temp2!=NULL; 
        temp2=temp2->next){ /*removes from is_depended of every dependency*/
            NODELISTremove(temp2->task->is_depended, id, 1);
        }
        NODELISTremove(list, id, 1);
        NODEHASHremove(id, hash);
        return 0;
    }
    else{
        printf("task with dependencies\n");
        return 0;
    }
}

void early_start(Task task){
    link aux;
    unsigned long early = 0, num;
    for(aux =task->depends->head; aux != NULL; aux=aux->next)
        if((num = (aux->task->time + aux->task->early)) > early)
            early = num;
    task->early = early;
}

void late_start(Nodelist all, unsigned long total){
    link aux1, aux2;
    unsigned long temp;
    for(aux1 = all->tail; aux1!=NULL; aux1 = aux1->prev){
        if(aux1->task->is_depended->head == NULL)
            aux1->task->late = (total)-(aux1->task->time); /*First tasks*/
        else{
            temp = total;
            for(aux2=aux1->task->is_depended->head;
            aux2!=NULL;aux2=aux2->next)
                if(aux2->task->late < temp)
                    temp = aux2->task->late;
            aux1->task->late = temp - aux1->task->time;
        }
    }
}

unsigned long _total(Nodelist list){
    link aux;
    unsigned long total=0, num;
    for(aux = list->tail; aux!=NULL; aux = aux->prev){
        if(aux->task->is_depended->head == NULL)
            if((num=(aux->task->time)+(aux->task->early)) > total)
                total = num;
    }
    return total;
}


int path(Nodelist all, Nodehash hash){
    char c;
    if((c=getchar())!='\n'){ /*Checks if nothing else was written*/
        illegal_arguments();
        return 1;
    }
    if(hash->bad_path)
        hash->total = _total(all); 
    late_start(all, hash->total);
    hash->bad_path = 0;
    NODELISTprint(all, hash->bad_path,1);
    printf("project duration = %lu\n", hash->total);
    return 0;
}

    

void illegal_arguments(){
    printf("illegal arguments\n");
}

void clearbuffer(){
    char c;
    do{
        c=getchar();
    }while(c != '\n' && c != EOF);
}
    

int first(Nodelist list){
    unsigned long max = 0;
    int maxvalue = 0, size;
    link aux;
    char c = getchar();
    if(c != '\n'){
        illegal_arguments();
        return 1;
    }
    for(aux = list->head; aux!=NULL; aux=aux->next)
        if((size=NODELISTsize(aux->task->depends)) > maxvalue){
            maxvalue = size;
            max = aux->task->id;
        }
    printf("%lu\n", max);
    return 0;
}

int second(Nodelist list, Nodehash hash){
    unsigned long maxearly = 0;
    unsigned long max = 0;
    link aux;
    char c = getchar();
    if(c != '\n'){
        illegal_arguments();
        return 1;
    }
    if(hash->bad_path){
        hash->total = _total(list); 
        late_start(list, hash->total);
        hash->bad_path = 0;
    }
    for(aux = list->head; aux!=NULL; aux=aux->next)
        if(aux->task->early + aux->task->time > maxearly){
            maxearly = aux->task->late;
            max = aux->task->id;
        }
    printf("%lu\n", max);
    return 0;
}


    

