#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Token
//Token kind
typedef enum{
	TK_RESERVED,	//記号
	TK_NUM,		//整数トークン
	TK_EOF,		//入力の終わりを表すトークン
}TokenKind;

//Token type
typedef struct Token Token;
struct Token{
	TokenKind kind;	//トークンの型
	Token *next;	//次の入力トークン
	int  val;	//kindがTK_NUMの場合、その数値
	char *str;	//トークン文字列
	int len;	//token length
};

//Input program
char *user_input;

//Current token
Token *token;

void error(char *fmt, ...);
Token *tokenize();


//Node
//抽象構文木のノードの種類
typedef enum{
	ND_ADD, // +
	ND_SUB, // -
	ND_MUL, // *
	ND_DIV, // /
	ND_EQ,	// ==
	ND_NE,	// !=
	ND_LT,	// <
	ND_LE,	// <=
	ND_NUM,	// Integer
} NodeKind;

typedef struct Node Node;

//抽象構文木のノードの型
struct Node{
	NodeKind kind;	// Node type
	Node *lhs;	// left-hand side
	Node *rhs;	// right-hand side
	int val;	// only use ND_NUM
};

Node *expr();

//
void gen(Node *node);