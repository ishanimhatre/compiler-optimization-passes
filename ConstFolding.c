/*
 *********************************************
 *  314 Principles of Programming Languages  *
 *  Spring 2023                              *
 *  Student Version                          *
 *  Author: Ishani Mhatre                    *
 *********************************************
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "InstrUtils.h"
#include "Utils.h"

/*
* Will traverse instructions in groups of 3, searching for a patten of LOADI, LOADI, and ADD | SUB | MUL instructions 
*/

Instruction *constant_folding(Instruction *head) {

	Instruction *first;
 	Instruction *second;
 	Instruction *third;

	Instruction *instr = head;

	do {
		first = instr;
		second = instr->next;
		third = second->next;

		if (first->opcode == LOADI && second->opcode == LOADI) {
		
			switch (third->opcode) {
				case ADD:
			
					if ((first->field2 == third->field1 && second->field2 == third->field2) || (first->field2 == third->field2 && second->field2 == third->field1)) {
						
						 third->field1=first->field1 + second->field1;
						 third->opcode=LOADI;
						 third->field2=third->field3;
						 third->field3=0;

					}		
					break;
				case MUL:
					if ((first->field2 == third->field1 && second->field2 == third->field2) || (first->field2 == third->field2 && second->field2 == third->field1)) {
						
						 third->field1=first->field1 * second->field1;
						 third->opcode=LOADI;
						 third->field2=third->field3;
						 third->field3=0;

					}		
					break;		
				case SUB:
					if ((first->field2 == third->field1 && second->field2 == third->field2) || (first->field2 == third->field2 && second->field2 == third->field1)) {
						third->field1=first->field1 - second->field1;
						 third->opcode=LOADI;
						 third->field2=third->field3;
						 third->field3=0;


					}
					break;
				default:

					break;
			}
		}
		instr = instr->next;
	} while (instr != NULL && instr->next != NULL && instr->next->next != NULL);

 		

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

	head = constant_folding(head);

	if (head) 
		PrintInstructionList(stdout, head);
	
	return EXIT_SUCCESS;
}

