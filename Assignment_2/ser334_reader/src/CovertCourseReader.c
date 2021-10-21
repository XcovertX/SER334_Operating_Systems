#ifndef _HW2_H
#define _HW2_H

/**
* This program loads data from a file provided by the user.
* The user can navigate the program through a console and retrieve various portions of information
* about the course, students and their grades all contained within the file
*
* Completion time: 8hours
*
* @author James Covert, Acuna
* @version 1.0
*
*/

////////////////////////////////////////////////////////////////////////////////
// INCLUDES
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

////////////////////////////////////////////////////////////////////////////////
// MACROS: CONSTANTS
#define MAX_STUDENT_NAME_LENGTH 20
#define MAX_COURSE_NAME_LENGTH 10
#define MAX_TEACHER_NAME_LENGTH 20
#define MAX_COMMENT_LENGTH 20
#define MAX_ASSIGN_NAME_LENGTH 20
#define MAX_FILENAME_LENGTH 30

////////////////////////////////////////////////////////////////////////////////
//DATA STRUCTURES

typedef enum AcademicLevel { Freshman = 0, Sophomore, Junior, Senior } AcademicLevel;

typedef struct ScoreStruct {
	int studentNumber;
	int assignNumber;
	float score;
	char* comment;
} ScoreStruct;

typedef struct Assign {
	int number;
	char* name;
} Assign;

typedef struct Student {
	int number;
	char* name;
	AcademicLevel al;
} Student;

typedef struct Course {
	int number;
	char* name;
	char* teacher;
	int numAssigns;
	Assign* assigns;
	ScoreStruct** scores;
} Course;

////////////////////////////////////////////////////////////////////////////////
//GLOBAL VARIABLES
//place to store student information
Student* students = NULL;
//place to store course information
Course* courses = NULL;
int numCourses, numStudents;

////////////////////////////////////////////////////////////////////////////////
//FORWARD DECLARATIONS

// the following should be used to read student/course data in from the file
void readFile(char* filename);
void readString(FILE* file, char** dst, int max_length);
Student* readStudents(FILE* file);
Course* readCourses(FILE* file);
Assign* readAssigns(FILE* file, int numAssigns);
ScoreStruct** readScores(FILE* file, int numAssigns);

// the following should be used to free all heap data allocated during the programs runtime
// and handle dangling pointers
void terminate();
void studentsDestructor();
void coursesDestructor();
void assignsDestructor(Assign** assigns, int numAssign);
void scoresDestructor(ScoreStruct*** scores, int numAssigns);

// the following should be used to cleanly print the data used in the program
void printStudents();
void printAssigns(Assign* assigns, int numAssigns);
void printGrades(ScoreStruct** scores, int numAssigns);
void printCourse(Course course);

// the following are mostly complete functions that define and
// control the CLI menu associated with the program
void mainMenu();
void mainMenuBranch(int option);
void subMenu(Course course);
void subMenuBranch(int option, Course course);

// these are the 'special' functions that you are being asked to implement
void studentMenu(Course course);
void getStudentScores(Course course, int studentNo);
void assignmentMenu(Course course);
void getAssignmentScore(Course course, int assignmentNo);

// this is a utility function to be called when input filenames are invalid
void print_usage();

/////////////////////////////////////////////////////////////////////////////////
//IMPLEMENTATION

/**
* reads the given file
*/
void readFile(char* filename){

  FILE* file = fopen(filename, "r");

  fscanf(file, "%d", &numStudents);
  students = readStudents(file);

  fscanf(file, "%d\n", &numCourses);
  courses = readCourses(file);

  fclose(file);
}

/**
* Reads strings within a file
*/
void readString(FILE* file, char** charLocation, int max){

	char buf[max];
	fgets(buf, max, file);
	int length = strlen(buf);
	if(buf[length-1] == '\n'){
		buf[length-1] = 0;
	}
	char* string = (char*) malloc(sizeof(char) * length);
	strcpy(string, buf);
	*charLocation = string;
}

