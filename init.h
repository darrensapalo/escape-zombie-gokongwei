/* Basic C Programming requirements and structures */
#include "standard.h"

/* Basic derived functions */
#include "rpg.h"

/* Player functions */
#include "player.h"

/* Items and item database */
#include "items.h"

/* Mobs database */
#include "mob.h"

/* Locations and location database */
#include "locations.h"

/* Battle */
#include "battle.h"

/* Items and item database */
#include "interests.h"

/* Database */
#include "database.h"

/* Commands */
#include "commands.h"

/* Map Editor */
#include "map_editor.h"

/* Gameplay */
#include "gameplay.h"



void tutorialLookAtThings(game *g){
	frame("Tutorial: Some zombies are weak to specific weapons.");
	display("After a bloody mess, you quietly slip back into the comfort of your room. You close the door behind you and decide to think things through. What is happening?"); 
	SPACE; PAUSE;
	frame("Tutorial: Final [help] instructions.");
	display("You have a wide array of actions you can pick from. This list can be viewed any time when you type in [help] as your action."); 
	
	cmd_help(); NL;
	SPACE; PAUSE;
	
	frame("Tutorial: A quick zombie surival guide!");
	display("You will need every little thing that will add to your survival! You should (look) at all interesting things such as benches, (take) whatever items you see and can carry, look for (where) you can go until you can escape!"); 
	SPACE; PAUSE;
	
	frame("Tutorial: Start game!");
	display("You will now be set out to the game-world. Have fun playing!"); 
	SPACE; PAUSE;
}

void tutorialFight(game *g){
	String choice;
	Boolean err;
	do{
	err = true;
	frame("Tutorial: A simple tip, you are about to combat a zombie.");
	display("You peek outside the dark room into the corridor. A few meters away from you is a bloody, tattered student with a missing eye, slowly approaching, having noticed your scent. What do you do? (scream, befriend, combat)"); 
	grabs(choice, 50); NL;
	
	if (!strcmp(choice,"scream")){
		display("It notices you immediately and it gains speed as it runs towards you. It sinks its teeth and you lose 3% of your health."); 
		playerApplyHealth(g, -3);
		if (playerHealth(g) <= 0){
			CLEAN_SCREEN;
			return;
		}
		SPACE; PAUSE;
	}else if (!strcmp(choice,"befriend")){
		display("You approach it slowly with open arms, ready for an embrace. It shows a wide smile with blood trickling from its mouth and bit you for 10% HP."); 
		playerApplyHealth(g, -10);
		if (playerHealth(g) <= 0){
			CLEAN_SCREEN;
			return;
		}
		SPACE; PAUSE;
		
	}else if (!strcmp(choice,"combat")){
		display("You look if the coast is clear and find that the zombie is alone. Unsheathing your ball-pen from its safety cap, you leap towards him and plunge your ball-pen into a fury of stabs. The zombie has no chance."); 
		SPACE; PAUSE;
		err = false;
		battle(g, 2);
		tutorialLookAtThings(g);
	}else{
		display("The bloody student stumbles on to the floor and tries to stand up."); 
		SPACE; PAUSE;
	}
	}while(err);
}

void tutorialInventory(game *g){
	String choice;
	Boolean err;
	
	do{
	err = true;
	frame("Tutorial: You are about to view your inventory.");
	display("You could view the item you just picked up a while ago by choosing to view your (inventory) or you can keep (look)ing around in your current room."); 
	grabs(choice, 50); NL;
	
	if (!strcmp(choice,"inventory")){
		display("You will now view your equipment."); 
		SPACE; PAUSE;
		displayInventory(g);
		SPACE; PAUSE; 
		tutorialFight(g);
		err = false;
	}else{
		display("The room was clean. There was nothing out of the ordinary."); 
		SPACE; PAUSE;
	}
	}while(err);
}

