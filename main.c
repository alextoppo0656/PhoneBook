#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1000

typedef struct{
    char name[100];
    char number[100];
}Record;

int menu();
FILE *openFile(FILE *, char);
int sizePhonebook(FILE *);
int validNumber(char [100]);
int duplicateNumber(FILE *, char [100]);
void add(FILE *);
void list(FILE *);
void edit(FILE *);
void delete(FILE *);

int main(){

    FILE *fptr;

    fptr = openFile(fptr, 'r');

    if(fptr == NULL){
        fclose(fptr);
        fptr = openFile(fptr, 'w');
    }
    fclose(fptr);

    int choice, size;
    while(1){
        choice = menu();

        size = sizePhonebook(fptr);

        if(choice == 1){
            if(size == 1000){
                printf("\nMaximum Limit Reached!");
                continue;
            }
            add(fptr);
        }
        else if(choice == 2){
            if(size == 0){
                printf("\nPhonebook empty!");
                continue;
            }
            list(fptr);
        }
        else if(choice == 3){
            if(size == 0){
                printf("\nPhonebook empty!");
                continue;
            }
            edit(fptr);
        }
        else if(choice == 4){
            if(size == 0){
                printf("\nPhonebook empty!");
                continue;
            }
            delete(fptr);
        }
        else if(choice == 5){
            printf("\n\nTerminated. . .");
            break;
        }
        else{
            printf("\nInvalid Choice!");
        }
    }

    return 0;
}

int menu(){
    printf("\n\n-----------MENU------------");
    printf("\n\t1. Add");
    printf("\n\t2. List");
    printf("\n\t3. Edit");
    printf("\n\t4. Delete");
    printf("\n\t5. Terminate");
    printf("\n---------------------------");
    printf("\nEnter your choice : ");
    int choice;
    scanf("%d", &choice);
    return choice;
}

// Function to open the phonebook file in the required form.
FILE *openFile(FILE *fptr, char ch){
    if(ch == 'r'){
        fptr = fopen("phonebook.txt", "r");
    }
    else if(ch == 'w'){
        fptr = fopen("phonebook.txt", "w");
    }
    else if(ch == 'a'){
        fptr = fopen("phonebook.txt", "a");
    }

    return fptr;
}

// Function to get the size of the phonebook.
int sizePhonebook(FILE *fptr){

    Record records[MAX_SIZE];

    fptr = openFile(fptr, 'r');
    char s[100];
    int c = 0, i = 0;
    while(fgets(s, 100, fptr) != NULL){
        strtok(s, "\n");
        if(c % 2 != 1){
            strcpy(records[i].name, s); 
        }
        else{
            strcpy(records[i++].number, s);
        }
        c++;
    }
    fclose(fptr);

    return i;
}

// Function to check if the number is valid or not.
int validNumber(char number[100]){
    strtok(number, "\n");
    int size = strlen(number);

    if(size < 10 || size > 12){
        return 0;
    }

    if(size == 10){
        char f = number[0];
        int flag = 0;
        for(int i=6; i<=9; i++){
            if((f-'0') == i){
                flag = 1;
                break;
            }
        }
        return flag;
    }
    else if(size == 11){
        char f = number[0];
        if(f != '0'){
            return 0;
        }
        char s = number[1];
        int flag = 0;
        for(int i=6; i<=9; i++){
            if((s-'0') == i){
                flag = 1;
                break;
            }
        }
        return flag;
    }
    else if(size == 12){
        char f = number[0];
        char s = number[1];
        if(f != '9' || s != '1'){
            return 0;
        }
        char t = number[2];
        int flag = 0;
        for(int i=6; i<=9; i++){
            if((t-'0') == i){
                flag = 1;
                break;
            }
        }
        return flag;
    }
}

// Function to check if there is a duplicate number in the Phonebook already.
int duplicateNumber(FILE *fptr, char number[100]){
    strtok(number, "\n");
    fptr = openFile(fptr, 'r');
    char s[100];
    int c = 0, i = 0;
    int flag = 0;
    char name[100];
    while(fgets(s, 100, fptr) != NULL){
        strtok(s, "\n");
        if(c % 2 != 1){
            strcpy(name, s);
        }
        else{
            flag = strcmp(s, number) == 0 ? 1 : 0;
            if(flag == 1){
                fclose(fptr);
                printf("\nNumber Already Exists with the name '%s'!", name);
                return 1;
            }
        }
        c++;
    }
    fclose(fptr);
    return flag;
}

// Function to add record in the phonebook file.
void add(FILE *fptr){
    char name[100];
    char number[100];
    printf("\nEnter the name : ");
    scanf("\n");
    fgets(name, 100, stdin);
    printf("\nEnter the number : ");
    scanf("\n");
    fgets(number, 100, stdin);
    char numberCopy[100];
    strcpy(numberCopy, number);
    if(!validNumber(numberCopy)){
        printf("\nInvalid Number!");
        fclose(fptr);
        return;
    }
    if(duplicateNumber(fptr, numberCopy)){
        fclose(fptr);
        return;
    }
    fptr = openFile(fptr, 'a');
    fprintf(fptr, "%s%s", name, number);
    fclose(fptr);
    printf("\nAdded Record Successfully!");
}

// Function to List the records in the phonebook.
void list(FILE *fptr){
    fptr = openFile(fptr, 'r');

    char prev, curr;
    int count = 0;
    printf("\n-----------------Phonebook-----------------\n");
    printf("\nName : ");
    prev = fgetc(fptr);
    curr = prev;
    while(curr != EOF){
        curr = fgetc(fptr);
        if(prev== '\n' && count % 2 == 0){
            printf("\t\tNumber : ");
            count++;
        }
        else if(prev == '\n' && count % 2 != 0 && curr != EOF){
            printf("\nName : ");
            count++;
        }
        else{
            putchar(prev);
        }
        prev = curr;
    }
    printf("\n-------------------------------------------\n");

    fclose(fptr);
}

