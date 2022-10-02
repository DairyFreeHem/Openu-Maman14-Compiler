#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "defines.h"
#include "input.h"
#include "textverify.h"

/*Remove newline from end of string*/
void removeNewline(char *line)
{
    int len = strlen(line);
    if ((line[len-1]) == '\n')
    {
        line[strlen(line)-1] = '\0';
    }

}
/*Function to free up array of strings created by seperateCommands*/
void clearCommands(char ***array, int length)
{
    char *strPtr;
    char **temp = *array;
    int i;

    for (i = 0; i < length; ++i)
    {
        strPtr = *(temp + i);
        free(strPtr);
    }
    free(temp);
    array = NULL;
}

/*Function used to seperate one full line into an array of strings*/
char **seperateCommands(char *line, int *count ,const char *delim)
{
    char *temp, placeholder[LINELEN];
    char **commandArr;
    int indices = 0; /*number of strings inside commandArr*/
    
    strcpy(placeholder, line);
    commandArr = (char **)malloc((indices +1) * sizeof(char *));
    if (checkAlloc(commandArr) == FALSE)
    {
        return NULL;
    }

    temp = strtok(placeholder,delim);
    while (temp != NULL)
    {
        /*if temp is just a newline, skip reading it*/
        if ((strlen(temp) == 1) && *temp == '\n')
        {
            goto ENDLOOP;
        }
        commandArr = (char **)realloc(commandArr,((indices +1) * sizeof(char *)));
        if (checkAlloc(commandArr) == FALSE)
        {
        return NULL;
        }
        
        removeNewline(temp);
        commandArr[indices] = (char *)malloc(strlen(temp));
        if (checkAlloc(commandArr) == FALSE)
        {
            clearCommands(&commandArr, indices);
            return NULL;
        }
        strcpy(commandArr[indices],temp);
        indices++;
ENDLOOP:temp = strtok(NULL,delim);
    }
    *count = indices;
    return commandArr;
}


/*adds the number in line that is represented as a string from index 'iterator' to index 'endNum'. returns (1) above.*/
int addNumber(char *line, int index){

    int endNum;
    int newNumber, i;
    char numberStr[5] = {0};

    if ((endNum = isNumber(line,index)) == PARAM_NOT_NUMBER)
    {
        return PARAM_NOT_NUMBER;
    }

    endNum++;

    for (i = 0; index < endNum; i++, index++)
    {
        numberStr[i] = line[index];
    }
    
    newNumber = atoi(numberStr);

    if(newNumber>512 || newNumber<-512)
        return NUMBER_OVERFLOW;

    return newNumber;
}

/*Cut string until a certain char, or end of line return NULL*/
char *cutSubstr(char *line,int index, char endChar)
{
    int size = 10;
    int i;
    char *nuStr = (char *)malloc(size);
    CHECKMEM_PTR(nuStr);
    for (i = 0; (line[i+index] != endChar) || (line[i+index] == '\0'); i++)
    {
        nuStr[i] = line[i];
        if (i > (size-1))
        {
            ++size;
            nuStr = (char*)realloc(nuStr,size);
            CHECKMEM_PTR(nuStr);
        }
    }
    nuStr[i] = '\0';
    return nuStr;
}

/*Get code parameter by parameter number*/
char *getCodeParam(char *line,int paramNum)
{
    char *word = NULL;
    int startIndex = LINE_START,endIndex,length, i;
    if (paramNum >= 2 )
    {
        while (line[startIndex] != ',')
        {
            startIndex++;
        }
        startIndex++;
    }
    /*If param is number*/
    if (line[startIndex] == '#')
    {
        startIndex++;
    }
    
    startIndex = jumpSpace(line, startIndex);
    for (endIndex = startIndex; !isspace(line[endIndex]) && (line[endIndex] != ','); i++)
    {

        endIndex++;
    }
    length = endIndex- startIndex;

    word = (char *)malloc((length+1) * sizeof(char));
    CHECKMEM_PTR(word);
    
    for (i = 0; i < length; i++,startIndex++)
    {
        word[i]=line[startIndex];
    }
    word[i] = '\0';
    return word;
}