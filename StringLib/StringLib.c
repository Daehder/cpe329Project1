#include <String.h>
#include "StringLib.h"


/* Takes in a char pointer and returns the index of the last space before
 *  position LINE_SIZE.
 * A negative index indicates that no spaces were found
 */
int findLastSpace(char *input) {
   int ndx;
   int lastSpace = -1;
   
   /* The condition checks to make sure ndx doesn't exceed 16 charactrs or the
    * length of the string */
   for (ndx = 0; ndx < LINE_SIZE && ndx < strlen(input); ndx++)
      if(input[ndx] == ' ')
         lastSpace = ndx;
   
   return lastSpace;
}

/* Gets the next 16 characters in a string.
 * Input takes in a char pointer to split into a 16 char string and the rest.
 * Extra is used to pass back the rest of input, and will be overwritten.
 *  (Since extra is overwritten, no memory should be allocated for it)
 *
 * Returns a pointer to a 16 character char.
 * If there is not a space in the first 16 characters or if the input is 
 *  invalid, NULL is returned
 *
 * This function modifies the contents of input, and thus input should not be
 *  used after calling this function. The address, on the other hand is left
 *  intact, in the event that the pointer needs to be freed.
 *
 * The pointer that is returned and the double pointer extra don't have any
 *  memory allocated to them, so they do not need to be freed later.
 *  However, they point to a location in input, so their behavior is undefined
 *  if input is freed or modified by another function.
 */
char *getLine(char *input, char **extra) {
   int lastSpace;
   char *line = NULL;
   
   /* Checks both for valid input and strings shorter than 16 characters 
    *  If either is true, input can be returned without processing
    */
   if (!input || strlen(input) < LINE_SIZE) {
      *extra = NULL;
      return input;
   }
   
   lastSpace = findLastSpace(input);
   
   /* If there is a space in the first 16 characters, set the last space to a
    *  NULL character, then point extra to the character after it */
   if (lastSpace >=0) {
      input[lastSpace] = '\0';
      line = input;
      *extra = input + lastSpace + 1;
   }
   else  /* if there is not a space within the first 16 characters */
      *extra = input;   /* Returns the input through extra without setting */
                        /*  the output */
   return line;
}

