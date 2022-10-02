/*
* Write out data received from compiler to an accompanying .ob file
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"
#include "defines.h"
#include "converters.h"
#include "../tables/labelTable.h"
#include "../tables/LabelReferenceTable.h"
#include "../tables/dataTable.h"
#include "../tables/codeTable.h"


/*Pointer to the .OB file*/
FILE *output = NULL;

/*End the output of .OB*/
void endWrite(void)
{
    closeFile(&output);
}

/*Open writing to filename.ob file*/
int startWrite(char *filename)
{
    output = getFile(filename, ".ob", "w");
    
    if (output == NULL)
    {
        printf("ERROR: Could not open %s.ob file", filename);
        return MEMORY_ERROR;
    }

    return ALL_CLEAR;
}

/*Write out one line into output file*/
void writeOBLine(int first, int second)
{
    /*Write only if file is open*/
    if (output != NULL)
    {
        char *Line1, *Line2;
        Line1 = decimalToBase32(first);
        fputc('\t', output);
        fputs(Line1, output);
        fputc('\t', output);

        Line2 = decimalToBase32(second);
        fputs(Line2,output);
        fputc('\n',output);
        
        FREEUP(Line1);
        FREEUP(Line2);
    }
    
}

/*Write entiraty of data list to .ob file
 *Parse through each node in data list,
 *and convert values of data->values array in each node to base32 
*/
int writeData(void)
{
    dataList *data = getDataList();
    int i;
    int memloc;
    while (data != NULL)
    {
        memloc = data->addressInDec;
        for (i = 0; i < data->DC; i++,memloc++)
        {
            writeOBLine(memloc, (data->values[i]));
        }
        data = data->next;
    }  
    return ALL_CLEAR;
}


/*Write entiraty of code list to .ob file
*Parse through each node in code list,
*and convert values of code->values array in each node to base32 
*/
int WriteCode(void)
{
    int i;
    int memloc;
    codeList *code = getCodeHead();
    while (code != NULL)
    {
        memloc = code->addressInDec;
        for (i = 0; i < code->IC; i++,memloc++)
        {
            writeOBLine(memloc, (code->paramsInDec[i]));
        }
        code = code->next;
    }  
    /*Move to writing the data automatically*/
    return writeData();
}