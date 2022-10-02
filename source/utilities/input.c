/*input.c handles parsing from external files and handling of code*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "converters.h"
#include "defines.h"



/*Check if pointer was allocated correctly*/
int checkAlloc(void *ptr)
{
    if (ptr == NULL)
    {
        printf("ERROR: Issue allocating memory. \n Exiting.");
        return FALSE;
    }
    return TRUE;
}

/*Get the first line in a filestream,
  place text in line pointer
  return 0 if couldn't return line or NULL*/
int getCodeLine(char **line, FILE *file)
{
    *line = NULL;
    *line = (char *)malloc((LINELEN +1));
    checkAlloc(line);

    *line = fgets(*line,(LINELEN + 1), file);
    
    if (*line == NULL)
    {
        return EOF;
    }
    if (strlen(*line) >= LINELEN)
    {
        printf("ERROR: Each line of code must not be larger than %d characters \n",LINELEN );
        return EOF;
    }
    
    return TRUE;
}


/*Extention to fopen that adds file extentions before opening (i.e. .as .ob) */
FILE *getFile(const char *filename, char *extention, char *mode)
{
    FILE *file;

    /*Try adding extention to filename*/
    char *fullfile = concat(filename, extention);
    /*Open file*/
    file = fopen(fullfile, mode);
    /*Free up char data*/
    FREEUP(fullfile);

    return file;
}

/*Extention to remove() that adds file extentions before deleting (i.e. .as .ob) */
void removeFile(const char *filename, char *extention)
{
    /*Try adding extention to filename*/
    char *fullfile = concat(filename, extention);
    /*Open file*/
    remove(fullfile);
    /*Free up char data*/
    FREEUP(fullfile);
}



/*Check if file names passed to program actually exist 
 *Exit if not.*/
void checkFileValidity(int argc, char **argv)
{
    int i;
    FILE *asmfile;

    if (argc <= 1)
    {
        printf("ERROR: No files inputted. \nExiting program\n ");
        exit(1);
    }

    for ( i = 1; i < argc; i++) 
    {
        asmfile = getFile(*(argv+i), ".as", "r");
        if (asmfile == NULL)
        {
            printf("ERROR: %s.as not found. \nExiting program \n", *(argv + i));
            exit(1);   
        }
        fclose(asmfile);
        asmfile = NULL;
    }    
}

/*Returns first index of string after space in given input*/
int jumpSpace(char *line, int index)
{
	char c;
	while((c = line[(index)]) != '\0' && isspace(c))
		index++;
	return index;
}

/*Returns first index of string after a word in given input*/
int jumpWord(char *line, int index)
{
    char c;
	while((c = line[(index)]) != '\0' && !isspace(c))
		index++;
    index = jumpSpace(line,index);
	return index;
}

/*Check if input is only spaces*/
int isBlank(char *line, int index)
{
	index = jumpSpace(line,index);
    /*Check if we have reached the end of the line*/
	if (line[index] == '\0')
    {
        return TRUE;
    }

    return FALSE;
}

/*Close a file (Memory safe)*/
void closeFile( FILE **toClose)
{
  if (*toClose != NULL)
  {
    fclose(*toClose);
    *toClose = NULL;
  }
}
