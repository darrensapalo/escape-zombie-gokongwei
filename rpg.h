/*
 * This file contains most of the game-wide functions. 
 */
 
void sleep(int);
 
void grabs(char *text, int size){
	p(" ->");
	rewind(stdin);
	fgets(text, size, stdin);
	*(text + (strlen(text) - 1)) = '\0';
	
}

int grabi(char *text, int size){
	grabs(text, size);
	return atoi(text);
}

char grabc(char*text, int size){
	grabs(text, size);
	return text[0];
}

char* strtolower(char *text){
	int i = 0;
	while( text[i] != '\0' ){
		if (text[i] >= 65 && text[i] <= 91)
			text[i] = text[i] + 32;
		i++;
	}
	return text;
}

int spaceExists(char *text){
	int i = 0;
	while(i < strlen(text) - 1){
		if (*(text + i) == ' ')
			return true;
		i++;
	}
	return false;
}

void speak(char *text){
	int i = 0;
	while(*(text + i) != '\0'){
		p("%c", *(text + i));
		++i;
		if (SPEAK_SPEED)
			sleep(SPEAK_SPEED);
	}
}


void display(char *text){
	Text text_extended;
	Text text_show;
	int j, i = 0;
	while (text[i] && i < 70){
		text_show[i] = text[i];
		i++;
	}

	text_show[i] = '\0';
	j = i;
	
	while (text[i]){
		text_extended[i - j] = text[i];
		i++;
	}
	
	if (j)
		*(text_extended + i - j) = '\0';

	SPACE; speak(text_show); NL;
	
	if (j != 0)
		display(text_extended);
}

void display_(char *text){
	Text text_extended;
	Text text_show;
	int j, i = 0;
	while (text[i] && i < 70){
		text_show[i] = text[i];
		i++;
	}

	text_show[i] = '\0';
	j = i;
	
	while (text[i]){
		text_extended[i - j] = text[i];
		i++;
	}
	
	if (j)
		*(text_extended + i - j) = '\0';

	if (strcmp(text_show,""))
		SPACE; speak(text_show); NL;
	
	if (j != 0 && strcmp(text_extended,""))
		display(text_extended);
}

void frame(char *text){
	int i;
	// Top
	SPACE;
	p("%c", 218);
	for (i = 0; i < 72; i++)
		p("%c", 196);
	p("%c", 191);
	NL;
	
	
	// Mid
	SPACE;
	p("%c", 179);
	p("%-72s", text);
	p("%c", 179);

	
	NL;
	// Bottom
	SPACE;
	p("%c", 192);
	for (i = 0; i < 72; i++)
		p("%c", 196);
	p("%c", 217);
	NL;
	
}

int rotateString(char *text){

	/* Pointer for current character */
	char *current = text;
	
	/* Copy of the first character to be saved later */
	char tempFirst = *text;
	
	/* Iterator for until the end of the string */
	int i = 0;
	
	/* The last two characters, '\0' and '"' need not be copied */
	while(i < strlen(text) - 1){
		*(current + i) = *(current + i + 1);
		i++;
	}
	/* Make the last char equal to the first char */
	*(text + strlen(text) - 1) = tempFirst;
	
	return 1;
}

char* removeWhitespace(char *text){
	char *current = text;
	int i = 0;
	while(*current == ' '){
		i += rotateString(text);
		}
	*(current + strlen(text) - i) = '\0';
	return text;
}

char* removeQuotes(char *text){
	char *current = text;
	char *end = current + strlen(text) - 1;
	strcpy(text, removeWhitespace(text));
	if (*current == '"'){
		/* If the first is equal to a quote, then 
		 * rotate the string once and reallocate 
		 * to cut it off.
		 */
		rotateString(text);
		*(text + strlen(text) - 2) = '\0';
		//text = realloc(text, sizeof(char) * (strlen(text) - 2) );
		
	}else if (*end == '"'){
		/* But if it is only the end, then just reallocate
		 * and set last character to '\0'.
		 */
		 *(text + strlen(text) - 1) = '\0';
		//text = realloc(text, sizeof(char) * (strlen(text) - 1) );
	}
	return text;
}

char* parseCsvStr(char *text){
	text = removeQuotes(text);
	return text;
}

int parseCsvInt(char *text){
	text = removeQuotes(text);
	return atoi(text);
}

int rand_(int nMax){
	return rand() % nMax;
}

char* getTime(game *g){
	int n = g->time;
	int hours = 0;
	n = n % 144;
	while (n >= 6){
		hours++;
		n -= 6;
	}
	sprintf(g->time_format,"%02d:%02d", hours, n * 10);
	return g->time_format;
	
}

void prepareGameScreen(){
	system("CLS"); NL;
}
