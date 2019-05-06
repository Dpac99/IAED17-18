/*Diogo Pacheco, n 89433*/

#include "Description.h"
#include <stdio.h>

/* End of compiler instructions*/

Matrix sparse; /*THE MATRIX*/

int main(int argc, char *argv[])
{   
    int clear=1;
    char c;
    if(argc == 2)
    {
        sscanf(argv[1], "%80s", sparse.file_name);
        readfile();
        sparse.in_file = 1;
    }
    while ((c=getchar())!='q'){
        switch (c)
        {
            case 'a':   
                add_entry(); 
                break;
            case 'p':   
                list_sparse();
                break;
            case 'i':
                matrix_properties();
                break;
            case 'l':
                list(L);
                break;
            case 'c':
                list(C);
                break;
            case 'o':
                clear = sort();
                break;
            case 'z':
                new_zero();
                break;
            case 's':
                compress();
                break;
            case 'w':
                savefile();
                break;
            case 'x':
                under_diagonal();
                break;
            case 'y':
                one_value();
                break;
            case '\n':
                clear = 0;
                break;
            default:
                break;
        }
        if(clear)
            clearbuffer();
        clear = 1;
    }
    return 0;
}

/*----------a command----------*/

/* Adds an element to the matrix. 
   If the value is Zero, nothing happens.
   If the coordinates already exist, replaces the value or
   Deletes the entry if the value is Zero. */

void add_entry() /*Uses: pullback, reboot_limits*/
{
    int i;
    Entry a ;
    scanf("%lu %lu %lf",&a.line, &a.col, &a.value);
    for (i=0; i<sparse.last_pos; i++) 
        if (ENTRY(sparse,i).line == a.line && ENTRY(sparse,i).col == a.col)
        {
            if (a.value == (sparse.zero))
            {
                if (a.line == MAX(L,sparse) ||
                        a.line == MIN(L,sparse) ||
                        a.col == MIN(C,sparse) ||
                        a.col == MAX(C,sparse))
                    sparse.limits_changed = 1;
                pullback(i); /* deletes entry */
            }
            else
                ENTRY(sparse,i).value = a.value;
            return;
        }
    if(a.value == sparse.zero){return;}
    else{sparse.limits_changed = 1;}
    ENTRY(sparse,sparse.last_pos++) = a;
    if(sparse.last_pos == 1)
        reboot_limits();
}

/*----------p command----------*/

/*Lists all elements in the order they where introduced.
  Order changes after 'o' command.
  Does not list value=Zero elements. */

void list_sparse() 
{
    int i;
    if (sparse.last_pos==0){printf("empty matrix\n");return;} 
    for (i=0; i<sparse.last_pos; i++)
        printf("[%lu;%lu]=%.3f\n",LINECOL(L,ENTRY(sparse,i)), 
                LINECOL(C,ENTRY(sparse,i)),ENTRY(sparse,i).value);
}

/*----------i command----------*/

/*Prints matrix properties.
  Min line, min Column
  Max line, Max Column
  Number of non-Zero elements
  Number of elements
  Density*/

void matrix_properties() /*Uses: reboot_limits, density*/
{
    float percent;
    if(sparse.last_pos == 0){printf("empty matrix\n");return;}
    if (sparse.limits_changed ==1)
        reboot_limits();
    percent = density();
    printf("[%lu %lu] [%lu %lu] %d / %lu = %.3f%%\n", MIN(L,sparse), 
            MIN(C,sparse), MAX(L,sparse), MAX(C,sparse), 
            sparse.last_pos, AREA(sparse), percent);

}


/*----------c/l command----------*/

/*takes an int C(1) or L(0)
  Lists accordingly*/

void list(int crit) /* Uses: reboot_limits, distcount*/
{
    if(sparse.limits_changed)
        reboot_limits();
    unsigned long num,j,i,k;
    Entry aux[LENGTH(!crit,sparse)]; 
    scanf("%lu", &num);
    if (num < MIN(crit,sparse)  || num > MAX(crit,sparse))
    {
        crit? printf("empty column\n") : printf("empty line\n");
        return;
    }
    j=fetch(aux,num,crit);
    if(!j)
    {
        crit? printf("empty column\n"): printf("empty line\n");
        return;
    }
    distcount(aux, 0, j-1, !crit);
    for(i=MIN(!crit,sparse),k=0; i<=MAX(!crit,sparse); i++)
    {
        if(crit)
            if( i == aux[k].line && k<j)
            {
                printf("[%lu;%lu]=%.3f\n",aux[k].line,aux[k].col,aux[k].value);
                k++;
            }
            else
                printf("[%lu;%lu]=%.3f\n",i,num,sparse.zero);
        else
            if( i == aux[k].col && k<j)
                printf(" %.3f", aux[k++].value);
            else
                printf(" %.3f", sparse.zero);
    }
    if(!crit){printf("\n");}
}

/*----------o command----------*/

/*Sorts the array. If 'column' is writen, sorts by column and,
  inside of each column, by line. Otherwise, sorts in the 
  opposite way.*/

