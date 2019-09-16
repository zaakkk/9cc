#include <assert.h>
#include <stdnoreturn.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//util.c
noreturn void error(char *fmt, ...);

typedef struct{
	void **data;
	int capacity;
	int len;
}Vector;

Vector *new_vec(void);
void vec_push(Vector *v, void *elem);

//token.c
enum{
	TK_NUM = 256,		//整数トークン
	TK_EOF,		//入力の終わりを表すトークン
};

//Token type
typedef struct{
	int ty;
	int val;
	char *input;
}Token;

Vector *tokenize(char*p);

//parse.c
enum{
	ND_NUM = 256, //Number literal
};

typedef struct Node{
	int ty;				// Node type
	struct Node *lhs;	// left-hand side
	struct Node *rhs;	// right-hand side
	int val;			// only use ND_NUM
}Node;

Node *parse(Vector *tokens);

//ir.c
enum{
	IR_IMM,
	IR_MOV,
	IR_RETURN,
	IR_KILL,
	IR_NOP,
};

typedef struct{
	int op;
	int lhs;
	int rhs;
}IR;

Vector *gen_ir(Node *node);

//regalloc.c

extern char *regs[];
void alloc_regs(Vector *irv);

//codegen.c
void gen_x86(Vector *irv);