
int bullshit_commands(game *g, char *command, char *param){
	if ( ( (!strcmp(command, "get"))  && (!strcmp(param, "out")) ) || (!strcmp(command, "exit")) || (!strcmp(command, "die"))){
		display("The zombies laugh in chorus at your pathetic attempt."); 
		return 0;
	} else if ((!strcmp(command, "what") && !strcmp(param, "do i do")) || (!strcmp(command, "what") && !strcmp(param, "now"))){
		display("Beats me. Why don't you type [help] to find out what you can do?"); 
		return 0;
	}else if (!strcmp(command, "fuck") || !strcmp(command, "putang") || !strcmp(command, "putangina") || !strcmp(command, "puta") || !strcmp(command, "gago") || !strcmp(command, "pota") || (!strcmp(command, "what") && (!strcmp(param, "the fuck")))){
	// Column 89, and 159 are where the lines start
	//		  93, and 163 if you tabbed
		display("Your intestines, kidneys, liver, and heart start to bleed internally. You lose 10% of your health. Blood is scattering on the floor. The    zombies are now aware of your scent. "); 
		playerApplyHealth(g, -10);
		return 0;
	}else if (!strcmp(command, "jump")){
		display("You jump like an idiot and nothing happens."); 
		return 0;
	}else if (!strcmp(command, "run")){
		display("You run around in circles."); 
		return 0;
	}else if (!strcmp(command, "scream")){
		display("You inhaled deeply and screamed like there's no tomorrow. Sadly for   you, it attracted the zombies. They are now aware of your scent."); 
		return 0;
	}else if (!strcmp(command, "smoke")){
		display("You found a fresh cigarette stick in your bag and lit it. You feel a  little hopeful for survival now, at the cost of 5% HP."); 
		playerApplyHealth(g, -5);
		return 0;
	}else if (!strcmp(command, "billiards")){
		display("There aren't any nearby billiard tables and the zombies would own you in a game of nine balls anyway."); 
		return 0;
	}else if (!strcmp(command, "laugh")){
		display("You start with a small smile and a giggle, working your way to a heartwarming laugh. Things feel better now, don't they? :)"); 
		if (playerHealth(g) < 50)
			playerApplyHealth(g, 1);
		return 0;
	}else if (!strcmp(command, "smile")){
		display("A little down on the inside, you try to smile. You look around and    realize, hey! You're still alive. You grin a little, filled with hope.Things feel better now, don't they? :)"); 
		if (playerHealth(g) < 50)
			playerApplyHealth(g, 1);
		return 0;
	}else if (!strcmp(command, "joke")){
		if (rand_(2)){
			display("You crack a funny joke and the zombies are amused. Lucky for you, theydecide to stay away... for now."); 
			display("HAHAHAHAHA. They said."); 
			return 0;
		}else{
			display("The zombies weren't amused with your corny joke. Neither did you. Too bad for you, I guess."); 
			return 0;
		}
	}else if ((strcmp(command, "live") == 0) && (strcmp(param, "jesus in our hearts") == 0)){
		display("forever."); 
		playerApplyHealth(g, 100-playerHealth(g));
		return 0;
	}else if (!strcmp(command, "whosyourdaddy") || !strcmp(command, "pray")){
		display("Saint John Baptist de La Salle,");
		display("Pray for us.");
		return 0;
	}else if(!strcmp(command,"quit")){
		exit(1);
	}
return 1;
}

int bullshit_go(game *g, char *param){
	if (!strcmp(param, "home") || !strcmp(param, "out")){
		display_("Haha, you wish."); 
		return 0;
	}
return 1;
}

void cmd_go (game *g, char* param){
	player *pl = g->pl;
	location *loc = getLocation(g, pl->location);
	String message;
	
	
	if (!strcmp(param, loc->location_name)){
		display_("You are already here."); 
	}else if (locationIsConnected(loc, param)){
		moveLocation(g, locationIsConnected(loc, param));
		sprintf(message, "You decided to go to %s.", (getLocation(g, locationIsConnected(loc, param)))->location_name );
		display_(message); 
	}else if (bullshit_go(g, param)){
		display_("The places are not connected."); 
	}
}
void cmd_look(game *g, char* param, int i){
	String message;
	interest *found_interest;
	if (DEBUG && (VIEW_ALL_INTERESTS == 1 || strcmp(param,"all") == 0)){
		viewInterests(g);
	}else{
		found_interest = findInterestPoint(g, getLocation(g, g->pl->location), param);
		if (found_interest != NULL){
			if (canViewInterestPoint(g, found_interest)){
				display(found_interest->description);
				exploreInterestPoint(g, found_interest);
			}else{
				interestPointRequires(g, found_interest);
			}
		}else{
			switch(i){
				case 0:
					sprintf(message, "You cannot find any %s.", param);
				break;
				case 1:
					sprintf(message, "There's nothing interesting about the %s.", param);
				break;
				case 2:
					sprintf(message, "You were unable to visit %s.", param);
				break;
			}
			display_(message);
		}
	}
}
void cmd_take (game *g, char* param){
	String message;
	if (!itemToInventory(g, param)){
		sprintf(message, "You could not find any %s.", param);
		display(message);
	}
}

void cmd_examine(game *g, char* param){
	
}
void cmd_save (game *g, char* param){

}
void cmd_load (game *g, char* param){

}



void cmd_where(game *g){
	location *loc = getLocation(g, g->pl->location);
	display(loc->where);

}

void cmd_help(){
	display_("List of actions: ");
	SPACE; p("[inventory] - (no param) view all your items."); NL;
	SPACE; p("[examine] - (object) looks at an object in your inventory in more detail.");NL;
	SPACE; p("[look]  - (object) inspects a certain object."); NL;
	SPACE; p("[where] - (no param) states what places you can go to from where you are."); NL;
	SPACE; p("[go]    - (location) goes to a specific location."); NL;
	SPACE; p("[take]  - (object) picks up a certain object if it can be taken."); NL;
	SPACE; p("[help]  - (no param) opens up the list of actions you can choose from."); NL;
	SPACE; p("[save]  - (num) saves your current game."); NL;
	SPACE; p("[load]  - (num) loads your previous game."); NL;
}

char* unknown_command(int n){
	switch(n){
		default:
		case 0:
			return "You stare blankly into space.";
		break;
		case 2:
			return "You can hear the crickets in the background.";
		break;
		case 1:
			return "The action you are trying is not allowed.";
		break;
	}
}

char* area_restricted(int n){
	switch(n){
		default:
		case 0:
			return "You stare blankly into space.";
		break;
		case 1:
			return "The action you are trying is not allowed.";
		break;
	}
}

void parseCommand(char *command, char *text){
	int i = 0;
	while( *(text + i) && !( *(text + i) == ' ' || *(text + i) == '\n') ){
		*(command + i) = *(text + i);
		i++;
	}
	*(command + i) = '\0';
}

void parseParameters(char *param, char *text){
	int j, i = 0;
	
	while(*(text + i) != ' ')
		i++;
	i++;
	j = i;
	
	while(*(text + i)){
		*(param + i - j) = *(text + i);
		i++;
	}
	*(param + i - j) = '\0';
}
