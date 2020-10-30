#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <assert.h> 
#include "image.h"

typedef int bool;
#define TRUE 1
#define FALSE 0

Image readFile (char* filePath)
{
    // printf("+ opening file...\n\n");
    // open the file for the image
    FILE* fp = fopen(filePath,"r");
    assert(filePath!=NULL);
    if (fp == NULL)
	{
		printf("\tERROR: Failed to load file \"%s\"\n\n", filePath);
		exit(1);
    }
    Image img;
    // get the dimensions of the 2D array
    fscanf(fp, "%u%u\n", &(img.width),&(img.height));
    // create rows
    img.characters = (char**) calloc(img.height,sizeof(char*));
    for (unsigned int i=0; i<img.height; i++) 
    {
        // create col
        img.characters[i] = (char*) calloc(img.width+1, sizeof(char)); // QUESTION: should we do brackets?
        //read in each line character by character
        for (unsigned int j=0; j<img.width; j++) 
        {
            fscanf(fp, "%c", &(img.characters[i][j]));
        }
        char eol;
        // consume the last character
        fscanf(fp,"%c", &eol);
    }
    fclose(fp);
	return img;
}
void printImage(Image img)
{
    for (int i=0; i<img.height; i++)
    {
        printf("\t%s\n", img.characters[i]);
    }
    printf("\n");
}
void fileCount (DIR* dir, char* root)
{
    int count = 0;
    struct dirent* file;
    dir = opendir(root); 
    while ((file = readdir(dir)) != NULL)
    {
        char* name = file->d_name;
        char* fExtention = strchr(name,'.');
        if ((name[0] != '.') && (file->d_type == DT_REG) && strcmp(fExtention,".img") == 0)
        {
			count++;
        }
    }
	closedir(dir);
    printf("  [%d files found]\n\n", count);
}
char* buildPath(char* dirName,char* fileName, int numChars)
{
    char* path = (char*) malloc(numChars*sizeof(char));
    strcpy(path,dirName);
    if (dirName[strlen(dirName)-1] != '/')
    {
        strcat(path,"/");
    }
    strcat(path,fileName);
    return path;
}
int isImage (char* fileName)
{
    char* fExtention = strchr(fileName,'.');
    if (strcmp(fExtention,".img") == 0 && fileName[strlen(fileName)-3]=='i')
    {
        return TRUE;
    }
    return FALSE;
}
