#define SIZE 10000

typedef struct FileName File;

struct FileName {

	int counter;		/* Counter */
	char *fileName;		/* The file Name */
	File *nextFile;		/* Next File */
	File *resultNextFile;	/* Next File in result list */
};

typedef struct Word Token;

/** 
   Structure to hold a node for the token 
*/
struct Word {

	char *word;		/* The token */
	File *headFile;		/* The beginning of file node */
	Token *nextToken;	/* The link to the next word structure */
};

typedef struct HashMap HashMap;

/* 
  HashMap structure 
*/
struct HashMap {

	Token *word;			/* Holds the beginning token node in the HashMap */
};

typedef struct ResultsList Results;

struct ResultsList {

	File *headFileList;		/* Holds the beginning node in the result list */
};

extern HashMap hashMap[SIZE]; 
extern Results rl;

int initializeResultsList();
int initializeHashMap();
int initializeFileNode(File **file, char *fileName);
int initializeTokenNode(Token **token, char *word);
void addFileToToken (File *newFile, char *fileName, unsigned long index);
void addTokenToHashMap (Token *newToken, char *word, unsigned long index);