void initLocation(game *g){
	player *pl = g->pl;
	inventory *inv = g->pl->inv;
	String choice;
	Boolean err;
		err = true;	
	do{
		frame("Tutorial: Practicing the game-play interaction.");
		SPACE; speak("You find yourself in a room inside the "); speak(selectLocation(g, pl->course)); speak(" building."); NL;
		display("You can see tables, chairs, benches, lockers, and a room. Probably taking some time to <look> <at things> would be wise, unless you know what to do already. In that case, you can just (leave) the room."); 
		display("-- By leaving, you will skip the tutorial.");
		display_("What do you want to do?"); 
		grabs(choice, 50); NL;
		if (!strcmp(choice, "look lockers")){
			err = false;
			addToInventory(g, spawnItem(g, 1, DEFAULT_DURABILITY, 1));
			display("You found a new item. You should check your (inventory) later."); 
			SPACE; PAUSE;
			tutorialInventory(g);
		}else if (!strcmp(choice, "leave")){
			err = false;
			display("You decide to leave the room immediately."); 
			SPACE; PAUSE;
		}else{
			display("There was nothing interesting."); 
			SPACE; PAUSE;
		}
	}while(err);
}

void initPlayerCourse(game *g){
	player *pl = g->pl;
	String choice, frameMessage;
	Boolean err;
	strcpy(frameMessage, "Tutorial: Course selection for ");
	strcat(frameMessage, pl->player_name);
	strcat(frameMessage, "...");
	frame(frameMessage);
	
	do{
		err = false;
		display_("You look at your test paper again. You check your course ID... It says that you're a student from the college... (ccs, cos, cob, ced, soe, cla, coe)"); 
		grabs(choice, 10); NL;
		if (!strcmp(choice, "ccs")){
			setPlayerCourse(pl, COURSE_CCS);
		}else if (!strcmp(choice, "cos")){
			setPlayerCourse(pl, COURSE_COS);
		}else if (!strcmp(choice, "cob")){
			setPlayerCourse(pl, COURSE_COB);
		}else if (!strcmp(choice, "ced")){
			setPlayerCourse(pl, COURSE_CED);
		}else if (!strcmp(choice, "soe")){
			setPlayerCourse(pl, COURSE_SOE);
		}else if (!strcmp(choice, "cla")){
			setPlayerCourse(pl, COURSE_CLA);
		}else if (!strcmp(choice, "coe")){
			setPlayerCourse(pl, COURSE_COE);
		}else {
			display("You couldn't read your hand writing."); NL; 
			SPACE; PAUSE;
			err = true;
			frame("Zombies are getting hungry...");
		}
	}while(err);
	SPACE; speak("You remember that you are from the "); speak(selectCollege(pl->course, 1)); speak("."); NL; NL;
	SPACE; PAUSE;
	initLocation(g);
	
}

void initPlayerName(game *g){
	player *pl = g->pl;
	String playerName, choice;
	LongString message;
	Boolean err;
	
	initPlayer(g);
	do{
		frame("Tutorial: Player name initialization...");
		display("The room was completely empty and you find that some chairs were scattered. Confusion hits you slightly. You can't completely remember everything so you turn to your desk for some clarity.");
		display("You look at the crummy test paper. Your name is... (max 7 letters)"); 
		grabs(playerName, 9); NL;
		
		err = true;
		
		if (!strcmp(playerName,"")){
			display("Please input a valid name."); 
			SPACE; PAUSE;
		}else{
			sprintf(message, "You read <%s>.", playerName);
			display_(message);
			if (!strcmp(playerName, "input") || !strcmp(playerName, "7 chara")){
				increaseSarcasm(pl);
				display("The zombies are now aware of scent and the blood hounds are set loose.I urge you to control your sarcasm."); 
				SPACE; PAUSE;
			}else if (strlen(playerName) > 7){
				display("The given name is too long."); 
				SPACE; PAUSE;
			}else{
				display_("You think again slowly. Is this right? (yes, no)"); 
				grabs(choice, 50); NL;
				//choice = strtolower(choice);
				if (!strcmp(choice, "yes")){
					sprintf(message, "You remember a few things now. Your name is %s. Your class ended a few hours ago but you must have fallen asleep.", playerName);
					display(message);
					SPACE; PAUSE;
					setPlayerName(pl, playerName);
					err = false;
				}else if (!strcmp(choice, "no")){
					CLEAN_SCREEN; NL; 
					frame("The zombies are getting impatient with your visual incompetence...");
					display("You hear the growing moans of zombies somewhere near by."); 
					SPACE; PAUSE;
				}else{
					display("You were not able to read your hand writing.");
					SPACE; PAUSE;
				
				}
			}
		}
	}while(err);
	
	initPlayerCourse(g);
}

