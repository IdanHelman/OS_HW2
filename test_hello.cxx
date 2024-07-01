#include <iostream>
#include <unistd.h>

#define HELLO 333

int main(){
    long r = syscall(333);
    std::cout << "sys_hello returned " << r << std::endl;
    return 0;
}