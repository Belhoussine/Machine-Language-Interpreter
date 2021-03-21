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
#### Syntax:
The syntax of this ML is the following:
` 
+/- <OP> <OPD1> <OPD2> <OpType>
`
- **Sign bit**: + or -
- **Operation**: **1** digit  - from **0** to **9**
- **OpCode**: combination of ***sign bit*** and ***operation***
- **Operand 1**: **4** digits - ranges from **0000** to **9999** (10,000 values)
- **Operand 2**: **4** digits - ranges from **0000** to **9999** (10,000 values)
- **Operands type**:**1** digits - describes whether the operands are ***Literals*** or ***Addresses*** (described in detail in **ML Specifications**)

- The instruction: ***+1 2345 6789 0*** could be described as follows:

| sign_bit  | operation |operand1  |operand2  | operands_type |
| --------- | ----------| ---------|----------|---------------|
| +         | 1         |     2345 |  6789    |    0          |

#### ML Specifications:

- This language considers ***\<OPD1>*** and ***\<OPD2>*** as a ***source*** and ***destination*** respectively.
- This language uses **2** registers: **IP** & **ACC**
- This language allows to store both **literals** and **addresses** in an instruction (using ***operands_type*** digit).

- The ***operands_type*** digit can be one of the following: **{ 0 , 1 , 2 , 3 , 7 , 8 , 9 }**
    | operands_type | operand1 |operand2  |
    | ------------  | ---------| ---------|
    | **0**         | Address  |  Address |
    | **1**         | Address  |  Literal |
    | **2**         | Literal  |  Address |
    | **3**         | Literal  |  Literal |
    
    - **7**  : special "operands_type" value. Signifies that we should assign the value in **ACC** to **operand2** (address).
    - **8**  : special "operands_type" value. Signifies that we should assign the value in **operand1** (address) to **ACC**.
    - **9**  : special "operands_type" value. Signifies that we should assign the value in **operand1** (literal) to **ACC**.
    
- Since we have a memory of **20,000** words we divided it into two parts:
    - ***Program section***: consists of **10,000** words (0 - 9999)
    - ***Data section***: consists of **10,000** words (0 - 9999)
    
- With this design, we can access all the memory addresses since we have **4 digits** in each operand.

**NOTES:** 
- The user should set the accumulator before using an instruction that requires a third operand.
- The result of all operations that use **two operands** store the result in the **ACC**.
- In order to store a value in an array, the user should:
    - Compute the destination address (**Base address** + **index**)
    - Store that address from **ACC** to a memory address
    - Use the corresponding function (**ATV** or **VTA** / **+6** or **-6**) to store a variable in that address, or store the content of that address in a variable.

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
| +7     |       test, jump | -7     | label          |
| +8     | read             | -8     | print          |
| +9     | stop             | -9     | (not used)     |

#### Challenges:
- We found it a bit hard to design an instruction that gives us the possiblity to access all the memory cells (0 - 9999). However, we decided to make our operands 4 digits long and **put the 3rd operand**, if needed, in the **ACC**.

## 2. Interpreter:
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

#### Interpreter implementation timeline:

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


## 3. Symbolic Machine Language Design (AL):
#### Syntax
The syntax of this assembly language is the following:
` 
<OP> <type><OPD1> <type><OPD2>
`
- **Operation** could be one of the following: {"ASN",  "ASN", "ADD",  "SUB",  "MUL", "DIV", "SQR",
                            "SQRT", "EQL", "NEQL", "GOE",  "LT", "ATV", "VTA",
                            "JMP",  "LBL", "READ", "PRNT", "STOP"}
- **Type** describes the type of the next operand. "L" means the next operand is a literal. "A" means the next operand is an address.
- **Operand1**: 4 digits - ranges from 0000 to 9999 (10000 values)
- **Operand2**: 4 digits - ranges from 0000 to 9999 (10000 values)
- The instruction: ASN L2341 A1001 could be described ass follows:

| operation  | type     |operand1  |type      | operand2      |
| ---------  | ---------| ---------|----------|---------------|
| ASN        | L        |     2341 |  A       |    1001       |

#### AL Specifications:
- This language is similar to assembly.
- It implements an easy and direct mapping between its syntax and ML syntax.
- This language will be translated into ML.

#### Full Operations table:
| OpCode | OpCode | Description      | OpCode | OpCode | Description    |
| ------ |--------|------------------| ------ |--------|----------------|
| ASN    | +0     | assign           |        | -0     | (not used)     |
| ADD    | +1     | +                | SUB    | -1     | -              |
| MUL    | +2     | x                | DIV    | -2     | /              |
| SQR    | +3     | square           | SQRT   | -3     | square root    |
| EQL    | +4     | equals           | NEQL   | -4     | does not equal |
| GOE    | +5     | greater or equal | LT     | -5     | less than      |
| ATV    | +6     | z <- A[x]        | VTA    | -6     | A[x] <- z      |
| JMP    | +7     | test, jump       | LBL    | -7     | label          |
| READ   | +8     | read             | PRNT   | -8     | print          |
| STOP   | +9     | stop             |        | -9     | (not used)     |

## 4.Assembler
#### Usage:
The assembler executable outputs a file named ***source.nml*** that contains the corresponding ML code to be interpreted.

***Compile source code:***
```
    >> gcc assembler.c -o assembler
```
***Run executable:***
-  *For Linux Based OS:*
```
    >> ./assembler <source_AL_file_name>

    i.e:  ./assembler source.al
```
- *For Windows OS:*
```
    >> interpreter <source_AL_file_name>

    i.e:  interpreter source.al
```

#### Assembler implementation timeline:
- TODO