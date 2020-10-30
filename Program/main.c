#include "startup.h"
#include "image.h"
#include "pattern_matching.h"

int main (int argc, char** argv) 
{
    startup();
    printf("\nSTART\n");
    // if (argc != 4) {
    //     printf("  Usage: %s <script> <file> <dir>\n\n",argv[0]);
    //     exit (1);
    // }
    if (argc != 3) {
        printf("  Usage: %s <file> <dir>\n\n",argv[0]);
        exit (1);
    }
    char* filePath = argv[1];
    char* imgdir = argv[2];
    processImages(imgdir,filePath);
    printf("\nEND\n\n");
    return 0;
}