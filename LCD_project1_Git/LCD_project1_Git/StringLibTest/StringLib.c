#include <String.h>
#include <stdlib.h>

// Takes a string in as line1, then splits the string into line1, line2, and extra
// This program allocates space for line1 and line 2, 
//  so you MUST free line1 and line2 later
// Extra simply points to a location in  input, 
//  so do not use extra after freeing or modifying input
void getLines(char *input, char **line1, char **line2, char **extra) {
   int charsUsed = 0;
   char *token;
   char *addString;

   *line1 = calloc(17, sizeof(char));
   *line2 = calloc(17, sizeof(char));
   *extra = calloc(1, sizeof(char));
   
   addString = *line1;

   token = strtok(input, " ");
   
   while(token) {
      if (charsUsed) {
         strcat(addString, " ");
         charsUsed++;
      }
      charsUsed += strlen(token);

      if (charsUsed > 16 && addString != *extra) {
         if(addString == *line1)
            addString = *line2;
         else
            addString = *extra;
         charsUsed = strlen(token);
      }
      else if (addString == *extra)
         *extra = realloc(*extra, (charsUsed + 1) * sizeof(char));
      
      strcat(addString, token);
      
      token = strtok(NULL, " ");
   }
}

