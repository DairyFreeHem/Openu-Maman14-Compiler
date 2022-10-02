

/*Remove newline from end of string*/
void removeNewline(char *line);

/*Function to free up array of strings created by seperateCommands*/
void clearCommands(char ***array, int length);

/*Function used to seperate one full line into an array of strings*/
char **seperateCommands(char *line, int *count, const char *delim);

/*adds the number in line that is represented as a string from index 'iterator' to index 'endNum'. returns (1) above.*/
int addNumber(char *line,int index);

/*Return substring from start of line until reaching endChar*/
char *cutSubstr(char *line,int index, char endChar);

/*Get code parameter by parameter number*/
char *getCodeParam(char *line,int paramNum);
