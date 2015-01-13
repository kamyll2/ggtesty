#include "parser.h"

int getLengthTo(char input [], char sign) {
	int i = 1;
	for (i; i<strlen(input); i++) {
		char current = input[i];
		if (current == '<') {
			return i;
		}
	}
	return 0;
}

void getMessageSection(char input[], char buff[], char typeHeader[]) {
	//char typeHeader[] = "<to>";
	char buff1[100];
	strcpy(buff1,strstr(input, typeHeader));
	int typeLength = getLengthTo(buff1, '<');
	int typeLen = strlen(typeHeader);
	size_t x = typeLength - typeLen;
	strncpy(buff,buff1+typeLen,x);
	buff[x]='\0';
}
Message parseCharToMessage(char input[]) {
	Message m;
	char buff[100];
	getMessageSection(input,buff,"<type>");
	m.type = atoi(buff);
	if(m.type==0){
		getMessageSection(input,buff,"<from>");
		m.from = atoi(buff);
		getMessageSection(input,buff,"<to>");
		m.to = atoi(buff);
		getMessageSection(input,buff,"<body>");
		strcpy(m.body,buff);
	}
	else if(m.type==1 || m.type==2){
		getMessageSection(input,buff,"<login>");
		strcpy(m.login,buff);
		getMessageSection(input,buff,"<password>");
		strcpy(m.password,buff);
	}
	else{
		getMessageSection(input, buff, "<body>");
		strcpy(m.body,buff);
	}
	return m;
}

void printMessage(Message m) {
	if(m.type==0){
		printf("message type:\t%d\n",m.type);
		printf("message from:\t%d\n",m.from);
		printf("message to:\t%d\n",m.to);
		printf("message body:\t%s\n",m.body);
	}
	else if(m.type==1 || m.type==2){
		printf("message type:\t%d\n",m.type);
		printf("message login:\t%s\n",m.login);
		printf("message password:\t%s\n",m.password);
	}
	else{
		printf("message type:\t%d\n",m.type);
		printf("message body:\t%s\n",m.body);
	}
}

void parseMessageToChar(Message m, char buff[]) {
	char typeHeader[] = "<type>";
	char bodyHeader[] = "<body>";
	char fromHeader[] = "<from>";
	char toHeader[] = "<to>";
	char endHeader[] = "<end>";
	char loginHeader[]="<login>";
	char passwordHeader[]="<password>";

	char * output = malloc(strlen(m.body)
					+ strlen(typeHeader)
					+ strlen(bodyHeader)
					+ strlen(fromHeader)
					+ strlen(toHeader)
					+ strlen(loginHeader)
					+ strlen(passwordHeader)
					+ 20 );
					
	char * body = malloc(strlen(m.body));
	
	char from [INDEX_SIZE];
	char to [INDEX_SIZE];
	char type[1];
	
	
	if(m.type==0){
	sprintf(type, "%d", m.type);
	//puts(type);
	sprintf(to, "%d", m.to);
	sprintf(from, "%d", m.from);
	strcpy(body, m.body);

	strcat(output, fromHeader);
	strcat(output, from);
	strcat(output, toHeader);
	strcat(output, to);
	strcat(output, typeHeader);
	strcat(output, type);
	strcat(output, bodyHeader);
	strcat(output, body);
	strcat(output, endHeader);
	strcpy(buff, output);
	//return output;*/
						
	}
	else if(m.type==1 || m.type==2){
	sprintf(type, "%d", m.type);
	strcpy(body, m.body);

	strcat(output, typeHeader);
	strcat(output, type);
	strcat(output, loginHeader);
	strcat(output, m.login);	
	strcat(output, passwordHeader);
	strcat(output, m.password);
	strcat(output, endHeader);
	strcpy(buff, output);
	//return output;*/
	}
	else{
		sprintf(type, "%d", m.type);
	 	strcpy(body, m.body); 
      	
        	strcat(output, typeHeader);
        	strcat(output, type);
        	strcat(output, bodyHeader);
        	strcat(output, body);
        	strcat(output, endHeader);
        	strcpy(buff, output);
        	//return output;*/
	}
	
}
