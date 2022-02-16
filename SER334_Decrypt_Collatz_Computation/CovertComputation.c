/**
* Calculates the volume of any given number of cylinders
*
* Completion time: .5 hours
*
* @author: James Covert
* @version 1.0
*/

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdio.h>

int main(void)
{

	int numberOfCylinders;
	float height, radius, volume, result;
	const float pi = 3.141593;

	// prompt for user to enter in number of cylinders
	printf("Enter number of cylinders to sum:\n");
	fflush(stdout);

	// collect user input and store it in numberOfCylinders
	scanf("%d", &numberOfCylinders);

	// for loop to calculate the area of each cylinder
	for(int i = 0; i < numberOfCylinders; i++){

		// prompt for user to enter in height of cylinder
		printf("Enter height of cylinder # %d:\n", i + 1);
		fflush(stdout);

		// collect user input and store it in height
		scanf("%f", &height);

		// prompt for user to enter in radius of cylinder
		printf("Enter radius of cylinder # %d:\n", i + 1);
		fflush(stdout);

		// collect user input and store it in radius
		scanf("%f", &radius);

		// calculate the volume of specific cylinder
		volume = (pi * (radius * radius) *height);

		// add volume to the total sum
		result = volume;

	}

	// prints the results
	printf("The total volume of the cylinders is: %f", result);
	fflush(stdout);

	return 0;
}
