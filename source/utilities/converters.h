#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


/*Convert decimal num to base32*/
char *decimalToBase32(int num);

/*Convert binary string to decimal*/
int binToDecimal(char *bin);            

/*Convert decimal to binary string*/
char *decimalToBin(int *num,int bitLength);

/*Return a new string from two other strings*/
char *concat(const char *s1, const char *s2);

/*Convert command parameter values (except register) to binary string,
 *return decimal representation of that binary string*/
int binCodeCreator(int *params, int bitLength);

/*Convert register values to binary string,
 *return decimal representation of that binary string*/
int binRegisterCreator(int *regs);

/*Convert machine instruction to binary string,
 *return decimal representation of that binary string*/
int codeHeadtoInt(int *params);