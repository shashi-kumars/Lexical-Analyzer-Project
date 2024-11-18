#include "lexical.h"

extern FILE *fptr_src; 

/* 
 * Function: Do_Lexical
 * Analyzes the lexical components of the source file.
 * 1. Loop through the file character by character until EOF.
 * 2. Classify each character into categories such as header file, number, operator, symbol, etc.
 * 3. For each category, call the respective function to process further.
 * 4. Handle any errors such as unmatched brackets or invalid constants.
 * file_name: The name of the file to analyze.
 * Returns: Status indicating success or failure.
 */
Status Do_Lexical(char *file_name)
{
    /* Run while loop and get characters till end of file */   
    while ((ch = fgetc(fptr_src)) != EOF)
    {

	/* Check for Header file or Macro */
	if (ch == '#')
	{
	    if (Header(fptr_src, str))
	    {
		printf(" Header File\t: %s\n", str);
	    }
	}
	/* Check for Hexadecimal or Octal Number */
	else if (ch == '0')
	{
	    int hex = 0, oct = 0;
	    str[idx++] = ch;
	    ch = fgetc(fptr_src);
	    if (ch == 'x' || ch == 'X')
		hex = 1;
	    if (isdigit(ch))
		oct = 1;
	    str[idx++] = ch;
	    if (isHexa(fptr_src, str) && hex)
	    {
		/* Hex error */
		if (hex && HEX_ERR)
		{
		    printf("\nCompiler error: Invalid use of this '%s' hexadecimal constant! \n",str);
		    exit(0);
		}
		else
		    printf(" Hexadecimal\t: %s\n", str);
	    }
	    else if (oct)
	    {
		/* oct error */
		if ( oct && OCT_ERR)
		{
		    printf("\nCompiler error: Invalid use of octal '%s' constant!\n",str);
		    exit(0);
		}
		else
		    printf(" Octal Number\t: %s\n", str);
	    }
	    else
		printf(" Constant\t: %s\n", str);

	}
	/* Check for Digit */
	else if (isdigit(ch))
	{
	    str[idx++] = ch;
	    if (Constant(fptr_src, str))
	    {
		printf(" Constant\t: %s\n", str);
	    }

	    /* check for Numeric error */
	    if (NUM_ERR)
	    {
		printf("\nCompiler error: Invalid use of this '%s' numeric constant!\n",str);
		exit(0);
	    }

	    /* Check for dot error */
	    if (DOT_ERR)
	    {
		printf("\nCompiler error: Invalid use of this '%s' float constant!\n",str);
		exit(0);
	    }
	}
	/* Check for Alphanumeric */
	else if (isalnum(ch))
	{
	    str[idx++] = ch;
	    if (AlpaNumeric(fptr_src, str))
	    {
		/* Check the string for keyword */
		Keyword_Checker(str);
	    }
	}
	/* Check for sigle quote comma */
	else if( ch == '\'')
	{
	    if(AlpaNumeric(fptr_src,str))
	    {
		fseek(fptr_src, 1, SEEK_CUR);
		/* check string length is 1 */
		if(strlen(str) == 1)
		{
		    printf(" Charecter \t: %c\n",str[1]);
		}
		else
		{
		    printf("\nCompiler error: '%s' Multi character error!\n", str);
		    exit(0);
		}

	    }
	}
	/* Check for Operator */
	else if (isOperater(ch))
	{
	    str[idx++] = ch;
	    /* Check for double char operater */
	    if (Double_oper(fptr_src, str))
	    {
		printf(" Operator(2) \t: %s\n", str);
	    }
	    else
	    {
		printf(" Operator(1) \t: %s\n", str);
	    }
	}
	/* Check for Symbols */
	else if (isSymbol(ch))
	{
	    printf(" Symbol  \t: %c\n", ch);
	    Bracket_Handler(ch);
	}
	/* Check for Identifier */
	else
	{
	    /* Check the character is not newline or space */
	    if (ch != ' ' && ch != '\n')
	    {
		printf(" Identifier\t: %c\n", ch);
	    }
	}
    }

    /* Checking bracket error */
    for (int i = 0; i < 3; i++)
    {
	if (Bracket[i] != 0)
	{
	    printf("\nCompiler error: Brackets not opened or closed!\n");
	}
    }

    return Success;
} 

/*
 * Function: initializer
 * Opens the source file in read mode.
 * 1. Use fopen() to open the file in read mode.
 * 2. Check if the file is successfully opened.
 * 3. If the file cannot be opened, print an error message.
 * 4. Return Success or Failure based on the result.
 * filename: The name of the file to open.
 * Returns: Status indicating success or failure.
 */
