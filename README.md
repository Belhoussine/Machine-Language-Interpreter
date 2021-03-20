# Machine-Language-Interpreter
This is a machine language interpreter written in C, its purpose is translating a designed machine language and run it on a virtual machine.

## Inputs:
### 1. Numeric Machine Language (ML):
    - Similar to pseudo-code seen in Unit 3
    - Has 20,000 words in memory: each word -> 10 digits + sign bit
    - Uses 2 registers: IP & ACC
    - Should allow to store literals in instruction, not just memory addresses.
### 2. Symbolic Machine Language (AL):
    - Similar to assembly, will be translated to ML - should implement an easy/direct mapping.

## Interpreter:
- [x] Read, line by line, file containing Numeric Machine Language (ML)
- [x] Parse and tokenize each line, then create instruction structure with given tokens
- [ ] Execute the instruction

## ML Design:
Note: The user sets the accumulator before using an instruction that requires a third operand. Thus,  we can come up with the following instruction design:

- The “+0 0001 1000 1” instruction could be described by the table below:

| sign_bit  | opcode |operand_1 |operand_2 | operands_type |
| --------- | -------| ---------|----------|---------------|
| +         | 0      |     0001 |  1000    |    3          |

- The operands_type digit can be = {0,1,2,3}
    - 0 means: operand_1 is an address & operand_2 is an address
    - 1 means: operand_1 is an address & operand_2 is a literal
    - 2 means: operand_1 is a literal & operand_2 is an address
    - 3 means: operand_1 is a literal & operand_2 is a literal
- Since we have a memory of 20000 words we divided it into two parts:
    - Data part: consists of 10000 words (0 - 9999)
    - Program part: consists of 10000 words (0 - 9999)
- With this design, we can access all the memory addresses since we have 4 digits in each operand.
