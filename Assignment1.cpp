
//Hi Ibrahim, I did this with Visual Studio and it wouldn't compile for me without putting this next line in: 
#define _CRT_SECURE_NO_WARNINGS


//My modifications are all in blocks so you can search for them and they look like: 
// 
//   /* BEGIN MODIFICATION *************************************************/
//		<MY MODIFICATIONS>
//	 /* END MODIFICATION ***************************************************/
// 
//Also see line 514, I had an issue running the post-mod file.



// -------------------------------------------
// Program: parser.cpp 
//
// Description: This is a pascal like parser
// Student: Sasha Greene
// -------------------------------------------
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

// -------------------------------------------
// These are some global definitions
// -------------------------------------------
#define TRUE		1
#define FALSE		0

#define MAX_TABLE 	500
#define MAX_SYMBOL	47
#define MAX_SYM_SZ	20

// -------------------------------------------
// These are some ERROR definitions
// -------------------------------------------
#define ERROR_SEMICOLON		1
#define ERROR_IDENT		2
#define ERROR_UNKNOWN		3
#define ERROR_ASSIGN		4
#define ERROR_ASSIGN_PROC	5
#define ERROR_PROCEDURE		6
#define ERROR_END_SYM		7
#define ERROR_DO_SYM		8
#define ERROR_THEN_SYM		9
#define ERROR_VARIABLE		10
#define ERROR_RPAREN		11
#define ERROR_IS_PROCEDURE	12
#define ERROR_PROG_SIZE		13
#define ERROR_END_PROG		14
#define ERROR_NUMBER		15
#define ERROR_LPAREN		16
#define ERROR_REL		17
#define ERROR_NUMBER_IDENT      18
#define ERROR_NOPROCEDURE       19

/* BEGIN MODIFICATION ******************************************************************************/ 

#define ERROR_UNTIL 20
#define ERROR_TODOWNTO 21
#define ERROR_OF 22
#define ERROR_NUMBERORCONST 23
#define ERROR_COLON 24

/* END MODIFICATION ******************************************************************************/

// -------------------------------------------
// some enumerated data types
// -------------------------------------------
typedef enum tag_symbol
{
	VARSYM,
	CONSTSYM,
	BEGINSYM,
	ENDSYM,
	PERIOD,
	SEMICOLON,
	COLON,
	LPAREN,
	RPAREN,
	GRTHEN,
	LSTHEN,
	GREQL,
	LSEQL,
	EQL,
	ASSIGN,
	IFSYM,
	IDENT,
	NUM,
	PROCSYM,
	NOTEQL,
	MINUS,
	PLUS,
	DIV,
	MULT,
	COMMA,
	ODDSYM,
	CALL,
	THENSYM,
	WHILESYM,
	DOSYM,
	/* BEGIN MODIFICATION ******************************************************************************/
	ELSESYM,
	REPEATSYM,
	UNTILSYM,
	FORSYM,
	TOSYM,
	DOWNTOSYM,
	CASESYM,
	WRITE,
	WRITELN,
	CEND,
	OFSYM
	/* END MODIFICATION ******************************************************************************/
}symbol;

// -------------------------------------------
typedef enum tag_Objtype
{
	NOTYPE,
	CONSTANT,
	VARIABLE,
	PROCEDURE
}Objtype;

typedef enum tag_intype
{
	ALPHA,
	DIGIT,
	EOL,
	NONE,
	PUNCT,
	SPACE
}intype;

// -------------------------------------------
typedef struct tag_symtable    // Symbols Table Structure
{
	char name[MAX_SYM_SZ];     // Symbol name
	Objtype kind;         	// Type of symbol
}symtable;

// -------------------------------------------
// These are some global variables
// -------------------------------------------
int  linelength,        // line length
linecount,         // line counter
charcount,         // a character counter
number;

symbol prevsym;         // holds the previous symbol
symtable  table[MAX_TABLE];     // table array
char   line[MAX_SYM_SZ];      // an identification string
char   punc[MAX_SYM_SZ];      // punction array
char   symstr[MAX_SYMBOL][MAX_SYM_SZ];  // symbols array

// -------------------------------------------
// These are some function prototypes
// -------------------------------------------
void   block(symbol&, int);
void   statement(symbol&, int);
void   condition(symbol&, int);
void   expression(symbol&, int);
void   term(symbol&, int);
void   factor(symbol&, int);
char   getchar(char&);
void   getsym(symbol&);
intype chartype(char  ch);
void   enter(Objtype, char[], symbol&, int&, int&);
int    position(int);
void   error(int);

