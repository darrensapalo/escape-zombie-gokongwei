
void incrementTime(game *g){
	g->time += 1;
}

void execute_commands(game *g, char* command, char* param){
int look = 0;
	if (!strcmp(command,"help")){
		cmd_help();
		
		if (!strcmp(param,"me")){
			NL; display("The zombies mock your waivering confidence."); 
			
		}
	}else if (!strcmp(command,"where")){
		cmd_where(g);
	}else if (!strcmp(command,"go")){
		cmd_go(g, param);
		incrementTime(g);
	}else if (!strcmp(command,"look") || !strcmp(command,"visit") || !strcmp(command,"examine") ){
		if (!strcmp(param,""))
			display_("Please input what you wish to look at.");
		else{
			if (!strcmp(command,"look"))
				look = 0;
			else if (!strcmp(command,"examine"))
				look = 1;
			else if (!strcmp(command,"visit"))
				look = 2;
			cmd_look(g, param, look);
			incrementTime(g);
		}
	}else if (!strcmp(command,"take")){
		cmd_take(g, param);
		incrementTime(g);
	}else if (!strcmp(command,"save")){
		cmd_save(g, param);
	}else if (!strcmp(command,"load")){
		cmd_load(g, param);
	}else if (!strcmp(command,"inventory") || !strcmp(command,"inv") || !strcmp(command,"items")){
		displayInventory(g);
	}else if (bullshit_commands(g, command, param)){
		display(unknown_command(rand_(2))); 
	}
}

void gameOver(game *g){
	frame("Game over");
	display("The horde of zombies unfortunately over powered you. They now laugh and mock your petty attempts at escape.");
	SPACE; PAUSE;
}
void gameSuccess(game *g){
	frame("You were able to escape");
	display("The horde of zombies were no match for you. You took your Hummer vehicle and plowed through the zombies in Taft. Reaching Roxas Boulevard, you can already feel the sweet taste freedom.");
	SPACE; PAUSE;
}

int assessGame(game *g){
	player *pl = g->pl;
	if (playerHealth(g) <= 0){
		return GAME_OVER;
	}else if (isInInventory(g, 999))
		return GAME_SUCCESS;
	return 0;
}

void currentFrame(game *g){
	player *player = g->pl;
	location *current_loc;
	String message;
	current_loc = getLocation(g, player->location);
	sprintf(message, "Name: %s | HP: %d%% | Location: %s | Time : %s", player->player_name, player->HP, current_loc->location_name, getTime(g));
	frame(message);
}


/* The game itself */
int playGame(game *g){
	player *player = g->pl;
	location *current_loc;
	LongString data, command, param;
	Boolean get_param;
	int mob_id = 0;
	int status;
	for(;;){
		if (status = assessGame(g))
			return status;

			current_loc = getLocation(g, player->location);
		currentFrame(g);
		get_param = false;

		if ((mob_id = simulateFieldCombat(current_loc)))
			battle(g, mob_id);
		else{
			display(current_loc->description); 
			display("What do you want to do?"); 
			grabs(data, 50);
			if (strlen(data) > 0){
				/* If there is a space, then there is a parameter */
				if (spaceExists(data))
					get_param = true;
				else
					strcpy(param,"");
					
				/* Tokenize the command */
				parseCommand(command, data);
				
				/* Tokenize the parameter if there is a parameter */
				if (get_param){
					parseParameters(param, data);
				}
				
				/* Lower case the command */
				strtolower(command);
				
				/* Do the specified command */
				execute_commands(g, command, param);
				

			}else{
				display(unknown_command(rand_(2))); 
			}
			SPACE; PAUSE;
		}
	}
}


