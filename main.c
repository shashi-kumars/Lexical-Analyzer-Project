/* 
 * Name: Shashi kumar S
 * Date: 08-06-2024
 * Description: Lexical Analyzer project
 */

#include "lexical.h"

FILE *fptr_src;

int main(int argc, char *argv[])
{
    /* Check if the source file is passed as a command-line argument */
    if(argc < 2)
    {
        printf("ERROR: Source file not passed through CLA\n");
        return 0;
    }
   
    /* Initialize the source file for lexical analysis */
    if(initializer(argv[1]) == Success)
    {
        printf("\nINFO: Source file opened successfully:\n");
    }
    else
    {
        printf("ERROR: File not opened.\n");
        return 0;
    }
    
    /* Perform lexical analysis on the source file */
    if(Do_Lexical(argv[1]) == Success)
    {
        printf("INFO: The Parsing successful.\n");
    }
    else
    {
        printf("It has some errors.\n");
    }

    return 0;
}