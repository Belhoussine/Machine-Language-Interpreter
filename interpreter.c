#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char sign_bit;
    unsigned short operation, operand1, operand2;
    char operands_type;
} instruction;

//=================START-GLOBAL-VARIABLES==================
//=========================================================

instruction CODE_MEMORY[10000];
unsigned short total_instructions = 0;

//=========================================================
//==================END-GLOBAL-VARIABLES===================

// --------------------------------------------------------

//=====================START-FUNCTIONS=====================
//=========================================================

// Parse lines into tokens
instruction tokenize(char *line)
{
    // A line looks like: +0243194875
    instruction inst;
    char op1[5], op2[5];

    // Isolate operand1 and operand2 substrings
    memcpy(op1, line + 2, 4);
    memcpy(op2, line + 6, 4);

    // Fill instruction structure 
    inst.sign_bit = line[0] == '+' ? 1 : -1;
    inst.operation = line[1] - '0';
    inst.operand1 = atoi(op1);
    inst.operand2 = atoi(op2);
    inst.operands_type = line[10] - '0';

    return inst;
}

// Displaying instruction
void display(instruction inst)
{
    printf("Sign bit: %d \t- ", inst.sign_bit);
    printf("OpCode: %d - ", inst.operation);
    printf("Op1: %d - ", inst.operand1);
    printf("Op2: %d - ", inst.operand2);
    printf("OpType: %d\n", inst.operands_type);
}

// Read file and store instructions in RAM (code section)
void read_source_file(FILE *infp)
{
    char line[50];

    // Read line by line
    while (fgets(line, sizeof(line), infp) != NULL)
    {
        CODE_MEMORY[total_instructions] = tokenize(line);
        display(CODE_MEMORY[total_instructions]);
        total_instructions++;
    }
}

// Execute instructions in RAM
void execute_instructions()
{
    return;
}

//=========================================================
//======================END-FUNCTIONS======================

int main()
{
    // Open source file (Machine Language)
    FILE *infp = fopen("source.nml", "r");
    if (infp == NULL)
    {
        printf("File not found.");
        return 1;
    }

    read_source_file(infp);

    execute_instructions();

    return 0;
}