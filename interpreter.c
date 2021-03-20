#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct
{
    char sign_bit;
    unsigned short operation, operand1, operand2;
    char operands_type;
} instruction;

//=================START-GLOBAL-VARIABLES==================
//=========================================================

instruction CODE_MEMORY[10000];
int DATA_MEMORY[10000];
unsigned short total_instructions = 0, IP = 0, VERBOSE = 1;
int ACC;

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

//=================START-LOG-FUNCTIONS==================
//=========================================================
// assign log
void assign_log(instruction inst)
{
    printf("Storing %d in %d \n", inst.operand1, inst.operand2);
}

// add_subtract_log
void add_subtract_log(instruction inst)
{
    if (inst.sign_bit == 1)
        printf("Adding %d to %d", inst.operand1, inst.operand2);
    else
        printf("Subtracting %d from %d", inst.operand1, inst.operand2);
}
// multiply_divide_log

void multiply_divide_log(instruction inst)
{
    if (inst.sign_bit == 1)
        printf("Multiplying %d by %d", inst.operand1, inst.operand2);
    else
        printf("Dividing %d by %d", inst.operand1, inst.operand2);
}

//=========================================================
//==================END-LOG-FUNCTIONS===================

//toLiteral

instruction to_literal(instruction inst)
{
    instruction literal_inst = inst;
    if (inst.operands_type == 0)
    {
        literal_inst.operand1 = DATA_MEMORY[inst.operand1];
        literal_inst.operand2 = DATA_MEMORY[inst.operand2];
    }
    else if (inst.operands_type == 1)
    {
        literal_inst.operand1 = DATA_MEMORY[inst.operand1];
        literal_inst.operand2 = inst.operand2;
    }
    else if (inst.operands_type == 2)
    {
        literal_inst.operand2 = DATA_MEMORY[inst.operand2];
        literal_inst.operand1 = inst.operand1;
    }

    return literal_inst;
}

// assign
void assign(instruction inst)
{
    instruction literal_inst = to_literal(inst);
    if (inst.sign_bit == 1)
    {
        //check operands_type and store in data memory
        DATA_MEMORY[inst.operand2] = literal_inst.operand1;
    }
    if (VERBOSE)
    {
        assign_log(inst);
    }
}

// add_subtract
void add_subtract(instruction inst)
{
    instruction literal_inst = to_literal(inst);
    if (inst.sign_bit == 1)
    {

        ACC = literal_inst.operand1 + literal_inst.operand2;
    }
    else
    {
        ACC = literal_inst.operand1 - literal_inst.operand2;
    }
    if (VERBOSE)
        add_subtract_log(inst);
}

// multiply_divide
void multiply_divide(instruction inst)
{
    instruction literal_inst = to_literal(inst);
    if (inst.sign_bit == 1)
    {
        ACC = literal_inst.operand1 * literal_inst.operand2;
    }
    else
    {
        ACC = literal_inst.operand1 / literal_inst.operand2;
    }

    if (VERBOSE)
        multiply_divide_log(inst);
}

// square_squareRoot
void square_squareRoot(instruction inst)
{
    instruction literal_inst = to_literal(inst);
    if (inst.sign_bit == 1)
    {
        DATA_MEMORY[inst.operand2] = literal_inst.operand1 * literal_inst.operand1;
    }
    else
    {
        DATA_MEMORY[inst.operand2] = sqrt(literal_inst.operand1);
    }
    //add log fcn
}

// equals_notEquals
void equals_notEquals(instruction inst)
{
    instruction literal_inst = to_literal(inst);
    if (literal_inst.operand1 == literal_inst.operand2)
        ACC = 1;
    else
        ACC = 0;
    // add log fcn
}

// greaterOrEquals_lessThan
void greaterOrEquals_lessThan(instruction inst)
{
    instruction literal_inst = to_literal(inst);
    if(literal_inst.operand1 >= literal_inst.operand2)
        ACC = 1;
    else
        ACC = 0;
    // add log fcn
}
// Execute a single instruction in RAM
void execute_instruction(instruction inst)
{
    switch (inst.operation)
    {
    case 0:
        assign(inst);
        break;
    case 1:
        add_subtract(to_literal(inst));
        break;
    case 2:
        multiply_divide(to_literal(inst));
        break;
    case 3:
        square_squareRoot(inst);
        break;
    case 4:
        equals_notEquals(to_literal(inst));
        break;
    case 5:
        greaterOrEquals_lessThan(to_literal(inst));
        break;
    case 6:
        //arrToVar_vatToArr(inst)
    case 7:
        //loop_label(inst)
    case 8:
        //read_print(inst)
    case 9:
        //stop(inst)
    }
}
// Execute instructions in RAM
void execute_instructions()
{
    int IP;
    for (IP = 0; IP < total_instructions; IP++)
    {
        execute_instruction(CODE_MEMORY[IP]);
    }
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