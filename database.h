void parseCSV_item_spawn(game *g, FILE *fp){
	char data[2048];
	String interest_name;
	LongString message;
	int loc_id, item_id, amount;
	loc_id = item_id = amount = 0;
	inventory *cItem;
	interest *cInterest;
	location *cLoc;
	Boolean attachToLocation;
	while(!feof(fp)){
		attachToLocation = false;
		/* Gets entry from csv */
		fgets(data, 2047, fp);
		CHECK_IF_COMMENT_TOK; CHECK_IF_BLANK_TOK;

		loc_id = parseCsvInt(START_FTOK);
		strcpy(interest_name,parseCsvStr(NEXT_FTOK));
		item_id = parseCsvInt(NEXT_FTOK);
		amount = parseCsvInt(NEXT_FTOK);
		
		if (!strcmp(interest_name,"!map"))
			attachToLocation = true;
			
		
		
		cLoc = getLocation(g, loc_id);
		cItem = spawnItem(g, item_id, DEFAULT_DURABILITY, amount);
		
		if (!attachToLocation)
			cInterest = findInterestPoint(g, cLoc, interest_name);
		
		if (!cLoc){
			sprintf(message, "Invalid location [%d] used for attaching a new item to an interest.", loc_id);
		}else if (!cItem){
			sprintf(message, "Invalid item [%d] to be attached to interest [%s].", item_id, cInterest->name);
		}else if (!cInterest && !attachToLocation){
			sprintf(message, "Invalid interest [%s] missing in location [%s].", interest_name, interest_name);
		}else{
			if (!attachToLocation){
				attachItemToInterestPoint(g, cItem, cInterest);
				sprintf(message, "Item [%d:%s] attached to interest [%s].", item_id, getItemName(g, item_id), cInterest->name);
			}else{
				attachItemToLocation(g, cItem, cLoc);
				sprintf(message, "Item [%d:%s] attached to location [%s].", item_id, getItemName(g, item_id), cLoc->location_name);
			}
		}
		
		if (ITEM_TO_INTEREST)
			display_(message);
    }
}

interest* construct_interest(game *g, int loc_id, char* name, char* description, int item_req, int isDiscovered){
	interest *current, *start_interest;
	inventory *item;
	location *current_loc;
	String message;
	int item_id;
	current = NULL;
	
	if ((current = malloc(sizeof(interest))) == NULL){
		display("There was an error with allocating memory for a new interest instance.");
		SPACE; PAUSE; exit(1);
	}else{
		/* Error checking */
		if (loc_id < 1){
			sprintf(message, "The given location in the database is incorrect. [ID %d]", loc_id); 
			display_(message);
			SPACE; PAUSE; exit(1);
		}
		if (!strcmp(name,"")){
			sprintf(message, "An entry in the location database has a field `name` value of \"\". [ID %d]", loc_id);
			display_(message);
			SPACE; PAUSE; exit(1);
		}
		if (!strcmp(description,"")){
			sprintf(message, "An entry in the location database has a field `description` value of \"\". ID %d.", loc_id); 
			display_(message);
			SPACE; PAUSE; exit(1);
		}
		/* Gets the data from CSV db file */
		strcpy(current->name,name);
		strcpy(current->description,description);

		if (item_req < 0)
			current->item_requirement = -1;
		else
			current->item_requirement = item_req;
			
		if (isDiscovered)
			current->isDiscovered = 1;
		else
			current->isDiscovered = 0;
			
		current->next_item = NULL;
		current->next_interest = NULL;
		
		
		/* Given the location ID, find out the current location */
		current_loc = getLocation(g, loc_id);
		start_interest = current_loc->interests;

		/* Attaching the new interest to the end of the interest linked list.
		 *
		 * If interests of a location is undefined, then set the location's first interest 
		 * to the current interest. If not, then loop until you get to the last interest, 
		 * then set the next interest to be the current interests.
		 */

		attachInterestPointToLocation(g, current, current_loc);
		return current;
	}
}

