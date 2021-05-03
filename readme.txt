CMPT 300 Assignment 4
Ahmad As Sami, SFU ID: 301404717


Resources Used To Learn about directories and STAT function:
https://pubs.opengroup.org/onlinepubs/007904875/basedefs/sys/stat.h.html
https://pubs.opengroup.org/onlinepubs/7908799/xsh/dirent.h.html
https://man7.org/linux/man-pages/man2/lstat.2.html
https://www.mkompf.com/cplus/posixlist.html#files_and_directories
https://en.wikipedia.org/wiki/Unix_file_types
https://linux.die.net/man/2/stat
https://linuxize.com/post/how-to-list-files-in-linux-using-the-ls-command/
https://www.gnu.org/software/libc/manual/html_node/Directory-Entries.html

More info on the longlisting and specific modes:
https://www.garron.me/en/go2linux/ls-file-permissions.html
https://stackoverflow.com/questions/33814292/what-does-mean-in-filestat-st-mode-s-irusr
https://stackoverflow.com/questions/23955876/permissions-of-current-directory(use of &)

Manuals for pwd and grp used in infodemo:
https://pubs.opengroup.org/onlinepubs/000095399/basedefs/pwd.h.html
https://pubs.opengroup.org/onlinepubs/7908799/xsh/grp.h.html

Lexicographical sorting:
https://stackoverflow.com/questions/18408558/logic-challenge-sorting-arrays-alphabetically-in-c/18408786
For creating the date string, learning how to use time funcs and access file dates:
https://www.gnu.org/software/libc/manual/html_node/Time-Types.html
https://c-for-dummies.com/blog/?p=3004

Extra resources: 
//https://cboard.cprogramming.com/c-programming/112085-should-i-include-libraries-header-file-source-file.html


Question 2:
The program took more than 2 weeks to code and to test. Mainly I struggled to print and understand
all the different -rwd..etc fields. Another thing I struggled with was the user input if user
inputted a directory name specificaly in the myls. I mainly learnt the properties of the file system,
all different facts of file and used the stat struct fields a lot. I also struggled to recursively
display the sub directories, and the sorting took a lot of trial and error. 
Also for the first time in
all assignments, I used modularity; I separated the functions in different .h files to 
make the code look better. 
As an added function, like last assignment, I noticed in ls the directories have different
color compared to the files; that's something which could be added as a feature here too;
where directories can have a different color compared to files. 

