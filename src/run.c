#include <stdio.h>
#include <stdlib.h>
#include "instructions.h"

#define STACK_SIZE 255
#define HEAP_SIZE 255
#define REG_COUNT 8
int main() {


	uint8_t code[] = {
		CONST, 6,
		CONST, 2,
		MULT,
		POP, 0,  // store the value into register 0
		PUSH, 0, // load the value from register 0
		STOREI, 1,
		LOADI, 1,

		// register way to store 10 to address 4
		CONST, 10,
		CONST, 0xff, // push the address
		POP, 4, // store the address in the 4th register
		STORE, 4,
		HALT,
	};


	printf("code: ");
	for (int i = 0; i < sizeof(code); i++) {
		printf("%02x ", code[i]);
	}
	printf("\n");
	// the instruction pointer
	// is started out at the first
	// byte byte
	uint8_t *ip = code;

	uint8_t *stack = calloc(1, STACK_SIZE);
	uint8_t *sp = stack;


	uint8_t *heap = calloc(1, HEAP_SIZE);


	// there are 8 "registers" in the language
	uint8_t *registers = calloc(1, REG_COUNT);

	// the jump_table is where the goto labels
	// are stored for threaded code exec
	void *jump_table[255];

	// set all the op jumps to nop to be safe
	for (int i = 0; i < 255; i++) {
		jump_table[i] = &&op_nop;
	}

	jump_table[ADD] = &&op_add;
	jump_table[MULT] = &&op_mult;
	jump_table[CONST] = &&op_const;
	jump_table[PRINT] = &&op_print;
	jump_table[HALT] = &&op_halt;
	jump_table[POP] = &&op_pop;
	jump_table[PUSH] = &&op_push;

	jump_table[STORE] = &&op_store;
	jump_table[LOAD] = &&op_load;

	jump_table[STOREI] = &&op_storei;
	jump_table[LOADI] = &&op_loadi;

	#define dispatch() goto *jump_table[*ip++]

	dispatch();

	// terminology;
	// pop:  *--sp
	// push: *sp++ = ...

	// the nop instruction
	op_nop: dispatch();
	op_add: {
			uint8_t a = *--sp;
			uint8_t b = *--sp;
			*sp++ = a + b;
			dispatch();
		}
	op_mult: {
			uint8_t a = *--sp;
			uint8_t b = *--sp;
			*sp++ = a * b;
			dispatch();
		}
	op_const: {
			*sp++ = *ip++;
			dispatch();
		}
	op_print: {
			printf("print: %d\n", *--sp);
			dispatch();
		}

	op_pop: {
			registers[*ip++] = *--sp;
			dispatch();
		}

	op_push: {
			*sp++ = registers[*ip++];
			dispatch();
		}


	op_store: {
			heap[registers[*ip++]] = *--sp;
			dispatch();
		}

	op_load: {
			*sp++ = heap[registers[*ip++]];
			dispatch();
		}

	op_storei: {
			heap[*ip++] = *--sp;
			dispatch();
		}

	op_loadi: {
			*sp++ = heap[*ip++];
			dispatch();
		}
	op_halt: {
			// this is a bunch of printing stuff for debugging the exit stack
			printf("\nexit stack:\n");
			printf(" stk: ");
			for (int i = 0; i < 20; i++) {
				printf("%02x ", stack[i]);
			}
			printf("\n");
			printf("  sp: ");
			for (int i = 0; i < sp - stack; i++) {
				printf("   ");
			}

			printf("^^\n");


			printf(" reg: ");
			for (int i = 0; i < 8; i++) {
				printf("%02x ", registers[i]);
			}
			printf("\n\n");

			printf("exit heap:\n");

			int width = 8;
			for (int i = 0; i < HEAP_SIZE; i += width) {
				printf("0x%02x: ", i);
				for (int o = i; o < i + width; o++) {
					printf("%02x ", heap[o]);
				}
				printf("\n");
			}
			free(registers);
			free(stack);
			exit(0);
		}

	fprintf(stderr, "exiting erroneously\n");
	free(registers);
	free(stack);
	return 1;
}
