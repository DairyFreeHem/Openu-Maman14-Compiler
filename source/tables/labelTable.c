/*
* Compiler's label table.
* Represents the label table,
* and parses data for each label both external and internal
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../utilities/defines.h"
#include "../utilities/input.h"
#include "../utilities/textverify.h"
#include "../utilities/converters.h"
#include "labelTable.h"
#include "LabelReferenceTable.h"


/*Represents an internal value*/
#ifndef ENT
    #define ENT 0
#endif
/*Represents an external value*/
#ifndef EXT
    #define EXT 1
#endif

labels *labelHead = NULL;


/*Do we have any lables ?*/
int writeInternal = FALSE;
int internalFlag = FALSE; 
int externalFlag = FALSE;


/* Insert a new label into the Linked-List */
int insertLabel(char *name, int memloc, int location)
{
    labels **headptr = &labelHead;
    labels *temp, *follow;
    char *base32 = NULL;


    if (location == ENT)
    {
        internalFlag = TRUE;
    }
    
    /*Convert memory value into Maman14's Base32*/
    base32 = decimalToBase32(memloc);

    /*Create labels node*/
    temp = NEW_NODE(labels);
    CHECKMEM(temp);
    temp->name = name;
    temp->addressInBase32 = base32;
    temp->addressInDec = memloc;
    temp->location = location;
    temp->next = NULL;

    /*Inserts temp to end of table*/
    ADD_TO_TABLE(labelHead,follow,temp,headptr);
    
    return ALL_CLEAR;
}

/* Get a macro by name of macro */
labels *getLabel(char *name)
{
    labels *temp = labelHead;
    while (temp != NULL)
    {
        if (strcmp(temp->name, name) == 0)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

/*Return a label's memory location in decimal*/
int getLabelMemLoc(char *name)
{
    
    labels *temp = getLabel(name);
    if (temp != NULL)
    {
        if (temp->location == EXT)
        {
            return NONE;
        }
        
        return temp->addressInDec;
    }
    return NONE;
}

/* Clear all labels in our linked list */
void clearLabels(void)
{

    labels *temp , *pt;
    labels **headptr = &labelHead;
    temp = *headptr;

    if (labelHead == NULL)
    {
        return;
    }
    
     /* While we haven't rached the end of the list,
     * free up all nodes in the list
     */
    while (temp != NULL)
    {
        pt = temp->next;
        free(temp->name);
        free(temp->addressInBase32);
        free(temp);
        temp = pt;
    }

    labelHead = NULL;
    internalFlag = FALSE;
    externalFlag = FALSE;
    writeInternal = FALSE;
}



/*Get length of label*/
int labelLen(char *line, int index,int location)
{
    int iterator=0;
    /*Does it start with a word?*/
    if (isalpha(line[index])){

        while(isalpha(line[index+iterator]) || isdigit(line[index+iterator]))
        {
            iterator++;
        }

        if (location == ENT && line[index+iterator] == ':')
        {
            internalFlag = TRUE;
        }
        
        return index+iterator;
        
        
    }
    return FALSE;
    
}

/*Check for errors in Label info*/
int checkLabelErrors(char *line, int index, int endIndex,int location,char *name)
{
        /*Length of label*/
        int labelLen;
        if (endIndex == MEMORY_ERROR)
        {
            return MEMORY_ERROR;
        }

        labelLen = endIndex - index;

        if ((location == ENT) && getLabel(name) != NULL)
        {
            return EXISTING_LABEL;
        }
        if (isDirective(line, index) >= 0 )
        {
            return NAME_AS_DIRECTIVE;
        }
        if (isOrder(line,index) >= 0)
        {
            return NAME_AS_ORDER;
        }
        if (labelLen > LABELMAXLEN)
        {
            return LABEL_TOO_LONG;
        }
        if ((location == EXT)  && (isBlank(line, endIndex) == FALSE))
        {
            return PARAM_OVERFLOW;
        }
        
        return ALL_CLEAR;
}

/*Get name of label*/
int getLabelName(char *src, char **dst, int startIndex, int location)
{   
    int i, endIndex = 0;
    char *str = NULL;

    /*Skip any spaces*/
    startIndex = jumpSpace(src, startIndex);

    endIndex = labelLen(src,startIndex,location);

    str = (char *)malloc(endIndex - startIndex);
    CHECKMEM(str);
    
    for (i=0;  startIndex < endIndex; i++, startIndex++)
    {
        str[i] = src[startIndex];
    }
    str[i] = '\0';

    *dst = str;
    return (endIndex);
}


/*writes labels into the appropriate .ext or .ent files */
int writeLabels(char *filename)
{
    labels *temp = labelHead;
    FILE *internal = NULL, *external = NULL;

    /*Create files based on if we have internal and external labels*/
    CREATE_FILE(internal,writeInternal,filename,".ent");
    CREATE_FILE(external,externalFlag,filename,".ext");

    /*Output label data to appropriate files*/
    while (temp != NULL)
    {
        if (doesLabelReferenceExist(temp->name,temp->location) == TRUE)
        {
            if (temp->location == ENT)
            {
                OUTPUT_FILE(internal, temp);
            }
            else
            {
                OUTPUT_FILE(external, temp);
            }
        }
        temp = temp->next;
    }
    
    /*Close up and finish*/
    CLOSE_FILE(internal);
    CLOSE_FILE(external);
    return TRUE ;
}

/*Get label from source */
int processLabel(char *line, int index,int memloc, int location)
{
        char *name = NULL;
        int endIndex, errorIndex;
        
        
        endIndex = getLabelName(line,&name,index,location);

        
        /*Check for errors in naming the label*/
        if ((errorIndex = checkLabelErrors(line, index,endIndex,location,name)) != ALL_CLEAR)
        { 
            FREEUP(name);
            return errorIndex;
        }

        /*If label is a .entern or .extern, then add to reference table */
        if (memloc == NO_ADDR)
        {
            errorIndex = insertLabelReference(name,location);
            if (location == EXT)
            {
                externalFlag = TRUE;  
            }
            if (location == ENT)
            {
                writeInternal = TRUE;
            }
            return errorIndex;
        }
        
        insertLabel(name, memloc, location);
        
        return ALL_CLEAR;
}