int sort () /*Uses: reboot_limits, distcount (x2). Returns 0 or 1*/
/*Radix sort LSD*/
{
    char c;
    int crit;
    if(sparse.limits_changed)
        reboot_limits();
    c=getchar();
    crit = (c!='\n' && c!=EOF);
    distcount(sparse.s,0, sparse.last_pos-1, !crit);
    distcount(sparse.s,0, sparse.last_pos-1, crit);
    return crit;
}

/*----------z command----------*/

/* Sets the value given as the new Zero.
   Deletes all Entries with that value*/

void new_zero() /*Uses: clean_zeros*/
{
    scanf("%lf", &sparse.zero);
    clean_zeros();
}

/*----------s command----------*/

/*Compresses the matrix. Prints a line with the values, 
  a line with indexes and
  a line with offsets*/ 

void compress() /*Uses: reboot_limits, density, fill_control, try_place*/
{
    if(sparse.limits_changed)
        reboot_limits();
    unsigned long i;
    unsigned long control[LENGTH(L,sparse)][2];
    unsigned long lines[2*sparse.last_pos];
    double values[2*sparse.last_pos];
    int off, maxoff=0; 
    int offset[LENGTH(L,sparse)];  
    double percent = density();
    if (percent>=50.0){printf("dense matrix\n");return;}
    for(i=0; i<2*sparse.last_pos; i++)
    {
        lines[i]=0;
        values[i]=sparse.zero;
    }
    fill_control(control); /*Fills the control matrix, sorted backwards by 
                             density and then forwards by line number*/
    for (i=0;i<LENGTH(L,sparse);i++)
    {
        off = try_place(control[i][0], values, lines); /*tries to place a line. 
                                                         Efectively does it 
                                                         once a good offset is found*/
        if (off>maxoff)
            maxoff = off;
        offset[control[i][0]-MIN(L,sparse)] = off;
    }
    printf("value =");
    for(i=0; i<LENGTH(C,sparse)+maxoff;i++)
        printf(" %.3lf", values[i]);
    printf("\nindex =");
    for(i=0; i<LENGTH(C,sparse)+maxoff; i++)
        printf(" %lu", lines[i]);
    printf("\noffset =");
    for(i=0; i<LENGTH(L,sparse); i++)
        printf(" %d", offset[i]);
    printf("\n");
}

/*----------w command----------*/

/* Saves matrix status. Needs a name of file to be passed
   unless this program is being run with a file as argument
   or a file has already been specified. In these cases, 'w' will
   save to specified file.*/

void savefile()
{
    int i;
    FILE *fp;
    if(!sparse.in_file)
    {
        scanf("%s",sparse.file_name);
        sparse.in_file = 1;
    }
    fp=fopen(sparse.file_name, "w+");
    for(i=0; i<sparse.last_pos; i++)
        fprintf(fp, "\n%lu %lu %lf", LINECOL(L,ENTRY(sparse,i)), 
                LINECOL(C,ENTRY(sparse,i)), ENTRY(sparse,i).value);
    fclose(fp);
} 

/*----------x command-------------*/

/*Prints all entries that are under the diagonal. Meaning, prints
  all entries whose line is greater than the column*/

void under_diagonal(){
    int i;
    if(sparse.limits_changed)
        reboot_limits();
    for(i=0; i<sparse.last_pos; i++)
        if((LINECOL(L,ENTRY(sparse,i))-MIN(L,sparse))>
            (LINECOL(C,ENTRY(sparse,i))- MIN(C,sparse)))
            printf("[%lu;%lu]=%.3f\n",LINECOL(L,ENTRY(sparse,i)),
                    LINECOL(C,ENTRY(sparse,i)),sparse.s[i].value);
}




/*----------y command-------------------*/

/*Prints all lines (numbers) that have at least one entry*/

void one_value(){
    int lines[MAXENTRY]={0},i;
    if(sparse.limits_changed)
        reboot_limits();
    for(i=0; i<sparse.last_pos; i++){
        unsigned long lines_pos;
        lines_pos = LINECOL(L,ENTRY(sparse,i)) - MIN(L,sparse);
        if(lines[lines_pos] != 1)
            lines[lines_pos] = 1;
    }
    for(i=0; i<LENGTH(L,sparse); i++)
        if(lines[i])
            printf("%lu\n", i+MIN(L,sparse));
}

/*----------Auxiliary functions----------*/

void clean_zeros()
/*Removes al zero elements from matrix*/
{
    int i,j=0;
    for(i=0; i<sparse.last_pos;i++)
        if(ENTRY(sparse,i).value != sparse.zero)
            ENTRY(sparse,j++)=ENTRY(sparse,i);
    sparse.last_pos -= i-j;
}

void pullback(int start) 
/*deletes an element from an array
  by pulling the next element on top 
  and so on*/
{
    int i;
    for (i=start; i<sparse.last_pos-1; i++)
        ENTRY(sparse,i)=ENTRY(sparse,i+1); 
    sparse.last_pos--;
}

void clearbuffer()
{
    char c;
    do{
        c=getchar();
    }while(c != '\n' && c != EOF);
}

