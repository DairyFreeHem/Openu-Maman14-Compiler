/*
* File containing converters for numbers and strings that aren't found in the standard C libraries
*/

#include "converters.h"
#include "input.h"
#include "defines.h"

/*The program's base32 numbering system*/
char base32nums[32]={'!','@','#','$','%','^','&','*','<','>','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v'};

/*Convert decimal num to base32*/
char *decimalToBase32(int num){    
    int j, count=0, B32=0, position=1, i=0;
    unsigned int abs = num;
    char temp;
    char bin[10];
    char * Base32 = malloc(3*sizeof(char)); 
    CHECKMEM_PTR(Base32);   
    while(i<10){
        bin[i]= abs%2+'0';
        abs=abs/2;
        i++;
    }
    for (j=0;j<5;j++){
        temp = bin[9 - j];
        bin[9 - j] = bin[j];
        bin[j] = temp;
    }    
    if(!Base32)
        return NULL;
    for (i=9;i>-1;i--){
        B32+=pow(2,count)*(bin[i]-'0');
        count++;
        if(count==5){
            Base32[position]=base32nums[B32];
            position--;
            count=0;

            B32=0;
        }
    }
    Base32[2]='\0';

    return Base32;    
}

/*Convert binary string to decimal*/
int binToDecimal(char *bin){
    int i;
    int num=0;

    for (i=9;i>-1;i--){
        num+=pow(2,9-i)*(bin[i]-'0');
    }
    if(bin[0]=='1')
        return 0-(pow(2,10)-num);
    return num;
}

/*flip between set of bits in array from startBit to endBit*/
void flipSpecificBits(char **bits, int startBit, int endBit)
{
    int j, length = ((endBit - startBit +1) /2);
    char temp;
    for (j=0;j<length;j++){
        temp = bits[0][(endBit - j)];
        bits[0][(endBit - j)] = bits[0][j+startBit];
        bits[0][j+startBit] = temp;
    }
}

/*Convert decimal to binary string*/
char *decimalToBin(int *num,int bitLength){
    unsigned int i,abs = num[0];
    int counter = 0;
    char *bin = (char *)malloc(11 * sizeof(char));
    CHECKMEM_PTR(bin);
    for (i = 0, counter = 0; i < 10; i++, bitLength--)
    {
        if (bitLength == 0)
        {
            counter++;
            abs = num[counter];
            bitLength = 2;
        }
        
        bin[i]= abs%2+'0';
        abs=abs/2;
    }
    bin[10] = '\0';
    return bin;
}



/*Return a new string from two other strings*/
char* concat(const char *s1, const char *s2)
{
    int length = (strlen(s1)+strlen(s2)+1);
    char *result = malloc(length);
    if(result)
    {
        strcpy(result, s1);
        strcat(result, s2);
        return result;
    }
    else
    {
        printf("Issue allocating memory");
        return NULL;
    }
}

/*Convert command parameter values (except register) to binary string,
 *return decimal representation of that binary string*/
int binCodeCreator(int *params,int bitLength )
{
    char *bin = NULL;
    int binNum;
    bin = decimalToBin(params,bitLength);
    if (bitLength == 8)
    {
        flipSpecificBits(&bin,0,7);
    }
    flipSpecificBits(&bin,8,9);

    CHECKMEM(bin);
    binNum = binToDecimal(bin);
    FREEUP(bin);
    return binNum;
}

/*Convert machine instruction to binary string,
 *return decimal representation of that binary string*/
int codeHeadtoInt(int *params)
{
    char *bin = NULL;
    int binNum;
    bin = decimalToBin(params,4);
    CHECKMEM(bin);
    flipSpecificBits(&bin,0,3);
    flipSpecificBits(&bin,4,5);
    flipSpecificBits(&bin,6,7);

    binNum = binToDecimal(bin);
    FREEUP(bin);
    return binNum;
}

/*Convert register values to binary string,
 *return decimal representation of that binary string*/
int binRegisterCreator(int *regs)
{
    char *numreciev = NULL;
    char temp[11] = "";
    int binNum, i, j,m = 0;
    for (i = 0; i < 11; i++)
    {
        temp[i] = '0';
    }
    temp[10] = '\0';
    for (i = 0; i < 2; i++)
    {   
        if (regs[i] == 0)
        {
            m+=4;
            continue;
        }   
        numreciev = decimalToBin(&regs[i],4);
        flipSpecificBits(&numreciev,0,3);

        for ( j = 0; j < 4; j++,m++)
        {
            temp[m] = numreciev[j];
        }
        FREEUP(numreciev);
    }
    binNum = binToDecimal(temp);
    return binNum;
}


