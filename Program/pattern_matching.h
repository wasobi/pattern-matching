#ifndef PATTERN_MATCHING_H
#define PATTERN_MATCHING_H
#include "image.h"

void processImages (char* root, char* filePath); // DONE
void reset (Image img, char* path, char* matches); // DONE
char* match (Image img, Image pat); // DONE
char* getFilename (char* path); // DONE
void generateOutputFile (char* imageFile, char* outputFile, char* matches); // DONE
int countDigits (int num); // DONE

#endif