#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctype.h>
#include <string.h>
#include "hangman.h"

int get_word(char secret[])
{
    // check if file exists first and is readable
    FILE *fp = fopen(WORDLIST_FILENAME, "rb");
    if (fp == NULL)
    {
        fprintf(stderr, "No such file or directory: %s\n", WORDLIST_FILENAME);
        return 1;
    }

    // get the filesize first
    struct stat st;
    stat(WORDLIST_FILENAME, &st);
    long int size = st.st_size;

    do
    {
        // generate random number between 0 and filesize
        long int random = (rand() % size) + 1;
        // seek to the random position of file
        fseek(fp, random, SEEK_SET);
        // get next word in row ;)
        int result = fscanf(fp, "%*s %20s", secret);
        if (result != EOF)
            break;
    } while (1);

    fclose(fp);

    return 0;
}

int is_word_guessed(const char secret[], const char letters_guessed[])
{
    int findFlag = 0;
    for (int i = 0; i < strlen(secret); i++)
    {
        findFlag = 0;
        for (int j = 0; j < strlen(letters_guessed); j++)
        {
            if (secret[i] == letters_guessed[j])
            {
                findFlag = 1;
                break;
            }
        }
        if (findFlag == 0)
        {
            return 0;
        }
    }
    return 1;
}

void get_guessed_word(const char secret[], const char letters_guessed[], char guessed_word[])
{
    int find = 0;
    for (int i = 0; i < strlen(secret); i++)
    {
        find = 0;
        for (int j = 0; j < strlen(letters_guessed); j++)
        {
            if (secret[i] == letters_guessed[j])
            {
                find = 1;
                guessed_word[i] = secret[i];
            }
        }
        if (find == 0)
        {
            guessed_word[i] = '_';
        }
    }
}

void get_available_letters(const char letters_guessed[], char available_letters[])
{

    char alph[28] = "abcdefghijklmnopqrstuvwxyz";
    int alph_len = strlen(alph);
    int letters_len = strlen(letters_guessed);

    for (int i = 0; i < alph_len; i++)
    {
        available_letters[i] = alph[i];
        for (int j = 0; j < letters_len; j++)
        {
            if (letters_guessed[j] == alph[i])
            {
                available_letters[i] = '_';
                break;
            }
        }
    }

    available_letters[alph_len] = '\0';

    int done = 0; // Наш флаг
    while (!done)
    {
        done = 1;
        for (int i = 0; i < strlen(available_letters) - 1; i++)
        {
            if (available_letters[i] == '_')
            {
                // available_letters[i] = available_letters[i + 1];
                for (int j = i; j < strlen(available_letters); j++)
                {
                    available_letters[j] = available_letters[j + 1];
                }
                done = 0;
            }
        }
    }
}

int letter_in_string(const char letter, const char string[])
{
    for (int i = 0; i < strlen(string); i++)
    {
        if (string[i] == letter)
        {
            return 1;
        }
    }
    return 0;
}

void hangman(const char secret[])
{
    int attempts = 8;
    int secretLen = strlen(secret);
    int l_g_index = 0;
    char letters_guessed[30] = ""; // history
    char available_letters[30] = "";
    char guessed_word[20] = "";

    int find_letter_flag = 0;
    int string_flag = 0;

    printf("Welcome to the game, Hangman!\n");
    printf("I am thinking of a word that is %d letters long.\n", secretLen);

    while (!is_word_guessed(secret, letters_guessed) && attempts > 0)
    {
        find_letter_flag = 0;
        printf("-------------\n");
        printf("You have %d guesses left.\n", attempts);
        get_available_letters(letters_guessed, available_letters);
        printf("Available letters: %s\n", available_letters);
        printf("Please guess a letter: ");
        char tmp[30];
        fgets(tmp, 30, stdin);
        if (tmp[1] == '\n')
        {
            char letter = '\0';
            if (tmp[0] >= 97 && tmp[0] <= 122)
            {
                letter = tmp[0];
                if (letter_in_string(letter, letters_guessed))
                {
                    printf("Oops! You've already guessed that letter: ");
                    find_letter_flag = 1;
                }
                else
                {
                    letters_guessed[l_g_index] = letter;
                    l_g_index++;
                }
            }
            else if (tmp[0] >= 65 && tmp[0] <= 90)
            {
                letter = tmp[0] + 32;
                if (letter_in_string(letter, letters_guessed))
                {
                    printf("Oops! You've already guessed that letter: ");
                    find_letter_flag = 1;
                }
                else
                {
                    letters_guessed[l_g_index] = letter;
                    l_g_index++;
                }
            }
            else
            {
                printf("Oops! '%c' is not valid letter: ", tmp[0]);
            }

            get_guessed_word(secret, letters_guessed, guessed_word);

            if (find_letter_flag == 0)
            {
                if (letter_in_string(letter, secret))
                {
                    printf("Good guess: ");
                }
                else
                {
                    printf("Oops! That letter is not in my word: ");
                    attempts--;
                }
            }
            printf("%s\n", guessed_word);
        }
        else
        {

            string_flag = 1;
            for (int i = 0; i < strlen(tmp); i++)
            {
                tmp[i] = tolower(tmp[i]);
            }
            if (is_word_guessed(secret, tmp))
            {
                printf("Congratulations, you won!\n");
            }
            else
            {
                printf("Sorry, bad guess. The word was %s.\n", secret);
            }
            break;
        }
    }
    if (string_flag == 0)
    {
        if (attempts == 0)
        {
            printf("You are loooser!\n");
        }
        else
        {
            printf("Congratulations, you won!\n");
        }
    }
}