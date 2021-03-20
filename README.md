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

- The ***operands_type*** digit can be one of the following: **{ 0 , 1 , 2 , 3 , 7 , 8 , 9 }**
    - **0**  : operand1 is an **address** & operand2 is an **address**.
    - **1**  : operand1 is an **address** & operand2 is a **literal**.
    - **2**  : operand1 is a **literal** & operand2 is an **address**.
    - **3**  : operand1 is a **literal** & operand2 is a **literal**.
    - **7**  : special "operands_type" value. Signifies that we should assign the value in **ACC** to **operand2** (address).
    - **8**  : special "operands_type" value. Signifies that we should assign the value in **operand1** (address) to **ACC**.
    - **9**  : special "operands_type" value. Signifies that we should assign the value in **operand1** (literal) to **ACC**.
    
- Since we have a memory of **20,000** words we divided it into two parts:
    - ***Program section***: consists of **10,000** words (0 - 9999)
    - ***Data section***: consists of **10,000** words (0 - 9999)
    
- With this design, we can access all the memory addresses since we have **4 digits** in each operand.

**NOTE:** The user should set the accumulator before using an instruction that requires a third operand.

#### Full OpCode table:
| OpCode | Description      | OpCode | Description    |
|--------|------------------|--------|----------------|
| +0     | assign           | -0     | (not used)     |
| +1     | +                | -1     | -              |
| +2     | x                | -2     | /              |
| +3     | square           | -3     | square root    |
| +4     | equals           | -4     | does not equal |
| +5     | greater or equal | -5     | less than      |
| +6     | z <- A[x]        | -6     | A[x] <- z      |
| +7     | incr, test, jump | -7     | label          |
| +8     | read             | -8     | print          |
| +9     | stop             | -9     | (not used)     |

#### Challenges:
- We found it a bit hard to design an instruction that gives us the possiblity to access all the memory cells (0 - 9999). However, we decided to make our operands 4 digits long and **put the 3rd operand**, if needed, in the **ACC**.


## 2. Symbolic Machine Language (AL):
#### AL Specifications:
- This language is similar to assembly.
- It implements an easy and direct mapping between its syntax and ML syntax.
- This language will be translated into ML.

## 3. Interpreter:
#### Usage:
The executable interpreter should be compiled from the C source interpreter file "interpreter.c".  
The executable takes one or two command line arguments:
1. The ML source file to read instructions from.
2. The options. *(i.e: -v for verbose)*

***Compile source code:***
```
    >> gcc interpreter.c -lm -o interpreter
```
***Run executable:***
-  *For Linux Based OS:*
```
    For Linux Based OS:
    >> ./interpreter <source_ML_file_name>
    OR
    >> ./interpreter <source_ML_file_name> -v

    i.e:  ./interpreter source.nml -v
```
- *For Windows OS:*
```
    >> interpreter <source_ML_file_name>
    OR
    >> interpreter <source_ML_file_name> -v

    i.e:  interpreter source.nml -v
```
***Options:***
```
    -v : VERBOSE, logs all executed instructions sequentially.
```

#### Implementation timeline:

- [x] Read, line by line, file containing Numeric Machine Language (ML).
- [x] Decode and tokenize each line as an ***instruction*** structure.
- [x] Load all ***instructions*** to RAM (***CODE_MEMORY***):
    - TODO: explanation
- [ ] Execute the ***instructions*** in RAM one by one:
    - [x] Loop through ***CODE_MEMORY*** array.
    - [x] Fetch each decoded instruction.
    - [ ] Execute instruction:
        - [ ] Implement a function for each opCode.
        - [ ] Implement a log function for each function. 
- [x] Add command line input arguments:
    - [x] Input source ML file as a command line argument.
    - [x] Input options as command line arguments (-v for verbose).
