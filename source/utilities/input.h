
/*Get the first line in a filestream,
  place text in line pointer
  return 0 if couldn't return line or NULL*/
int getCodeLine(char **line, FILE *file); 

/*Extention to fopen that adds file extentions before opening (i.e. .as .ob) */
FILE *getFile(const char *filename, char *extention, char *mode);

/*Extention to remove() that adds file extentions before deleting (i.e. .as .ob) */
void removeFile(const char *filename, char *extention);

/*Check if file names passed to program actually exist 
 *Exit if not.*/
void checkFileValidity(int argc, char **argv);

/*Check if pointer was allocated correctly*/
int checkAlloc(void *ptr);

/*Returns first index of string after space in given input*/
int jumpSpace(char *line, int index);

/*Returns first index of string after a word in given input*/
int jumpWord(char *line, int index);

/*Check if given input is only spaces*/
int isBlank(char *line, int index);

/*Close a file (Memory safe)*/
void closeFile( FILE **toClose);

