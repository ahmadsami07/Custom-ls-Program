//CMPT 300
//Assignment 4: myls
//Following header files contains the struct options, and a function useroptions,
//which determines the commandline entry if user entered any specific option of myls.

#include "secret_headers.h"

// Struct for myls options. 
//The struct helps us to always maintain which options have been
//declared by any order by user.
struct opts {
    unsigned char printIndex;
    unsigned char useLongListing;
    unsigned char listSubdirectories;
};



//The following function will read the command line, and mainly check
//which type the user wants. If each of 'i' or 'l' or 'R' appear
//in the argv, instantly we will update the opts struct's values.
//-i:print the index number of each file
//-l: use a long listing format
//-R: List directories recursively
struct opts * userOptions(int argc, char ** argv) {
    struct opts * userop = (struct opts *)malloc(1 * sizeof(struct opts));

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') { // skip files
            continue;
        }

        for (int j = 1; argv[i][j]; j++) {

            if(argv[i][j]=='i')
            {
                userop->printIndex=1;
            }
            if(argv[i][j]=='l')
            {
                userop->useLongListing=1;
            }
            if(argv[i][j]=='R')
            {
                userop->listSubdirectories=1;
            }
            
        
        }
    }

    return userop;
}