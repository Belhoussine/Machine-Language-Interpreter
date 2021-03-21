#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char operation[5], operand1[6], operand2[6], operand1_type, operand2_type;
} instruction;

//=================START-GLOBAL-VARIABLES==================
//=========================================================

instruction ASSEMBLY_CODE[10000];
unsigned short total_instructions = 0;
char instructions[20][5] = {"ASN",  "ASN", "ADD",  "SUB",  "MUL", "DIV", "SQR",
                            "SQRT", "EQL", "NEQL", "GOE",  "LT", "ATV", "VTA",
                            "JMP",  "LBL", "READ", "PRNT", "STOP"};

//=========================================================
//==================END-GLOBAL-VARIABLES===================

// --------------------------------------------------------

//=====================START-FUNCTIONS=====================
//=========================================================

// Convert from assembly operation to opCode
int operation_to_opcode(char *operation) {
    for (int i = 0; i < 19; i++) {
        if (strcmp(operation, instructions[i]) == 0) {
            return (i % 2) ? -(i / 2) : (i / 2);
        }
    }
}

// Get operand type digit from individual operand types (Literal or Address)
int get_operands_type(instruction inst) {
    char op1_type = inst.operand1_type, op2_type = inst.operand2_type;

    if (strcmp(inst.operand1, "AAAA") == 0) {
        return 7;
    }

    if (strcmp(inst.operand2, "AAAA") == 0) {
        if (inst.operand1_type == 'A') {
            return 8;
        } else {
            return 9;
        }
    }

    if (op1_type == op2_type) {
        if (op1_type == 'A') {
            return 0;
        } else {
            return 3;
        }
    } else {
        if (op1_type == 'A') {
            return 1;
        } else {
            return 2;
        }
    }
}

// Parse lines into tokens
instruction parse(char *line) {
    // A line looks like: ASN L0005 A1000
    instruction inst;
    char op1[6], op2[6];

    // Divide line into substrings substrings
    sscanf(line, "%s %s %s", inst.operation, op1, op2);

    memcpy(inst.operand1, op1 + 1, 4);
    memcpy(inst.operand2, op2 + 1, 4);

    inst.operand1_type = op1[0];
    inst.operand2_type = op2[0];

    return inst;
}

// Displaying instruction
void display(instruction inst) {
    printf("OP: %s - ", inst.operation);
    printf("Op1: %s - ", inst.operand1);
    printf("Op2: %s - ", inst.operand2);
    printf("Op1Type: %c - ", inst.operand1_type);
    printf("Op2Type: %c\n", inst.operand2_type);
}

// Read file and store instructions in RAM (code section)
void read_source_file(char *file_name) {
    char line[50];
    // Open source file (Machine Language)
    FILE *infp = fopen(file_name, "r");
    if (infp == NULL) {
        printf("File not found.");
        exit(1);
    }

    // Read line by line
    while (fgets(line, sizeof(line), infp) != NULL) {
        ASSEMBLY_CODE[total_instructions] = parse(line);
        // display(ASSEMBLY_CODE[total_instructions]);
        total_instructions++;
    }
    fclose(infp);
}

// Translate assembly instruction to machine language instruction
void translate_instruction(instruction inst, FILE *outfp) {
    int opcode = operation_to_opcode(inst.operation);
    char op1[5], op2[5];

    strcpy(op1, inst.operand1);
    strcpy(op2, inst.operand2);

    // If AAAA is in of the operands, this means that the accumulator will be
    // used
    if (strcmp(op1, "AAAA") == 0) {
        strcpy(op1, "0000");
    }
    if (strcmp(op2, "AAAA") == 0) {
        strcpy(op2, "0000");
    }

    if (opcode >= 0) {
        fprintf(outfp, "+");
    }
    fprintf(outfp, "%d%s%s%d\n", opcode, op1, op2, get_operands_type(inst));
}

// Translates from assembly to machine language
void translate_instructions(char *file_name) {
    FILE *outfp = fopen(file_name, "w");
    int i;
    for (i = 0; i < total_instructions; i++) {
        translate_instruction(ASSEMBLY_CODE[i], outfp);
    }
    fclose(outfp);
}

//=========================================================
//======================END-FUNCTIONS======================

int main(int argc, char *argv[]) {
    // Getting command line arguments
    if (argc != 2) {
        printf("Error: you must input a source AL file.\n");
        printf("[Usage]:\n\t %s <source_AL_file>\n", argv[0]);
        return 1;
    }

    read_source_file(argv[1]);

    translate_instructions("source.nml");
    return 0;
}