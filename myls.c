//CMPT 300
//Assignment 4: myls
//Ahmad As Sami
//SFU ID 301404717
#include "secret_headers.h"
#include "options.h"

void printDirs(char ** dirs, int dirCount, struct opts * options);
const char * returnDatestr(char * datestr, struct tm * ctm);
struct group* getAndPrintGroup(gid_t grpNum);
struct passwd * getAndPrintUserName(uid_t uid);
void readUserinput(int argc, char ** argv, char *** pDirs, int* pDirCount, char *** pFiles, int * pFileCount);
void printallopt(const char * name, const char * path, struct opts * options);


//main function
int main(int argc, char ** argv) {

    //First, we take the user options which they provide
    //in the command line.
    struct opts * options = userOptions(argc, argv);

    if (!options) 
    { // failed to read the options
        return EXIT_FAILURE;
    }

    int dirCount = 0;
    int fileCount = 0;
    char ** dirs = NULL;
    char ** files = NULL;
    

    //We read user input in commandline for particular directory or file
    readUserinput(argc, argv, &dirs, &dirCount, &files, &fileCount);


    //To print all the files in directory according to particular option (-i,R,l)
    for (int i = 0; i < fileCount; i++) {
        printallopt(files[i], files[i], options);
    }
    if (fileCount) {
        if (dirCount)
            printf("\n");
    }

    //print further directories and subdirectories recursively (depends in options if -R enabled) if they are present in dir
    printDirs(dirs, dirCount, options);

    free(dirs);
    free(options);
    free(files);

    return EXIT_SUCCESS;
}


// Read the arguments for files and directories
// The files and directories are copied to separate arrays.
//As we are using an array of strings, we use char *** in the parameters of readUserinput.
//The directory which is sent in, will be the pDirs, and the files which it will contain will be 
//contained in pFiles.
void readUserinput(int argc, char ** argv, char *** pDirs, int* pDirCount, char *** pFiles, int * pFileCount) {
    char ** dnames = NULL;
    char ** fnames = NULL;
    int count = 0;
    int fcount = 0;
    struct stat st;

    //We check for each of the arguments for file names inputted by the user; we input it into
    //our listdirs of directory/file names.
    for (int i = 1; i < argc; i++) {
        //First we check if arguments include options. 
        //If so, we will skip it for now, by accessing the string
        //inside the 2nd element of argv.
        if (argv[i][0] == '-') { // skip options
            continue;
        }


        //We will check if the file actually exists by using stat, and store info about file into st
        int checkiffile = stat(argv[i], &st);

        if (checkiffile != 0) {
            printf("Can't open %s. No such file or directory. \n",argv[i]);
            continue;
        }

        //We will use S_ISDIR to check if it is a directory.
        //If the inputted argument is a directory, we will 
        //reallocate the directory name array to increase size by 1,
        //then we will input the directory names into the dnames array for later printing.
        if (S_ISDIR(st.st_mode)) 
        { // add to directory listdirs
            dnames = (char **)realloc(dnames, sizeof(char *) * (count + 1));

            int j = count - 1;
            //Here we sort the d-names in lexicographical order by using ASCII string comparisons 
            for (; j >= 0 && strcmp(argv[i], dnames[j]) < 0; j--) {
                dnames[j + 1] = dnames[j];
            }

            //We finally input the latest directory in the head.
            dnames[j + 1] = argv[i];

            count++;
        }
        //Similarly if it is not a directory, it has to be a file. 
        //We do the same thing in this case for afile, but we input the filename
        //in the fnames array
        else 
        { // add to file listdirs
            fnames = (char **)realloc(fnames, sizeof(char *) * (fcount + 1));

            int j = fcount - 1;
            for (; j >= 0 && strcmp(argv[i], fnames[j]) < 0; j--) {
                fnames[j + 1] = fnames[j];
            }

            fnames[j + 1] = argv[i];

            fcount++;
        } 
    }

    if (!count && !fcount) {
        //if both directory and file are empty
        dnames = (char **)malloc(1 *sizeof(char*));
        dnames[0] = ".";
        count++;
    }

    //We assign the directoryname array, the directory count,
    //the filename array, and the file count to the parameters
    //to be used in printDirs function later in main func

    *pDirs = dnames;
    *pDirCount = count;

    *pFiles = fnames;
    *pFileCount = fcount;
}


