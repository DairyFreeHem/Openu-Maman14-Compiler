/*
* Write out data received from compiler to an accompanying .ob file
*/

/*Open writing to filename.ob file*/
int startWrite(char *filename);

/*End the output of .OB*/
int endWrite(void);

/*Write out one line into output file*/
void writeOBLine(int First, int Second);

/*Write entiraty of data list to .ob file
*Parse through each node in data list,
*and convert values of data->values array in each node to base32 
*/
int writeData(void);



/*Write entiraty of code list to .ob file
*Parse through each node in code list,
*and convert values of code->values array in each node to base32 
*/
int WriteCode(void);
