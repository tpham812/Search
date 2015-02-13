#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "store.c"
#include "util2.c"

#define SIZE 10000


/* 
  Add file node to result list.
*/
void addToResultList(File *file) {

	file -> counter++;
	file -> resultNextFile = rl.headFileList;
	rl.headFileList = file;
}

/* 
  Check in result list to see if file already exist. If it exists increment the counter and return 1. Else return 0.
*/
int checkInList(File *file) {

	File *curr;
	curr = rl.headFileList;
	while(curr != NULL) {
		if(strcmp(curr -> fileName, file -> fileName) == 0) {curr -> counter++; return 1;}
		curr = curr -> resultNextFile;
	}
	return 0;
}

/** 
Output results given user's choice. If it's sa then output only the files in the result list if the node's counter matches the inputted counter (which is the number of words to query). If it's so, then output all the files in the result list.
*/
void outputResults(int counter, char *choice) {

	File *file;
	file = rl.headFileList;
	if(file == NULL) return;
	/* Output if choice is "sa" */
	if(strcmp(choice, "sa") == 0) {
		while(file != NULL) {
			if(file -> counter == counter) printf("%s\n", file -> fileName);
			file = file -> resultNextFile;
		}
	}
	/* Output if choice is so */
	else {
		while(file != NULL) {
			printf("%s\n", file -> fileName);
			file = file -> resultNextFile;
		}
	}
}
/**
 Find the given word in the hash map and put the files of the word in the result lsit if it does not exist and increment the counter. If it doesnt not exist then add it to the result list.
*/
void query(char *word) {

	Token *token; File *file;
	token = hashMap[getIndex(word) % SIZE].word; /* Get token node from hash map */
	if(token == NULL) return; /* Return word if hash map is empty */
	while(token != NULL) {
		if(strcmp(word, token -> word) == 0) {
			file = token -> headFile;
			while(file != NULL) {
				/* Check to see if the file already exist in result list */
				if(checkInList(file) == 0) {addToResultList(file);} /* Call function to add to result list */
				file = file -> nextFile;
			}
			return;
		}
		token = token -> nextToken;
	}
}

/** 
  Main function 
*/
int main(int argc, char **argv) {

	if(argc == 1) {printf("Did not specify an indexer file to read in. Program will exit.\n"); return 0;}
	initializeHashMap();		/* Initialize hash map */	
	initializeResultsList();	/* Initialize result list */
	if(readIndexerFile(argv[1]) == 0) {printf("Could not open indexer file. Program will now exit.\n"); return 0;}
	char *token, *choice, *input;
	printf("\nTo quit input \"q\". To query use follow formats: sa <term>.. or so <term>...\n");
	/* Get input from user */
	while(strcmp((input = getStringFromInput()), "q") != 0) {
		/* If input is blank then, ask for input again */
		if(input[0] == '\0') {
			 printf("String is null. Please enter in a valid input.\n");
			printf("\nTo quit input \"q\". To query use follow formats: sa <term>.. or so <term>...\n");
		}
		/* Get first token from input string to see if it's "sa" or "so" */
		token = strtok(input, " ");
		/* If input is not "sa" or not "so", then ask for input again */
		if(strcmp(token, "sa") != 0 && strcmp(token, "so") != 0) {
			printf("Not a valid input. Please enter in a valid input.\n");
			printf("\nTo quit input \"q\". To query use follow formats: sa <term>.. or so <term>...\n");
			continue;
		}
		choice = token;
		int counter = 0; /* Holds number of words that user wants to query */
		/* Tokenize to get each word form input string */
		while((token = strtok(NULL, " ")) != NULL) {
			query(token);
			counter++; 	/* Increment counter for each token */
		}
		outputResults(counter, choice); /* Output results */
		resetResultList();	/* Reset result list */
		free(input); 		/* Free the input string */	
		printf("\nTo quit input \"q\". To query use follow formats: sa <term>.. or so <term>...\n");
	
	}
	free(input);		/* Free input string */
	resetResultList();	/* Reset result list */
	freeHashMap();		/* Free hash map */
	printf("Program has ended.\n");		
}