void displayTitle(){
if (SCREEN_FAST)return;
p("                                  ir,                        "); NL;
sleep(50);
p("                                  @@@@@3,                    "); NL;
sleep(50);
p("                           H@#S.  @@@@@@@@3                  "); NL;
sleep(50);
p("                   ,9@@@@#r,@@@@# r@@@@@@@@@                 "); NL;
sleep(50);
p("                .9@@@@@@@@@@@@@@@@X@@@@@@@@@                 "); NL;
sleep(50);
p("                i@@@@@@@@@@@@@@@@@@@@@@@@@@@                 "); NL;
sleep(50);
p("              .rS2GB#@@@@@@@@@@@@@@@@@@@@@@@                 "); NL;
sleep(50);
p("            r@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#9;          "); NL;
sleep(50);
p("           @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@        "); NL;
sleep(50);
p("          @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#: :@@@@@@@@@@       "); NL;
sleep(50);
p("         A@@@@@@MM@@@@@@#&irh@@@@@@@@@#;     i@@@@@@@@@      "); NL;
sleep(50);
p("        ;@X#@@@2          ,A@@@@@@@@h,        ,@@@@@@@@:     "); NL;
sleep(50);
p("          A@@@S         2@@@@@@@M2:       ..   .@@@@#@@M     "); NL;
sleep(50);
p("         @@@@9      ,;;;;.          .rXXS;.     ;@@@X @@     "); NL;
sleep(50);
p("        @@@@@   ,X@@@@@@@@2       :@@@@@@@@@r    A@@@.       "); NL;
sleep(50);
p("       r@@@@:  A@@@       @@r ...r@@       @@@,   @@@@       "); NL;
sleep(50);
p("       3@@@@ .@@@           2,,,.s          @@@:  M@@@3      "); NL;
sleep(50);
p("     59HM@@i @@@@      @    ,;.  ,   @       @@@  r@@@@ABs   "); NL;
sleep(50);
p("    @H.  9@ ,@@@@           s.;h:,            @@2 .@s  ,r@@  "); NL;
sleep(50);
p("   @M    @@ ,@@@@          r,,@@@::          @@@&  @#  .  @2 "); NL;
sleep(50);
p("   @: .  @@  H@@@@      .,r: @@@@# r;,      @@@@:  @@  .. X@ "); NL;
sleep(50);
p("   @i .  @@;  r@@@@@@@@@X;,.,@@@@@ .X@@@@@@@@@@;   @@  ,, HH "); NL;
sleep(50);
p("   r@ .. @@@     :risr:..., S@@&#@r   ;XB##BX;  . :@@ .,. @  "); NL;
sleep(50);
p("    @@   X@@&ir        .... @@.  rr .          ., M@A,.  @S  "); NL;
sleep(50);
p("     #@, i@@@AB&  23,  ,;:. i       .. ;r     ., ;@@@2 r@i   "); NL;
sleep(50);
p("      ;@X3@@@@3Ar:3G, s#@#H:  ../;AG:..2&. :rr:  @@@@GAM     "); NL;
sleep(50);
p("         :#@@@@hBM: :;#M  v  v  v  v;    .3@@@#2@@@@M:       "); NL;
sleep(50);
p("            rH@@AhAB#B&          ~     ir5     @@h;          "); NL;
sleep(50);
p("               ,@@hA&Gh   ^   ^    ^X# ##A922@@,             "); NL;
sleep(50);
p("                 @@#AM#  #M   HH     @H@#BA@@X               "); NL;
sleep(50);
p("                  r@#2XhA3X2 XXX3   h  959@@.                "); NL;
sleep(50);
p("                    X@#,,5GhXXXX3&  XX2&@#:                  "); NL;
sleep(50);
p("                      S9r,,i2XXXS;  ,M@G,                    "); NL;
sleep(50);
p("                        :522X3XSisXG5:                       "); NL; NL;
sleep(50);

p("                                  _                        "); NL;
sleep(50);
p("                                 | |                       "); NL;
sleep(50);
p("     ____ ____   ___   ____  ____| |_   _ ____   ___  ____ "); NL;
sleep(50);
p("    / _  |  _ \\ / _ \\ / ___)/ _  | | | | |  _ \\ /___)/ _  )"); NL;
sleep(50);
p("   ( ( | | | | | |_| ( (___( ( | | | |_| | | | |___ ( (/ / "); NL;
sleep(50);
p("    \\_||_| ||_/ \\___/ \\____)\\_||_|_|\\__  | ||_/(___/ \\____)"); NL;
sleep(50);
p("         |_|                       (____/|_|               "); NL; NL;
}