void parseCSV_interests(game *g, FILE *fp){
	char data[2048];
	String name;
	LongString description;
	int loc_id, item_req, isDiscovered;

	while(!feof(fp)){
		/* Gets entry from csv */
		fgets(data, 2047, fp);
		
		CHECK_IF_COMMENT_TOK; CHECK_IF_BLANK_TOK;
		loc_id = parseCsvInt(START_FTOK);
		strcpy(name,parseCsvStr(NEXT_FTOK));
		strcpy(description,parseCsvStr(NEXT_FTOK));
		item_req = parseCsvInt(NEXT_FTOK);
		isDiscovered = parseCsvInt(NEXT_FTOK);
		/* Construct new interest and add to database */
		construct_interest(g, loc_id, name, description, item_req, isDiscovered);
		
    }
}

mob* construct_mob_spawn(game *g, int loc_id, int mob_id){
	mob *current, *start;
	location *cLoc;
	LongString message;
	current = start = NULL;
	
	cLoc = getLocation(g, loc_id);
	
	if (!locationIsValid(g, loc_id)){
		sprintf(message, "Invalid location id [%d] for mob [%d] spawn.", loc_id, mob_id);
		display_(message);
		return NULL;
	}else if (!mobIsValid(g, mob_id)){
		sprintf(message, "Invalid mob id [%d] for mob spawn in location [%s].", mob_id, cLoc->location_name);
		display_(message);
		return NULL;
	}
	
	/* Get one entry per line */
	if ((current = malloc(sizeof(mob))) == NULL){
		display("There was an error with allocating memory for a new mob instance.");
		SPACE; PAUSE; exit(1);
	}else{
		/* Given the location ID, find out the current location */
		
		
		/* Save the current mob list at start */
		start = cLoc->mob;
		
		current = spawnMonster(g, mob_id);
	 	
		/* If mobs of a location is undefined, then set the location's first mob to the current mob. 
		 * If not, then loop until you get to the last mob, then set the next mob to be the current
		 * mob.
		 */
		
		if (start == NULL){
			cLoc->mob = current;
		}else{
		
			while(start->next_mob != NULL){
				start = start->next_mob;
			}
		
			start->next_mob = current;
		}
	}
	return current;
}

void parseCSV_mob_spawn(game *g, FILE *fp){
	char data[2048];
	
	int mob_id = 0;
	int loc_id = 0;
	while(!feof(fp)){
		/* Gets entry from csv */
		fgets(data, 2047, fp);
		
		CHECK_IF_COMMENT_TOK; CHECK_IF_BLANK_TOK;
		loc_id = parseCsvInt(START_FTOK);
		mob_id = parseCsvInt(NEXT_FTOK);
		construct_mob_spawn(g, loc_id, mob_id);

    }
}

itemdb* construct_item(game *g, int item_id, char* item_name, char* description, int damage, int default_durability){
	itemdb *current, *db = g->itemdb;
	String message;
	current = NULL;
	
	if (itemIsValid(g, item_id)){
		sprintf(message, "Invalid item id [%d]. Already used by [%s]..", item_id, getItemName(g, item_id));
		display_(message);
		return NULL;
	}
	
	/* Get one entry per line */
	if ((current = malloc(sizeof(itemdb))) == NULL){
		display("There was an error with allocating memory for a new item structure.");
		SPACE; PAUSE; exit(1);
	}else{
		current->item_id = item_id;
		strcpy(current->item_name, item_name);
		strcpy(current->description, description);
		current->damage = damage;
		current->default_durability = default_durability;
		current->next_item = NULL;
		
		if (db == NULL){
			g->itemdb = current;
		}else{
			while(db->next_item != NULL)
				db = db->next_item;
			db->next_item = current;
		}
		
		if (ITEM_CREATION){
			sprintf(message, "New item structure [%d:%s] added to database.", item_id, current->item_name);
			display_(message);
		}
	}
	return current;
}