//The following function lists the files and subdirectories
//in the directory provided in name. This works with 
//the print entry in printing the files and directories
void listdirs(const char * name, struct opts * options) {
    DIR * dir = opendir(name);

    struct dirent* entry = NULL;
    struct dirent** entries = NULL;
    int count = 0;
    int cSubDirs = 0;
    
    while ((entry = readdir(dir)) != NULL) {
        // skip hidden files - starting with .
        //we use continue to forward the iteration from the start again
        if (entry->d_name[0] == '.') {
            continue;
        }

        // resize array of entries, as new dir is encountered
        entries = (struct dirent **)realloc(entries, sizeof(struct dirent*) * (count + 1));

        // shift items to the right, according to lexicographical order
        //Here we sort the d-names in lexicographical order by using ASCII string comparisons (mentioned in Piazza)
        //Reference:https://stackoverflow.com/questions/18408558/logic-challenge-sorting-arrays-alphabetically-in-c/18408786
        //We shift the directory names by 1 to the right if an earlier alphabet is found 
        int i = count - 1;
        //as we know, if return value of strcmp is <0 this indicates that string 1 is "less"
        //than string 2
        for (; i >= 0 && strcmp(entry->d_name, entries[i]->d_name) < 0; i--) {
            entries[i + 1] = entries[i];
        }    

        // insert the new entry into the array
        entries[i + 1] = entry;

        count++; // update count of array

        //count sub directories if entry is directory rather than file
        if (entry->d_type & DT_DIR) { 
            cSubDirs++;
        }
    }   

    char path[300];
    char ** subDirs = (char **)malloc(cSubDirs *sizeof(char *));
    //char ** newentries=(char**)malloc(300*sizeof(char**));
   // char ** newpaths=(char**)malloc(300*sizeof(char**));

    int iSubDir = 0;
    for (int i = 0; i < count; i++) {
        // generate full path of entry
        //sprintf(newpaths[i], "%s/%s", name, entries[i]->d_name);
        sprintf(path, "%s/%s", name, entries[i]->d_name);

        // add to sub directory array if entry is a directory, rather than a file
        if (entries[i]->d_type == DT_DIR) {
            subDirs[iSubDir] = (char *)malloc(strlen(path) + 1 * sizeof(char));
            strcpy(subDirs[iSubDir], path);
            iSubDir++;
        }

        printallopt(entries[i]->d_name, path, options);
    }
    

    //After printing the files and directories, print subdirectories
    if (options->listSubdirectories && cSubDirs > 0) {
        printf("\n");
        printDirs(subDirs, cSubDirs, options);
    }

    // free memory
    for (int i = 0; i < cSubDirs; i++) {
        free(subDirs[i]);
    } 
    free(subDirs);
    free(entries);

    closedir(dir);
}

// display directories using the dirs array and if there are subdirs
void printDirs(char ** dirs, int dirCount, struct opts * options) {
    for (int i = 0; i < dirCount; i++) {
        if (i > 0) {
            printf("\n");
        }
        if (dirCount > 1 || options->listSubdirectories ) { // for multiple directories listed, display header
            printf("%s:\n", dirs[i]);
        }

        listdirs(dirs[i], options);
    }
}


