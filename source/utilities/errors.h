

/*Generic macro to write errors*/
#define ERROR_OUT(line,currIndex,description)  {\
  printf("ERROR: in line %d index %d %s\n",line, currIndex, description);\
  errorFlag = TRUE;\
}


#ifndef ERROR_AREA_START
  #define ERROR_AREA_START -4999
  #define STR_PARSE_ERROR -5000
  #define NUMBER_OVERFLOW -5001
  #define PARAM_NOT_NUMBER -5002
  #define NO_STR_END -5004
  #define PARAM_UNDERFLOW -5003
  #define PARAM_OVERFLOW -5005
  #define NAME_AS_DIRECTIVE -5006
  #define NAME_AS_ORDER -5007
  #define MEMORY_ERROR -5008
  #define NO_DATA -5009
  #define NO_PARAMETERS -5010
  /*Label length is longer than 30 characters*/
  #define LABEL_TOO_LONG -5011
  /*There's already a label with that name*/
  #define EXISTING_LABEL -5012
  #define INVALID_REGISTER -5013
  #define MISSING_MEMORY -5014
  #define INVALID_STRUCT_POS -5015
  #define INVALID_LINE -5016
  #define INVALID_CODE_PARAM -5017
  #define COMMAND_NOT_FOUND -5018
  #define LABEL_NOT_FOUND -5019
  #define LABEL_REFERENCE_ALREADY_EXISTS -5020
#endif


/*Display messages for each compiler error*/
int compilerErrors(int currLine, int currIndex,int ID);