/**
* Retrieves student information from file and allocates it dynamically in an
* array of student structs that returns a pointer to array of student structs
*/
Student* readStudents(FILE* file){

	Student* students = (Student*) malloc(sizeof(Student) * numStudents);

	int i = 0;

	while(i < numStudents){

		fscanf(file, "%d\n", &students[i].number);

		readString(file, &students[i].name, MAX_STUDENT_NAME_LENGTH);

		int al;
		fscanf(file, "%d\n", &al);
		students[i].al = al;

		i++;
	}
	return students;
}



/**
* Retrieves course information from file and allocates it dynamically in an
* array of course structs that returns pointer to array of course structs
*/
Course* readCourses(FILE* file){
	Course* courses = (Course*) malloc(sizeof(Course) * numCourses);
	int i = 0;
	while(i < numCourses){
		fscanf(file, "%d\n", &courses[i].number);
		readString(file, &courses[i].name, MAX_COURSE_NAME_LENGTH);
		readString(file, &courses[i].teacher, MAX_TEACHER_NAME_LENGTH);
		fscanf(file, "%d\n", &courses[i].numAssigns);
		courses[i].assigns = readAssigns(file, courses[i].numAssigns);
		courses[i].scores = readScores(file, courses[i].numAssigns);
		i++;
	}
	return courses;
}



/**
* Retrieves assignment information from file and allocates it dynamically in an array of
* assign structs that returns a pointer to array of assign structs
*/
Assign* readAssigns(FILE* file, int numAssigns){
	Assign* assigns = (Assign*) malloc(sizeof(Assign) * numAssigns);
	int i = 0;
	while(i < numAssigns){
		fscanf(file, "%d\n", &assigns[i].number);
		readString(file, &assigns[i].name, MAX_ASSIGN_NAME_LENGTH);
		i++;
	}
	return assigns;
}

/**
* Reads scores data from file into a dynamically allocated
* 2D array of ScoreStruct structs that returns a pointer to array of grade structs
*/
ScoreStruct** readScores(FILE* file, int numAssigns){

	ScoreStruct** scores = (ScoreStruct**) malloc(sizeof(ScoreStruct*) * numStudents);

	int i = 0;
	while(i < numStudents){
		scores[i] = (ScoreStruct*) malloc(sizeof(ScoreStruct) * numAssigns);
		i++;
	}

	int j = 0;
	while(j < numAssigns * numStudents){

		int studentNumber, assignNumber;

		fscanf(file, "%d\n%d\n", &assignNumber, &studentNumber);
		scores[studentNumber-1][assignNumber-1].studentNumber = studentNumber;
		scores[studentNumber-1][assignNumber-1].assignNumber = assignNumber;
		fscanf(file, "%f\n", &scores[studentNumber-1][assignNumber-1].score);
		readString(file, &scores[studentNumber-1][assignNumber-1].comment, MAX_COMMENT_LENGTH);
		j++;
	}
	return scores;
}

/**
* retrieves a string version of the student's AcedemicLevel that returns a string rep of the enum
*/
char* getLevelString(AcademicLevel level){

	if(level == Freshman){

		return "Freshman";

	} else if(level == Sophomore){

		return "Sophomore";

	} else if(level == Junior){

		return "Junior";

	} else {

		return "Senior";
	}
}

/**
* Prints students
*/
void printStudents(){

	int i = 0;

	while(i < numStudents){

		printf(" %d %s (%s)\n", students[i].number, students[i].name, getLevelString(students[i].al));
		i++;
	}
}

/**
* Prints assigns
*/
void printAssigns(Assign* assigns, int numAssigns){

	int i = 0;

	while(i < numAssigns){

		printf(" %d %s\n", assigns[i].number, assigns[i].name);
		i++;
	}
}

/**
* Prints grades
*/
void printGrades(ScoreStruct** scores, int numAssigns){

	int i = 0;
	int j = 0;

	while(i < numStudents){

		while(j < numAssigns){

			printf(" %d %d %.02f %s\n", scores[i][j].studentNumber, scores[i][j].assignNumber, scores[i][j].score, scores[i][j].comment);
			j++;
		}
		i++;
	}
}

/**
* Destroys students
*/
void studentsDestructor(){

	int i = 0;
	while(i < numStudents){

		free(students[i].name);
		i++;
	}
	free(students);
	students = NULL;
}