void parseCSV_item(game *g, FILE *fp){
	char data[2048];
	
	int item_id, damage, default_durability;
	String item_name;
	LongString description;
	
	
	while(!feof(fp)){
		/* Gets entry from csv */
		fgets(data, 2047, fp);
		//if (EOF)break;
		
		CHECK_IF_COMMENT_TOK; CHECK_IF_BLANK_TOK;
		
		item_id = parseCsvInt(START_FTOK);
		strcpy(item_name, parseCsvStr(NEXT_FTOK) );
		strcpy(description, parseCsvStr(NEXT_FTOK) );
		damage = parseCsvInt(NEXT_FTOK);
		default_durability = parseCsvInt(NEXT_FTOK);
		
		construct_item(g, item_id, item_name, description, damage, default_durability);
		
    }
}

location* construct_location(game *g, int location_id, char* location_name, char* description, char* where){
	location *current, *db = g->loc;
	String message;
	current = NULL;
	int i;
	
	if (locationIsValid(g, location_id)){
		sprintf(message, "Invalid location id [%d]. Already used by [%s].", location_id, getLocationName(g, location_id));
		display_(message);
		return NULL;
	}
	
	/* Get one entry per line */
	if ((current = malloc(sizeof(location))) == NULL){
		display("There was an error with allocating memory for a new item structure.");
		SPACE; PAUSE; exit(1);
	}else{
		current->location_id = location_id;
		strcpy(current->location_name, location_name);
		strcpy(current->description, description);
		strcpy(current->where, where);
		current->interests = NULL;
		current->next_loc = NULL;
		current->mob = NULL;
		current->items = NULL;
		
		for (i =0; i < 5; i++){
			current->connect[i] = NULL;
		}
		
		if (db == NULL)
			g->loc = current;
		else{
			while(db->next_loc != NULL)
				db = db->next_loc;
			db->next_loc = current;
		}	
		
		if (LOCATION_CREATION){
			sprintf(message, "New location instance [%s] created.", current->location_name); 
			display_(message);
		}
	}
	return current;
}

void parseCSV_location(game *g, FILE *fp){
	char data[2048];
	int location_id;
	String location_name;
	LongString description, where;
	
	while(!feof(fp)){
		/* Gets entry from csv */
		fgets(data, 2048, fp);
		CHECK_IF_COMMENT_TOK;
		CHECK_IF_BLANK_TOK;
		
		location_id = parseCsvInt(START_FTOK);
		strcpy(location_name, parseCsvStr(NEXT_FTOK));
		strcpy(description, parseCsvStr(NEXT_FTOK));
		strcpy(where,parseCsvStr(NEXT_FTOK));
		
		construct_location(g, location_id, location_name, description, where);
    }
}

void parseCSV_connection(game *g, FILE *fp){
	char data[2048];
	int from, dest;
	
	while(!feof(fp)){
		/* Gets entry from csv */
		fgets(data, 2047, fp);
		
		CHECK_IF_COMMENT_TOK; CHECK_IF_BLANK_TOK;
			
		from = parseCsvInt(START_FTOK);
		dest = parseCsvInt(NEXT_FTOK);
		
		connectLocation(g, from, dest);
		

    }
}

