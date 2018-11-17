#ifndef TREE_H
#define TREE_H

#define SIZEofBUFF 20
#define SSizeofBUFF 6
#define SizeofLONG 8
#define SizeofINT 3
#define SizeofFLOAT 4

typedef struct{
	long  	custid;
	char 	FirstName[SIZEofBUFF];
	char 	LastName[SIZEofBUFF];
	char	Street[SIZEofBUFF];
	int 	HouseID;
	char	City[SIZEofBUFF];
	char	postcode[SSizeofBUFF];
	float  	amount;
} MyRecord;

int strArraySearch(char const *array[], int len, char *delim);

#endif