/**
* Destroys courses
*/
void coursesDestructor(){

	int i = 0;
	while(i < numCourses){

		assignsDestructor(&courses[i].assigns, courses[i].numAssigns);
		scoresDestructor(&courses[i].scores, courses[i].numAssigns);

		free(courses[i].name);
		free(courses[i].teacher);

		i++;
	}
	free(courses);
	courses = NULL;
}

/**
* Destroys assigns
*/
void assignsDestructor(Assign** assigns, int numAssigns){

	int i = 0;
	Assign* assignsArray = *assigns;

	while(i < 0){

		free(assignsArray[i].name);
		i++;
	}
	free(*assigns);
	*assigns = NULL;
}

/**
* Destroys scores
*/
void scoresDestructor(ScoreStruct*** scores, int numAssigns){

   int i = 0;
   int j = 0;

   ScoreStruct** scoresArray = *scores;

   while(i < 0){

	   while(j < 0){

		   free(scoresArray[i][j].comment);
		   j++;
	   }
	   free(scoresArray[i]);
	   i++;
   }
   free(scoresArray);
   *scores = NULL;
}

/**
* Prompts user to select an assignment
*/
void assignmentMenu(Course course){

	printf("\nPlease choose from the following assignments: \n");
	printAssigns(course.assigns, course.numAssigns);
	printf(" 0 RETURN TO PREVIOUS MENU\n");

	int assignNumber;
	printf("Please enter your choice ---> ");
	scanf(" %d", &assignNumber);

	while(assignNumber < 0 || assignNumber > course.numAssigns){

		scanf(" %d", &assignNumber);
	}

	if (assignNumber == 0){

		printf("Returning to previous menu...\n");
		return;

	} else {

		getAssignmentScore(course, assignNumber);
	}
}


/**
* Prints the average score for a chosen assignment
*/
void getAssignmentScore(Course course, int assignNo){
double total = 0, average = 0;
int i;
for(i = 0; i < numStudents; i++){
total += course.scores[i][assignNo-1].score;
}
average = total / numStudents;
printf("\nThe average grade on %s was %.2f.\n", course.assigns[assignNo-1].name, average);
}

/**
* Prompts user to select a student
*/
void studentMenu(Course course){

	printf("\nPlease choose from the following students: \n");
	printStudents();
	printf(" 0 RETURN TO PREVIOUS MENU\n");

	int studentNumber;
	printf("Please enter your choice ---> ");
	scanf(" %d", &studentNumber);

	while(studentNumber < 0 || studentNumber > numStudents ){

		printf("Please enter a valid option ---> ");
		scanf(" %d", &studentNumber);
	}

	if (studentNumber == 0){

		printf("Returning to previous menu...\n");
		return;

	} else {

		getStudentScores(course, studentNumber);
	}
}

/**
* Prints student scores
*/
void getStudentScores(Course course, int studentNumber){

	double total = 0, average = 0;

	printf("\n%s's assignment specific grades were: \n\n", students[studentNumber-1].name);
	printf(" %-16s%-16s%-16s\n", "Assign Name", "Score", "Comment");
	printf("-----------------------------------------------\n");

	int i = 0;
	while(i < course.numAssigns){

		total += course.scores[studentNumber - 1][i].score;
		printf(" %-16s%-16.2f%-16s\n", course.assigns[i].name, course.scores[studentNumber - 1][i].score, course.scores[studentNumber - 1][i].comment);
		i++;
	}

	average = total / course.numAssigns;
	printf("\n%s's final grade was %.2f.\n", students[studentNumber - 1].name, average);
}

/**
* Prints course information
*/
void printCourse(Course course){

	printf("\nCourse ID: %d\n", course.number);
    printf("Course Name: %s\n", course.name);
    printf("Teacher: %s\n", course.teacher);
    printf("Assigns: \n");

    printAssigns(course.assigns, course.numAssigns);

    printf("Grade Data: \n");
    printGrades(course.scores, course.numAssigns);
}

/**
* Implements main menu functionality, which allows user to select a course to interact with
*/
void mainMenu(){
  int input_buffer;
  printf("Course Searcher");
  do {
	printf("\n-----------------------------------\n");
    printf("Course Options");
	printf("\n-----------------------------------\n");
	int i;
	for(i = 0; i < numCourses; i++){
      printf("   %d %s\n", courses[i].number, courses[i].name);
    }
	printf("   0 REPEAT OPTIONS\n");
    printf("  -1 TERMINATE PROGRAM\n");
	printf("Please enter your choice ---> ");
	scanf(" %d", &input_buffer);
    mainMenuBranch(input_buffer);
  } while (1);
}

