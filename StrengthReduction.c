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
#include <math.h>
#include "InstrUtils.h"
#include "Utils.h"

/*
* Will return log2(n) if it is an integer (meaning n is a power of 2), and -1 otherwise
*/
int int_log2(int n) {

	int s = 1, i = 0;

	while (s < n) {
		s = s << 1;
		i++;
	}

	if (s == n)
		return i;
	else
		return -1;

}

/*
* Will traverse instructions in groups of two, searching for a patten of LOADI and MUL | DIV instructions 
* where LOADI is a power of 2, and collapse these instructions into on shift operation of LSHIFTI | RSHIFTI
*/
Instruction *strength_reduction(Instruction *head) {

		/* STUDENTS - BEGIN */
	Instruction *first, *second, *currentinstr;
	currentinstr = head;

	if (currentinstr->next != NULL)
		first = currentinstr->next;
	if (currentinstr->next != NULL)
		second = currentinstr->next->next;

	while (second != NULL)
	{

		if (first->opcode == LOADI && second->opcode == MUL)
		{
			if (first->field1 % 2 == 0)
			{
				second->opcode = LSHIFTI;
				second->field2 = int_log2(first->field1);
				
			}
		} else if (first->opcode == LOADI && second->opcode == DIV)
		{
			if (first->field1 % 2 == 0)
			{
				second->opcode = RSHIFTI;
				second->field2 = int_log2(first->field1);
			} 
		} else {
			first = second;
			second = first->next;
		}
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

	head = strength_reduction(head);

	if (head) 
		PrintInstructionList(stdout, head);
	
	return EXIT_SUCCESS;
}

