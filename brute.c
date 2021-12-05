/*
 *	Name: 			In Woo Park
 *	Date: 			November 25, 2021
 *  Assignment: 	Final Project
 *
 ************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <omp.h>

#define MAX_SIZE 466550
#define MAX_WORD_LEN 30

char * chosen_key;
char * chosen_salt;
unsigned chosen_passphrase;

uint32_t jenkins_one_at_a_time_hash(char *key, size_t len) {
    uint32_t hash, i;
    for(hash = i = 0; i < len; ++i)
    {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

//https://stackoverflow.com/questions/7666509/hash-function-for-string
unsigned hash(char *s) {
   
    unsigned hashval;

    for (hashval = 0; *s != '\0'; s++) {
        hashval = *s + 31*hashval;
    }
    return hashval;
}

void unlock(unsigned trial_passphrase, char * temp1, char * temp2){

    if(trial_passphrase == chosen_passphrase && temp1 == chosen_key && temp2 == chosen_salt) {
        printf("Success!: The passphrase is %X, separate words are %s, %s\n", trial_passphrase, temp1, temp2);
        exit(0);
    }
}

int main(int argc, char *argv[]) {
 
    /* Setup: Grab words and store them into 2D array */
    FILE * fp = fopen("dictionary.txt", "r");

    char ** dictionary = NULL;
    char * line = calloc(MAX_SIZE, sizeof(char));

    for (int i = 0 ; fgets(line, sizeof(line) + 1, fp) != NULL ; i++) { 
        dictionary = realloc(dictionary, (i + 1) * sizeof(char *));
        line[strcspn(line, "\n")] = 0; 
        dictionary[i] = strdup(line);
        memset(line, MAX_SIZE, '\0');
    }

    fclose(fp);

    //printf("dictionary 2: %s", dictionary[0]);

    srand(time(NULL));
    int random_word1 = rand() % 466549;
    int random_word2 = rand() % 466649;
    chosen_key = dictionary[796648];
    chosen_salt = dictionary[796649];

    printf("The words selected are: #%d: %s, #%d: %s,\n", random_word1 + 1, chosen_key, random_word2 +1, chosen_salt);


    int row = MAX_SIZE;
    size_t column1 = strlen(chosen_key);

    unsigned key = jenkins_one_at_a_time_hash(chosen_key, column1); 
    unsigned salt = hash(chosen_salt);
    chosen_passphrase = key + salt;


    #pragma omp parallel for 
    for(int i = 0; i < MAX_SIZE; i++) {
        for (int j = 0; j < MAX_SIZE; j++) {

                char *temp1 = dictionary[i];     
                char *temp2 = dictionary[j];      
                int row = MAX_SIZE;

                size_t column1 = strlen(temp1);   //3 (length of the word)

                unsigned trial_key = jenkins_one_at_a_time_hash(temp1, column1);
                unsigned trial_salt = hash(temp2);
                unsigned trial_passphrase = trial_key + trial_salt;

                unlock(trial_passphrase, temp1, temp2); 
        }
    }
    printf("Failed to find password.\n");
}