/**
* Handles menu options of main menu
* @param option is the chosen operation's option #
*/
void mainMenuBranch(int option){
  if (option < -1 || option > numCourses){
    printf("Invalid input. Please try again...\n");;
    while(option < -1 || option > numCourses){
  	  printf("Please enter a valid option ---> ");
      scanf(" %d", &option);
    }
  }
  if(option == -1){
	printf("Terminating program...\n");
    terminate();
  } else if (option == 0) {
	printf("Repeating options...\n");
  } else {
    Course course = courses[option - 1];
    printf("Course with name %s selected.\n", course.name);
	subMenu(course);
  }
}

/**
* Implements sub menu functionality, which allows users to select how they want to interact with course
* @course is the course to be queried
*/
void subMenu(Course course){
  int input_buffer;
  do {
    printf("\n-----------------------------------\n");
    printf("Menu Options");
	printf("\n-----------------------------------\n");
    printf("   1 Get a student's final grades in the course\n");
	printf("   2 Get the average grade of an assignment in the course\n");
    printf("   3 Print all course data\n");
	printf("   0 REPEAT OPTIONS\n");
	printf("  -1 RETURN TO PREVIOUS MENU\n");
    printf("  -2 TERMINATE PROGRAM\n");
	printf("Please enter your choice ---> ");
	scanf(" %d", &input_buffer);
    subMenuBranch(input_buffer, course);
  } while (input_buffer != -1);
}

/**
* Handles menu options of submenu
* @param option is the chosen operation's option #
* @param course is the course struct to be queried
*/
void subMenuBranch(int option, Course course){
  if (option < -2 || option > 3){
    printf("Invalid input. Please try again...\n");;
    while(option < -2 || option > 3){
  	  printf("Please enter a valid option ---> ");
      scanf(" %d", &option);
    }
  }
  if(option == -2){
    printf("Terminating program...\n");
	terminate();
  } else if (option == -1){
	printf("Returning to previous menu...\n");
  } else if (option == 0){
    printf("Repeating options...\n");
  } else if (option == 1){
    studentMenu(course);
  } else if (option == 2){
	  assignmentMenu(course);
  } else if (option == 3){
	  printCourse(course);
  }
}


/**
* Prints basic usage instructions for the program to the command line
*/
void print_usage(){
  printf("USAGE:\n./BaseReader -d <data_file_name(char*)> -c <instruction_file_name(char*)>\n");
  printf("-d refers to the required input data file containing student & course information; this must be a valid .txt file\n");
  printf("-i refers to the optionally supported 'instruction file' that provides directions for how the program should execute without CLI input; \n\t must be a valid .txt.file\n");
}

void terminate(){
  //FREE EVERYTHING HERE
  exit(1);
}

int main(int argc, char* argv[]){
  char* datafile;
  char* instructionfile;
  int opt;
  while((opt = getopt(argc, argv, ":d:i:")) != -1){
    switch(opt){
      case 'd':
        datafile = optarg;
        break;
      case 'i':
        instructionfile = optarg;
        break;
      case ':':
		printf("option needs a value\n");
		break;
      case '?':
        printf("unknown option: %c\n", optopt);
        break;
    }
  }
  for (; optind < argc; optind++){
    printf("Given extra arguments: %s\n", argv[optind]);
  }

  int dflen;
  if(datafile != NULL){
    dflen = strlen(datafile);
    if(dflen >= 5
		&& (strcmp(&datafile[dflen-4], ".txt") == 0)
        && (access(datafile, F_OK) != -1)){
      printf("Importing data from %s\n\n", datafile);
	  readFile(datafile);
    } else {
	  printf("Data file has an invalid name or does not exist.\n");
      print_usage();
	  exit(1);
    }
  } else {
    printf("No data file name provided. This is a required field.\n");
    print_usage();
	exit(1);
  }

  printf("No instruction file provided. Using CLI:\n");
  mainMenu();

  return 0;
}

#endif