void change_limits(Entry a) 
/*takes an Entry and sees if it changes the limits*/
{
    if(a.col > MAX(C,sparse))
        sparse.maxCol = a.col;
    else if (a.col < MIN(C,sparse))
        sparse.minCol = a.col;
    if(a.line > MAX(L,sparse))
        sparse.maxLine = a.line;
    else if(a.line < MIN(L,sparse))
        sparse.minLine = a.line;
}

void reboot_limits() /*Uses: change_limits*/
/*Recalculates limits */
{
    int i;
    sparse.minLine = LINECOL(L,ENTRY(sparse,0));
    sparse.maxLine = LINECOL(L,ENTRY(sparse,0));
    sparse.minCol = LINECOL(C,ENTRY(sparse,0));
    sparse.maxCol = LINECOL(C,ENTRY(sparse,0));
    for (i=0; i< sparse.last_pos; i++)
        change_limits(sparse.s[i]);
}

void readfile()/*Uses: reboot_limits*/
/*Opens and reads a file. 
  Format is:
  First line is the zero.
  All subsequent lines are Entries.
  Resets limits*/
{
    FILE *fp;
    Entry a;
    fp=fopen(sparse.file_name, "r");
    while(!feof(fp))
    {
        fscanf(fp,"%lu %lu %lf", &a.line,&a.col,&a.value);
        if(a.value!=sparse.zero)
            ENTRY(sparse,sparse.last_pos++) = a;
    }
    reboot_limits();
    fclose(fp);
}

int try_place(unsigned long line, double values[],unsigned long lines[]) /*Uses: fetch. Returns Offset*/
/*Tries to place a line. If failed, increases offset and tries again. Once a correct offset is found,
drops the line onto the arrays*/
{
    unsigned long i, col;
    int offset=0;
    unsigned long dist = LENGTH(C,sparse);
    Entry curr_line[dist];
    unsigned long last = fetch(curr_line, line, L);
    for (i=0; i<last;) /*Loops for every entry of the line. Resets and increases offset once failed.*/
    {
        col = curr_line[i].col;
        if( values[col+offset-MIN(C,sparse)] != sparse.zero)
        {
            offset++;
            i=0;
        }
        else
            i++;
    }
    for(i=0; i<last; i++) /*placement*/
    {
        col = curr_line[i].col;
        values[col+offset-MIN(C,sparse)] = curr_line[i].value;
        lines[col+offset-MIN(C,sparse)] = line;
    }
   return offset;
}

int fetch(Entry v[], unsigned long num, int crit)/*Uses: distcount. Returns number of elements*/
/*fetches a line/column*/
{
    unsigned long i, j;
    for(i=0, j=0; i<sparse.last_pos; i++)
        if(LINECOL(crit,ENTRY(sparse,i)) == num)
            v[j++] = ENTRY(sparse,i);
    distcount(v,0,j-1,L);
    return j;
}

void fill_control(unsigned long v[][2]) /*Uses: insertion,  num_density*/
/*Fills control matrix, used only on s command*/
{
    unsigned long i;
    for(i=MIN(L,sparse); i<=MAX(L,sparse); i++)
    {
        v[i-MIN(L,sparse)][0]=i;
        v[i-MIN(L,sparse)][1] = num_density(i,L);
    }
    insertion(v,0,i-MIN(L,sparse)-1);
}

int num_density(unsigned long num, int crit) 
/*Returns the density of a line or column*/
{
   int cnt=0;
   unsigned long i;
   for(i=0; i<sparse.last_pos; i++)
       if(LINECOL(crit,ENTRY(sparse,i)) == num)
           cnt++;
   return cnt;
}

double density()
/* Returns the density of the matrix*/
{
    double percent;
    percent = (sparse.last_pos/(float)AREA(sparse))*100;
    return percent;
}

/*----------Sorting algorithms----------*/


void distcount(Entry a[], int l, int r, int crit)
/* Counting sort adapted to Entries*/
{
    unsigned long dist = MAX(crit, sparse);
    unsigned long i, j, cnt[dist+1];
    Entry b[sparse.last_pos];
    if(l>r){return;}
    for(j=0; j<=dist; j++)
        cnt[j] = 0;
    for(i=l; i<=r; i++)
        cnt[LINECOL(crit,a[i]) +1]++;
    for(j=1; j<=dist; j++)
        cnt[j] += cnt[j-1];
    for(i=l; i<=r; i++)
        b[cnt[LINECOL(crit,a[i])]++] = a[i];
    for(i=l; i<=r; i++)
        a[i] = b[i-l];
}

void insertion(unsigned long a[][2],int l, int r)
/* Insertion sort adapted to unsigned longs*/
{
    int i, j;
    for(i=l+1; i<=r; i++)
    {
        unsigned long v[2];
        v[0] = a[i][0];
        v[1] = a[i][1];
        j=i-1;
        while(j>=l && v[1]>a[j][1])
        {
            a[j+1][0] = a[j][0];
            a[j+1][1] = a[j][1];
            j--;
        }
        a[j+1][0]=v[0];
        a[j+1][1]=v[1];
    }
}


