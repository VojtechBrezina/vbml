#include <iostream>

#ifndef TEST

int main(int argc, char ** argv){

    return 0;
}

#else // TEST

#include "tests.hpp"

int main(void){
    return runTests() ? 0 : 1;
}

#endif
