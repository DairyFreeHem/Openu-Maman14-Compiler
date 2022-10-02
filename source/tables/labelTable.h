

/*Represents an internal value*/
#define ENT 0
/*Represents an external value*/
#define EXT 1

#define CODE 3

#define NO_ADDR 0


/*Max size of a Label (as per project guidelines)*/
#define LABELMAXLEN 30

typedef struct labels
{
  char *name; /*Name of label*/
  char *addressInBase32; /*Number in the Maman's base32*/
  int addressInDec; /*Address in base 10*/
  int location; /*Is it internal or external or EXT_TO_WRITE ENT_TO_WRITE*/
  struct labels *next;
  
}labels;




/*Creates a new output file to output labels if needed*/
#define CREATE_FILE(outf,condition, name, extention){\
                                                if (condition == TRUE)\
                                                {\
                                                    outf = getFile(name, extention, "w");\
                                                    if (outf == NULL)\
                                                    {\
                                                        printf("ERROR: unable to create file %s%s",filename,extention);\
                                                        return FALSE;\
                                                    }\
                                                }\
                                            }
/*Close a file (If exists)*/
#define CLOSE_FILE(outf){\
                            if(outf != NULL)\
                            {\
                                fclose(outf);\
                            }\
}

/*Outputs label data to a file*/
#define OUTPUT_FILE(file,node){\
                        fputs(node->name, file);\
                        fputc('\t',file);\
                        fputs(node->addressInBase32, file);\
                        fputc('\n',file);\
                        }

/* Get a macro by name of macro */
labels *getLabel(char *);


/*Get label's memory location*/
int getLabelMemLoc(char *name);

/* Clear all labels in our linked list */
void clearLabels(void);

/*writes labels into the appropriate .ext or .ent files */
int writeLabels(char *filename);

/*Add label*/
int processLabel(char *line, int index,int memloc, int location);

/*Get length of label*/
int labelLen(char *line, int index, int location);

/*Get name of label and store in dst*/
int getLabelName(char *src, char **dst, int startIndex, int location);

/*Inserts new label to list*/
int insertLabel(char *name, int memloc, int location);

/*Get head of labelTable*/
labels *getLabelHead(void);