// -------------------------------------------
// This handles our system errors
// -------------------------------------------
void error(int num)
{
	cout << endl;

	switch (num)
	{
	case ERROR_NOPROCEDURE:
		cout << "Procedure not accepted here";
		break;
	case ERROR_NUMBER_IDENT:
		cout << "number or ident expected";
		break;
	case ERROR_ASSIGN:
		cout << "Assignment operator expected";
		break;
	case ERROR_ASSIGN_PROC:
		cout << "Assignment not allowed here";
		break;
	case ERROR_END_PROG:
		cout << "Premature end of program";
		break;
	case ERROR_DO_SYM:
		cout << "DO symbol Expected";
		break;
	case ERROR_END_SYM:
		cout << "END symbol Expected";
		break;
	case ERROR_IDENT:
		cout << "Identifier Expected";
		break;
	case ERROR_IS_PROCEDURE:
		cout << "Assignment to PROCEDURE not allowed";
		break;
	case ERROR_NUMBER:
		cout << "A number was Expected";
		break;
	case ERROR_PROG_SIZE:
		cout << "Program size is too large...";
		break;
	case ERROR_RPAREN:
		cout << "RIGHT Parenthesis Expected";
		break;
	case ERROR_LPAREN:
		cout << "LEFT Parenthesis Expected";
		break;
	case ERROR_SEMICOLON:
		cout << "Semicolon Expected";
		break;
	case ERROR_THEN_SYM:
		cout << "THEN symbol Expected";
		break;
	case ERROR_UNKNOWN:
		cout << "Unknown Identifier";
		break;
	case ERROR_VARIABLE:
		cout << "Variable or Expression Expected";
		break;
	case ERROR_REL:
		cout << "Relational operator expected";
		break;

	/* BEGIN MODIFICATION ******************************************************************************/
	case ERROR_UNTIL:
		cout << "UNTIL expected";
		break;
	case ERROR_TODOWNTO:
		cout << "TO or DOWNTO expected";
		break;
	case ERROR_OF:
		cout << "OF expected";
		break;
	case ERROR_COLON:
		cout << "COLON expected";
		break;
	case ERROR_NUMBERORCONST:
		cout << "NUMBER or CONST expected"<<endl << "**Ibrahim, See Line 514**" << endl;
		break;
	/* END MODIFICATION ******************************************************************************/

	}
	cout << endl;
	exit(1);
}

// -------------------------------------------
// Insert Block Identifier
// -------------------------------------------
void enter(Objtype kind, char name[], symbol& sym, int& tableinx)
{
	tableinx++;
	strcpy(table[tableinx].name, name);
	table[tableinx].kind = kind;

	if (kind == CONSTANT)
	{
		if (sym != IDENT)
			error(ERROR_IDENT);
		getsym(sym);
		if (sym != EQL)
			error(ERROR_ASSIGN);
		getsym(sym);
		if (sym != NUM)
			error(ERROR_NUMBER);
		getsym(sym);
	}
	else if (kind == VARIABLE)
	{
		if (sym != IDENT)
			error(ERROR_IDENT);
		getsym(sym);
	}
	else if (kind == PROCEDURE)
		getsym(sym);
}

// -------------------------------------------
// Locate Position
// -------------------------------------------
int position(int tableinx)
{
	int i = tableinx;

	for (strcpy(table[0].name, line); strcmp(table[i].name, line) != 0; i--);

	return i;
}

// -------------------------------------------
// Block
// -------------------------------------------
void block(symbol& sym, int tableinx)
{
	if (sym == CONSTSYM)
	{
		// ---- CONSTANT SYM ----
		getsym(sym);
		enter(CONSTANT, line, sym, tableinx);

		while (sym == COMMA)
		{
			getsym(sym);
			enter(CONSTANT, line, sym, tableinx);
		}
		if (sym != SEMICOLON)
			error(ERROR_SEMICOLON);
		getsym(sym);
	}
	// ---- VARIABLE SYM ----
	if (sym == VARSYM)
	{
		getsym(sym);
		enter(VARIABLE, line, sym, tableinx);
		while (sym == COMMA)
		{
			getsym(sym);
			enter(VARIABLE, line, sym, tableinx);
		}
		if (sym != SEMICOLON)
			error(ERROR_SEMICOLON);
		getsym(sym);
	}
	// ---- PROCEDURE SYM ----
	while (sym == PROCSYM)
	{
		while (sym == PROCSYM)
		{
			getsym(sym);
			if (sym != IDENT)
				error(ERROR_IDENT);
			enter(PROCEDURE, line, sym, tableinx);
			getsym(sym);

			block(sym, tableinx);//inc static link for functions inside of functions, table current pointer

			if (sym != SEMICOLON)
				error(ERROR_SEMICOLON);
			getsym(sym);
		}
	}

	statement(sym, tableinx);
}

