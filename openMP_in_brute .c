/*
 *  Name:           In Woo Park
 *  Date:           November 25, 2021
 *  Assignment:     Final Project
 *  Description:    A Program that will do array comparisons 
 *                   in parallel using OpenMP.
 *
 ************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <omp.h>

#define MAX_SIZE 466553
#define MAX_WORD_LEN 30

/* These will be used by the unlock() function */
char *chosen_key;
char *chosen_salt;
unsigned chosen_passphrase;
struct timespec begin, end;
double total;
long arg;

/* Jenkins, Bob (November 3, 2013). "A hash function for hash Table lookup". Retrieved December 5, 2021. */
uint32_t jenkins_one_at_a_time_hash(char *key, size_t len) {
    uint32_t hash, i;
    for(hash = i = 0; i < len; ++i) {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

/* Brian Kernighan, Dennis Ritchie (1978 First Edition). "The C Programming Language". Retrieved December 5, 2021. */
unsigned hash(char *s) {
   
    unsigned hashval;

    for (hashval = 0; *s != '\0'; s++) {
        hashval = *s + 31*hashval;
    }
    return hashval;
}

/* Check if the hashes are the same. Also check the actual word to remove hash collisions. */
void unlock(unsigned trial_passphrase, char * temp1, char * temp2){

    if(trial_passphrase == chosen_passphrase && temp1 == chosen_key && temp2 == chosen_salt) {
        clock_gettime(CLOCK_MONOTONIC_RAW, &end);
        total = (end.tv_nsec - begin.tv_nsec) / 1000000000.0 + (end.tv_sec  - begin.tv_sec);

        printf("Success!: The passphrase is %X, separate words are %s, %s\n", trial_passphrase, temp1, temp2);
        printf ("Finished all computations in %f seconds\n", total);
        exit(0);
    }
}

int main(int argc, char *argv[]) {

    clock_gettime(CLOCK_MONOTONIC_RAW, &begin);

    /* Setup: Grab words and store them into 2D array */
    FILE *fp = fopen("dictionary.txt", "r");

    char **dictionary = NULL;
    char *line = (char*) malloc(MAX_WORD_LEN);

    int idx = 0;
    
    while(fgets(line, MAX_WORD_LEN, fp) != NULL) {
        dictionary = (char**) realloc(dictionary, (idx + 1) * sizeof(char*));
        line[strcspn(line, "\n")] = 0;
        dictionary[idx] = strdup(line);
        memset(line, '\0', MAX_WORD_LEN);
        idx+=1;
    }

    fclose(fp);

    /* Create a Master Passphrase */
    srand(time(NULL));
    int random_word1 = rand() % 466549;
    int random_word2 = rand() % 466549;

    chosen_key = dictionary[random_word1];  //466552
    chosen_salt = dictionary[random_word2]; //466552

    if (argc == 2) {
        arg = strtol(argv[1], NULL, 10);
        chosen_key = dictionary[arg];  //466552
        chosen_salt = dictionary[arg]; //466552
        printf("The words selected are: #%ld: %s, #%ld: %s,\n", arg, chosen_key, arg, chosen_salt);
    } else {
        printf("The words selected are: #%d: %s, #%d: %s,\n", random_word1 + 1, chosen_key, random_word2 +1, chosen_salt);
    }

    int row = MAX_SIZE;
    size_t column1 = strlen(chosen_key);

    unsigned key = jenkins_one_at_a_time_hash(chosen_key, column1); 
    unsigned salt = hash(chosen_salt);
    chosen_passphrase = key + salt;

    /* Find the master passphrase in O(n^2) time. */
    int i, j;
    for(i = 0; i < MAX_SIZE; i++) {
        #pragma omp parallel for
        for (j = 0; j < MAX_SIZE; j++) {

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

//(time ./brute 1000) >> a.out 2>&1