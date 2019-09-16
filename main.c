#include "9cc.h"
int main(int argc, char **argv){
	if(argc != 2){
		error("%s:, invalid number of arguments", argv[0]);
	}
		
	// Tokenize and parse.
	Vector *tokens = tokenize(argv[1]);
	Node *node = parse(tokens);

	Vector *irv = gen_ir(node);
	alloc_regs(irv);

	//Print out the first half of assembly.
	printf(".intel_syntax noprefix\n");
	printf(".global main\n");
	printf("main:\n");

	gen_x86(irv);

	/*
	//Traverse the ASY to emit assembly.
	gen(node);

	//A result must be at the top of the stack, so pop it
	// to RAX to make it a program exit code.
	printf("	pop rax\n");
	printf("	ret\n");
	*/
	
	return 0;
		
}