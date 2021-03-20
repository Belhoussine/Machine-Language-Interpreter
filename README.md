# Machine-Language-Interpreter
This is a machine language interpreter written in C, its purpose is to translate a designed machine language and run it on a virtual machine.


### Abbreviation Table:
| Abbreviation  	| Description  									|
| ------------- 	| ----------------------------------------------|
|     **ACC**   	|Accumulator (register)							|
|     **IP**    	|Instruction Pointer (register)					|
|     **\<OP>**   	|Operation: from **0** to **9**							|
|     **OpCode**   	|Operation code: **sign_bit** + operation **\<op>**	|
|     **OpType**   	|Operands type: specifies the types of the operands	|
|     **\<OPD1>**   |Operand1: first 4 digits after **opCode**			|
|     **\<OPD2>**   |Operand2: first 4 digits after **\<OPD1>**		|
|     **\<SRC>**   	|Source address or literal similar to **\<OPD1>**			|
|     **\<DEST>**   |Destination address or literal, similar to **\<OPD2>**		|




## 1. Numeric Machine Language (ML) Design:
### Syntax:
The syntax of this ML is the following:
` 
+/- <OP> <OPD1> <OPD2> <OpType>
`
- The instruction: ***+1 2345 6789 0*** could be described as follows:

| sign_bit  | operation |operand1  |operand2  | operands_type |
| --------- | ----------| ---------|----------|---------------|
| +         | 1         |     2345 |  6789    |    0          |

#### ML Specifications:

- This language considers ***\<OPD1>*** and ***\<OPD2>*** as a ***source*** and ***destination*** respectively.
- This language uses **2** registers: **IP** & **ACC**
- This language allows to store both **literals** and **addresses** in an instruction (using ***operands_type*** digit).

- The ***operands_type*** digit can be one of the following: **{ 0 , 1 , 2 , 3 , 8 , 9 }**
    - **0**  : operand1 is an **address** & operand2 is an **address**.
    - **1**  : operand1 is an **address** & operand2 is a **literal**.
    - **2**  : operand1 is a **literal** & operand2 is an **address**.
    - **3**  : operand1 is a **literal** & operand2 is a **literal**.
    - **8**  : special "operands_type" value. Signifies that we should assign the value in **ACC** to **operand2**.
    - **9**  : special "operands_type" value. Signifies that we should assign the value in **operand1** to **ACC**.
    
- Since we have a memory of **20,000** words we divided it into two parts:
    - ***Program section***: consists of **10,000** words (0 - 9999)
    - ***Data section***: consists of **10,000** words (0 - 9999)
    
- With this design, we can access all the memory addresses since we have **4 digits** in each operand.

**NOTE:** The user should set the accumulator before using an instruction that requires a third operand. 

## 2. Symbolic Machine Language (AL):
#### AL Specifications:
- This language is similar to assembly.
- It implements an easy and direct mapping between its syntax and ML syntax.
- This language will be translated into ML.

## Interpreter Implementation:
- [x] Read, line by line, file containing Numeric Machine Language (ML).
- [x] Parse and tokenize each line as an ***instruction*** structure.
- [x] Load all ***instructions*** to RAM (***CODE_MEMORY***).
- [ ] Execute the ***instructions*** in RAM one by one.