Status initializer(const char* filename)
{
    /* Open source file in read mode */
    fptr_src = fopen(filename,"r");

    /* Validate file opening */
    if (fptr_src == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", "Source.c");
	return Failure;
    }
    return Success;
}


/* 
 * Function: Header
 * Extracts the header file name or macro after the '#' symbol.
 * 1. Start reading the characters after the '#' symbol.
 * 2. If a '<' is encountered, start storing characters in the str buffer.
 * 3. Stop storing when '>' is encountered or end of line is reached.
 * 4. Null-terminate the string and reset the index.
 * fptr_src: File pointer to the source file.
 * str: Buffer to store the extracted header or macro.
 * Returns: 1 if a header or macro is found.
 */
int Header(FILE *fptr_src, char *str)
{
    int add = 0;
    /* run loop till newline */
    while ((ch = fgetc(fptr_src)) != '\n')
    {
	if (ch == '<')
	    add = 1;
	if (add)
	    str[idx++] = ch;
	if (ch == '>')
	    add = 0;
    }
    str[idx] = '\0';
    idx = 0;
    return 1;
}

/* 
 * Function: isHexa
 * Checks if the given sequence is a hexadecimal number.
 * 1. Read characters from the file until a non-hexadecimal digit is found.
 * 2. Store each character in the str buffer.
 * 3. Null-terminate the string and reset the index.
 * 4. Rewind the file pointer to the last valid position.
 * fptr_src: File pointer to the source file.
 * str: Buffer to store the hexadecimal number.
 * Returns: 1 if the sequence is a valid hexadecimal number.
 */
int isHexa(FILE *fptr_src, char *str)
{
    /* running loop till character alpha numeric */
    while ((isalnum(ch = fgetc(fptr_src))))
    {
	if(!isxdigit(ch))
	    HEX_ERR=1;
	if( !(ch >= '0' && ch <= '7'))
	    OCT_ERR=1;
	str[idx++] = ch;
    }
    str[idx] = '\0';
    idx = 0;
    fseek(fptr_src, -1, SEEK_CUR);
    return 1;
}

/* 
 * Function: Constant
 * Extracts numeric constants from the source file.
 * 1. Read characters from the file until a non-digit is found.
 * 2. Store each digit in the str buffer.
 * 3. Null-terminate the string and reset the index.
 * 4. Rewind the file pointer to the last valid position.
 * fptr_src: File pointer to the source file.
 * str: Buffer to store the numeric constant.
 * Returns: 1 if a valid numeric constant is found.
 */
int Constant(FILE *fptr_src, char *str)
{
    int dot_count=0;
    /* run loop till alphanumeric */
    while ((isalnum(ch = fgetc(fptr_src))) || ch=='.')
    {
	/* check for valid constant numeric error */
	if ( ch != '.' && !(isdigit(ch)) && !(isOperater(ch)) && !(isFUL(ch)))
	{
	    NUM_ERR=1;
	}
	/* Count the dots in each constant*/
	if(ch == '.')
	{
	    dot_count++;
	}
	str[idx++] = ch;
    }
    /* check dot count is more then 1 then error */
    if(dot_count>1)
	DOT_ERR=1;

    str[idx] = '\0';
    idx = 0;
    fseek(fptr_src, -1, SEEK_CUR);
    return 1;
}

/* 
 * Function: isFUL
 * It will check if the present character is 'f','u','l'.
 * Returns: 1 if it is true ,if not then returns false
 */
int isFUL(char ch)
{
    if( ch == 'l' || ch =='u' || ch=='f' || ch == 'L' || ch == 'U' || ch == 'F')
	return 1;
    else 
	return 0;
}

/* 
 * Function: AlpaNumeric
 * Extracts alphanumeric keywords or identifiers from the source file.
 * 1. Read characters from the file until a non-alphanumeric character is found.
 * 2. Store each character in the str buffer.
 * 3. Null-terminate the string and reset the index.
 * 4. Rewind the file pointer to the last valid position.
 * fptr_src: File pointer to the source file.
 * str: Buffer to store the keyword or identifier.
 * Returns: 1 if a valid alphanumeric keyword or identifier is found.
 */
int AlpaNumeric(FILE *fptr_src, char *str)
{
    /* run loop till alphanumeric char */
    while ((isalnum(ch = fgetc(fptr_src))))
    {
	str[idx++] = ch;
    }
    str[idx] = '\0';
    idx = 0;
    fseek(fptr_src, -1, SEEK_CUR);
    return 1;
}

