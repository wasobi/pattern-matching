#ifndef IMAGE_H
#define IMAGE_H

#include <dirent.h>

typedef struct Image 
{
    unsigned int height;
    unsigned int width;
    char** characters;
} Image;
 
char** readDirectory(char* root); // ERRORS
Image readFile(char* filePath); // DONE
void printImage(); // DONE
void fileCount (DIR* dir, char* root); // DONE
char* buildPath(char* folder,char* fileName,int numChars); // DONE
int isImage (char* fileName); // DONE

#endif