#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define RESET "\x1B[0m"
#define BOLD "\x1B[1m"

typedef struct {
    char sign_bit;
    int operation, operand1, operand2;
    char operands_type;
} instruction;

//=================START-GLOBAL-VARIABLES==================
//=========================================================

instruction CODE_MEMORY[10000];
int DATA_MEMORY[10000];
unsigned short total_instructions = 0, IP = 0, VERBOSE = 0;
int ACC = -1;
char STDOUT[10000];

//=========================================================
//==================END-GLOBAL-VARIABLES===================

// --------------------------------------------------------

//=====================START-FUNCTIONS=====================
//=========================================================

// Parse lines into tokens
instruction tokenize(char *line) {
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
void display(instruction inst) {
    printf("Sign bit: %d \t- ", inst.sign_bit);
    printf("OpCode: %d - ", inst.operation);
    printf("Op1: %d - ", inst.operand1);
    printf("Op2: %d - ", inst.operand2);
    printf("OpType: %d\n", inst.operands_type);
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
        CODE_MEMORY[total_instructions] = tokenize(line);
        // display(CODE_MEMORY[total_instructions]);
        total_instructions++;
    }
    fclose(infp);
}

// Transforms instruction's operands from addresses to corresponding literals
instruction to_literal(instruction inst) {
    instruction literal_inst = inst;

    // Both <OPD1> and <OPD2> are addresses
    if (inst.operands_type == 0) {
        literal_inst.operand1 = DATA_MEMORY[inst.operand1];
        literal_inst.operand2 = DATA_MEMORY[inst.operand2];
    }
    // <OPD1> is an address, <OPD2> is a literal
    else if (inst.operands_type == 1) {
        literal_inst.operand1 = DATA_MEMORY[inst.operand1];
    }
    // <OPD1> is a literal, <OPD2> is an address
    else if (inst.operands_type == 2) {
        literal_inst.operand2 = DATA_MEMORY[inst.operand2];
    }

    return literal_inst;
}

//=================START-LOG-FUNCTIONS==================
//=========================================================
// Log info od assign function
void assign_log(instruction inst) {
    if (inst.operands_type == 7) {
        printf("[ASN] Storing ACC: %d in address %d.\n", ACC, inst.operand2);
    } else if (inst.operands_type == 8 || inst.operands_type == 9) {
        printf("[ASN] Storing %d in ACC.\n", inst.operand1);
    } else {
        printf("[ASN] Storing %d in address %d.\n", inst.operand1,
               inst.operand2);
    }
}

// Log info of add_subtract function
void add_subtract_log(instruction inst) {
    if (inst.sign_bit == 1)
        printf("[ADD] Adding %d to %d, storing result in ACC.\n", inst.operand1,
               inst.operand2);
    else
        printf("[SUB] Subtracting %d from %d, storing result in ACC.\n",
               inst.operand1, inst.operand2);
}

// Log info of multiply_divide function
void multiply_divide_log(instruction inst) {
    if (inst.sign_bit == 1)
        printf("[MUL] Multiplying %d by %d, storing result in ACC.\n",
               inst.operand1, inst.operand2);
    else
        printf("[DIV] Dividing %d by %d, storing result in ACC.\n",
               inst.operand1, inst.operand2);
}

// Log info of square_squareRoot function
void square_squareRoot_log(instruction inst) {
    if (inst.sign_bit == 1)
        printf("[SQR] Square of %d stored in address %d. \n", inst.operand1,
               inst.operand2);
    else
        printf("[SQRT] Sqrt of %d stored in address %d. \n", inst.operand1,
               inst.operand2);
}

// Log info of equals_notEquals function
void equals_notEquals_log(instruction inst) {
    if (inst.sign_bit == 1)
        printf("[EQL] %d == %d. storing result in ACC.\n", inst.operand1,
               inst.operand2);
    else
        printf("[NEQL] %d != %d, storing result in ACC.\n", inst.operand1,
               inst.operand2);
}