// -------------------------------------------
// Statement
// -------------------------------------------
void statement(symbol& sym, int tableinx)
{
	int i;

	switch (sym)
	{
		// IDENT
	case IDENT:
		i = position(tableinx);
		if (i == 0)
			error(ERROR_UNKNOWN);

		switch (table[i].kind)
		{
		case VARIABLE:
			getsym(sym);
			if (sym != ASSIGN)
				error(ERROR_ASSIGN);
			getsym(sym);
			expression(sym, tableinx);
			break;

		default:
			error(ERROR_ASSIGN_PROC);
		}
		break;

		// PROCEDURE CALL
	case CALL:
		getsym(sym);

		if (sym != IDENT)
			error(ERROR_IDENT);

		i = position(tableinx);
		if (i == 0)
			error(ERROR_UNKNOWN);

		if (table[i].kind != PROCEDURE)
			error(ERROR_PROCEDURE);

		getsym(sym);
		break;

		// BEGIN and END block
	case BEGINSYM:
		getsym(sym);
		statement(sym, tableinx);
		while (sym == SEMICOLON)
		{
			getsym(sym);
			statement(sym, tableinx);
		}
		if (sym != ENDSYM)
			error(ERROR_END_SYM);
		getsym(sym);
		break;

		// WHILE SYMBOL
	case WHILESYM:
		getsym(sym);
		condition(sym, tableinx);
		if (sym != DOSYM)
			error(ERROR_DO_SYM);
		getsym(sym);
		statement(sym, tableinx);
		break;

		// IF - THEN
	case IFSYM:
		getsym(sym);
		condition(sym, tableinx);
		if (sym != THENSYM)
			error(ERROR_THEN_SYM);
		getsym(sym);
		statement(sym, tableinx);

		/* BEGIN MODIFICATION */
		if (sym == ELSESYM) {
			getsym(sym);
			statement(sym, tableinx);
		}
		/* END MODIFICATION */
		break;

	/*BEGIN MODIFICATION*/
	// REPEAT - UNTIL
	case REPEATSYM:
		getsym(sym);
		statement(sym, tableinx);
		while (sym == SEMICOLON) {
			getsym(sym);
			statement(sym, tableinx);
		}

		if (sym == UNTILSYM) {
			getsym(sym);
			condition(sym, tableinx);
		}
		else {
			error(ERROR_UNTIL);
		}

		break;

	// FOR - TO/DOWNTO - DO
	case FORSYM:
		getsym(sym);

		if (sym != IDENT) {
			error(ERROR_IDENT);
		}

		getsym(sym);

		if (sym != ASSIGN) {
			error(ERROR_ASSIGN);
		}

		getsym(sym);

		expression(sym, tableinx);

		if (sym == TOSYM || sym == DOWNTOSYM) {
			getsym(sym);
			expression(sym, tableinx);

			if (sym == DOSYM) {
				getsym(sym);
				statement(sym, tableinx);
			}
			else {
				error(ERROR_DO_SYM); 
			}
		}
		else {
			error(ERROR_TODOWNTO);
		}

		break;

	// CASE - OF - CEND
	case CASESYM:
		getsym(sym);
		expression(sym, tableinx);

		if (sym == OFSYM) {
			getsym(sym);
			while (sym != CEND) {
				if (sym == NUM || sym == CONSTANT   /* || sym == IDENT*/) {   //NOTE For Ibrahim: I probably just misunderstood how this is supposed to work but:  I added this here to make the post-mod.pas file work if you uncomment the sym==ident part, but that will make the error case pass as well. I think the problem is that in the pas file, you have A as a variable, which wasn't mentioned in the statement briefing. Briefing: <statement> ::= CASE <expression> OF {(<number> | <constant>):<statement>;} CEND  
					getsym(sym);
					if (sym == COLON) {
						getsym(sym);
						statement(sym, tableinx);
						if (sym == SEMICOLON) {
							getsym(sym);
						}
						else {
							error(ERROR_SEMICOLON);
						}
					}
					else {
						error(ERROR_COLON);
					}
				}
				else {
					error(ERROR_NUMBERORCONST);
				}
			}
			getsym(sym);
		}
		else {
			error(ERROR_OF);
		}
		break;
	case WRITE:
		
		getsym(sym);
		if (sym != LPAREN) {
			error(ERROR_LPAREN);
		}

		getsym(sym);
		expression(sym, tableinx);

		while (sym == COMMA) {
			getsym(sym);
			expression(sym, tableinx);
		}

		if (sym != RPAREN) {
			error(ERROR_LPAREN);
		}

		getsym(sym);

		break;

	case WRITELN:

		getsym(sym);
		if (sym != LPAREN) {
			error(ERROR_LPAREN);
		}

		getsym(sym);
		expression(sym, tableinx);

		while (sym == COMMA) {
			getsym(sym);
			expression(sym, tableinx);
		}

		if (sym != RPAREN) {
			error(ERROR_LPAREN);
		}
		getsym(sym);
		
		break;

	/*END MODIFICATION*/
	}

	
}

