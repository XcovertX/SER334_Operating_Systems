/**
* decrypts global chunk variables
*
* Completion time: 2 hours
*
* @author James Covert
* @version 1.0
*/

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdio.h>
#include <string.h>

//macros: constants
#define CHUNK_LENGTH (20+1)  //each chunk has twenty characters, we add 1 so
                           //there is space for the null terminator.
#define NUMBER_OF_CHUNKS 4 //the message is spread across 4 chunks.
#define DECRYPTION_SHIFT 5 //this is the ASCII table shift used for decryption.

//forward declarations
void sort_chunks();
void decrypt_chunks();
void display_chunks();

char chunks[NUMBER_OF_CHUNKS][CHUNK_LENGTH];

int main() {
	//copy message into memory.
	strcpy(chunks[0], "2i1%fsi%fs%jstwrtzx%");
	strcpy(chunks[1], "1'H%nx%vznwp~1%kqf|j");
	strcpy(chunks[2], "4R3%Wnyhmnj%%%%%%%%%");
	strcpy(chunks[3], "3xzhhjxx3'%2%Ijssnx%");

	//the format of a chunk is a single number indicating its order in overall
	//message followed by twenty encrypted characters.


	//reorder chunks in message by sorting them based on the first digital
	//they contain. looking above, one can see they are currently in the order
	//2, 1, 4, 3 but should be ordered 1, 2, 3, 4.
	sort_chunks();

	//shift the characters in the message to produce the original characters.
	decrypt_chunks();

	//display the decrypted message.
	display_chunks();


	return 0; 
}

//given two strings, swaps their contents in memory.
void swap_strings(char* x, char* y) {
	//create a temporary holding place for the data so we don't lose it.
	char temp[CHUNK_LENGTH];

	strcpy(temp, x);
	strcpy(x, y);
	strcpy(y, temp);
}

//sorts the strings the global chunks variable by the first character they contain.
void sort_chunks() {

	char *pointer = 0;
	int i = 0;
	int j;

	while(i < NUMBER_OF_CHUNKS){

		int mIndex = i;
		pointer = chunks[i];
		j = i + 1;

		while(j < NUMBER_OF_CHUNKS){
			if(*pointer > chunks[j][0]){
				pointer = chunks[j];
				mIndex = j;
			}

			j++;
		}

		if(mIndex != i){
			swap_strings(chunks[i], chunks[mIndex]);
		}

		i++;
	}
}

//for each string in the global chunks variable, shifts the characters in it by
//DECRYPTION_SHIFT.
void decrypt_chunks() {

	char *pointer = 0;
	int i = 0;

	while(i < NUMBER_OF_CHUNKS){

		pointer = chunks[i];

		while(*pointer){
			*pointer = *pointer - DECRYPTION_SHIFT;
			pointer++;
		}
		i++;
	}
}


//displays the strings in the global chunks variable
void display_chunks() {

	char *pointer = 0;
	int i = 0;

	while(i < NUMBER_OF_CHUNKS){

		pointer = chunks[i];

		pointer++;

		while(*pointer){

			printf("%c", *pointer);
			fflush(stdout);
			*pointer = *pointer + 1;
			pointer++;

		}

		i++;
	}
}
