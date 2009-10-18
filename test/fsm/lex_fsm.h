#include <stdio.h>

// General functions    
inline void fire_event(int event, char c);

typedef void (*fsm_action)(char c);

typedef enum {
    REST,
    COMMENT,
    WORD,
    STRING,
	DELIMITER_WAIT,
	HARD_STRING,
    LastState
} lex_state;

typedef enum { 
	SPACE,
	QUOTE,
	BACKSLASH,
	CHARACTER,
	PAREN_OPEN,
	PAREN_CLOSE,
	END_STR,
	LastEvent
} lex_event;

typedef struct {    
	fsm_action action;         // function-pointer to the action
	lex_state next_state;            // Enumerator for the next state    
} fsm_trans;

typedef enum {
	UNKNOWN,
	NO_TOK,
	WORD_TOK,
	STR_TOK,
	INVALID_TOK
} token_type;

typedef union {
	int integer;
	char *string;
} token_value;

typedef struct {
	token_type type;
	token_value value;
} token;

token return_token = { UNKNOWN, {0} };

fsm_trans trans_table[LastState][LastEvent];
int currentState = REST;

char token_buffer[128];
char *tok_scanp = token_buffer;



// Declare action functions
void comment(char e);    
void add_to_buffer(char e);    
void no_op(char e);
void no_token(char e);
void end_token(char e);
void set_delimiter(char e);

void illegal_event(char c) {
	puts("illegal event");
}

fsm_trans transition(fsm_action a, int next) {
		fsm_trans trans = { a, next};
		return trans;
}

void init_trans_table(fsm_trans table[][LastEvent]) {
	int i, j;
	for( i = 0; i < LastState; ++i) {
		for(j = 0; j < LastEvent; ++j) {
			fsm_trans trans = { illegal_event, i};
			table[i][j] = trans;
		}
	}
	
	table[REST][SPACE] = 				transition(no_op, REST);
	table[REST][CHARACTER] = 			transition(add_to_buffer, WORD_TOK);
	table[REST][PAREN_OPEN] = 			transition(comment, COMMENT);
	table[REST][END_STR] = 				transition(no_token, REST);
	table[REST][QUOTE] = 				transition(no_op, STRING);
	table[REST][BACKSLASH] = 			transition(no_op, DELIMITER_WAIT);
	                                	
	table[WORD_TOK][CHARACTER] = 		transition(add_to_buffer, WORD_TOK);
	table[WORD_TOK][SPACE] = 			transition(end_token, REST);
	                                	
	table[COMMENT][PAREN_CLOSE] = 		transition(no_op, REST);
	table[COMMENT][END_STR] = 			transition(no_op, COMMENT);
	                                	
	table[STRING][QUOTE] = 				transition(end_token, REST);
	table[DELIMITER_WAIT][CHARACTER] = 	transition(set_delimiter, HARD_STRING);
}

void lex(char **cp) {
	char *scanp = *cp;
	
	// reset return_token
	return_token.type = UNKNOWN;
	tok_scanp = token_buffer;
	
	while (return_token.type == UNKNOWN) {
		switch (*scanp) {
			case ' ':	fire_event(SPACE, *scanp);
				break;
			case '(':	fire_event(PAREN_OPEN, *scanp);
				break;
			case ')':	fire_event(PAREN_CLOSE, *scanp);
				break;
			case '\0':	fire_event(END_STR, *scanp);
				break;
			default:	fire_event(CHARACTER, *scanp);
				break;
		}
		scanp++;
	}
	
	*cp = scanp;
}


void fire_event(int event, char c)
{
	fsm_trans transition = trans_table[currentState][event];
	currentState = transition.next_state;
	transition.action(c);
}



// action definitions
void comment(char c) {
	
}

void set_delimiter(char c) {
	
}

void no_token(char c) {
	return_token.type = NO_TOK;
}

void add_to_buffer(char c) {
	*tok_scanp++ = c;
}
void no_op(char c) {
	
}
void end_token(char c) {
	return_token.type = WORD;
}
