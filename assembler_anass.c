#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char instructions[20][5] = {"ASN\0", "ASN\0",  "ADD\0",  "SUB\0", "MUL\0",
                            "DIV\0", "SQR\0",  "SQRT\0", "EQL\0", "NEQL\0",
                            "GOE\0", "LOE\0",  "ATV\0",  "VTA\0", "JMP\0",
                            "LBL\0", "READ\0", "PRNT\0", "STOP\0"};
int InstructionToOpcode(char instrunction[]) {
    for (int i = 0; i < 19; i++) {
        if (strcmp(instrunction, instructions[i]) == 0) {
            return ((i % 2 != 0) * -1 + (i % 2 == 0)) * i / 2;
        }
    }
}
int Toint(char operand[]) {
    int ans = 0;
    for (int i = 0; i < strlen(operand); i++) {
        if (isdigit(operand[i])) {
            ans *= 10;
            ans += (operand[i] - '0');
        }
    }
    return ans;
}
int decode(char Line[]) {
    char temp[10];
    int p = 0, j = 0, t1 = 0, t2 = 0, t3 = 0;
    int MACHINELANGUAGE[4];
    for (int i = 0; i < strlen(Line); i++) {
        if (Line[i] == ' ' || (i + 1 == strlen(Line))) {
            MACHINELANGUAGE[p] = 0;
            temp[j] = '\0';
            if (i + 1 == strlen(Line)) {
                temp[j] = Line[i];
                j++;
            }
            if (p == 0) {
                MACHINELANGUAGE[p] = InstructionToOpcode(temp);
            } else if (p == 1) {
                t2 = temp[0];
                temp[0] = '0';
                if (temp[1] == 'A')
                    t3 = 7;
                else
                    MACHINELANGUAGE[p] = Toint(temp);
            } else if (p == 2) {
                t1 = temp[0];
                temp[0] = '0';
                if (temp[1] == 'A')
                    t3 = 8;
                else
                    MACHINELANGUAGE[p] = Toint(temp);
            }
            j = 0;
            temp[0] = '\0';
            p++;
            continue;
        }
        temp[j] = Line[i];
        j++;
    }
    int optype = ((t1 == 'L') << 0) | ((t2 == 'L') << 1);
    optype += t3;
    MACHINELANGUAGE[p] = optype;
    long long ans = MACHINELANGUAGE[0] *
                    ((MACHINELANGUAGE[0] < 0) * -1 + (MACHINELANGUAGE[0] >= 0));
    for (int i = 1; i <= 2; i++) {
        ans *= 10000;
        ans += MACHINELANGUAGE[i];
    }
    ans *= 10;
    ans += MACHINELANGUAGE[p];
    if (MACHINELANGUAGE[0] < 0) ans *= -1;
    return ans;
}
int main(void) {
    FILE *ptr = fopen("source.al", "r");
    FILE *ptr1 = fopen("source.nml", "w");
    while (!feof(ptr)) {
        char Line[20];
        fgets(Line, 20, ptr);
        long long ML = decode(Line);
        if (ML >= 100000000) {
            if (ML > 0) fprintf(ptr1, "+");
            fprintf(ptr1, "%lld\n", ML);
        } else {
            if (ML > 0) fprintf(ptr1, "+00000");
            fprintf(ptr1, "%lld\n", ML);
        }
    }

    return 0;
}