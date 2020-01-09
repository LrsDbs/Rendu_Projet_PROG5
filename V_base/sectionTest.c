#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include "mains.h"

int testIfFileExists(char* name_file, char* val) {
    return main_section(name_file, val);
}

int testIfSecondArgumentIsMissing(char* name_file, char* val) {
    return main_section(name_file, val);
}

int testIfSectionIsFound(char* name_file, char* val) {
    return main_section(name_file, val);
}

int main(int argc, char *argv[]) {
    /*
        Test series 1 - If the file exists.
    */
    printf("Test series 1 : If the given file exists\n");
    printf("Test series 1 : start\n");
    // Test 1 : File does not exist
    char *name_file = "rigthgrthtr";
    char *val = "1";
    printf("Test 1 : The given file does not exit\n");
    if(testIfFileExists(name_file, val) == -1) {
        printf("Test 1 : clear, the file does not exist\n");
    } else {
        printf("Test 1 : fail, the file exists\n");
        return EXIT_FAILURE;
    }

    //Test 2 : The file exist 
    name_file = "./Examples_loader/example1.o";
    printf("Test 2 : The given file exist\n");
    if(testIfFileExists(name_file, val) == 0) {
        printf("Test 2 : clear, the file exist\n");
    } else {
        printf("Test 2 : fail, the file does not exist\n");
        return EXIT_FAILURE;
    }
    printf("\n----------------------------------\n");
    /*
        Test series 2 - If second argument is missing
        We assume that the file exist
    */
    printf("Test series 2 : If the second argument is missing\n");
    printf("Test series 2 : start\n");
    //Test 1 - The second argument is missing
    printf("Test 1 - The second argument is missing\n");
    if(testIfSecondArgumentIsMissing(name_file, NULL) == -2) {
        printf("Test 1 : clear, the second argument is missing\n");
    } else {
        printf("Test 1 : fail, the second argument is here\n");
        return EXIT_FAILURE;
    }

    //Test 2 - The second argument is here
    printf("Test 2 - The second argument is here\n");
    if(testIfSecondArgumentIsMissing(name_file, val) == 0) {
        printf("Test 2 : clear, the second argument is here\n");
    } else {
        printf("Test 2 : fail, the second argument is missing\n");
        return EXIT_FAILURE;
    }
   printf("\n----------------------------------\n");
    /*
        Test series 3 - If the section is not found (by name or by num)
        We assume that the file exist
    */
    printf("Test series 3 : If the section is not found (by name or by num)\n");
    printf("Test series 3 : start\n");
    // Test 1 - The section by name is not found
    printf("Test 1 - The section by name is not found\n");
    if(testIfSectionIsFound(name_file, "a section") == -3) {
        printf("Test 1 : clear, the section was not found\n");
    } else {
        printf("Test 1 : fail, the section was found\n");
        return EXIT_FAILURE;
    }

    // Test 2 - The section by name found
    printf("Test 2 - The section by name is found\n");
    if(testIfSectionIsFound(name_file, ".text") == 0) {
        printf("Test 2 : clear, the section was found\n\n");
    } else {
        printf("Test 2 : fail, the section was not found\n\n");
        return EXIT_FAILURE;
    }

    // Test 1 - The section by num is not found
    printf("Test 3 - The section by num is not found\n");
    if(testIfSectionIsFound(name_file, "-190") == -3) {
        printf("Test 3 : clear, the section was not found\n\n");
    } else {
        printf("Test 3 : fail, the section was found\n\n");
        return EXIT_FAILURE;
    }

    // Test 4 - The section by num is found
    printf("Test 4 - The section by num is found\n");
    if(testIfSectionIsFound(name_file, "1") == 0) {
        printf("Test 4 : clear, the section was found\n\n");
    } else {
        printf("Test 4 : fail, the section was not found\n\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}