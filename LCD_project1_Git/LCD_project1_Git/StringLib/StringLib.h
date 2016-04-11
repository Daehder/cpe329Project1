/* A small library of string utilities for partitioning strings into manageable
 * sizes for displays with limited slots for characters
 */

#ifndef StringLib_h

#define LINE_SIZE 17

/* Takes an input string and returns a string that is less LINE_SIZE characters.
 *  Anything after the returned string is passed back through the
 *  char double pointer, extra.
 *
 * If a NULL is returned, then the input string was too long
 * If extra is NULL, then there is nothing else to partition
 *
 * This function may trash input, so it should not be used after calling
 *  getLine(), except to free it.
 * The returned string and extra do not need to be freed, but are their contents
 *  are undefined if input is freed or modified.
 */
char *getLine(char *input, char **extra);
#define StringLib_h


#endif /* StringLib_h */
