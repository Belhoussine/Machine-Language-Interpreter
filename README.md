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