// -------------------------------------------
// Condition
// -------------------------------------------
void condition(symbol& sym, int tableinx)
{
	// ODD symbol
	if (sym == ODDSYM)
	{
		getsym(sym);
		expression(sym, tableinx);
	}
	else
	{
		expression(sym, tableinx);
		if ((sym == EQL) || (sym == NOTEQL) || (sym == LSTHEN) || (sym == LSEQL) || (sym == GRTHEN) || (sym == GREQL))
		{
			getsym(sym);
			expression(sym, tableinx);
		}
		else {

			error(ERROR_REL);
		}
	}
}
// -------------------------------------------
// Expression
// -------------------------------------------
void expression(symbol& sym, int tableinx)
{
	if ((sym == PLUS) || (sym == MINUS))
	{
		getsym(sym);
		term(sym, tableinx);
	}
	else
		term(sym, tableinx);

	while (sym == PLUS || sym == MINUS)
	{
		getsym(sym);
		term(sym, tableinx);
	}
}

// -------------------------------------------
// TERM
// -------------------------------------------
void term(symbol& sym, int tableinx)
{
	factor(sym, tableinx);
	while ((sym == MULT) || (sym == DIV))
	{
		getsym(sym);
		factor(sym, tableinx);
	}
}

// -------------------------------------------
// FACTOR
// -------------------------------------------
void factor(symbol& sym, int tableinx)
{
	int i;

	switch (sym)
	{
		// IDENTIFER
	case IDENT:
		i = position(tableinx);
		if (i == 0)
			error(ERROR_UNKNOWN);
		if (table[i].kind == PROCEDURE)
			error(ERROR_IS_PROCEDURE);
		getsym(sym);
		break;

		// NUMBER
	case NUM:
		getsym(sym);
		break;

		// LEFT PARENTHESE
	case LPAREN:
		getsym(sym);
		expression(sym, tableinx);
		if (sym != RPAREN)
			error(ERROR_RPAREN);
		getsym(sym);
		break;
	default:
		error(ERROR_VARIABLE);
	}
}

// -------------------------------------------
// This is our GET CHARACTER function
// -------------------------------------------
char getchar(char& ch)
{
	static char line[255];           // local array
	if (charcount == linelength)
	{
		charcount = linelength = 0;       // zero out counters
		cin.get(ch);
		while (chartype(ch) != EOL && cin)
		{
			line[linelength] = ch;
			cin.get(ch);
			linelength++;
		}
		if (linelength == 0 && charcount == 0 && !cin)
			error(ERROR_END_PROG);

		line[linelength] = 0;
		cout << line << endl;

		line[linelength] = ' ';
		linecount++;                    // count lines
		linelength++;                   //
	}
	ch = toupper(line[charcount]);
	charcount++;                      // count characters
	return ch;
}