//The following function prints a file/directory, with the necessary 
//format style according to user options. We will send the name of the file,
//file path and the struct options where hte user preference (-i,-R,or -l) is stored,
//https://man7.org/linux/man-pages/man2/lstat.2.html
//for -l, many fields from stat struct are used
void printallopt(const char * name, const char * path, struct opts * options) {
    struct stat st;
    char datestrng[100];

    if (stat(path, &st) != 0) {
        printf("Can't open '%s'.\n", name);
        return;
    }

    if (options->printIndex) {
    	//for creating spacing
    	for (int i=0;i<4;i++)
    	{
    		printf(" ");
    	}
        printf("%lu ", st.st_ino);
    }

    if (options->useLongListing) {
        //Ref:https://www.garron.me/en/go2linux/ls-file-permissions.html (better info about -l)
        //We will use the mode field of stat struct to find file type
   
    	//For spacing
        for (int i=0;i<2;i++)
    	{
    		printf(" ");
    	}


        char type = '-';
        if (S_ISDIR(st.st_mode)) {
            type = 'd';
        } 

        printf("%c", type);


        //we check the user permission using the mode field
        //of struct and various flags
        //Ref: https://stackoverflow.com/questions/33814292/what-does-mean-in-filestat-st-mode-s-irusr
        //https://stackoverflow.com/questions/23955876/permissions-of-current-directory
        //We will use the bitwise & operator ro get the various permission bits associated with the file
        if (st.st_mode & S_IRUSR) 
        {
            printf("r");
        } else 
        {
            printf("-");
        }
        if (st.st_mode & S_IWUSR) 
        {
            printf("w");
        } else 
        {
            printf("-");
        }
        if (st.st_mode & S_IXUSR) {
            printf("x");
        } else {
            printf("-");
        }

        // we check permission of group
        if (st.st_mode & S_IRGRP) {
            printf("r");
        } else {
            printf("-");
        }
        if (st.st_mode & S_IWGRP) {
            printf("w");
        } else {
            printf("-");
        }
        if (st.st_mode & S_IXGRP) {
            printf("x");
        } else {
            printf("-");
        }

        // we check permission of user
        if (st.st_mode & S_IROTH) 
        {
            printf("r");
        } else {
            printf("-");
        }
        if (st.st_mode & S_IWOTH) 
        {
            printf("w");
        } else {
            printf("-");
        }
        if (st.st_mode & S_IXOTH) {
            printf("x");
        } else {
            printf("-");
        }
        printf(" ");

        // print number of hard links
        printf("%lu ", st.st_nlink);

        //taken from infodemo.c
        // print username
        struct passwd * pd = getAndPrintUserName(st.st_uid);
        printf("%s ", pd->pw_name);


        //print grp name
        struct group * gd = getAndPrintGroup(st.st_gid);
        printf("%s ", gd->gr_name);

        // print the size of file in bytes
        printf("%lu ", st.st_size);

        // print date string using our return date function
        //We can find file date and time using ctime field in stat structure
        //Ref: https://c-for-dummies.com/blog/?p=3004

        printf("%s ", returnDatestr(datestrng, localtime(&st.st_ctime)));
    }

    printf("%s\n", name);
}

//The following function just takes the localtime, 
//and converts it into the date string required.
const char * returnDatestr(char * datestr, struct tm * ctm) {


    //We keep all the months in one array of strings and whatever month it is, it is put into the 
    //datestring
    const char* monthsstr[]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sept","Oct","Nov","Dec"};

    for (int i=0;i<12;i++)
    {
        if(i==ctm->tm_mon)
        {
            sprintf(datestr,"%s",monthsstr[i]);
        }
    }

    //After the month, we skip 3 characters and input the day, year, hour and minute.
    sprintf((datestr + 3), " %2d %4d %02d:%02d", ctm->tm_mday, 1900 + ctm->tm_year, ctm->tm_hour, ctm->tm_min);

    return datestr;
}



//Functions to return group and user name using uid and grpnum
struct group* getAndPrintGroup(gid_t grpNum)
{
    struct group *grp = getgrgid(grpNum);

    if (grp) {
         return grp;
    } else {
        printf("No group name for %u found\n", grpNum);
    }

    return NULL;
}

struct passwd * getAndPrintUserName(uid_t uid)
{
    struct passwd *pw = getpwuid(uid);

    if (pw) {
        return pw;
    } else {
        perror("Hmm not found???");
        printf("No name found for %u\n", uid);
    }
    return NULL;
}