// Log info of square_squareRoot function
void greaterOrEquals_lessThan_log(instruction inst) {
    if (inst.sign_bit == 1)
        printf("[GOE] %d >= %d, storing result in ACC.\n", inst.operand1,
               inst.operand2);
    else
        printf("[LT] %d < %d, storing result in ACC.\n", inst.operand1,
               inst.operand2);
}

void arrToVar_varToArr_log(instruction inst) {
    if (inst.sign_bit == 1)
        printf("[ATV] Storing address %d (array cell) in %d (variable)\n",
               inst.operand1, inst.operand2);
    else
        printf("[VTA] Storing %d (variable) in %d (array cell)\n",
               inst.operand1, inst.operand2);
}

void jump_label_log(instruction inst, int flag) {
    instruction literal_inst = to_literal(inst);
    if (inst.sign_bit == 1)
        if (flag)
            printf("[JMP] Jumping to labeled IP %d.\n",
                   literal_inst.operand1 + 2);
        else
            printf("[JMP] Not jumping.\n");

    else
        printf("[LBL] Labeling current IP %d in address %d\n", IP,
               inst.operand2);
}

// Log info of stop function
void stop_log() { printf("Program is exiting...\n"); }

//=========================================================
//==================END-LOG-FUNCTIONS===================

// Assign to DATA_MEMORY or ACC
void assign(instruction inst) {
    instruction literal_inst = to_literal(inst);

    // If special operands_type digit {7, 8, 9}
    if (inst.operands_type == 7) {
        DATA_MEMORY[inst.operand2] = ACC;
    } else if (inst.operands_type == 8) {
        ACC = DATA_MEMORY[inst.operand1];
    } else if (inst.operands_type == 9) {
        ACC = inst.operand1;
    } else {
        // If normal assign operation, put <OPD1> in <OPD2> (address)
        DATA_MEMORY[inst.operand2] = literal_inst.operand1;
    }

    if (VERBOSE) assign_log(inst);
}

// Add / substract <OPD1> and <OPD2> and put result in ACC
void add_subtract(instruction inst) {
    if (inst.sign_bit == 1) {
        ACC = inst.operand1 + inst.operand2;
    } else {
        ACC = inst.operand1 - inst.operand2;
    }

    if (VERBOSE) add_subtract_log(inst);
}

// Multiply / divide <OPD1> by <OPD2> and put result in ACC
void multiply_divide(instruction inst) {
    if (inst.sign_bit == 1) {
        ACC = inst.operand1 * inst.operand2;
    } else {
        ACC = inst.operand1 / inst.operand2;
    }

    if (VERBOSE) multiply_divide_log(inst);
}

// Compute square / square root of <OPD1> and put result in <OPD2>
void square_squareRoot(instruction inst) {
    instruction literal_inst = to_literal(inst);
    if (inst.sign_bit == 1) {
        DATA_MEMORY[inst.operand2] =
            literal_inst.operand1 * literal_inst.operand1;
    } else {
        DATA_MEMORY[inst.operand2] = sqrt(literal_inst.operand1);
    }

    if (VERBOSE) square_squareRoot_log(inst);
}

// Checks if <OPD1> and <OPD2> are equal or different
void equals_notEquals(instruction inst) {
    if (inst.sign_bit == 1) {
        ACC = inst.operand1 == inst.operand2;
    } else {
        ACC = inst.operand1 != inst.operand2;
    }

    if (VERBOSE) equals_notEquals_log(inst);
}

// Checks if <OPD1> >= <OPD2>
void greaterOrEquals_lessThan(instruction inst) {
    if (inst.sign_bit == 1) {
        ACC = inst.operand1 >= inst.operand2;
    } else {
        ACC = inst.operand1 < inst.operand2;
    }
    if (VERBOSE) greaterOrEquals_lessThan_log(inst);
}

