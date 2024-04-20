#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include "hangman.h"


int get_word(char secret[]){
    // check if file exists first and is readable
    FILE *fp = fopen(WORDLIST_FILENAME, "rb");
    if( fp == NULL ){
        fprintf(stderr, "No such file or directory: %s\n", WORDLIST_FILENAME);
        return 1;
    }

    // get the filesize first
    struct stat st;
    stat(WORDLIST_FILENAME, &st);
    long int size = st.st_size;

    do{
        // generate random number between 0 and filesize
        long int random = (rand() % size) + 1;
        // seek to the random position of file
        fseek(fp, random, SEEK_SET);
        // get next word in row ;)
        int result = fscanf(fp, "%*s %20s", secret);
        if( result != EOF )
            break;
    }while(1);

    fclose(fp);

    return 0;
}

int is_word_guessed(const char secret[], const char letters_guessed[]) {
    int findFlag = 0;
    for(int i = 0; i < strlen(secret); i++) {
        findFlag = 0;
        for(int j = 0; j < strlen(letters_guessed); j++) {
            if (secret[i] == letters_guessed[j]) {
                findFlag = 1;
                break;
            }
        }
        if (findFlag == 0) {
            return 0;
        }
    }
    return 1;
}

 void get_guessed_word(const char secret[], const char letters_guessed[], char guessed_word[]) {
    int find = 0;
    for(int i = 0; i < strlen(secret); i++) {
        find = 0;
        for(int j = 0; j < strlen(letters_guessed); j++) {
            if (secret[i] == letters_guessed[j]) {
                find = 1;
                guessed_word[i] = secret[i];
            }
        }
            if (find == 0) {
                guessed_word[i] = '_';
            }
    }
 }

 void get_available_letters(const char letters_guessed[], char available_letters[]) {

    char alph[28] = "abcdefghijklmnopqrstuvwxyz";
    int alph_len = strlen(alph);
    int letters_len = strlen(letters_guessed);

    for (int i = 0; i < alph_len; i++) {
        available_letters[i] = alph[i];
        for (int j = 0; j < letters_len; j++) {
            if (letters_guessed[j] == alph[i]) {
                available_letters[i] = '_';
                break;
            }
        }
    }

    available_letters[alph_len] = '\0';

    int done = 0; // Наш флаг
    while (!done) {
        done = 1;
        for (int i = 0; i < strlen(available_letters)-1; i++) {
        if (available_letters[i] == '_') {
            //available_letters[i] = available_letters[i + 1];
            for (int j = i; j < strlen(available_letters); j++) {
                available_letters[j] = available_letters[j + 1];
            }
            done = 0;
            }
        }
     }
 }
