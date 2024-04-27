#include <stdio.h>
#include "hangman.h"
#include <stdlib.h>
#include <time.h>

int main() {

    srand(time(NULL));
    char secret[16];
    get_word(secret);
    printf("%s\n", secret);

    hangman(secret);



    
    return 0;
}