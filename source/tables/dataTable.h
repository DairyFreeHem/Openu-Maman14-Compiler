

#ifndef directiveSize
#define directiveSize
#define stringSize 7
#define structSize 7
#define dataSize 5
#define MAX_STR_PARAMS 1
#define MAX_STRUCT_PARAMS 2
#define MIN_PARAMS 1
#endif



typedef struct dataList{
    int type;
    int *values;
    int addressInDec; /*Location in memory, in decimal*/
    int DC; /*number of memory positions occupied, in decimal*/
    int parameters; /*Number of parameters*/
    struct dataList *next;
} dataList;



int addStruct(char *line,int *memloc,int index, int *DC);  /*Input: a string which represents a struct directive, an integer from, a pointer to DC and a pointer to the head of the dataList
                                                                         *Output: if successful in adding the struct, it returns the index after all parameters. else, it returns -2 if there was a dynamic allocation error and -1 otherwise
                                                                         *Action: First it adds the number to the dataList, and then it adds each letter of the string to the dataList, including '\0'*/

int addData(char *line,int *memloc,int index, int *DC);     /*Input: Input: a string which represents a data directive, an integer from, a pointer to DC and a pointer to the head of the dataList
                                                                         *Output: if all the numbers have been added successfully, than the function returns the index after all the numbers, else return -2 if there was a dynamic allocation error and -1 otherwise.
                                                                         * Action: adds all the numbers provided in the .data directive to the dataList*/

/*Clear all nodes from data list*/
void clearDataList(void);                     
                                                                    

/*Adds .string value to data table,
 *each character being represented by their ASCII equivilent*/

int addString(char *line,int index,int *memloc,int *DC);

/*Get data table by memloc value
 *Returns NULL if no dada was found by that memory location*/

dataList *getDataByLoc(int addressInDec);


/*Gets head of data list*/
dataList *getDataList(void);