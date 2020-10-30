#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <assert.h> 
#include <unistd.h>
#include "pattern_matching.h"

#define TRUE 1
#define FALSE 0

// take a directory and check for matches in each of the images
// writes number of matches and the indices where the matches were found to an output file
// repeats process for next directory
void processImages(char* root, char* filePath) 
{
    Image img;
    Image pat = readFile(filePath);
    printf("\tpattern selected:\n");
    printf("\t________________________\n\n");
    printImage(pat);
    printf("\t________________________\n\n");

    printf("+ opening image directory...\n\n");
    if (root[strlen(root)-1] == '/')
    {
        root[strlen(root)-1] = '\0';
    }
    DIR* imgdir = opendir(root);
    if (imgdir == NULL)
    {
        puts("  Unable to read directory");
		printf("  ERROR: Folder %s not found\n", root);
		exit(1);
	}
    struct dirent* file;
    if ((file = readdir(imgdir)) == NULL)
    {
        puts("  Folder is empty");
		exit(1);
	}
    else
    {
        fileCount(imgdir,root);
    }
    // write all of the results to a file called <patternFile>_Matches.txt
    char* outputFileName = getFilename(filePath);
    // // create and open a new file
    FILE* fp = fopen(outputFileName,"w");
    fclose(fp);
    // search for the image files in the current directory
    while ((file = readdir(imgdir)) != NULL)
    {
        char* imgPath;
        // get each file with a '.img' extension to create an image then match with pattern
        if ((file->d_name[0] != '.') && (file->d_type == DT_REG) && isImage(file->d_name) == TRUE)
        {
            // build the file path for the image using the filename from the parent directory
            imgPath = buildPath(root,file->d_name,strlen(root)+strlen(file->d_name)+2);
            img = readFile(imgPath);
            printf("\tfile: %s\n",file->d_name);
            printf("\t________________________\n\n");
            printImage(img);
            printf("\t________________________\n");
            // calculate the number of matches between the pattern and image files
            char* matches = match(img,pat);
            if (strlen(matches) > 1)
            {
                // write all of the results to a file called <patternFile>_Matches.txt
                generateOutputFile(file->d_name,outputFileName,matches);
            }
            // free memory for the image, the path that was built to the image, 
            // and the results from matching
            reset(img,imgPath,matches);
        }
    }
    printf("+ closing directory.\n\n");
    closedir(imgdir);
}
void reset (Image img, char* path, char* matches)
{
    free(path);
    free(matches);
    for (unsigned int i = 0; i < img.height; i++)
    {
        free(img.characters[i]);
    }
}
// match each pattern block to an equal size block in the array
// for i rows, j cols in the image and k rows, l cols in pattern
char* match (Image img, Image pat)
{
    int count = 0;
    // create empty string for the matches
    // if there are no matches, then the length of the string will be equal to 1
    char* matches = (char*) calloc(1, 1);
    for (unsigned int i=0; i<img.height-pat.height; i++)
    {
        for (unsigned int j=0; j<img.width-pat.width; j++)
        {
            int isMatch =  TRUE;
            //	row index on pattern
            for (unsigned int k=0; k<pat.height && isMatch; k++)
            {
                for (unsigned int l=0; l< pat.width && isMatch; l++)
                {
                    // check if the character matches at the current position
                    // if no, do not compare the next block of the image to the pattern
                    if (pat.characters[k][l] != img.characters[i+k][j+l])
                    {
                        isMatch = FALSE;
                    }
                } // l
            } // k
            if (isMatch) 
                {
                    count++;
                    // get the number of characters in each of the numbers
                    int numCharI= countDigits(i);
                    int numCharJ = countDigits(j); 
                    if (count == 1)
                    {
                        // create new string the count, the location of the match, two spaces, and an endline character
                        matches = (char*) calloc(numCharI+numCharJ+3,sizeof(char));
                        sprintf(matches,"%d %d",i,j);
                    }
                    else
                    {
                        // create new string that holds the count, old matches, the location of the match, two spaces, and an endline character
                        char* temp =(char*) calloc (strlen(matches)+numCharI+numCharJ+3,sizeof(char));
                        sprintf(temp,"%s %d %d",matches,i,j);
                        free(matches);
                        matches = temp;
                    }
                }
        } // j
    } // i

    // add final count to the array of matches
    int numCharCount = countDigits(count);
    char* temp = (char*) calloc(strlen(matches)+numCharCount+1,sizeof(char));
    sprintf(temp,"%d %s",count,matches);
    free(matches);
    matches = temp;

    if (count == 1)
    {
        printf("\t[1 match found]\n\n\n");
    }
    else
    {
        printf("\t[%d matches found]\n\n\n",count);
    }
    return matches;
}
// create a new file for the matches that are found for 
// the pattern and the current image in the format of
// the count and the indices where the matches were found
char* getFilename (char* path)
{
    char* temp = strrchr(path,'/');
    char* end = "_Matches.txt";
    char* name = (char*) calloc(strlen(temp)-5+strlen(end)+1,sizeof(char));

    // extract the filename of the pattern from the string and save it to memory
    // remove the .pat file extension
    for (int i=0; i<strlen(temp); i++)
    {
        if (temp[i] != '.')
        {
            name[i] = temp[i];
        }
        else
        {
            break;
        }
    }
    // join the filename of the pattern and the "_Matches.txt" to create the filename for the output
    strcat(name,end);
    memmove(name,name+1, strlen(name));
    // printf("new file name: %s\n\n",name);
    return name;
}
void generateOutputFile (char* imageFile, char* outputFile, char* matches)
{
    char* folder = "Output";
    // build a path to the file in the outputfolder
    char* outputPath = buildPath(folder,outputFile,strlen(outputFile)+strlen(folder)+2);
    // if the file exists, open it. if not, create it
    FILE* output = fopen(outputPath,"a");
    // make sure that you can open the file
    if (output == NULL)
	{
		printf("\tERROR: Failed to open file \"%s\"\n\n", outputFile);
		exit(1);
    }
    // char* message = "THIS IS WORKING!!!!!!!!!";
    // fprintf(output,"%s\n",message);
    fprintf(output, "%s\n\t%s\n",imageFile,matches);
    fclose(output);
}
int countDigits (int num)
{
    // keep
    if (num < 10)
    {
        return 1;
    }
    else if (num < 100)
    {
        return 2;
    }
    else if (num < 1000)
    {
        return 3;
    }
    else if (num < 10000)
    {
        return 4;
    }
    else if (num < 100000)
    {
        return 5;
    }
    else
    {
        return 0;   
    }
    
}

// FILE* fp = fopen(outputFileName,"w");
    // // if file already exists in this folder with that name, delete it first
    // if (fp == fopen(outputFileName,"r"))
    // {
    //     int del = remove(outputFileName);
    //     if (del != 0)
    //     {
	// 	    printf("  ERROR: Unable to delete %s\n", outputFileName);
    //         exit(1);
    //     }
    //     FILE* fp = fopen(outputFileName,"w");
    //     fclose(fp);
    // }
    // else 
    // {
    //     FILE* fp = fopen(outputFileName,"w");
    //     fclose(fp);
    // }