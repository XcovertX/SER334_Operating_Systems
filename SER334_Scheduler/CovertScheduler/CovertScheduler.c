/**
* The scheduler program receives input from a user to build a schedule.
* The user can add, drop, print, load and save schedule information.
*
* Completion time: 10hrs
*
* @author James Covert
* @version 1.0
*/

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


////////////////////////////////////////////////////////////////////////////////
//MACROS: CONSTANTS



////////////////////////////////////////////////////////////////////////////////
//DATA STRUCTURES
typedef enum Subject{SER, EGR, CSE, EEE} Subject;

typedef struct CourseNode {
    enum Subject sub;
    int number;
    char teacher[1024];
    int credit_hours;
    struct CourseNode *next;
    struct CourseNode *last;
};


////////////////////////////////////////////////////////////////////////////////
//GLOBAL VARIABLES

//place to store course information
struct CourseNode* course_collection = NULL;

////////////////////////////////////////////////////////////////////////////////
//FORWARD DECLARATIONS
void branching(char option);

// creates a new node
void course_insert(struct CourseNode **head, Subject sub, int number, char *teacher, int credit_hours) {

    struct CourseNode *newNode = malloc(sizeof(struct CourseNode));
    newNode->sub = sub;
    newNode->number = number;
    strcpy(newNode->teacher, teacher);
    newNode->credit_hours = credit_hours;
    newNode->next = NULL;

    if (*head == NULL) {

        *head = newNode;

    } else {
        struct CourseNode *last = *head;

        while(last->next != NULL) {
            last = last->next;
        }

        last->next = newNode;
    }
}

// drops a specified course from course_collection
void course_drop(struct CourseNode** head, Subject sub, int num) {
    struct CourseNode *temp = *head, *prev;

    if (temp != NULL && temp->sub == sub && temp->number == num) {
        *head = temp->next; // Changed head
        free(temp); // free old head
        return;
    }

    while (temp != NULL && !(temp->sub == sub && temp->number == num)) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL)
        return;

    prev->next = temp->next;

    free(temp);
}

//prints the full class schedule
void schedule_print(struct CourseNode *head) {
    struct CourseNode *temp = head;
    printf("\nClass Schedule:\n");
    while (temp != NULL) {
        switch (temp->sub) {
            case SER: printf("SER");
            break;
            case EGR: printf("EGR");
            break;
            case CSE: printf("CSE");
            break;
            case EEE: printf("EEE");
            break;
        }
        printf("%d", temp->number);
        printf("   ");
        printf("%d", temp->credit_hours);
        printf("   ");
        printf("%s\n", temp->teacher);
        temp = temp->next;
    }
}

//retrieves the total credit hours
int totalCreditHours(struct CourseNode *head) {
    struct CourseNode *temp = head;
    int total = 0;

    while(temp != NULL) {
        total = total + temp->credit_hours;
        temp = temp->next;
    }
    return total;
}

// Loads the schedule for the 'scheduler.txt'
void schedule_load() {
    FILE *fp;
    fp = fopen("schedule.txt", "r"); // read mode

    if (fp == NULL) {
        fclose(fp);
        return;
    }
    int s;
    int n;
    char t[100];
    int chrs;

    while (fscanf (fp, "%d %d %s %d", &s, &n, t, &chrs) == 4) {
        course_insert(&course_collection, s, n, t, chrs);
    }
}
// Saves the schedule to the file 'scheduler.txt'
void course_save() {
    struct CourseNode *temp = course_collection;
    char sentence[1000];

    FILE *fp;

    fp = fopen("schedule.txt", "w");

    if (fp == NULL) {
        return;
    }
    while (temp != NULL) {
        fprintf(fp,"%d %d %s %d\n", temp->sub, temp->number, temp->teacher, temp->credit_hours);
        temp = temp->next;
    }
}
// frees the
void free_all_memory(struct Course_Node **head) {
    struct CourseNode *temp = *head, *next;

    while (temp != NULL) {
        next = temp->next;
        free(temp);
        temp = next;
    }
}

//main entry point. Starts the program by displaying a welcome and beginning an
//input loop that displays a menu and processes user input. Pressing q quits.
int main() {
    char input_buffer;

    printf("\n\nWelcome to ASU Class Schedule\n");

    schedule_load();

    //menu and input loop
    do {
        printf("\nMenu Options\n");
        printf("------------------------------------------------------\n");
        printf("a: Add a class\n");
        printf("d: Drop a class\n");
        printf("s: Show your classes\n");
        printf("q: Quit\n");
        printf("\nTotal Credits: %d\n\n", totalCreditHours(course_collection));
        printf("Please enter a choice ---> ");

        scanf(" %c", &input_buffer);

        branching(input_buffer);
    } while (input_buffer != 'q');

    course_save();
    free_all_memory(&course_collection);

    return 0;
}

//takes a character representing an inputs menu choice and calls the appropriate
//function to fulfill that choice. display an error message if the character is
//not recognized.
void branching(char option) {
    switch (option) {
        case 'a':
            printf("What is the subject? (SER=0, EGR=1 CSE=2, EEE=3)\n");
            enum Subject sub;
            scanf("%d", &sub);
            printf("What is the number?\n");
            int num;
            scanf("%d", &num);
            printf("Who is the teacher?\n");
            char teacher[1024], *p;
            scanf("%1023s", teacher);
            p = &teacher[0];
            printf("How many credit hours?\n");
            int ch;
            scanf("%d", &ch);

            printf("%s", teacher);
            course_insert(&course_collection, sub, num, p, ch);

            break;

        case 'd':
            printf("What is the subject? (SER=0, EGR=1 CSE=2, EEE=3)\n");
            scanf("%d", &sub);
            printf("What is the number?\n");
            scanf("%d", &num);

            course_drop(&course_collection, sub, num);
            break;

        case 's':
            schedule_print(course_collection);
            break;

        case 'q':
            // main loop will take care of this.
            break;

        default:
            printf("\nError: Invalid Input.  Please try again...");
            break;
    }
}
