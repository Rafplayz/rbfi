// characters:
// + 43 add 1
// - 45 sub 1
// < 60 move pointer left 1
// > 62 move pointer right 1
// . 46 print current character
// , 44 replace current cell with input
// [ 91 open loop
// ] 93 go to matching [ if value is 0
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <conio.h>

#define byte unsigned char
#define DEBUG false

bool output_specified = false;

struct mainReturn {
    /**
     * @brief if "main" is the index of a ']'
     */
    bool isIndex;
    int main;
};

int fatal_error(const char* msg) {
    printf("\nrbfi fatal error: %s\n", msg);
    exit(1);
}

struct mainReturn mainloop(char* ptr, char* bf_code, int startingIndex) {
    

    int loop_depth = 0;
    int loop_repititions = 0;

    int bf_code_len = strlen(bf_code);
    if(startingIndex == 0 && DEBUG)
        printf("length of bf_code: %d\n", bf_code_len);
    
    for(int i = startingIndex; i < bf_code_len;i++) {
        char c = bf_code[i];
        if(DEBUG)
            printf("evaluating character: %c\n", c);
        switch(c) {
            case '>':
                ptr++;
                break;
            case '<':
                ptr--;
                break;
            case '+':
                ++*ptr;
                break;
            case '-':
                --*ptr;
                break;
            case '.':
                putchar(*ptr);
                break;
            case ',':
                printf("char: ");
                *ptr = getch();

                printf("\b"); // remove the first character before starting the loop

                #define CHARS_TO_REMOVE 5
                // remove all CHARS_TO_REMOVE from cmd output
                for(int i = 0; i < CHARS_TO_REMOVE; i++) {
                    printf(" ");
                    printf("\b\b");
                }
                break;
            case '[': ; // what
                if(DEBUG)
                    printf("\nloop started\nptr value: %d", *ptr);
                while (*ptr) {
                    // recursive solution
                    // loops until *ptr == 0
                    struct mainReturn mainloopresult = mainloop(ptr, bf_code, i + 1);
                    if (mainloopresult.isIndex) 
                        i = mainloopresult.main;
                }
                break;
            case ']': ; // again
                struct mainReturn r = {false, 0};
                if(*ptr) {
                    r.main = *ptr;
                }
                else {
                    r.isIndex = true;
                    r.main = i + 1;
                }
                return r;
        }
        if(DEBUG)
            printf("<%c%d\n", c, i);
    }
}

int main(int argc, char* argv[])
{
    const char *bf_characters = ">+<-[],.";
    if(argc < 1) {
        fatal_error("no input file");
    }

    FILE* f = fopen(argv[1], "r");
    if(!f) {
        fatal_error("cannot open file");
    }

    fseek(f, 0L, SEEK_END);
    long fsize = ftell(f) + 1;
    rewind(f);

    // get all characters from file that are in the bf_characters string
    char* buffer = malloc(fsize);
    fread(buffer, 1, fsize, f);
    fclose(f);
    buffer[fsize] = '\0';

    char* bf_code = malloc(fsize);

    int i = 0;
    for(int j = 0; j < fsize; j++) {
        if(strchr(bf_characters, buffer[j])) {
            bf_code[i] = buffer[j];
            i++;
        }
    }
    free(buffer);
    bf_code[i] = '\0';

    // survey the code for any unmade loops
    int temp_loop_depth = 0;

    for (int j = 0; j < strlen(bf_code); j++)
    {
        if(DEBUG) {
            printf("\nloop number: %d\n", j);
            printf("current loop depth: %d\n", temp_loop_depth);
            printf("current character: %c\n", bf_code[j]);
        }
        if (bf_code[j] == ']')
        {
            temp_loop_depth--;
        }
        else if (bf_code[j] == '[')
        {
            temp_loop_depth++;
        }
        if(DEBUG)
            printf("end loop depth: %d\n", temp_loop_depth);
    }
    if (temp_loop_depth)
        fatal_error("unclosed loop");

    printf("how much memory do you want to dedicate\n(note: smart to set this around 100-500, most programs do not need more!!)\n\n>> ");
    int memToAllocate;
    scanf("%d", &memToAllocate);

    if(memToAllocate < 10) {
        fatal_error("too little memory");
    }

    char bf_interpreter_array[memToAllocate * sizeof(char)];
    memset(bf_interpreter_array, 0, memToAllocate * sizeof(char));
    char* ptr = bf_interpreter_array;

    mainloop(ptr, bf_code, 0);
    
    if(DEBUG) {
        printf("[DEBUG] all memory slots\n");
        for (char* i = bf_interpreter_array;i != bf_interpreter_array + memToAllocate;i++) {
            printf("%d ", *i);
        }
    }
    
    return 0;
}