/* 
 * Function: Keyword_Checker
 * Checks if the given string is a reserved keyword, non-reserved keyword,
 * or a predefined function and prints the appropriate classification.
 * 1. Compare the string with lists of reserved keywords, non-reserved keywords, and predefined functions.
 * 2. If a match is found, print the type and return.
 * 3. If no match is found, classify the string as an identifier.
 * str: The string to be checked.
 */
void Keyword_Checker(char *str)
{
    /* Check for Reserved_Keywords */
    for (int i = 0; i < RK_len; i++)
    {
	if (strcmp(str, Reserved_Keywords[i]) == 0)
	{
	    printf(" Rev Keywords\t: %s\n", str);
	    return;
	}
    }
    /* Check for Non-Reserved_Keywords */
    for (int i = 0; i < NRK_len; i++)
    {
	if (strcmp(str, Non_Reserved_Keywords[i]) == 0)
	{
	    printf(" NR Keywords\t: %s\n", str);
	    return;
	}
    }
    /* Check for predefined functions */
    for (int i = 0; i < Pre_fun; i++)
    {
	if (strcmp(str, Predefined_functions[i]) == 0)
	{
	    printf(" Pre defined\t: %s\n", str);
	    return;
	}
    }
    /* If not any above keyword then its a Identifier */
    printf(" Identifier\t: %s\n", str);
}

int isChar(FILE *fptr_src,char *str)
{
    /* run loop till next sigle quote */
    while(ch = fgetc(fptr_src) != '\'')
    {
	str[idx++] = ch;
    }
    str[idx]='\'';
    return 1;
}
/* 
 * Function: isOperater
 * Checks if the given character is an operator.
 * 1. Compare the character with the list of operator symbols.
 * 2. Return 1 if the character is an operator, 0 otherwise.
 * ch: The character to check.
 * Returns: 1 if the character is an operator, 0 otherwise.
 */
int isOperater(uint ch)
{
    for (int i = 0; i < Op_len; i++)
    {
	if (ch == Operators[i])
	{
	    return 1;
	}
    }
    return 0;
}

/* 
 * Function: Double_oper
 * Checks if the current character and the next form a double operator.
 * 1. Read the next character from the file.
 * 2. Store both characters in the str buffer.
 * 3. Compare the string with the list of double operators.
 * 4. If a double operator is found, return 1.
 * 5. If not, rewind the file pointer to the previous position and return 0.
 * fptr_src: File pointer to the source file.
 * str: Buffer to store the operator.
 * Returns: 1 if a double operator is found, 0 otherwise.
 */
int Double_oper(FILE *fptr_src, char *str)
{
    /* check next character also oprater */
    if (isOperater(ch = fgetc(fptr_src)))
    {
	str[idx++] = ch;
    }
    str[idx] = '\0';
    idx = 0;
    for (int i = 0; i < Op2_len; i++)
    {
	if (strcmp(str, Operaters2[i]) == 0)
	{
	    idx = 0;
	    return 1;
	}
    }
    fseek(fptr_src, -1, SEEK_CUR);
    return 0;
}

/* 
 * Function: isSymbol
 * Checks if the given character is a symbol.
 * 1. Compare the character with the list of symbol characters.
 * 2. Return 1 if the character is a symbol, 0 otherwise.
 * ch: The character to check.
 * Returns: 1 if the character is a symbol, 0 otherwise.
 */

int isSymbol(uint ch)
{
    for (int i = 0; i < Sy_len; i++)
    {
	/* Check for symbol match */
	if (ch == Symbols[i])
	{
	    return 1;
	}
    }
    return 0;
}

/* 
 * Function: Bracket_Handler
 * Keeps track of bracket counts to ensure proper opening and closing.
 * 1. Increment or decrement the count for each type of bracket.
 * 2. Use an array Bracket to store the counts for round, curly, and square brackets.
 * ch: The character representing the bracket.
 */
void Bracket_Handler(uint ch)
{
    if (ch == '(')
    {
	Bracket[0]++;
    }
    else if (ch == ')')
    {
	Bracket[0]--;
    }
    else if (ch == '{')
    {
	Bracket[1]++;
    }
    else if (ch == '}')
    {
	Bracket[1]--;
    }
    else if (ch == '[')
    {
	Bracket[2]++;
    }
    else if (ch == ']')
    {
	Bracket[2]--;
    }
}
