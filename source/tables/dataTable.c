/*
* Compiler's data table.
* Represents the data table,
* and parses data for each line and parameter
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include "../utilities/textverify.h"
#include "../utilities/converters.h"
#include "../utilities/formatter.h"
#include "../utilities/input.h"
#include "../utilities/defines.h"



#include "dataTable.h"

dataList *dataHead = NULL;

/* Insert a new data value into the Linked-List */
int insertData(int type,int *vals,int params,int memloc, int DC)
{
    dataList **headptr = &dataHead;
    dataList *temp = NULL;
    dataList *follow = NULL;

    /*Create new dataList node*/
    temp = NEW_NODE(dataList);
    CHECKMEM(temp);
    temp->type = type;
    temp->values = vals;
    temp->DC = DC;
    temp->addressInDec = memloc;
    temp->parameters = params;
    temp->next = NULL;

    /*Inserts temp to end of table*/
    ADD_TO_TABLE(dataHead,follow,temp,headptr);
    
    return ALL_CLEAR;
    
}


void clearDataList(void)
{   
    dataList *temp , *follow;

    dataList **headptr = &dataHead;
    temp = *headptr;

    if (dataHead == NULL)
    {
        return;
    }
    /* While we haven't rached the end of the list,
     * free up all nodes in the list
     */
    while (temp != NULL)
    {
        follow = temp->next;
        free(temp->values);
        free(temp);
        temp = follow;
    }
    dataHead = NULL;
}

/*Get data values by its location*/
dataList *getDataByLoc(int addressInDec)
{
    dataList *temp = dataHead ;
    while (temp != NULL)
    {
        if (temp->addressInDec ==  addressInDec)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

/*Gets head of data list*/
dataList *getDataList(void)
{
        return dataHead;
}

/*Check for errors in data info*/
int checkDataErrors(char *line, int index,int strLength,int type)
{
        int endIndex = (index + strLength);

        if (strLength == NO_STR_END)
        {
            return NO_STR_END;
        }
        if (strLength == MEMORY_ERROR)
        {
            return MEMORY_ERROR;
        }
        if ((type == STRING) && (isBlank(line, (endIndex+1)) == FALSE))
        {
            return PARAM_OVERFLOW;
        }
        
        return ALL_CLEAR;
}


/*Gets length of inputted string, returns its length*/
int getStrLength(char *line, int index)
{
    int endIndex = index;
    while (line[endIndex] != '\"')
    {
        endIndex++;
        if (line[endIndex] == '\n' || line[endIndex] == '\0')
        {
            return NO_STR_END;
        }

    }

    return (endIndex - index);
    
}

/*Get a string as array of ASCII characters*/
int getString(char *line,int **dst,int index)
{
    int i = 0, strLength;
    int *name = NULL;

    if (line[index] != '\"')
    {
            return STR_PARSE_ERROR;
    }

    index++; /*Skip first quotation mark*/

    strLength = getStrLength(line,index);
    if (strLength == NO_STR_END)
    {
        return NO_STR_END;
    }

    name = (int *)malloc(strLength * sizeof(int));
    CHECKMEM(name);


    for (; i < strLength; i++, index++)
    {
        name[i] = line[index];
    }
    name[i]= 0;
    *dst = name;

    strLength++;

    return strLength;

}

/*Adds new .string to data table*/
int addString(char *line,int index,int *memloc, int *DC)
{
    int *vals = NULL;
    int strLength, errorIndex;

    /*get string in ASCII and store its length in strLength*/
    strLength = getString(line,&vals,index);

    if ((errorIndex = checkDataErrors(line,index,strLength,STRING)) != ALL_CLEAR)
    {
        return errorIndex;
    }

    /*Add values to dataList*/
    if (insertData(STRING,vals,MAX_STR_PARAMS,*memloc,strLength) != ALL_CLEAR)
    {
        return MEMORY_ERROR;
    }
    

    UPDATE_MEMORY(DC,memloc, strLength);
    return ALL_CLEAR;
}


int concatStruct(int number,int *str,int strLength,int **dest)
{
    int i;
    int *vals = NULL;
    vals = (int *)malloc((strLength+1) * sizeof(int));
    CHECKMEM(vals);
    vals[0] = number;
    for (i = 1; i < strLength; i++)
    {
        vals[i] = str[i-1];
    }
    *dest = vals;
    return ALL_CLEAR;
    
}

/*Adds a .struct to the dataTable*/
int addStruct(char *line,int *memloc,int index, int *DC)
{
    char **params = NULL;
    int *vals = NULL;
    int *tempVals = NULL;

    int  number,placeholeder, paramCounter, strLength = 0;

    params = seperateCommands(line, &paramCounter, ",");
    CHECKMEM(params);

    /*Check if params received from struct are actually 2*/
    if (paramCounter > MAX_STRUCT_PARAMS )
    {
        clearCommands(&params, paramCounter);
        return PARAM_OVERFLOW;
    }
    else if (paramCounter < MAX_STRUCT_PARAMS )
    {
        clearCommands(&params, paramCounter);
        return PARAM_UNDERFLOW;
    }
    
    /*get number (if exists)*/
    number = addNumber(params[0],index);
    strLength++;
    if (number < ERROR_AREA_START)
    {
        return number;
    }
    
    index = jumpSpace(params[1],LINE_START);
    /*Get string and allocate area*/
    placeholeder = getString(params[1],&tempVals,index);
    if (placeholeder <= ERROR_AREA_START)
    {
        clearCommands(&params, paramCounter);
        return placeholeder;
    }
    strLength += placeholeder;

    if(concatStruct(number,tempVals,strLength,&vals) != ALL_CLEAR)
    {
        /*Clear **params since it's unused at this point*/
        clearCommands(&params, paramCounter);
        FREEUP(tempVals);
        return MEMORY_ERROR;
    }
    
     /*Clear **params since it's unused at this point*/
    clearCommands(&params, paramCounter);
    FREEUP(tempVals);

    /*Add values to dataList*/
    if (insertData(STRUCT,vals,MAX_STRUCT_PARAMS,*memloc,strLength) != ALL_CLEAR)
    {
        return MEMORY_ERROR;
    }

    UPDATE_MEMORY(DC,memloc, strLength);

    return ALL_CLEAR;
}

/*Adds a .data to the dataTable*/
int addData(char *line,int *memloc,int index, int *DC)
{
    char **params = NULL;
    int *vals = NULL;
    int i;
    int  number, paramCounter, strLength = 0;

    params = seperateCommands(line, &paramCounter, ",");
    CHECKMEM(params);
    /*Check if params received are one or more*/
    if (paramCounter < MIN_PARAMS )
    {
        free(params);
        return PARAM_UNDERFLOW;
    }
    vals = (int *)malloc(paramCounter * sizeof(int));
    for (i = 0; i < paramCounter; i++)
    {
        if ((number = addNumber(params[i],index)) < ERROR_AREA_START)
        {
            return number;
        }
        vals[i] = number;
        strLength++;
        index = LINE_START;
    }

    clearCommands(&params,paramCounter);

    /*Add values to dataList*/
    if (insertData(DATA,vals,paramCounter,*memloc,strLength) != ALL_CLEAR)
    {
        return MEMORY_ERROR;
    }

    UPDATE_MEMORY(DC,memloc, strLength);
    return ALL_CLEAR;
}