void quitGame(){
	system("CLS");
	NL; frame("Thank you for playing!"); 
	display("This program was coded by Darren Karl A. Sapalo ID # 11127821, a Frosh student of DLSU-Manila. He is taking up Computer Science and is currently unsure of what he will be majoring in."); NL; 
	display("If you enjoyed this program, please let him know by... trying to figure out how to contact him."); NL;
	SPACE; PAUSE;
}


void account_manage(game *g){
	String choice;
	LongString message;
	String username, password;
	int level;
	
	for(;;){
		CLEAN_SCREEN;
		NL;
		frame("Account management");
		display_("[add] new account");
		display_("[remove] account");
		display("[exit] to main menu");
		
		display_("What do you want to do?");
		grabs(choice, 10);
		if (!strcmp(choice,"add")){
			
			CLEAN_SCREEN;NL;
			frame("Creating new account");
			display_("Enter username:");
			grabs(username, 32);
			display_("Enter password:");
			grabs(password, 32);
			display_("Enter user level: (0 - regular, 1 - admin)");
			level = grabi(choice, 10);
			
			CLEAN_SCREEN;NL;
			frame("Confirm account creation");
			sprintf(message, "Username: %s", username);
			display_(message);
			sprintf(message, "Password: %s", password);
			display_(message);
			sprintf(message, "This user's account %s an administrator.", level ? "is" : "is not");
			display(message);
			
			display_("Is this data correct? [yes or no]");
			grabs(choice, 32);
			
			if (!strcmp(choice,"yes")){
				display("The account was created successfully.");
				createAccount(g, username, password, level);
				SPACE; PAUSE;
			}else
				continue;
				
		}else if (!strcmp(choice,"remove")){
			CLEAN_SCREEN; NL;
			frame("Account deletion");
			display("This feature is currently not yet implemented.");
			SPACE; PAUSE;
		}else if (!strcmp(choice,"exit")){
			CLEAN_SCREEN;
			return;
		}
	}

}

player* login_interface(game *g){
	String choice;
	LongString message;
	String username, password;
	player *cPl;
	NL;
	for(;;){
		frame("Log in");
		display_("type [exit] as username to return to the main menu.");
		display_("Please enter your username: ");
		grabs(username, 32);
		
		if (!strcmp(username,"exit"))
			return NULL;
		
		display_("Please enter your password: ");
		grabs(password, 32);
		
		
		
		cPl = loginPlayer(g, username, password);
		if (!cPl){
			display("Invalid user details.");
			SPACE; PAUSE;
			continue;
		}
		display("You are now logged in.");
		g->pl = cPl;
		SPACE; PAUSE;
		return cPl;
	}
}

/* Game Menu */
int gameMenu(game *g){
	String choice;
	Boolean err, doneInput;
	err = doneInput = false;
	do{
		CLEAN_SCREEN;
		displayTitle();
		display_("[login] and start playing");
		display_("[account] management");
		display_("[dev] mode");
		display("[quit] game"); 
		
		display_("What would you like to do?");
		
		grabs(choice, 10); NL;
		strtolower(choice);
		
		if (!strcmp(choice,"login")){
			CLEAN_SCREEN;
			if (login_interface(g))
				return 1;
		}else if (!strcmp(choice,"account")){
			account_manage(g);
		}else if (!strcmp(choice,"dev")){
			display("Please note that the default admin account is admin:admin");
			SPACE; PAUSE;
			CLEAN_SCREEN;
			if (login_interface(g))
				edit_mode(g);
		}else if (!strcmp(choice,"quit")){
			return 0;
		}
	}while(!err);
	return 0;
}
