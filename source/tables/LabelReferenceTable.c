/*
* Compiler's external label table.
* Represents the .extern tables,
* for the second pass to then add them to the .ext file
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../utilities/defines.h"
#include "../utilities/input.h"
#include "../utilities/textverify.h"
#include "../utilities/converters.h"
#include "LabelReferenceTable.h"
#include "labelTable.h"



labelRefs *labelRefsHead = NULL;

/*Get a label reference by name and type */
labelRefs *getLabelByName(char *name );

/* Clear all labelRefs in our linked list */
void clearLabelReferences(void)
{
    labelRefs **labelRefsHeadptr = &labelRefsHead;
    labelRefs *temp , *pt;
    /* While we haven't rached the end of the list,
     * free up all nodes in the list
     */
    temp = *labelRefsHeadptr;
    while (temp != NULL)
    {
        pt = temp->next;
        FREEUP(temp->name);
        FREEUP(temp);
        temp = pt;
    }
    labelRefsHead = NULL;
}

/* Insert a new label into the Linked-List */
int insertLabelReference(char *name, int type)
{
    labelRefs **labelRefsHeadptr = &labelRefsHead;
    labelRefs *temp = NULL, *follow;

    if (doesLabelReferenceExist(name,EXT) == TRUE)
    {
        return LABEL_REFERENCE_ALREADY_EXISTS;
    }
    if (getLabelByName(name) != NULL)
    {
        return LABEL_REFERENCE_ALREADY_EXISTS;
    }

    temp = NEW_NODE(labelRefs);
    CHECKMEM(temp);
    temp->name = (char *)malloc(strlen(name)+1);
    CHECKMEM(temp->name);
    temp->name = name;
    temp->type = type;
    temp->next = NULL;

    if (labelRefsHead == NULL)
    {
        labelRefsHead = temp;
    }
    else/*If node isn't empty, add from end*/
    {
        follow = *labelRefsHeadptr;
        while (follow->next != NULL)
        {
            follow = follow->next;
        }
        follow->next = temp;
    }
    return ALL_CLEAR;
}

/*Get a label reference by name and type */
labelRefs *getLabelByName(char *name )
{    
    labelRefs *temp = labelRefsHead;
    if (labelRefsHead == NULL)
    {
        return NULL;
    }
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


/*Get a label reference by name and type */
int doesLabelReferenceExist(char *name, int type)
{
    labelRefs *temp = NULL;
    
    temp = getLabelByName(name);
    if (temp == NULL )
    {
        return FALSE;
    }
    if (temp->type == type)
    {
        return TRUE;
    }
    return FALSE;
}
