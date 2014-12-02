void attachItemToLocation(game *g, inventory *cInv, location *cLoc){
	inventory *current = cLoc->items;
	cInv->isDiscovered = true;
	if (!current){
		cLoc->items = cInv;
		return;
	}
	
	while(current->next_item != NULL)
		current = current->next_item;
	
	current->next_item = cInv;
}

void viewItemListOfLocation(game *g, location* cLoc){
	inventory *current = cLoc->items;
	String message, item_name;
	SPACE; p("%-4s %-8s %-32s", "ID", "Amount", "Item Name"); NL;
	if (!current)
		display_("There were no items attached to this location.");
	
	while(current){
		sprintf(message, "%-4d %-8d %-32s", current->item_id, current->amount, getItemName(g, current->item_id)); 
		display_(message);
		current = current->next_item;
	}
}

location* getLocation(game *g, int loc_id){
	location *loc = g->loc;
	while(loc != NULL){
		if (loc->location_id == loc_id)
			break;
		loc = loc->next_loc;
	}
	return loc;
}
char* getLocationName(game *g, int loc_id){
	location *loc = g->loc;
	while(loc != NULL){
		if (loc->location_id == loc_id)
			break;
		loc = loc->next_loc;
	}
	return loc->location_name;
}



void viewLocationDatabase(game*g){
	location* cLoc = g->loc;
	
	SPACE; p("%- 4s %-32s", "ID", "Name"); NL;
	if(!cLoc)
		display_("There were no locations in the location database.");
	
	while(cLoc){
		SPACE; p("%-4d %-32s", cLoc->location_id, cLoc->location_name); NL;
		cLoc = cLoc->next_loc;
	}
}

void viewConnections(game *g, int loc_id){
	location *cLoc = getLocation(g, loc_id);
	int i;
	String message;
	
	for (i = 0; i < 5; i++)
		if (cLoc->connect[i]){
			sprintf(message, "%s", cLoc->connect[i]->location_name);
			display_(message);
			
		}
}

char* selectLocation(game *g, int i){
	g->pl->location = i;
	
	switch(i){
		case COURSE_CCS: 
		return "Gokungwei";
		
		case COURSE_COS: 
		return "Saint Joseph";
		
		case COURSE_COB: 
		return "Yuchengco";
		
		case COURSE_CED: 
		return "Andrew";
		
		case COURSE_SOE: 
		return "La Salle";
		
		case COURSE_CLA: 
		return "William Hall";
		
		case COURSE_COE: 
		return "Velasco";
		
		default:
		return "???";
	}
}


void spawn_mob(game *g, int loc_id, int mob_id){
	mob *new_spawn, *current;
	location *loc;
	loc = g->loc;
	
	/* Move to the proper location */
	while(loc->location_id < loc_id){
	loc = loc->next_loc;
	}
	
	new_spawn = spawnMonster(g, mob_id);
	current = loc->mob;
	// Assign new value
	if (current == NULL){
		current = new_spawn;
	}else{
		while(current->next_mob != NULL)
			current = current->next_mob;
		current->next_mob = new_spawn;
	}

	if (MOB_SPAWN_DATA){
		SPACE; p("New mob [%s] instanced at location [%s].", new_spawn->mob_name, loc->location_name);
		NL;
	}
}



Boolean locationIsValid(game *g, int loc_id){
	location *loc = g->loc;
	if (loc == NULL)
		return false;
	
	while(loc){
		if (loc->location_id == loc_id)
			return true;
		loc = loc->next_loc;
	}
	return false;
}

int locationIsConnected(location *loc, char *dest){
	int i;
	for(i = 0; i < 5; i++){
		if (loc->connect[i] != NULL)
			if (!strcmp(loc->connect[i]->location_name, dest))
				return loc->connect[i]->location_id;
	}
	return 0;
}



void moveLocation(game *g, int dest){
	player *pl = g->pl;
	pl->location = dest;
}

Boolean connectLocation(game *g, int current_loc, int dest_loc){
	location *loc, *current, *dest;
	int i = 0;
	Boolean added = false;
	LongString message;
	/* main location database */
	loc = g->loc;
	
	current = getLocation(g, current_loc);
	dest = getLocation(g, dest_loc);

	if( !(current && dest))
		return false;
	
	while ( !added ){
		if (current->connect[i] == NULL){
			current->connect[i] = dest;
			if (LOCATION_CONNECTION >= 2){
				sprintf(message, "[%s] to [%s]. Success. %dth link.", current->location_name, dest->location_name, i);
				SPACE; p(message); NL;
			}
			added = true;
		}else{
			if (LOCATION_CONNECTION >= 3){
				SPACE; p("%dth element unavailable for %s. occupied by %s.", i, current->location_name, current->connect[i]->location_name); NL;
			}
			i++;
		}
	}
	
	if (LOCATION_CONNECTION){
		SPACE; p("location [%s] is connected to location [%s].", current->location_name, dest->location_name); NL;
	}
	return true;
}

location* getLastLocation(game *g){
	location *loc = g->loc;
	if (loc == NULL)
		return loc;
	
	while(loc->next_loc != NULL){
		loc = loc->next_loc;
	}
	
	return loc;
}

