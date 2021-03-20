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
// Log info od assign function
void assign_log(instruction inst)
{
    printf("Storing %d in %d \n", inst.operand1, inst.operand2);
}

// Log info of add_subtract function
void add_subtract_log(instruction inst)
{
    if (inst.sign_bit == 1)
        printf("Adding %d to %d", inst.operand1, inst.operand2);
    else
        printf("Subtracting %d from %d", inst.operand1, inst.operand2);
}

// Log info of multiply_divide function
void multiply_divide_log(instruction inst)
{
    if (inst.sign_bit == 1)
        printf("Multiplying %d by %d", inst.operand1, inst.operand2);
    else
        printf("Dividing %d by %d", inst.operand1, inst.operand2);
}

// Log info of square_squareRoot function
void square_squareRoot_log(instruction inst)
{
    if (inst.sign_bit == 1)
        printf("Square of %d stored in %d", inst.operand1, inst.operand2);
    else
        printf("Square root of %d stored in %d", inst.operand1, inst.operand2);
}

// Log info of equals_notEquals function
void equals_notEquals_log(instruction inst)
{
    if (inst.sign_bit == 1)
        printf("Compare %d to %d", inst.operand1, inst.operand2);
    else
        printf("Compare %d to %d", inst.operand1, inst.operand2);
}

// Log info of square_squareRoot function
void greaterOrEquals_lessThan_log(instruction inst)
{
    if (inst.sign_bit == 1)
        printf("Square of %d stored in %d", inst.operand1, inst.operand2);
    else
        printf("Square root of %d stored in %d", inst.operand1, inst.operand2);
}

// Log info of read_print function
void read_print_log(instruction inst)
{
    if(inst.sign_bit == 1)
        printf("Reading %d", DATA_MEMORY[inst.operand2]);
    else
        printf("%d", inst.operand1);
}

// Log info of stop function
void stop_log()
{
    printf("program is exiting...\n");
}

//=========================================================
//==================END-LOG-FUNCTIONS===================

// Transforms instruction's operands from addresses to corresponding literals
instruction to_literal(instruction inst)
{
    instruction literal_inst = inst;

    // Both <OPD1> and <OPD2> are addresses
    if (inst.operands_type == 0)
    {
        literal_inst.operand1 = DATA_MEMORY[inst.operand1];
        literal_inst.operand2 = DATA_MEMORY[inst.operand2];
    }
    // <OPD1> is an address, <OPD2> is a literal
    else if (inst.operands_type == 1)
    {
        literal_inst.operand1 = DATA_MEMORY[inst.operand1];
    }
    // <OPD1> is a literal, <OPD2> is an address
    else if (inst.operands_type == 2)
    {
        literal_inst.operand2 = DATA_MEMORY[inst.operand2];
    }

    return literal_inst;
}

// Assign to DATA_MEMORY or ACC
void assign(instruction inst)
{
    instruction literal_inst = to_literal(inst);

    // If special operands_type digit {7, 8, 9}
    if (inst.operands_type == 7)
    {
        DATA_MEMORY[inst.operand2] = ACC;
        return;
    }
    if (inst.operands_type == 8)
    {
        ACC = DATA_MEMORY[inst.operand1];
        return;
    }
    if (inst.operands_type == 9)
    {
        ACC = inst.operand1;
        return;
    }

    // If normal assign operation, put <OPD1> in <OPD2> (address)
    DATA_MEMORY[inst.operand2] = literal_inst.operand1;

    if (VERBOSE)
        assign_log(inst);
}

// Add / substract <OPD1> and <OPD2> and put result in ACC
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

// Multiply / divide <OPD1> by <OPD2> and put result in ACC
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

// Compute square / square root of <OPD1> and put result in <OPD2>
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

    if (VERBOSE)
        square_squareRoot_log(inst);
}

// Checks if <OPD1> and <OPD2> are equal or different
void equals_notEquals(instruction inst)
{
    instruction literal_inst = to_literal(inst);
    ACC = literal_inst.operand1 == literal_inst.operand2;

    if (VERBOSE)
        equals_notEquals_log(inst);
}

// Checks if <OPD1> >= <OPD2>
void greaterOrEquals_lessThan(instruction inst)
{
    instruction literal_inst = to_literal(inst);
    ACC = literal_inst.operand1 >= literal_inst.operand2;
    if (VERBOSE)
        greaterOrEquals_lessThan_log(inst);
}

// Scan user input and put it in <OPD2>
// or prints from memory address <OPD1>
void read_print(instruction inst)
{

    instruction literal_inst = to_literal(inst);
    if (inst.sign_bit == 1)
    {
        printf(">> ");
        scanf("%d", &DATA_MEMORY[inst.operand2]);
    }
    else
    {
        printf("%d", literal_inst.operand1);
    }
    if(VERBOSE)
        read_print_log(inst);
}

// stop function
//instruction is maybe not necessary as an input argument
void stop(instruction inst)
{
    if(VERBOSE)
        stop_log();
    exit(0);
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
        read_print(inst);
        break;
    case 9:
        stop(inst);
        break;
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