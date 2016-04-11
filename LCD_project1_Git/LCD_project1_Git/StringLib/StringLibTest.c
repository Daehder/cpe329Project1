/* A simple driver to test the function of the StringLib functions */

#include <stdio.h>
#include <string.h>
#include "StringLib.h"

int main(int argc, const char * argv[]) {
   char inputArr[100];
   char *input, *line1, *line2, *extra = NULL;
   
   printf("Enter a string to split up:\n");
   scanf("%100[^\n]", inputArr);
   
   input = inputArr;
   
   line1 = getLine(input, &extra);
   input = extra;
   line2 = getLine(input, &extra);
   
   printf("Input is |%s\n", input);
   if (line1)
   printf("Line1 is |%-16s| %lu\n", line1, strlen(line1));
   if (line2)
   printf("Line2 is |%-16s| %lu\n", line2, strlen(line2));
   if (extra)
   printf("Extra is |%s| %lu\n", extra, strlen(extra));
   
   return 0;
}
