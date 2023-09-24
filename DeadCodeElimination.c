/*
 *********************************************
 *  314 Principles of Programming Languages  *
 *  Spring 2023                              *
 *  Student Version                          *
 *  Author: Ishani Mhatre                   *
 *********************************************
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "InstrUtils.h"
#include "Utils.h"

typedef struct requiredInstruct {
	int f1;
	int f2;
	OpCode opcode;
	struct requiredInstruct* next;
} RequiredInstruct;

int exists(RequiredInstruct* head, int field1, int field2) {
	if (head == NULL) return 0;
	RequiredInstruct* ptr = head;
	while (ptr != NULL) {
		if (ptr->f1 == field1 && ptr->f2 == field2 ) {
			
			return 1;
		}
		ptr = ptr->next;
	}
	return 0;
}



void add(RequiredInstruct* head, int field1, int field2, OpCode op) {
	if (head == NULL) return;
	if (exists(head, field1, field2) == 1) return;
	RequiredInstruct* ptr = (RequiredInstruct*)malloc(sizeof(RequiredInstruct));
	if (ptr == NULL) exit(1);
	ptr->f1 = field1;
	ptr->f2 = field2;
	ptr->opcode = op;
	ptr->next = NULL;
	while (head->next != NULL) {
		head = head->next;
	}
	head->next = ptr;
}


void removeInstr(Instruction * ptr) {
	if (ptr == NULL) return;
	Instruction* prev_instr_ptr = ptr->prev;
	Instruction* next_instr_ptr = ptr->next;
	if (prev_instr_ptr != NULL) {
		prev_instr_ptr->next = next_instr_ptr;
	}
	if (next_instr_ptr != NULL) {
		next_instr_ptr->prev = prev_instr_ptr;
	}
	ptr->next = NULL;
	ptr->prev = NULL;
	free(ptr);
}

Instruction *dead_code_elimination(Instruction *head) {

		/* STUDENTS - BEGIN */
		


	
	Instruction* instr_ptr = LastInstruction( head );
		Instruction* prev_instr_ptr = NULL;
		RequiredInstruct* chead = NULL;
		
		chead = (RequiredInstruct*)malloc(sizeof(RequiredInstruct));

		chead->f1 = 0;
		chead->f2 = -1;
		chead->next = NULL;
			
		while (instr_ptr != NULL) {
				prev_instr_ptr = instr_ptr->prev;
				//outputAI
				if (instr_ptr->opcode == OUTPUTAI) {
				//printf("OUTPUTAI f1=%d,f2= %d, f3=%d\n" ,instr_ptr->field1,instr_ptr->field2,instr_ptr->field3 );
				
					add(chead, instr_ptr->field1, instr_ptr->field2, instr_ptr->opcode);
				}
				//loadI
				else if (instr_ptr->opcode == LOADI) {
					if (exists(chead, instr_ptr->field2, -1) == 0) {
						instr_ptr->critical = 0;
						removeInstr(instr_ptr);
					}
				}
				//loadAI
				else if (instr_ptr->opcode == LOADAI || instr_ptr->opcode == LSHIFTI || instr_ptr->opcode == RSHIFTI) {
					if (exists(chead, instr_ptr->field3, -1) == 0) {
						instr_ptr->critical = 0; 
						removeInstr(instr_ptr);
					}
					else {
						add(chead, instr_ptr->field1, -1, instr_ptr->opcode);
						add(chead, instr_ptr->field1, instr_ptr->field2, instr_ptr->opcode);
					}
				}
				//storeAI
				else if (instr_ptr->opcode == STOREAI) {
				//printf("storeai f1=%d,f2= %d, f3=%d\n" ,instr_ptr->field1,instr_ptr->field2,instr_ptr->field3 );
					if (exists(chead, instr_ptr->field2, instr_ptr->field3) == 0) {
						instr_ptr->critical = 0;
						removeInstr(instr_ptr);
					}
					else {
						add(chead, instr_ptr->field2, -1, instr_ptr->opcode);
						add(chead, instr_ptr->field1, -1, instr_ptr->opcode);
					}
				}
				//add
				else if (instr_ptr->opcode == ADD || instr_ptr->opcode == SUB || instr_ptr->opcode == MUL || instr_ptr->opcode == DIV) {
					if (exists(chead, instr_ptr->field3, -1) == 0) {
						instr_ptr->critical = 0;
						removeInstr(instr_ptr);
					}
					else {
						add(chead, instr_ptr->field1, -1, instr_ptr->opcode);
						add(chead, instr_ptr->field2, -1, instr_ptr->opcode);
					}
				}
				//other
				else {}
				instr_ptr = prev_instr_ptr;
			}
		

	RequiredInstruct* cptrhead = chead;
	RequiredInstruct* cdelptr = chead;
	
	while (cptrhead != NULL) {
		
		cdelptr = cptrhead;
		cptrhead = cptrhead->next;
		free(cdelptr);
	}
 		/* STUDENTS - END */

	return head;
}

int main()
{
	Instruction *head;

	head = ReadInstructionList(stdin);
	if (!head) {
		WARNING("No instructions\n");
		exit(EXIT_FAILURE);
	}

	head = dead_code_elimination(head);

	if (head) 
		PrintInstructionList(stdout, head);
	
	return EXIT_SUCCESS;
}

