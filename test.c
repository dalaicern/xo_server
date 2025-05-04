#include "csapp.h"

int main() {
    printf("Testing csapp.c functions...\n");
    
    // Test socket functions
    int listenfd = Open_listenfd("8080");
    if (listenfd > 0) {
        printf("Successfully created listen socket\n");
        Close(listenfd);
    }
    
    // Test memory functions
    void *ptr = Malloc(100);
    if (ptr != NULL) {
        printf("Successfully allocated memory\n");
        Free(ptr);
    }
    
    // Test file I/O
    FILE *f = Fopen("/tmp/csapp_test", "w");
    if (f != NULL) {
        Fputs("Test file for csapp library\n", f);
        Fclose(f);
        printf("Successfully created and wrote to test file\n");
    }
    
    printf("All tests completed.\n");
    return 0;
}