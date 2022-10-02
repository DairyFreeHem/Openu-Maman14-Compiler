/*Interprets the code outputtted after the preprocessor
  Contains both the first pass and second pass*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utilities/input.h"
#include "utilities/defines.h"
#include "utilities/converters.h"
#include "utilities/formatter.h"
#include "utilities/textverify.h"
#include "utilities/errors.h"
#include "utilities/writer.h"
#include "tables/labelTable.h"
#include "tables/LabelReferenceTable.h"
#include "tables/dataTable.h"
#include "tables/codeTable.h"


#define MEMORY_START 100
#define MEMORY_MAX 255


#define MEMORY_CLOSURE(file,line,currLine,currIndex) \
{\
  if (returnID == MEMORY_ERROR)\
  {\
    closeFile(&code);\
    ERROR_OUT(currLine,currIndex,"Memory usage excedded. Exiting program.");\
    FREEUP(line);\
    exit(137); /*Return "program used up too much memory" error*/\
  }\
}\


#define PASS_ERRORS(currLine,currIndex,returnID) \
{\
  if( compilerErrors(currLine,currIndex,returnID) == TRUE)\
  {\
    errorFlag = TRUE;\
  }\
}\

void secondPass(char *filename, int IC,int DC);

/*Delete all our lists*/
void clearLists(void)
{
  clearDataList();
  clearLabels();
  clearLabelReferences();
  clearCodeList();
}


/*
* Function runs first pass of code
*/
void firstPass(char *filename)
{
    /*
    * DC - Data counter
    * IC - Instruction counter
    * memloc - current location in memory (starts at 100)
    */
    int errorFlag = FALSE;
    int IC = 0, DC = 0, memloc=MEMORY_START;

    int endFlag, currIndex, wordEnd, currLine = 0, dirID, returnID;

    char *line = NULL;

    FILE *code = getFile(filename, ".am", "r");

    /*Read out the assemmbly code line by line and process it*/
    while ((endFlag = getCodeLine(&line, code)) != EOF)
    {
      returnID = COMMAND_NOT_FOUND;
      /*If memory size is larger than 255 words*/
      if (memloc > MEMORY_MAX )
      {
          ERROR_OUT(currLine,currIndex,"Memory Full, cannot write any more commands");
          break;
      }

      /*Skip whitespace until reaching first char*/
      currIndex = jumpSpace(line, LINE_START);

      /*move to next line*/
      currLine++;

      /*If comment or blank line, then skip*/
      if (line[currIndex] == ';' || line[currIndex] == '\0')
      {
        FREEUP(line);
        continue;
      }

      /*If internal label was received*/
      if ((wordEnd = isInternalLabel(line, currIndex)) != FALSE)
      {
        returnID = processLabel(line,currIndex,memloc,ENT);
        PASS_ERRORS(currLine,currIndex,returnID);
        currIndex = jumpWord(line,currIndex);
      }

      /*If current index points to a directive*/
      if(line[currIndex] == '.')
      {
        dirID = isDirective(line, currIndex);
        /*Skip .value text in line variable */
        currIndex = jumpWord(line,currIndex);
        switch (dirID)
        {
        case EXTERN:
          returnID = processLabel(line, currIndex,NO_ADDR, EXT);
          break;

        case ENTRY:
          returnID = processLabel(line, currIndex, NO_ADDR, ENT);
          break;

        case STRUCT:
          returnID = addStruct(line,&memloc,currIndex, &DC);
          break;

        case STRING:
          returnID = addString(line,currIndex,&memloc, &DC);
          break;

        case DATA:
          returnID = addData(line,&memloc,currIndex,&DC);
          break;

        default: /*Failed to find data*/
          returnID = NO_DATA;
          break;
        }
      }
      /*Checks if a command has been inputted*/
      if ((dirID = isOrder(line,currIndex)) != NONE)
      {
        currIndex = jumpWord(line,currIndex);
        returnID = processCode(line,currIndex,&memloc,&IC,dirID);
      }

      /*If Memory error observed, then close the code*/
      if (returnID == MEMORY_ERROR)
      {
        MEMORY_CLOSURE(code,line,currIndex,currIndex);
      }
      
      /*move to next line, memory location*/
      FREEUP(line);

      /*Check for errors from directive*/
      PASS_ERRORS(currLine,currIndex,returnID);
    }

    /*End of first-pass*/
    if(errorFlag == FALSE)
    {
      secondPass(filename, IC, DC);
    }
    else
    {
      printf("Assembly files not optputted - fix errors before recompiling \n");
      clearLists();
      exit(EXIT_FAILURE);
    }
    
}

/*
* Function runs second pass for memory
* Only called right after first pass
* Use is to compile command parameters
* and pass data to output files
*/
void secondPass(char *filename, int IC,int DC)
{
  int errorFlag = FALSE;
    int endFlag, currIndex, wordEnd, currLine = 1, dirID, returnID;
    char *line = NULL;
    codeList *codeListPtr = getCodeHead();
    FILE *code = getFile(filename, ".am", "r");

    /*Read out the assemmbly code line by line and process it*/
    while (((endFlag = getCodeLine(&line, code)) != EOF) && (codeListPtr != NULL))
    {
      returnID = ALL_CLEAR;
      currIndex = jumpSpace(line,LINE_START);

      /*If comment or blank line, then skip*/
      if (line[currIndex] == ';' || line[currIndex] == '\0'|| line[currIndex] == '.')
      {
        currLine++;
        FREEUP(line);
        continue;
      }

      /*If internal label was received*/
      if ((wordEnd = isInternalLabel(line, currIndex)) == TRUE)
      {
        currIndex = jumpWord(line,currIndex);
      }

      /*Checks if a command has been inputted*/
      if ((dirID = isOrder(line,currIndex)) != NONE)
      {
        currIndex = jumpWord(line,currIndex);

        /*If code has more than one parameter*/
        if (codeListPtr->bin[0] < RTS)
        {
          returnID = processParams(codeListPtr,line,currIndex);
        }
        codeListPtr = codeListPtr->next;
      }

      /*If Memory error observed, then close the code*/
      if (returnID == MEMORY_ERROR)
      {
        MEMORY_CLOSURE(code,line,currIndex,currIndex);
      }
      
      /*move to next line, memory location*/
      FREEUP(line);

      /*Check for errors from directive*/
      PASS_ERRORS(currLine,currIndex,returnID);

      currLine++;
    }
    CLOSE_FILE(code);
  
  /*If no error was found during second-pass ,then write to .ob*/
  if (errorFlag == FALSE)
  {
    /*Start writing .ob file*/
    returnID = startWrite(filename);

    /*If .ob file was created successfully*/
    if (returnID == ALL_CLEAR)
    {

      writeOBLine(IC,DC);
      WriteCode();
      /*Close .ob file*/
      endWrite();
      
      /*Write to .end and .ext files*/
      writeLabels(filename);
      printf("%s.am compiled successfully!\n",filename);
      clearLists();
    }
  }
  else
  {
    clearLists();
    printf("Assembly files not optputted - fix errors before recompiling \n");
    exit(EXIT_FAILURE);
  }


}

