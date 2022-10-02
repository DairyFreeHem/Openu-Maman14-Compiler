
/*Clear all Externs*/
void clearLabelReferences(void);

/*Insert new Extern label*/
int insertLabelReference(char *name, int type);

/*Do we have a label by that name?*/
int doesLabelReferenceExist(char *name, int type);


typedef struct refLabels
{
  char *name; /*Name of label*/
  int type;
  struct refLabels *next;
  
}labelRefs;
