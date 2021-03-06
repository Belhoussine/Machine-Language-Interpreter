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
-  *For Linux/Unix Based OS:*
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

#### Explanation of ML operations:
- **+0**: Assign **operand1** to **operand2**
- **+1**: Add **operand1** to **operand2** and  store result in **ACC**
- **-1**: Subtract **operand1** from **operand2** and store result in **ACC**
- **+2**: Multiply **operand1** by **operand2** and store result in **ACC**
- **-2**: Divide **operand1** by **operand2** and store result in **ACC**
- **+3**: Square of **operand1** and store result in **operand2**
- **-3**: Square root of **operand1** and store result in **operand2**
- **+4**: If **operand1** equals **operand2**, put 1 in **ACC**
- **-4**: If **operand1** does not equal **operand2**, put 0 in **ACC**
- **+5**: If **operand1** is greater or equals to **operand2** then put 1 in **ACC**
- **-5**: If **operand1** is less than **operand2** then put 0 in **ACC**
- **+6**: **Operand1** is an address. We access the value inside of it and then put in the address of **operand2**.
- **-6**: **Operand1** is assigned to the address that is pointed at by the address of **operand2**.
- **+7**: Jump to the address in operand1 if the adress is **operand2** contains 1
- **-7**: Store instruction in the address pointed at by **operand2**.
- **+8**: Read input from user and put in the address pointed at by **operand2**.
- **-8**: Print the content of the address pointed at by **operand1**.
- **+9**: Stops the program from executing.


#### Interpreter implementation timeline:

- [x] Read, line by line, file containing Numeric Machine Language (ML).
- [x] Decode and tokenize each line as an ***instruction*** structure.
- [x] Load all ***instructions*** to RAM (***CODE_MEMORY***):
    - TODO: explanation
- [x] Execute the ***instructions*** in RAM one by one:
    - [x] Loop through ***CODE_MEMORY*** array.
    - [x] Fetch each decoded instruction.
    - [x] Execute instruction:
        - [x] Implement a function for each opCode.
        - [x] Implement a log function for each function. 
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
- The instruction: ASN L2341 A1001 could be described as follows:

| operation  | type     |operand1  |type      | operand2      |
| ---------  | ---------| ---------|----------|---------------|
| ASN        | L        |     2341 |  A       |    1001       |

#### AL Specifications:
- This language is similar to assembly.
- It implements an easy and direct mapping between its syntax and ML syntax.
- This language will be translated into ML.

#### Full Operations table:
| AL Operation | ML Operation | Description   | AL Operation | ML Operation | Description    |
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
-  *For Linux/Unix Based OS:*
```
    >> ./assembler <source_AL_file_name>

    i.e:  ./assembler source.al
```
- *For Windows OS:*
```
    >> assembler <source_AL_file_name>

    i.e:  assembler source.al
```
#### Explanation of AL operations:
- **ASN**: Assign **operand1** to **operand2**
- **ADD**: Add **operand1** to **operand2** and store result in **ACC**
- **SUB**: Subtract **operand1** from **operand2** and store result in **ACC**
- **MUL**: Multiply **operand1** by **operand2** and store result in **ACC**
- **DIV**: Divide **operand1** by **operand2** and store result in **ACC**
- **SQR**: Square of **operand1** and store result in **operand2**
- **SQRT**: Square root of **operand1** and store result in **operand2**
- **EQL**: If **operand1** equals **operand2**, put 1 in **ACC**
- **NEQL**: If **operand1** does not equal **operand2**, put 0 in **ACC**.
- **GOE**: If **operand1** is greater or equals to operand2 then put 1 in **ACC**.
- **LT**: If **operand1** is less than **operand2** then put 0 in **ACC**.
- **ATV**: **Operand1** is an address. We access the value inside of it and then put in the address of **operand2**.
- **VTA**: **Operand1** is assigned to the address that is pointed at by the address of **operand2**.
- **JMP**: Jump to the address in **operand1** if the adress is **operand2** contains 1.
- **LBL**: Store instruction in the address pointed at by **operand2**.
- **READ**: Read input from user and put in the address pointed at by **operand2**.
- **PRNT**: Print the content of the address pointed at by **operand1**.
- **STOP** Stops the program from executing.

**NOTES:** 
- The **#** character describes the start of a comment in assembly. The assembler will ignore lines that start with **#**
- If the operand2 in print instruction is 9999 regardless of its type, it means it is \n.
#### Assembler implementation timeline:
- [x] Add a simple complexity algorithm for testing:
    - Perform assignments and arithmetic operations.
    - Pseudo-code:
    ```python
        a = 10
        b = 20
        c = 4
        s = a + b
        sq = sqrt(c)
        res = s / sq
        print(res)
        print('\n')
    ```
- [x] Add a medium complexity algorithm for testing:
    - Find max in an array of 5 numbers (loop and if statement).
    - Pseudo-code:
    ```python
        arr = [342, 96, 5935, 436, 1220]
        maximum = 0
        counter = 0
        while(counter < 5):
            current = arr[counter]
            if(current > maximum):
                maximum = current
            counter = counter + 1
        print(maximum)
        print('\n')
    ```
- [x] Add a high complexity algorithm for testing:
    - Draw a triangle shape (nested loops), then use nested if statements to print a value.
    - Pseudo-code:
    ```python
        # Nested loops
        i = 0
        j = 0
        n = 10
        while (i < n):
            j = i
            while (j < n):
                print(0)
                j = j + 1
            print('\n')
            i = i+1

        # Nested if statements
        a = 15
        b = 5
        c = a + b
        if (c >= 15):
            d = c / 2
            if(d >= 5):
                print(1337)
        print('\n')
    ```

## Screenshots

### Simple problem:

![simple](https://user-images.githubusercontent.com/54045588/112025849-e36c8680-8b35-11eb-91e7-d4e6d834c9c8.png)

### Medium problem:

![medium](https://user-images.githubusercontent.com/54045588/112025971-0a2abd00-8b36-11eb-8451-37e89cbb6be4.png)

### Complex problem:

![complex](https://user-images.githubusercontent.com/54045588/112026020-144cbb80-8b36-11eb-9d53-491260e3a85f.png)


