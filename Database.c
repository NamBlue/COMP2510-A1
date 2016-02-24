#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#define LINESIZE 1024
#define eof -2

/*
Method prototypes
*/
void openFile(FILE **file, const char address[]);
void closeFile(FILE **file);
void run(FILE *file);
void displayMenu(void);
int getChoice(void);
int get_id(char *id);
int get_grade(int *grade);
void append(FILE *file);
void modifyRecord(int n, FILE * file);
void displayAll(FILE *file);

/*
Main, the entry point.
-argv, must specify filename in command-line to read/write to.
*/
int main (int argc, char *argv[]) {
    FILE *record = '\0';
    char address[LINESIZE] = "./";
    
    if (argc < 2) {
        printf("\n%s\n%s\n", "Please specify a filename to store data"
            ,"in command line when executing this program!");
        return 1;
    }
    strcat(address, argv[1]);
    openFile(&record, address);
    run(record);
    closeFile(&record);
    return 0;
}

/*
Opens the file for storing records
*/
void openFile(FILE **file, const char address[]) {
    if ((*file = fopen(address, "rb+")) == 0) {
        perror("fopen");
        exit(0);
    }
}

/*
Closes the file for storing records
*/
void closeFile(FILE **file) {
    if (fclose(*file) != 0) {
        perror("fclose");
        exit(0);
    }
}

/*
Main menu interface
*/
void run(FILE *file) {
    int choice = 0;
    
    while (1) {
        displayMenu();
        choice = getChoice();
        if (choice == -2) {
            exit(0);
        } else if (choice == -1) {
            append(file);
        } else if (choice == 0) {
            displayAll(file);
        } else if (choice > 0) {
            modifyRecord(choice, file);
        } else {
            printf("\n%s\n", "Invalid choice, try again!");
        }
    } 
}

/*
Displays the menu
*/
void displayMenu(void) {
    printf("\n%s\n\n%s\n\n", "Welcome to my database!"
            ,"Please choose a option: ");
    printf("%s\n%s\n%s\n%s\n\n%s"
            ," #) Modify record # if it exists"
            ," 0) Display All"
            ,"-1) Append a record"
            ,"-2) Exit", ">");
}

/*
Returns the choice from user input
*/
int getChoice(void) {
    char input[LINESIZE];
    int output = 0;
    
    if (!fgets(input, LINESIZE, stdin)) {
        clearerr(stdin);
        return eof;
    }
    if (sscanf(input, "%d", &output) == 1) {
        if (output == eof) {
            return eof;
        }
        return output;
    }
    return -3;
}

/*
Prompts for and validates user input for Student ID appending or modifying
-returns a non zero value on failure, 0 for success
*/
int get_id(char *id) {
    char input[LINESIZE];
    char buffer[LINESIZE];
    
    while (1) {
        printf("\n%s", "Enter the Student ID or -1 to return to main menu>");
        if (!fgets(input, LINESIZE, stdin)) {
            clearerr(stdin);
            return 1;
        }
        if (sscanf(input, "%s", buffer) == 1) {
            if (strcmp(buffer, "-1") == 0) {
                return -1;
            } 
            if (strlen(buffer) == 9 && (char)tolower((int)buffer[0]) == 'a') {
                int i;
                for (i = 1; i < 9; i++) {
                    if (buffer[i] < '0' || buffer[i] > '9') {
                        break;
                    }
                }
                if (i == 9) {
                    sscanf(buffer, "%s ", id);
                    return 0;
                }
            }
        }
        printf("\n%s\n", "Invalid input, format must be \"a12345678\" , try again!");
    }
}

/*
Prompts for and validates user input for Student Grade appending or modifying
*/
int get_grade(int *grade) {
    char input[LINESIZE];
    
    while (1) {
        printf("\n%s", "Enter the Student Grade or -1 to return to main menu>");
        if (!fgets(input, LINESIZE, stdin)) {
            clearerr(stdin);
            return 1;
        }
        if (sscanf(input, "%d", grade) == 1) {
            if (*grade == -1) {
                return -1;
            } 
            if (*grade >= 0 && *grade <= 100) {
                return 0;
            }
        }
        printf("\n%s%s\n", "Invalid input, grade must be ",
            "between 0 and 100 inclusive , try again!");
    }
}

/*
Appends a record to the file
*/
void append(FILE *file) {
    char id[11];
    int grade;
    
    if (fseek(file, 0, SEEK_END) != 0) {
        perror("fseek");
    }
    printf("\n%s\n", "Append a record");
    if (get_id(id) == 0 && get_grade(&grade) == 0) {
        fprintf(file, "%s %-3d\n", id, grade);
        printf("\n%s\n", "Appending record...");
    } else {
        printf("\n%s\n", "Exiting append mode...");
    }
}

/*
Modifies the record at n
*/
void modifyRecord(int n, FILE * file) {
    int recnum = n;
    char buffer[LINESIZE];
    char buffer2[LINESIZE];
    char id[11];
    int grade;
    
    n = 15 * (n - 1);
    if (fseek(file, n, SEEK_SET) != 0) {
        perror("fseek");
    }
    if (fscanf(file, "%s %s", buffer, buffer2) == EOF) {
        printf("\nRecord %d does not exist!\n", recnum);
        return;
    }
    printf("\nModifying record %d:\n", recnum);
    printf("%-3d %-8s %-3s\n", recnum, buffer, buffer2);
    if (fseek(file, n, SEEK_SET) != 0) {
        perror("fseek");
    }
    if (get_id(id) == 0 && get_grade(&grade) == 0) {
        fprintf(file, "%s %-3d", id, grade);
        printf("\n%s\n", "Modifying record...");
    } else {
        printf("\n%s\n", "Exiting modify mode...");
    }
}

/*
Displays all records
*/
void displayAll(FILE *file) {
    char buffer[LINESIZE];
    char buffer2[LINESIZE];
    int recnum = 0;
    
    rewind(file);
    while(fscanf(file, "%s %s", buffer, buffer2) != EOF) {
        recnum++;
        printf("%-3d %-8s %-3s\n", recnum, buffer, buffer2);
    }  
}