mob* construct_mob(game*g, int mob_id, char* mob_name, char* mob_description, int HP, int damage, int weakness, int immune, int rate){
	mob *current, *db;
	String message;
	current = NULL;
	
	db = g->mobdb;
	
	if(mobIsValid(g, mob_id)){
		sprintf(message, "Invalid mob id [%d]. Already used by [%s].", mob_id, getMobName(g, mob_id));
		display_(message);
		return NULL;
	}
	
	if ((current = malloc(sizeof(mob))) == NULL){
		display("There was an error with allocating memory for a new mob structure.");
		SPACE; PAUSE; exit(1);
	}else{
		
					
			current->mob_id = mob_id;
			strcpy(current->mob_name, mob_name);
			strcpy(current->mob_description, mob_description);
			current->HP = HP;
			current->damage = damage;
			current->weakness = weakness;
			current->immune = immune;
			current->rate = rate;
			current->next_mob = NULL;
		
		if (db == NULL)
			g->mobdb = current;
		else{
			while(db->next_mob != NULL)
				db = db->next_mob;
			db->next_mob = current;
		}
		
		if (MOB_CREATION){
			SPACE; p("New mob structure [%d:%s] added to database.", mob_id, current->mob_name);
			NL;
		}
	}
	return current;
}

void parseCSV_mob(game *g, FILE *fp){
	char data[2048];
	int mob_id, HP, damage, weakness, immune, rate;
	String mob_name;
	LongString mob_description; 

	while( !feof(fp) ){
		/* Gets entry from csv */
		fgets(data, 2047, fp);

		CHECK_IF_COMMENT_TOK; CHECK_IF_BLANK_TOK;
		
		mob_id = parseCsvInt(START_FTOK);
		strcpy(mob_name, parseCsvStr(NEXT_FTOK));
		strcpy(mob_description, parseCsvStr(NEXT_FTOK));
		HP = parseCsvInt(NEXT_FTOK);
		damage = parseCsvInt(NEXT_FTOK);
		weakness = parseCsvInt(NEXT_FTOK);
		immune = parseCsvInt(NEXT_FTOK);
		rate = parseCsvInt(NEXT_FTOK);
		
		construct_mob(g, mob_id, mob_name, mob_description, HP, damage, weakness, immune, rate);
		
    }
	
	fclose(fp);
}

/* Items Database */
void populateItemDatabase(game *g){
	FILE *fp;
	fp = fopen("items.db", "r");
	if (fp == NULL){
		display("The item database could not be loaded.");
		SPACE; PAUSE; exit(1);
	}
	parseCSV_item(g, fp);
	fclose(fp);
	
}

/* Mob Database */
void populateMobDatabase(game *g){
	FILE *fp;
	fp = fopen("mob.db", "r");
	if (fp == NULL){
		display("The mob database could not be loaded.");
		SPACE; PAUSE; exit(1);
	}
	//mob, id, name, desc, hp, damage, weakness, immune, rate
	parseCSV_mob(g, fp);
	fclose(fp);
}

/* Locations Database */
void populateLocations(game *g){
	FILE *fp;
	fp = fopen("location.db", "r");
	if (fp == NULL){
		display("The location database could not be loaded.");
		SPACE; PAUSE; exit(1);
	}
	parseCSV_location(g, fp);
	
}

/* Populate connections */
void populateConnections(game *g){
	FILE *fp;
	fp = fopen("connection.db", "r");
	
	if (fp == NULL){
		display("The connection database could not be loaded.");
		SPACE; PAUSE; exit(1);
	}
	
	parseCSV_connection(g, fp);
}

/* Populate connections */
void populateMobSpawns(game *g){
	FILE *fp;
	fp = fopen("mobspawn.db", "r");
	
	if (fp == NULL){
		display("The mob spawn database could not be loaded.");
		SPACE; PAUSE; exit(1);
	}
	
	parseCSV_mob_spawn(g, fp);
}

/* Populate interests */
void populateInterests(game *g){
	FILE *fp;
	fp = fopen("interests.db", "r");
	
	if (fp == NULL){
		display("The interests database could not be loaded.");
		SPACE; PAUSE; exit(1);
	}
	parseCSV_interests(g, fp);
}

/* Populate items found oninterests */
void populateItemsOnInterests(game *g){
	FILE *fp;
	fp = fopen("itemspawn.db", "r");
	if (fp == NULL){
		display("The item spawns database could not be loaded.");
		SPACE; PAUSE; exit(1);
	}
	parseCSV_item_spawn(g, fp);
}
