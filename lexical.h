#ifndef LEXICAL_H 
#define LEXICAL_H

#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>

typedef unsigned int uint;

/* enum for status of program */
typedef enum
{
    Failure,
    Success
} Status;

/* Variables declarations */

/* Character from the source file */
uint ch; 

/* Buffer to store tokens */
char str[100]; 

/* Index for the buffer */
static int idx; 

/* Array to keep track of different brackets */
static int Bracket[3]; 

/* Flags to check the error */
static int NUM_ERR;
static int HEX_ERR;
static int OCT_ERR;
static int DOT_ERR;

/* Reserver keywords */
static char* Reserved_Keywords[] = {"const","volatile","extern","auto","register","static","signed","unsigned","short",
        
                                   "long","double","char","int","float","struct","union","enum","void","typedef"};

/* Non reserved keywords */
static char* Non_Reserved_Keywords[] = {"goto","return","continue","break","if", "else", "for", "while","do","switch", "case", "default", "sizeof"};

/* Predefined function */
static char* Predefined_functions[] = {"printf","scanf"};

/* Operaters */
static char Operators[] = {'/', '+','*','-','%','=','<','>','~','&',',','!','^','|'};

/* Operaters with double character */
static char* Operaters2[]={"==","!=","++","--","<<",">>","+=","-=","*=","/=","&&","%=","||","|=","&=","<=",">="};

//static char* Format_specifier= {"%d","%f","%c","%g","%s"};

/* Symbols */
static char Symbols[] = {'(',')','{','}','[',']',':',';'};

static int RK_len=19;
static int NRK_len=13;
static int Pre_fun=2;
static int Op_len=14;
static int Op2_len=17;
static int Sy_len=8;

/* Functions declarations */

/* This function to open source file */
Status initializer(const char* filename);

/* This function to do lexical */
Status Do_Lexical(char *file_name);

/* This function is to handle macro */
int Header(FILE *fptr_src,char *str);

/* This funciton is to check the number is hexa or octal */
int isHexa(FILE *fptr_src,char *str);

/* This function is to handle the constants */
int Constant(FILE *fptr_src,char *str);

/* This function is to check charcter is f,u,l for numeric constant. */
int isFUL(char ch);

/* This function is to get alphanumeric strings and check for key words */
int AlpaNumeric(FILE *fptr_src,char *str);

/* This funcition will check all kinds of keywords */
void Keyword_Checker(char *str);

/* This funtion to get characters */
int isChar(FILE *fptr_src,char *str);

/* This function to check present character is operater */
int isOperater(uint ch);

/* This function to chekc double character operaters */
int Double_oper(FILE *fptr_src,char *str);

/* This function to check for symbols */
int isSymbol(uint ch);

/* This function to handle the brackets errors */
void Bracket_Handler(uint ch);

#endif
