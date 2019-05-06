/*Diogo Pacheco, n 89433*/

/*Description of program:
    This program is a sparse-matrix management shell.
    It has multiple commands that interact with the sparse.
    Allows to add elements, print all elements, or a single
    line, or a single column, allows to sort elements by line
    or column, allows to read and write in files and to compress.
    */

#define MAXENTRY 10000
#define MAXNAME 80
#define exch(A,B) {Entry T=A; A=B; B=T;}
#define ENTRY(A,B) (A.s[B]) 
#define MAX(A,B) (A? B.maxCol : B.maxLine) 
#define MIN(A,B) (A? B.minCol : B.minLine) 
#define LINECOL(A,B) (A? B.col : B.line) 
#define LENGTH(A,B) (MAX(A,B)-MIN(A,B)+1) 
#define AREA(A) (LENGTH(0,A)*LENGTH(1,A)) 
#define L 0 
#define C 1

typedef struct{
    unsigned long line,col;
    double value;
}Entry;

typedef struct{
    Entry s[MAXENTRY];
    unsigned long maxLine, minLine, maxCol, minCol;
    double zero;
    int limits_changed, in_file, last_pos;
    char file_name[MAXNAME];
}Matrix;

void add_entry();
void pullback(int start);
void list_sparse();
void matrix_properties();
void list(int crit);
void clearbuffer();
void change_limits(Entry a);
void reboot_limits();
int sort();
void new_zero();
void readfile();
void savefile();
void distcount(Entry a[], int l, int r, int crit);
void clean_zeros();
int try_place(unsigned long line, double values[], unsigned long lines[]);
int fetch(Entry v[], unsigned long line, int crit);
void fill_control(unsigned long v[][2]);
int num_density(unsigned long line,int crit);
void compress();
double density();
void insertion(unsigned long a[][2], int l, int r);
void under_diagonal();
void one_value();
