#define MAXLONG 20
#define INFOSIZE 8001

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int SCAN(unsigned long *n);
/*Scans Ulongs
  Returns 0 if whitespace or nodigit or value is 0,
  Returns 0 if read is good, but line ends,
  Returns 1 if read is good, line doesnt end*/
int getInfo(char **info);
/*Scans a string with 8000 chars
  Returns -2 if it doesnt start with ",
  Returns -1 if a \n or EOF was found,
  Returns 0 if doesnt end with ",
  Returns 1 if read is sucessful;*/
