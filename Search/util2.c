#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 10000


/**
  Function to check if the string is a number. Returns 0 if it is not a number, else returns 1.
*/
int checkTokenNum(char *token) {

	int i;
	for(i = 0; i < strlen(token); i++) {
		if(isdigit(token[i]) == 0) return 0;	/* Found a character that is not a digit. Return 0 */
	}
	return 1;
}

/**
   Function to convert a string to an integer value to map to hash map. Returns a long int.
   Note: this is a djb2 algorithm. Credit goes to Dan Bernstein for making this hash function.  
*/
unsigned long getIndex(unsigned char *word) {

	unsigned long hashValue = 5381;
	int index;

	for(index = 0; index < strlen(word); index ++) 
		hashValue = ((hashValue << 5) + hashValue) + word[index]; 
	
	return hashValue;
}

/**
This function reads in a line from an input of an unkown string size. The string to return will reallocate if space runs out and there    are  more characters to read in. Finally reallocate size to the actual length of the string line. Returns a line store in a string
*/

char *getStringFromInput() {

	int size = 150;
	int length = 0;
	int ch;

	char *stringLine = (char *)malloc(sizeof(char) * size);

	while((ch = getchar()) != '\n') {
		stringLine[length++] = ch;
		/* Reallocate size if length if the maximum size of string */
		if(length == size) {
			stringLine = realloc(stringLine, sizeof(char) * (size += 50));
			if(stringLine == NULL) return stringLine;
		}
	}
	stringLine[length++] = '\0';
	stringLine = realloc(stringLine, sizeof(char) * length); /* Final reallocation */

	return stringLine;	/* Return string */

}

/**
This function reads in a line from a file of an unkown string size. The string to return will reallocate if space runs out and there are  more characters to read in. Finally reallocate size to the actual length of the string line. Returns a line store in a string
*/
char *getStringFromFile(FILE *file) {

	int size = 150;
	int length = 0;
	int ch;

	char *stringLine = (char *)malloc(sizeof(char) * size);

	while(EOF != (ch = fgetc(file)) && ch != '\n') {
		stringLine[length++] = ch;
		/* Reallocate size if length if the maximum size of string */
		if(length == size) {
			stringLine = realloc(stringLine, sizeof(char) * (size += 50));
			if(stringLine == NULL) return stringLine;
		}
	}
	stringLine[length++] = '\0';
	stringLine = realloc(stringLine, sizeof(char) * length); /* Final reallocation */

	return stringLine;	/* Return string */
}

/** 
  Resetting the result list to empty to re-use for next query
*/
void resetResultList() {

	File *file, *temp;
	file = rl.headFileList;
	while(file != NULL) {
		file -> counter = 0;
		file = file -> resultNextFile;
	}
	rl.headFileList = NULL;
}
 /**
   Freeing the hash map
*/
void freeHashMap() {

	int i;
	Token *token, *temp;
	File *file, *temp2;
	for(i = 0; i < SIZE; i++) {
		token = hashMap[i].word;
		while(token != NULL) {
			file = token -> headFile;
			temp = token -> nextToken;
			while(file != NULL) {
				temp2 = file -> nextFile;
				free(file -> fileName);
				file -> nextFile = NULL;
				file ->	resultNextFile = NULL;
				free(file);
				file = temp2;
			}
			free(token -> word);
			token -> nextToken = NULL;
			token -> headFile = NULL;
			free(token);
			token = temp;
		}
		hashMap[i].word = NULL;
	}
}

/**
  Read in indexer file and store each word and it's file to hash map. Returns 0 if file cannot be opened. Else returns 1 for succesful read.
*/
int readIndexerFile(char *indexFile) {

	FILE *inFile;
	File *newFile;
	Token *newToken;
	char *token, *word, *ptr;
	unsigned long index = 0, begin = 0, isWord = 0;

	inFile = fopen(indexFile, "r"); /* Open file to read */
	if(inFile == NULL) return 0; 	/* Returns 0 if file cannot be open for reading */
	while(!feof(inFile)) {
		char *stringLine = getStringFromFile(inFile); /* Read in each line from indexer file */
		token = strtok(stringLine, " ");
		while(token != NULL) {
			/* indicate where the word begins */
			if(strcmp(token, "<list>") == 0) {begin = 1; token = strtok(NULL, " "); continue;}
			/* Indicate where the file begins */
			if(strcmp(token, "</list>") == 0) {begin = 0; isWord = 0; token = strtok(NULL, " "); continue;}
			/* Add word to hashMap */
			if(begin == 1 && isWord == 0) {
				word = token;
				index = getIndex(word); /* Get index from hash function */
				initializeTokenNode(&newToken, word);
				addTokenToHashMap(newToken, word, index % SIZE);
				isWord = 1;
			}
			/* Add file to word */
			else if(isWord == 1 && checkTokenNum(token) == 0) {
				initializeFileNode(&newFile, token);
				addFileToToken(newFile, token, index % SIZE);	
			}
			token = strtok(NULL, " ");
		}
	}
	fclose(inFile);	/* Close file */
	return 1;	/* Returns 1 to indicate success */
}
