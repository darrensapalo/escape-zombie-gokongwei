
/* C Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <unistd.h>
#include <conio.h>
#include <windows.h>

/* Standard Macros */
#define p printf
#define s error
#define PAUSE system("PAUSE");system("CLS");NL
#define SPACE p("   ")
#define NL p("\n")

/* File Macros */
#define START_FTOK (char*)strtok(data, "`\n;")
#define NEXT_FTOK strtok(NULL, "`\n;")

#define START_CTOK (char*)strtok(data, "\n ")
#define NEXT_CTOK (char*)strtok(NULL,"\n ")

#define CHECK_IF_COMMENT_TOK if (data[0] == '/' && data[1] == '/') continue;
#define CHECK_IF_BLANK_TOK if (data[0] == '\n') continue;

#define CLEAN_SCREEN system("CLS")

/* Basic true or false boolean values */
#define true (1)
#define false (0)

#define UNDEFINED (-1)

/* Item constants and enums */
#define DEFAULT_DURABILITY (0)

/* Course ID values */
#define COURSE_CCS 1
#define COURSE_COS 10
#define COURSE_COB 15
#define COURSE_CED 2
#define COURSE_SOE 14
#define COURSE_CLA 9
#define COURSE_COE 7

/* Gameplay constants */
#define GAME_OVER -1
#define KEEP_PLAYING 0
#define GAME_SUCCESS 1

#define STRING_LENGTH 40
#define LONG_STRING_LENGTH 128
#define TEXT_LENGTH 1024

/* Type definitions */
//typedef char String[32];
//typedef char LongString[128];

typedef char String[STRING_LENGTH];
typedef char LongString[LONG_STRING_LENGTH];
typedef char Text[TEXT_LENGTH];
typedef int Boolean;

/* Structures and Linked Lists */

typedef struct gameSettingsStructure {
	int game_type;
	int tutorial;
	int game_status;
	int time;
	String time_format;
	struct playerStructure *pl;
	struct playerStructure *pldb;
	struct itemDatabase *itemdb;
	struct interestStructure *interests;
	struct locationDatabase *loc;
	struct mobDatabase *mobdb;
	
}game;


typedef struct playerStructure {
	String player_name;
	String username;
	String password;
	int location;
	int level;
	int HP;
	int course;
	int sarcasm;
	int weapon;
	int weapon_durability;
	struct inventoryStructure *inv;
	struct playerStructure *next_pl;
}player;

typedef struct inventoryStructure {
	int item_id;
	int durability;
	Boolean isDiscovered;
	int amount;
	struct inventoryStructure *next_item;
}inventory;

/* Databases */

typedef struct itemDatabase { /* Items */
	int item_id;
	String item_name;
	LongString description;
	int default_durability;
	int damage;
	struct itemDatabase *next_item;
}itemdb;

typedef struct locationDatabase { /* Levels */
	int location_id;
	String location_name;
	LongString description;
	LongString where;
	struct locationDatabase *next_loc;
	struct locationDatabase *connect[5];
	struct mobDatabase *mob;
	struct interestDatabase *interests;
	struct inventoryStructure *items;
}location;

typedef struct mobDatabase { /* Monsters */
	int mob_id;
	int MaxHP;
	int HP;
	String mob_name;
	LongString mob_description;
	int damage;
	int weakness; // item weakness
	int immune; // item immunity
	int rate; // chance of meeting
	struct mobDatabase *next_mob;
}mob;

typedef struct interestDatabase { /* Points of interests */
	String name;
	LongString description;
	int item_requirement;
	Boolean isDiscovered;
	struct inventoryStructure *next_item;
	struct interestDatabase *next_interest;
}interest;

/* Freeing databases */

void freeItemDatabase(itemdb *start){
	itemdb *current, *next;
	current = start;
	do{
		next = current->next_item;
		free(current);
		current = next;
	}while(next != NULL);
}

void freeInventory(inventory *start){
	inventory *current, *next;
	next = NULL;
	current = start;
	do{
		next = current->next_item;
		free(current);
		current = next;
	}while(next != NULL);
}
