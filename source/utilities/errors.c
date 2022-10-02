/*
* File containing all errors that might occur during compilation process
*/


#include <stdio.h>
#include "defines.h"
#include "errors.h"

/*Checks if error was received when parsing errors*/
int compilerErrors(int currLine, int currIndex,int ID)
{
  int errorFlag = FALSE;
  /*If no error was found*/
  if (ID > ERROR_AREA_START)
  {
    return errorFlag;
  }
  switch (ID)
  {
    case EXISTING_LABEL:
      ERROR_OUT(currLine,currIndex,"Label Already exists");
      break;

    case PARAM_OVERFLOW:
      ERROR_OUT(currLine,currIndex,"Too many parameters inputted");
      break;
          
    case PARAM_UNDERFLOW:
      ERROR_OUT(currLine,currIndex,"Not enough parameters inputted");
      break;

    case NO_PARAMETERS:
      ERROR_OUT(currLine,currIndex,"Error parsing parametrs");
      break;

    case NAME_AS_DIRECTIVE:
      ERROR_OUT(currLine,currIndex,"Label cannot be named a directive");
      break;

    case NAME_AS_ORDER:
      ERROR_OUT(currLine,currIndex,"Label cannot be named as a command");
      break;

    case LABEL_TOO_LONG:
      ERROR_OUT(currLine,currIndex,"Label length too long, needs to be 30 characters or under");
      break;

    case NO_DATA:
      ERROR_OUT(currLine,currIndex,"Directive name not found, make sure you've written it correctly");
      break;

    case NO_STR_END:
      ERROR_OUT(currLine,currIndex,"No closing quotation mark for string");
      break;

    case STR_PARSE_ERROR:
      ERROR_OUT(currLine,currIndex,"Could not get string value, make sure it starts with a quotation mark");
      break;

    case PARAM_NOT_NUMBER:
      ERROR_OUT(currLine,currIndex,"Expected integer number, parameter is not an integer");
      break;

    case INVALID_STRUCT_POS:
      ERROR_OUT(currLine,currIndex,"Invalid call to struct");
      break;

    case INVALID_LINE:
      ERROR_OUT(currLine, currIndex, "Command, or directive not recognized");
      break;

    case INVALID_CODE_PARAM:
      ERROR_OUT(currLine, currIndex, "Invalid parameter inputted to command");
      break;

    case COMMAND_NOT_FOUND:
      ERROR_OUT(currLine,currIndex, "Command or directive not recognized");
      break;

    case LABEL_NOT_FOUND:
      ERROR_OUT(currLine,currIndex, "Label name not recognized");
      break;

    case LABEL_REFERENCE_ALREADY_EXISTS:
      ERROR_OUT(currLine,currIndex, "Duplicate definitions of label reference");
      break;

    default:
      ERROR_OUT(currLine,currIndex, "Undefined error occured, You're not suopposed to see this message... sorry");
      break;
      }
      return errorFlag;
}