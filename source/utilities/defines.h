

#include "errors.h"

/*Macro to free up dynamically allocated data*/
#define FREEUP(ptr) {\
                            if (ptr != NULL)\
                            {\
                                free(ptr);\
                                (ptr) = (NULL);\
                            }\
                          }

/*Checks if memory has been allocated,
 *return decimal representation if not*/
#define CHECKMEM(ptr){\
                        if (checkAlloc((ptr)) == (FALSE))\
                        {\
                          return ( (MEMORY_ERROR) );\
                        }\
                     }

/*Checks if memory has been allocated,
 *return NULL if not*/
#define CHECKMEM_PTR(ptr){\
                        if (checkAlloc((ptr)) == (FALSE))\
                        {\
                          return (NULL);\
                        }\
                     }


/*Update number of memory positions occupied by string*/
#ifndef UPDATE_MEMORY
  #define UPDATE_MEMORY(counter,memloc, strLength) \
  {\
    *counter += strLength;\
    *memloc += strLength;\
  }
#endif

/*Inserts new node into requested table*/
#ifndef ADD_TO_TABLE                    
  #define ADD_TO_TABLE(head,follow,temp,headptr)\
  {\
      if (head == NULL)\
      {\
          head = temp;\
      }\
      else\
      {\
          follow = *headptr;\
          while (follow->next != NULL)\
          {\
            follow = follow->next;\
          }\
          follow->next = temp;\
      }\
  }
#endif

/*Allocate new memory for a node*/
#define NEW_NODE(type) (type *)malloc(sizeof(type));

#ifndef TRUE
#define TRUE 1
#endif


#ifndef FALSE
#define FALSE 0
#endif

/*Length of a line in code (as per project guidelines)*/
#ifndef LINELEN
#define LINELEN 80
#endif

/*If no errors were found while testing*/
#ifndef ALL_CLEAR
  #define ALL_CLEAR 0
#endif

#ifndef NO_PARAMETERS
#endif

/*First index of the line*/
#ifndef LINE_START
  #define LINE_START 0
#endif