// Function to edit the number for a specific name in the phonebook.
void edit(FILE *fptr){

    char editName[100];
    printf("\nEnter the name you want to edit for : ");
    scanf("\n");
    fgets(editName, 100, stdin);
    strtok(editName, "\n");

    Record records[MAX_SIZE];

    fptr = openFile(fptr, 'r');
    char s[100];
    int c = 0, i = 0;
    int index[MAX_SIZE], ind = 0;
    while(fgets(s, 100, fptr) != NULL){
        strtok(s, "\n");
        if(strcmp(s, editName) == 0){
            index[ind++] = i;
        }
        if(c % 2 != 1){
            strcpy(records[i].name, s); 
        }
        else{
            strcpy(records[i++].number, s);
        }
        c++;
    }
    fclose(fptr);

    if(ind == 0){
        printf("\nNo name %s found in Phonebook!", editName);
    }
    else{
        printf("\n----------------------------------------------------------");
        if(ind == 1){
            printf("\n1. Name : %s \t\t Number : %s", records[index[0]].name, records[index[0]].number);
        }
        else{
            printf("\nThese are the names %s found in Phonebook :\n", editName);
            for(int ii = 0; ii < ind; ii++){
                printf("\n%d. Name : %s \t\t Number : %s",(ii+1), records[index[ii]].name, records[index[ii]].number);
            }
        }
        printf("\n----------------------------------------------------------");

        printf("\nEnter the serial number whose number you want to edit (1/2/3...) : ");
        int serial;
        scanf("%d", &serial);

        if(serial <= ind && serial > 0){
            char num[100];
            printf("\nEnter the new number : ");
            scanf("%s", &num);
            char numberCopy[100];
            strcpy(numberCopy, num);
            if(!validNumber(numberCopy)){
                printf("\nInvalid Number!");
                return;
            }
            if(duplicateNumber(fptr, numberCopy)){
                fclose(fptr);
                return;
            }

            strcpy(records[index[serial-1]].number, num);

            // Copy the new Data in the File.
            fptr = openFile(fptr, 'w');
            FILE *fp = fptr;
            fclose(fptr);
            fptr = fp;
            fptr = openFile(fptr, 'a');
            for(int ii=0; ii<i; ii++){
                fprintf(fptr, "%s\n%s\n", records[ii].name, records[ii].number);
            }
            fclose(fptr);
            printf("\nEdited Record Successfully!");
        }
        else{
            printf("\n\nINVALID SERIAL NUMBER!");
        }
    }
}

// Function to delete the record of a specific name in the Phonebook.
void delete(FILE *fptr){

    char deleteName[100];
    printf("\nEnter the name you want to delete : ");
    scanf("\n");
    fgets(deleteName, 100, stdin);
    strtok(deleteName, "\n");

    Record records[MAX_SIZE];

    fptr = openFile(fptr, 'r');
    char s[100];
    int c = 0, i = 0;
    int index[MAX_SIZE], ind = 0;
    while(fgets(s, 100, fptr) != NULL){
        strtok(s, "\n");
        if(strcmp(s, deleteName) == 0){
            index[ind++] = i;
        }
        if(c % 2 != 1){
            strcpy(records[i].name, s); 
        }
        else{
            strcpy(records[i++].number, s);
        }
        c++;
    }
    fclose(fptr);

    if(ind == 0){
        printf("\nNo name '%s' found in Phonebook!", deleteName);
    }
    else{
        printf("\n----------------------------------------------------------");
        if(ind == 1){
            printf("\n1. Name : %s \t\t Number : %s", records[index[0]].name, records[index[0]].number);
        }
        else{
            printf("\nThese are the names %s found in Phonebook :\n", deleteName);
            for(int ii = 0; ii < ind; ii++){
                printf("\n%d. Name : %s \t\t Number : %s",(ii+1), records[index[ii]].name, records[index[ii]].number);
            }
        }
        printf("\n----------------------------------------------------------");

        printf("\nEnter the serial number whose record you want to delete (1/2/3...) : ");
        int serial;
        scanf("%d", &serial);

        if(serial <= ind && serial > 0){
            fptr = openFile(fptr, 'w');
            FILE *fp = fptr;
            fclose(fptr);
            fptr = fp;
            fptr = openFile(fptr, 'a');
            for(int ii=0; ii<i; ii++){

                // Ignoring the Data that is to be deleted and printing the next record in the File.
                if(index[serial-1] == 0 && i == 1){
                    fclose(fptr);
                    fptr = openFile(fptr, 'w');
                    fclose(fptr);
                    printf("\nDeleted Record Successfully!");
                    return;
                }
                if(ii == index[serial-1] && ii != (i-1)){
                    fprintf(fptr, "%s\n%s\n", records[ii+1].name, records[ii+1].number);
                    ii++;
                    continue;
                }
                else if(ii == index[serial-1] && ii == (i-1)){
                    fprintf(fptr, "%s%s", records[ii+1].name, records[ii+1].number);
                    ii++;
                    continue;
                }
                fprintf(fptr, "%s\n%s\n", records[ii].name, records[ii].number);
            }
            fclose(fptr);
            printf("\nDeleted Record Successfully!");
        }
        else{
            printf("\n\nINVALID SERIAL NUMBER!");
        }
    }
}