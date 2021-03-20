#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int sign_bit, operation, operand1, operand2;
} instruction;

instruction CODE_MEMORY[10000];

instruction tokenize(char *line)
{
    // A line looks like: +0243194875
    instruction inst;
    char op1[4], op2[4];

    // Isolation operands substrings
    memcpy(op1, line + 2, 3);
    memcpy(op2, line + 5, 3);

    // Filling instruction
    inst.sign_bit = line[0] == '+' ? 1 : 0;
    inst.operation = line[1] - '0';
    inst.operand1 = atoi(op1);
    inst.operand2 = atoi(op2);

    return inst;
}

// Displaying instruction
void display(instruction inst)
{
    printf("Sign bit: %d - OpCode: %d - Op1: %d - Op2: %d \n", inst.sign_bit, inst.operation, inst.operand1, inst.operand2);
}

int main()
{
    // Initializing variables
    char line[50];
    int i = 0;

    // Opening source Machine language file for reading
    FILE *infp = fopen("source_ML.txt", "r");
    if (infp == NULL)
    {
        printf("File not found.");
        return 1;
    }

    // Reading file line by line
    while (fgets(line, sizeof(line), infp) != NULL)
    {
        // Parsing line and storing the instruction in code part of RAM
        CODE_MEMORY[i] = tokenize(line);
        display(CODE_MEMORY[i]);
        i++;
    }

    return 0;
}