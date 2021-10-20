/**
* uses termination(n) to calculate the number of iterations needed to terminate
*
* Completion time: .5 hours
*
* @author: James Covert
* @version 1.0
*/

#include <stdio.h>

int termination(int n)
{

	int iterations = 0;

	while(n != 1){

		if(n % 2 == 0){

			n = n / 2;

		} else {

			n = (3 * n) + 1;

		}

		iterations++;
	}

	printf("Finding the termination point took %d iterations.", iterations);
	fflush(stdout);

	return iterations;
}

int main(void)
{
	int n;

	// prompt user to enter a integer
	printf("Enter an integer:\n");
	fflush(stdout);

	// Retrieve user input
	scanf("%d", &n);

	termination(n);

	return 0;

}
