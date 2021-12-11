/*
 *  Name:           Your Name
 *  Date:           Month Date, Year
 *  Assignment:     XXXXXXXXXX
 *  Description:
 ************************************************************/

#include <stdio.h>

char *chosen_key;
char *chosen_salt;
unsigned chosen_passphrase;

/* Creates a hashed key */
/* Jenkins, Bob (November 3, 2013). "A hash function for hash Table lookup". Retrieved December 5, 2021. */
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

/* Creates a hashed salt */
/* Brian Kernighan, Dennis Ritchie (1978 First Edition). "The C Programming Language". Retrieved December 5, 2021. */
unsigned hash(char *s) {
   
    unsigned hashval;

    for (hashval = 0; *s != '\0'; s++) {
        hashval = *s + 31*hashval;
    }
    return hashval;
}

/* Simple Comparison to check unsigned values, and pointer values */
void unlock(unsigned trial_passphrase, char * temp1, char * temp2){

    if(trial_passphrase == chosen_passphrase && temp1 == chosen_key && temp2 == chosen_salt) {
        printf("Success!: The passphrase is %X, separate words are %s, %s\n", trial_passphrase, temp1, temp2);
        
        //Code Here: Print the program runtime!

        exit(0);
    }
}

int main(int argc, char *argv[]) {

    //  Read the dictionary and store it however you want

    //  if (argc == 2) [both c_key and c_salt point to the same word]
    //      accept command line argument as a dictionary index
    //      set chosen_key as dictionary index              
    //      set chosen_salt as dictionary index                    
    //  else
    //      set chosen_key as random dictionary index
    //      set chosen_salt as random dictionary index

    //  make a master key (globally so unlock() can see it)
    //  make a master salt (globally so unlock() can see it)
    //  make a master passphrase (globally so unlock() can see it)

    //  Some how loop through the dictionary and: [Hint: O(n^2)]
    //       grab first word          
    //       grab second word
    //       make a trial key
    //       make a trial salt
    //       make a trial passphrase
    //       unlock(?,?,?);  
    //  
}

// To compile: $ make <programName>
// To run:     $ (time ./brute 1000) >> a.out 2>&1          <if you want specific indexes>
// Or:         $ (time ./brute) >> a.out 2>&1               <if you want random indexes>