// -------------------------------------------
// This is our GETSYM
// -------------------------------------------
void getsym(symbol& sym)
{
	char ch;
	int index = 0;
	do
	{
		getchar(ch);
	} while (chartype(ch) == SPACE || chartype(ch) == EOL);

	if (chartype(ch) == ALPHA)
	{
		do
		{
			line[index++] = ch;
			getchar(ch);
		} while (chartype(ch) == ALPHA || chartype(ch) == DIGIT || ch == '_');
		line[index] = '\0';
		charcount--;
		if (strcmp(line, "BEGIN") == 0)
			sym = BEGINSYM;
		else if (strcmp(line, "CALL") == 0)
			sym = CALL;
		else if (strcmp(line, "CONST") == 0)
			sym = CONSTSYM;
		else if (strcmp(line, "DO") == 0)
			sym = DOSYM;
		else if (strcmp(line, "END") == 0)
			sym = ENDSYM;
		else if (strcmp(line, "IF") == 0)
			sym = IFSYM;
	

	/*BEGIN MODIFICATION*/

		else if (strcmp(line, "ELSE") == 0)
			sym = ELSESYM;
		else if (strcmp(line, "REPEAT") == 0)
			sym = REPEATSYM;
		else if (strcmp(line, "UNTIL") == 0)
			sym = UNTILSYM;
		else if (strcmp(line, "FOR") == 0)
			sym = FORSYM; 
		else if (strcmp(line, "TO") == 0)
			sym = TOSYM;	
		else if (strcmp(line, "DOWNTO") == 0)
			sym = DOWNTOSYM;
		else if (strcmp(line, "CASE") == 0)
			sym = CASESYM;
		else if (strcmp(line, "WRITE") == 0)
			sym = WRITE;	
		else if (strcmp(line, "WRITELN") == 0)
			sym = WRITELN;
		else if (strcmp(line, "OF") == 0) {
			sym = OFSYM;
		}
		else if (strcmp(line, "CEND") == 0) {
			sym = CEND;
		}

		/*END MODIFICATION*/


		else if (strcmp(line, "ODD") == 0)
			sym = ODDSYM;
		else if (strcmp(line, "PROCEDURE") == 0)
			sym = PROCSYM;
		else if (strcmp(line, "THEN") == 0)
			sym = THENSYM;
		else if (strcmp(line, "VAR") == 0)
			sym = VARSYM;
		else if (strcmp(line, "WHILE") == 0)
			sym = WHILESYM;
		else
		{
			sym = IDENT;
			strcpy(symstr[sym], line);
		}

		return;
	}

	if (chartype(ch) == DIGIT)
	{
		char strnum[10];
		sym = NUM;
		number = 0;
		do
		{
			strnum[index++] = ch;
			number = 10 * number + int(ch - 48);
			getchar(ch);
		} while (chartype(ch) == DIGIT);
		charcount--;
		strnum[index] = '\0';
		strcpy(symstr[sym], strnum);

		return;
	}

	if (chartype(ch) == PUNCT)
	{
		punc[index++] = ch;
		if (ch == ':' || ch == '<' || ch == '>')
		{
			getchar(ch);
			if (chartype(ch) == PUNCT && (ch == '=') || (ch == '>'))
				punc[index++] = ch;
			else
				charcount--;
		}
		punc[index] = '\0';
		if (strcmp(punc, ":=") == 0)
			sym = ASSIGN;
		else if (strcmp(punc, ":") == 0)
			sym = COLON;
		else if (strcmp(punc, ",") == 0)
			sym = COMMA;
		else if (strcmp(punc, "/") == 0)
			sym = DIV;
		else if (strcmp(punc, "=") == 0)
			sym = EQL;
		else if (strcmp(punc, ">=") == 0)
			sym = GREQL;
		else if (strcmp(punc, ">") == 0)
			sym = GRTHEN;
		else if (strcmp(punc, "(") == 0)
			sym = LPAREN;
		else if (strcmp(punc, "<=") == 0)
			sym = LSEQL;
		else if (strcmp(punc, "<") == 0)
			sym = LSTHEN;
		else if (strcmp(punc, "-") == 0)
			sym = MINUS;
		else if (strcmp(punc, "*") == 0)
			sym = MULT;
		else if (strcmp(punc, "<>") == 0)
			sym = NOTEQL;
		else if (strcmp(punc, ".") == 0)
			sym = PERIOD;
		else if (strcmp(punc, "+") == 0)
			sym = PLUS;
		else if (strcmp(punc, ")") == 0)
			sym = RPAREN;
		else if (strcmp(punc, ";") == 0)
			sym = SEMICOLON;

		strcpy(symstr[sym], punc);

		return;
	}
}

// -------------------------------------------
// determine a character type
// -------------------------------------------
intype chartype(char ch)
{
	if (ch == '\n' || ch == '\r')
		return EOL;        // character END-OF-LINE
	if (isspace(ch))
		return SPACE;      // character SPACE
	if (isdigit(ch))
		return DIGIT;      // character DIGIT
	if (isalpha(ch))
		return ALPHA;      // character ALPHA
	if (ispunct(ch))
		return PUNCT;      // character PUNCTUATION

	return NONE;
}

// -------------------------------------------
// This is our main program entry
// -------------------------------------------
int main(int argc, char* argv[])
{
	symbol sym;
	int i;
	char filen[40];

	// Initialize some variables
	linelength = 0;
	charcount = 0;
	linecount = 0;

	getsym(sym);                  // get the first symbol
	block(sym, 0);                  // start processing

	cout << "\nSuccessful parsing!\n\n";

	return 0;
}
