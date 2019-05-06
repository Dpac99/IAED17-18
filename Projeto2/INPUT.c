#include "INPUT.h"

int SCAN(unsigned long *total){
    char c=getchar();
    int i;
    *total=0;
    if(c==' ')
        return 0; /*Return 0 means buffer needs clearing*/
    if(c == '\n')
        return -1; /*Here, -1 means no need to clear buffer*/
    for(;c=='0';)
        c=getchar();
    if(c=='\n')
        return 1; /*special case where no clean, but cant be -1*/
    for(i=0; i<MAXLONG-1 && c !=' ' && c!= '\n' && c!=EOF; i++){
        if(isdigit(c)){
            *total *= 10;
            *total += c-'0';
        }
        else
            return 0;
        c=getchar();
    }
    if(c =='\n' || c==EOF)
        return -1; /*correct value for single read calls*/
    return (*total != 0); /*returning true -> keep reading*/
}
    

int getInfo(char** info){
    char buffer[INFOSIZE],a;
    int i=0;
    if((buffer[i++]=getchar())!='"')
        return -2;
    while(i<INFOSIZE && (a=getchar())!='"' && a!='\n' && a!=EOF)
        buffer[i++]=a;
    buffer[i] = a;
    if(buffer[i] == '\n' || buffer[i] == EOF)
        return -1;
    if(buffer[i] != '"')
        return 0;
    buffer[++i] = '\0';
    *info = (char*)malloc(i+1);
    strcpy(*info, buffer);
    return 1;
}
