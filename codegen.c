#include "9cc.h"

void gen_x86(Vector *irv){
	for(int i = 0; i < irv->len; i++){
		IR *ir = irv->data[i];

		switch(ir->op){
		case IR_IMM:
			printf("	mov %s, %d\n", regs[ir->lhs], ir->rhs);
			break;
		case IR_MOV:
			printf("	mov %s, %s\n", regs[ir->lhs], regs[ir->rhs]);
			break;
		case IR_RETURN:
			printf("	mov rax, %s\n", regs[ir->lhs]);
			printf("	ret\n");
			break;
		case '+':
			printf("	add %s, %s\n", regs[ir->lhs], regs[ir->rhs]);
			break;
		case '-':
			printf("	sub %s, %s\n", regs[ir->lhs], regs[ir->rhs]);
			break;
		case IR_NOP:
			break;
		default:
			assert(0 && "unknown operator");
		}
	}
}


/*
void gen(Node *node){
	if(node->kind == ND_NUM){
		printf("	push %d\n", node->val);
		return;
	}

	gen(node->lhs);
	gen(node->rhs);

	switch(node->kind){
		case ND_ADD:
			printf("	add rax, rdi\n");
			break;

		case ND_SUB:
			printf("	sub rax, rdi\n");
			break;

		case ND_MUL:
			printf("	imul rax, rdi\n");
			break;

		case ND_DIV:
			printf("	cqo\n");
			printf("	idiv rdi\n");
			break;
		
		case ND_EQ:
			printf("	cmp rax, rdi\n");
			printf("	sete al\n");
			printf("	movzb rax, al\n");
			break;

		case ND_NE:
			printf("	cmp rax, rdi\n");
			printf("	setne al\n");
			printf("	movzb rax, al\n");
			break;

		case ND_LT:
			printf("	cmp rax, rdi\n");
			printf("	setl al\n");
			printf("	movzb rax, al\n");
			break;

		case ND_LE:
			printf("	cmp rax, rdi\n");
			printf("	setle al\n");
			printf("	movzb rax, al\n");
			break;
	}

	printf("	pop rdi\n");
	printf("	pop rax\n");

	

	printf("	push rax\n");
}

*/