void startGame(game *g){
	String choice;
	Boolean err, skip = false;
	frame("Tutorial: A simple tip! Don't be a smart-ass in this game.");
	display("Before we start playing, let's get used to how the game will be interacting with you. When you enter your inputs, you must have specific answers. Letter-case is important.");
	display("This means that 'Andrew' and 'andrew' is different, as well as with 'locker' and 'lockers'.");
	display("Is that understood? (yes, no)"); 
	do{
		err = true;
		grabs(choice, 50); NL;
		if (!strcmp(choice, "yes")){
			display("Then you're all set to play then. Let's get started. "); 
			err = false;
			SPACE; PAUSE;
		}else if (!strcmp(choice, "skip")){
			err = false;
			skip = true;
		}else{
			display("Letter case is important when handling interest points or things you observe in the maps. Going to new locations require you to type down exactly how it is written. Do you understand now?");
		}
	}while(err);
	
	if (!skip){
		frame("Tutorial : We suggest you enjoy reading the descriptions!");
		display("A quiet breeze blows from the open window on your left. You're all alone in the classroom and dusk is about to hit the horizon. "); 
		SPACE; PAUSE; 
		
		initPlayerName(g);
	}else{
		initPlayer(g);
		setPlayerName(g->pl, "Darren");
		setPlayerCourse(g->pl, 0);
		g->pl->location =  1;
		addToInventory(g, spawnItem(g, 1, 1, 1));
		SPACE; PAUSE;
	}
}

game* initGame(){
	/* Game Settings */
	game *g;
	if ((g = malloc(sizeof(game))) == NULL){
		p("There was not enough memory to create a game structure.");
		SPACE; PAUSE;
		exit(1);
	}

	/* Player Settings */
	player *pl;
	if ((pl = malloc(sizeof(player))) == NULL){
		p("There was not enough memory to create a player structure.");
		SPACE; PAUSE;
		exit(1);
	}
	
	if (DEBUG){
		NL;
		frame("Game initialization..."); 
	}
	
	/* Game initialization */
	g->itemdb = NULL;
	g->pl = NULL;
	g->mobdb = NULL;
	g->loc = NULL;
	g->interests = NULL;
	g->pldb = NULL;
	
	/* Still playing */
	g->game_status = 1;
	
	/* time ranges from 0 to 144, the number of 10 minutes in a day */
	g->time = 6 * 17;
	
	/* Player has nothing in the inventory */
	pl->inv = NULL;
	
	/* Item Database */
	populateItemDatabase(g);
	
	/* Mob Database */
	populateMobDatabase(g);
	
	/* Location database */
	populateLocations(g);
	
	/* Connect levels together */
	populateConnections(g);

	/* Populate mob spawns */
	populateMobSpawns(g);
	
	/* Populate interest points */
	populateInterests(g);
	
	/* Populate items on interest points */
	populateItemsOnInterests(g);
	
	/* Sorting */
	sortItemDatabase(g);
	sortMobDatabase(g);
		
	createAccount(g, "admin", "admin", 1);
	
	if (DEBUG){
		NL;
		SPACE; p("Game has finished initialization.");
		NL; NL;
		SPACE; PAUSE;
	}
	
	return g;
}
