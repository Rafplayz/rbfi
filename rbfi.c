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

#define byte unsigned char;

bool output_specified = false;

int fatal_error(const char* msg) {
    printf("\nrbfi fatal error: %s\n", msg);
    exit(1);
}

int mainloop(char* bf_code) {

    char bf_interpreter_array[30000] = {0};
    char* ptr = bf_interpreter_array;

    int loop_depth = 0;
    int loop_repititions = 0;

    for(int i = 0; i < strlen(bf_code);i++) {
        char c = bf_code[i];
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
                for(int i = 0; i < CHARS_TO_REMOVE; i++) {
                    printf(" ");
                    printf("\b\b");
                }
                break;
            // case '[':
            //     if(*ptr) {
            //         int temp_loop_depth = 1;
            //         bool hasExit = false;
            //         for(int j = i; j < strlen(bf_code); j++) {
            //             if(bf_code[j] == ']') {
            //                 temp_loop_depth--;
            //             }
            //             else if (bf_code[j] == '[') {
            //                 temp_loop_depth++;
            //             }
                        
            //         }
            //         if(!hasExit) fatal_error("unmatched [");
            //         while(*ptr) {
            //             for(int j = i; j < strlen(bf_code);j++) {
                            
            //             }
            //         }
            //     }
            case '[':
                fatal_error("does not support loops yet");
                break;
            case ']':
                fatal_error("does not support loops yet");
                break;
        }
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

    mainloop(bf_code);
    
    
    return 0;
}