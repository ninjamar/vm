//  this is not a stack based vm
#define GCC_COMPILER (defined(__GNUC__) && !defined(__clang__))
#ifndef NSUPRESS_WARNINGS
	#ifdef GCC_COMPILER
		#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
		#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"
		#pragma GCC diagnostic ignored "-Wint-conversion"
	#endif
	#ifdef __clang__
		#pragma clang diagnostic ignored "-Wsizeof-pointer-div"
		#pragma clang diagnostic ignored "-Wint-conversion"
		#pragma clang diagnostic ignored "-Wformat"
	#endif
#endif

#define ASIZE(x)  (sizeof(x) / sizeof((x)[0]))
#define lp (reg[LP])
#define pc (reg[PC])


#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int label_key[256];
int label_val[256];

int findkeyindex(int *arr, int value){
	for (int i=0; i<=ASIZE(arr); i++){
		if (arr[i] == value){
			return i;
		}
	}
	return -1;
}

typedef enum {
	HLT, // halt
	ADD, // add r1, r2
	SUB, // sub r1, r2
	MUL, // mul r1, r2
	DIV, // div r1, r2
	MOV,  // mov r1, int(1)
	CPY, // copy r1, r2
	RLOG, // print register
	JE, // jump if equal
	JNE, // jump if not equal
	INC, // +1
	DEC, // -1
	LBL,
	CMP,
	JMP,
} InstructionSet;
typedef enum {
	G1, G2, G3, G4, G5, G6, // general purpose - (general)
	P1, P2, P3, P4, P5, P6, // general purpose - (private)
	R1, R2, R3, R4, R5, R6, // reserved - r1=return
	PC, LOG, LP, CF,
	NUM_OF_REGISTERS
} Registers;

int haserr = 0;
int reg[NUM_OF_REGISTERS];

bool running = true;
int fetch(int *p){
	return p[pc];
}
int execinstr(int instr, int *p){
	switch(instr){
		case HLT: {
			running = false;
			if (reg[LOG]){
				printf("HALT: program halting\n");
			}
			
			break;
		}
		case ADD: {
			int a = reg[p[++pc]];
			int b = reg[p[++pc]];
			int result = a+b;
			reg[R1] = result;
			if (reg[LOG]){
				printf("ADD: Adding %d + %d to %d\n", a, b, result);
			}
			break;
		}
		case SUB: {
			int a = reg[p[++pc]];
			int b = reg[p[++pc]];
			int result = a-b;
			reg[R1] = result;
			if (reg[LOG]){
				printf("SUB: Subtracting %d - %d to %d\n", a, b, result);
			}
			break;
		}
		case MUL: {
			int a = reg[p[++pc]];
			int b = reg[p[++pc]];
			int result = a*b;
			reg[R1] = result;
			if (reg[LOG]){
				printf("MUL: Multiplying %d * %d to %d\n", a, b, result);
			}
			break;
		}
		case DIV: {
			int a = reg[p[++pc]];
			int b = reg[p[++pc]];
			int result = a / b;
			reg[R1] = result;
			if (reg[LOG]){
				printf("DIV: Dividing %d / %d to %d\n", a, b, result);
			}
			break;
		}
		case MOV: {
			int a = p[++pc];
			int b = p[++pc];
			reg[a] = b;
			if (reg[LOG]){
				printf("MOV: Setting %d to %d\n", a, b);
			}
			break;
		}
		case CPY: {
			int a = p[++pc];
			int b = p[++pc];
			reg[a] = reg[b];
			if (reg[LOG]){
				printf("CPY: Copying %d to %d\n", a, b);
			}
			break;
		}
		case RLOG: {
			int a = reg[p[++pc]];
			if (reg[LOG]){
				printf("RLOG: %d\n", a);
			} else {
				printf("%d\n", a);
			}
			break;
		}
		case JE: {
			int a = reg[p[++pc]];
			int b = reg[p[++pc]];
			int t = reg[p[++pc]];
			if (a == b){
				int c = label_val[findkeyindex(&label_key, t)];
				reg[PC] = c;
				if (reg[LOG]){
					printf("JE: Jumping to %d\n", t);
				}
			} else {
				if (reg[LOG]){
					printf("JE: Not jumping to %d \n", t);
				}
			}
			break;
		}
		case JNE: {
			int a = reg[p[++pc]];
			int b = reg[p[++pc]];
			int t = reg[p[++pc]];
			if (a != b){
				int c = label_val[findkeyindex(&label_key, t)];
				reg[PC] = c;
				if (reg[LOG]){
					printf("JNE: Jumping to %d\n", t);
				}
			} else {
				if (reg[LOG]){
					printf("JNE: Not jumping to %d\n", t);
				}
			}
			break;
		}
		case JMP: {
			int t = reg[p[++pc]];
			reg[PC] = t;
			if (reg[LOG]){
				printf("JMP: Jumping to %d", t);
			}
			break;
		}
		case CMP: {
			int a = reg[p[++pc]];
			int b = reg[p[++pc]];
			if (a == b){
				reg[CF] = 1;
			} else {
				reg[CF] = 0;
			}
			if (reg[LOG]){
				printf("CMP: Comparrision between %d and %d is %d", a, b, reg[CF]);
			}
			break;
		}
		case INC: {
			int a = p[++pc];
			++reg[a];
			if (reg[LOG]){
				printf("INC: %d\n",a);
			}
			break;
		}
		case DEC: {
			int a = p[++pc];
			--reg[a];
			if (reg[LOG]){
				printf("DEC: %d\n", a);
			}
			break;
		}
		case LBL: {
			int a = p[++pc];
			label_val[lp] = pc; // lbl 0, point to PC
			label_key[lp] = a;
			if (reg[LOG]){
				printf("LBL: %d\n", a);		
			}
			++lp;
			break;
		}
		default: {
			printf("ERROR: instruction %d wasn't found\n", instr);
			haserr = 1;
			break;
		}
	}
	return 0;
}


int execprog(int *p){
	pc = 0;
	int instr = 0;
	int i = 0;
	while (running){
		//int instr = fetch(p);
		instr = p[pc];
		execinstr(instr, p);
		pc++;
		++i;
	}
	if (haserr){
		printf("ERROR: Exiting\n");
		exit(EXIT_FAILURE);
	}
	return 0;
}