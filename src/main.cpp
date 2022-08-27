#include <iostream>

#ifndef TEST

#include "cli.hpp"

int main(int argc, char ** argv){
    return runCli(argc, argv);
}

#else // TEST

#include "tests.hpp"

int main(void){
    return runTests() ? 0 : 1;
}

#endif // TEST
