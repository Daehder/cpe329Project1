//
//  main.c
//  StringLibTest
//
//  Created by Erik Miller on 4/6/16.
//  Copyright © 2016 Daehder. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include "StringLib.h"

int main(int argc, const char * argv[]) {
   char input[100];
   char *line1, *line2, *extra;
   
   printf("Enter a string to split up:\n");
   scanf("%100[^\n]", input);
   
   getLines(input, &line1, &line2, &extra);
   
   printf("Input is |%s\n", input);
   printf("Line1 is |%-16s| %lu\n", line1, strlen(line1));
   printf("Line2 is |%-16s| %lu\n", line2, strlen(line2));
   printf("Extra is |%s| %lu\n", extra, strlen(extra));
   return 0;
}
