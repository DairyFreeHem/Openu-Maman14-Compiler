
#define COMMANDPOS 0
#define SRC 1
#define DST 2
#define ARE 3


enum are{ABSOLUTE = 0, EXTERNAL= 1, RELOCATABLE = 2};
enum cases{NUMBERCASE = 0,LABELCASE,STRUCTCASE,REGISTERCASE};

typedef struct codeList{
    int addressInDec; /*First instance in memory, In decimal*/
    int IC; /*Number of memory positions occupied, in decimal*/
    int *bin; /*the character array represents a binary number*/
    int *paramsInDec;
    struct codeList *next;
} codeList;


/*Process code table*/
int processCode(char *line, int index, int *memloc,int *IC, int type);

/*Freeup memory taken by codeList*/
void clearCodeList(void); 

/*Get head of codeList*/
codeList *getCodeHead(void);

/*add params to code*/
int processParams(codeList *code,char *line, int index);