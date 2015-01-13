#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INDEX_SIZE 5

typedef struct Message {
	int type;
	char body[100];
	int from;
	int to;
	char login[20];
	char password[20];
}Message;

int getLengthTo(char input [], char sign);

void getMessageSection(char input[], char buff[], char typeHeader[]);
Message parseCharToMessage(char input[]);

void printMessage(Message m);

void parseMessageToChar(Message m, char buff[]);
#endif
