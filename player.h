
player* loginPlayer(game *g, char *username, char *password){
	player *cPl = g->pldb;
	while(cPl){
		if (!strcmp(cPl->username,username) && !strcmp(cPl->password,password))
			return cPl;
		cPl = cPl->next_pl;
	}
	return NULL;
}

void addPlayerToDB(game *g, player *nPl){
	player *cPl = g->pldb;
	
	if (!cPl){
		g->pldb = nPl;
		return;
	}
	
	while(cPl->next_pl)
		cPl = cPl->next_pl;
	
	cPl->next_pl = nPl;
}

void createAccount(game *g, char *username, char *password, int level){
	player *cPl;
	if ((cPl = malloc(sizeof(player))) == NULL){
		display("Cannot create a new account. Not enough memory.");
		SPACE; PAUSE; exit(1);
	}
	
	strcpy(cPl->username,username);
	strcpy(cPl->password,password);
	cPl->next_pl = NULL;
	cPl->level = level;
	cPl->location = 0;
	cPl->HP = 100;
	cPl->sarcasm = 0;
	cPl->inv = NULL;
	cPl->weapon = 0;
	cPl->weapon_durability = 0;
	
	addPlayerToDB(g, cPl);
}


void initPlayer(game *g){
	player *pl = g->pl;
	inventory *inv = NULL;
	strcpy(pl->password,"");
	pl->location = 0;
	pl->HP = 100;
	pl->sarcasm = 0;
	pl->inv = inv;
	pl->weapon = 0;
	pl->weapon_durability = 0;
}

void setPlayerName(player *pl, char *playerName){
	strcpy(pl->player_name, playerName);
}

void setPlayerCourse(player *pl, int courseID){
	pl->course = courseID;
}

void increaseSarcasm(player *pl){
	pl->sarcasm += 1;
}

void playerApplyHealth(game *g, int HP){
	player *pl = g->pl;
	pl->HP += HP;
}

int playerHealth(game *g){
	return g->pl->HP;
}


char* selectCollege(int i, int verbose){
	if (!verbose){
		switch(i){
			case COURSE_CCS: return "ccs";
			case COURSE_COS: return "cos";
			case COURSE_COB: return "cob";
			case COURSE_CED: return "ced";
			case COURSE_SOE: return "soe";
			case COURSE_CLA: return "cla";
			case COURSE_COE: return "coe";
			default: return "???";
		}
	}else{
		switch(i){
			case COURSE_CCS: return "College of Computer Science";
			case COURSE_COS: return "College of Science";
			case COURSE_COB: return "College of Business";
			case COURSE_CED: return "College of Education";
			case COURSE_SOE: return "School of Economics";
			case COURSE_CLA: return "College of Liberal Arts";
			case COURSE_COE: return "College of Engineering";
			default: return "???";
		}
	}
}