// Assign variable <OPD1> to array cell address <OPD2> and vice versa
void arrToVar_varToArr(instruction inst) {
    instruction literal_inst = to_literal(inst);
    if (inst.sign_bit == 1) {
        // Storing array cell value (computer address: base_val + index)in
        // variable (address)
        DATA_MEMORY[inst.operand2] = DATA_MEMORY[DATA_MEMORY[inst.operand1]];
    } else {
        // Storing variable (address or literal) in a variable (address)
        DATA_MEMORY[DATA_MEMORY[inst.operand2]] = literal_inst.operand1;
    }

    if (VERBOSE) arrToVar_varToArr_log(inst);
}

// Assign variable <OPD1> to array cell address <OPD2> and vice versa
void jump_label(instruction inst) {
    instruction literal_inst = to_literal(inst);
    if (inst.sign_bit == 1) {
        // Jumping to a specific IP address in CODE_MEMORY
        if (literal_inst.operand2) IP = literal_inst.operand1;
    } else {
        // Storing next instruction pointer (IP) in a specific data memory cell
        // (address)
        DATA_MEMORY[inst.operand2] = IP;
    }

    if (VERBOSE) jump_label_log(inst, literal_inst.operand2);
}

// Scan user input and put it in <OPD2>
// or prints from memory address <OPD1>
void read_print(instruction inst) {
    instruction literal_inst = to_literal(inst);
    char ch, str[100];
    // Read
    if (inst.sign_bit == 1) {
        printf(">> ");
        scanf("%d", &DATA_MEMORY[inst.operand2]);
    }
    // Print
    else {
        if (inst.operand2 == 9999) {
            if (VERBOSE) {
                // sprintf(STDOUT, "%s\n", STDOUT);
                ch = '\n';
                strncat(STDOUT, &ch, 1);
                printf("[PRNT]\n");
            } else
                printf("\n");
        } else {
            if (VERBOSE) {
                sprintf(str, "%d", literal_inst.operand1);
                strncat(STDOUT, str, sizeof(str));
                printf("[PRNT] %d\n", literal_inst.operand1);
            } else
                printf("%d", literal_inst.operand1);
        }
    }
}

// Stop Program
void stop() {
    if (VERBOSE) {
        stop_log();
        printf(BOLD GRN "\n=============PROGRAM-OUTPUT===========\n" RESET);
        printf("%s", STDOUT);
    }
    exit(0);
}
// Execute a single instruction in RAM
void execute_instruction(instruction inst) {
    if (VERBOSE) {
        printf(BOLD BLU "[INFO]" RESET GRN "%3d ~ " YEL, IP + 1);
    }
    switch (inst.operation) {
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
            arrToVar_varToArr(inst);
            break;
        case 7:
            jump_label(inst);
            break;
        case 8:
            read_print(inst);
            break;
        case 9:
            stop();
            break;
    }
}
// Execute instructions in RAM
void execute_instructions() {
    for (IP = 0; IP < total_instructions; IP++) {
        if (VERBOSE) printf(YEL);
        execute_instruction(CODE_MEMORY[IP]);
        printf(RESET);
    }
    if (VERBOSE) {
        printf(BOLD GRN "\n=============PROGRAM-OUTPUT===========\n" RESET);
        printf("%s", STDOUT);
    }
}

void debug() {
    printf("==============DEBUG=============\n");
    printf(">>> %d %d\n", DATA_MEMORY[2345], DATA_MEMORY[2346]);
    printf(">>> %d\n", ACC);
}

//=========================================================
//======================END-FUNCTIONS======================

int main(int argc, char *argv[]) {
    // Getting command line arguments
    if (argc < 2) {
        printf("Error: you must input a source ML file.\n");
        printf("[Usage]:\n\t %s <source_ML_file>\n", argv[0]);
        printf(
            "[Options]:\n\t -v : Verbose (log all executed instructions).\n");
        return 1;
    } else if (argc == 3 && strcmp(argv[2], "-v") == 0) {
        printf(BOLD GRN "[INFO] Verbose ON.\n" RESET);
        VERBOSE = 1;
    }

    read_source_file(argv[1]);

    execute_instructions();

    // debug();

    return 0;
}