/* a simple test to see if readline() is available */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(){
    char * results = readline ("test prompt");
    return 0;
}
