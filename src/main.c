#include <stdio.h>
#include <unistd.h>

int main(){
    write(STDOUT_FILENO, "hi\n", 4);
    return 0;
}