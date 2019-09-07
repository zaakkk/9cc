#include "9cc.h"

//
//Tokenizer
//

//Token kind
/*
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
*/


//Reports an error and exit.
void error(char *fmt, ...){
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	exit(1);
}

//Reports an error location and exit.
void error_at(char *loc, char *fmt, ...){
	va_list ap;
	va_start(ap, fmt);

	int pos = loc - user_input;
	fprintf(stderr, "%s\n", user_input);
	fprintf(stderr, "%*s", pos, ""); //print pos spaces.
	fprintf(stderr, "^ ");
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	exit(1);
}

//Consumes the current token if it matches 'op'.
bool consume(char *op){
	if(token->kind != TK_RESERVED || 
		strlen(op) != token->len ||
		memcmp(token->str, op, token->len)){
		return false;
	}
	token = token->next;
	return true;
}

//Ensure that the current token is 'op'.
void expect(char *op){
	if(token->kind != TK_RESERVED ||
		strlen(op) != token->len ||
		memcmp(token->str, op, token->len)){
		error_at(token->str, "expected \"%s\"", op);
	}
	token = token->next;
}

//Ensure that the current token is TK_NUM.
int expect_number(){
	if(token->kind != TK_NUM){
		error_at(token->str, "expected a number");
	}
	int val = token->val;
	token = token->next;
	return val;
}

bool at_eof(){
	return token->kind == TK_EOF;
}

//Create a new token and add it as the next token of 'cur'.
Token *new_token(TokenKind kind, Token *cur, char *str, int len){
	Token *tok = calloc(1, sizeof(Token));
	tok->kind = kind;
	tok->str = str;
	cur->next = tok;
	tok->len = len;
	return tok;
}

bool startswith(char *p, char *q){
	return memcmp(p, q, strlen(q)) == 0;
}

//Tokenize 'user_input' and returns new tokens.
Token *tokenize(){
	char *p = user_input;
	Token head;
	head.next = NULL;
	Token *cur = &head;

	while(*p){
		//Skip whitespace characters.
		if(isspace(*p)){
			p++;
			continue;
		}
		
		//Multi-letter punctuator
		if(startswith(p, "==") || startswith(p, "!=") ||
			startswith(p, "<=") || startswith(p, ">=")){
				cur = new_token(TK_RESERVED, cur, p, 2);
				p += 2;
				continue;
			}

		//Single-letter punctuator
		if(strchr("+-*/()<>", *p)){
			cur = new_token(TK_RESERVED, cur, p++, 1);
			continue;
		}
	
		//Integer literal
		if(isdigit(*p)){
			cur = new_token(TK_NUM, cur, p, 0);
			char *q = p;
			cur->val = strtol(p, &p, 10);
			cur->len = p - q;
			continue;
		}

		error_at(p, "invalid token");
	}

	new_token(TK_EOF, cur, p, 0);
	return head.next;
}

//
//Parser
//

/*

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

*/

Node *new_node(NodeKind kind){
	Node *node = calloc(1, sizeof(Node));
	node->kind = kind;
	return node;
}

Node *new_binary(NodeKind kind, Node *lhs, Node *rhs){
	Node *node = new_node(kind);
	node->lhs = lhs;
	node->rhs = rhs;
	return node;
}

Node *new_num(int val){
	Node *node = new_node(ND_NUM);
	node->val = val;
	return node;
}

Node *expr();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();

//expr = equality()
Node *expr(){
	return equality();
}

//equality = relational ("==" relational | "!=" relational)*
Node *equality(){
	Node *node = relational();

	for(;;){
		if(consume("==")){
			node = new_binary(ND_EQ, node, relational());
		}else if(consume("!=")){
			node = new_binary(ND_NE, node, relational());
		}else{
			return node;
		}
	}
}

//relational = add("<" add | "<=" add | ">" add | ">=" add)*
Node *relational(){
	Node *node = add();

	for(;;){
		if(consume("<")){
			node = new_binary(ND_LT, node, add());
		}else if(consume("<=")){
			node = new_binary(ND_LE, node, add());
		}else if(consume(">")){
			node = new_binary(ND_LT, add(), node);
		}else if(consume(">=")){
			node = new_binary(ND_LE, add(), node);
		}else{
			return node;
		}
	}
}

//add = mul("+" mul | "-" mul)*
Node *add(){
	Node *node = mul();

	for(;;){
		if(consume("+")){
			node = new_binary(ND_ADD, node, mul());
		}else if(consume("-")){
			node = new_binary(ND_SUB, node, mul());
		}else{
			return node;
		}
	}
}

//mul = unary("*" unary | "/" unary)*
Node *mul(){
	Node *node = unary();

	for(;;){
		if(consume("*")){
			node = new_binary(ND_MUL, node, unary());
		}else if(consume("/")){
			node = new_binary(ND_DIV, node, unary());
		}else{
			return node;
		}
	}
}

//unary = ("+" | "-")? unary | primary
Node *unary(){
	if(consume("+")){
		return unary();
	}
	if(consume("-")){
		return new_binary(ND_SUB, new_num(0), unary());
	}
	return primary();
}


//primary = "(" expr ")" | num
Node *primary(){
	// if next token is '(', "(" expr ")"
	if(consume("(")){
		Node *node = expr();
		expect(")");
		return node;
	}

	// if not, return number
	return new_num(expect_number());
}