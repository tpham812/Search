#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "store.h"

#define SIZE 10000	/* Final size of hash map */


HashMap hashMap[SIZE]; 	/* Hash Map */
Results rl;		/* Result List */

/** 
 Initialize result list. Returns 1 to indicate success
*/
int initializeResultsList() {

	rl.headFileList = NULL;
	return 1;
}

/**
 Initialize hash map. Returns 1 to indicate success 
*/
int initializeHashMap() {

	int count = 0;
	
	while(count < SIZE) {
		hashMap[count].word = NULL;
		count++;
	}
	return 1;
}

/**
 Allocate memory for file node. Returns 1 for success, 0 if no success 
*/
int initializeFileNode(File **file, char *fileName) {

	*file = (File *)malloc(sizeof(File));
	if(file == NULL) return 0;
	(*file) -> fileName = (char *)malloc(strlen(fileName) + 1);
	strcpy((*file) -> fileName, fileName);
	(*file) -> fileName[strlen(fileName)] = '\0';
	(*file) -> nextFile = NULL;
	(*file) -> resultNextFile = NULL;
	(*file) -> counter = 0;
	return 1;
}

/**
 Allocate memory for token Node. Returns 1 for success, 0 if no success
*/
int initializeTokenNode(Token **token, char *word) {

	*token = (Token *)malloc(sizeof(Token));
	if(token == NULL) return 0;
	(*token) -> word = (char *)malloc(sizeof(word) + 1);
	strcpy((*token) -> word, word);
	(*token) -> word[strlen(word)] = '\0';
	(*token) -> nextToken = NULL;
	(*token) -> headFile = NULL;
	return 1;
}

/** 
  Function to add file node to the give token if it does not exist 
*/
void addFileToToken (File *newFile, char *fileName, unsigned long index) {

	File *file;
	file = hashMap[index].word -> headFile;
	if(file == NULL) {hashMap[index].word -> headFile = newFile; return;}
	while(file != NULL) {
		/* If file exists then do nothing and return */
		if(strcmp(file -> fileName, fileName) == 0) return;
		file = file -> nextFile;
	}
	/* Add file to token node if it does not exist already */
	newFile -> nextFile = hashMap[index].word -> headFile;
	hashMap[index].word -> headFile = newFile;
}

/**
  Function to add token node to hash map if it does not exist 
*/
void addTokenToHashMap (Token *newToken, char *word, unsigned long index) {

	Token *token;
	token = hashMap[index].word;
	if(token == NULL) {hashMap[index].word = newToken; return;}
	while(token != NULL) {
		/* If token node exists then do nothing and return */
		if(strcmp(token -> word, word) == 0) return;
		token = token -> nextToken;
	}
	/* Add token node to hash map if it does not exist yet */
	newToken -> nextToken = hashMap[index].word;
	hashMap[index].word = newToken;

}
