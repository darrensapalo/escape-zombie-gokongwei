mob* mobBefore(mob *mob_db, mob *current){
	if (mob_db == current)
		return NULL;
	while(mob_db){
		if(mob_db->next_mob == current)
			return mob_db;
		mob_db = mob_db->next_mob;
	}
	return NULL;
}

mob* sortIntomob(mob *mob_db, mob *toBeSorted){
	int mob_id = toBeSorted->mob_id;
	mob *current = mob_db;
	mob *before_element = mobBefore(mob_db, toBeSorted);
	mob *after_element = toBeSorted->next_mob;
	
	/* Remove -toBeSorted- from the linked list */
	if (before_element)
		before_element->next_mob = after_element;
	
	/* Starting from the beginning find out if the item's 
	 * item ID is greater than the item you wish to
	 * stop loop then re-insert into the database */
	while(current){
		if (current->mob_id > mob_id)
			break;
		current = current->next_mob;
	}
	
	before_element = mobBefore(mob_db, current);
	if (before_element){
		after_element = before_element->next_mob;
		before_element->next_mob = toBeSorted;
		toBeSorted->next_mob = after_element;
	}else{
		toBeSorted->next_mob = mob_db;
		mob_db = toBeSorted;
		
	}
	return mob_db;
}

mob* sortMobDatabase(game *g){
	mob *mob_db = g->mobdb;
	mob *current = mob_db;
	
	while(current){
		mob_db = sortIntomob(mob_db, current);
		current = current->next_mob;
	}
	g->mobdb = mob_db;
	return mob_db;
}


void viewMobSpawnList(mob *cMob){
	String message;

	sprintf(message, "%-4s %-32s", "ID", "Mob Name");
	display_(message);
	
	if (!cMob){
		display("There are currently no mob spawns in this location.");
		return;
	}
		
	while(cMob){
		sprintf(message, "%-4d %-32s", cMob->mob_id, cMob->mob_name);
		display_(message);
		cMob = cMob->next_mob;
	}
}


char* getMobName(game *g, int mob_id){
	mob *cMob = g->mobdb;
	while(cMob){
		if (cMob->mob_id == mob_id)
			break;
		cMob = cMob->next_mob;
	}
	if (!cMob)
		return "???";
	return cMob->mob_name;
}

Boolean mobIsValid(game *g, int mob_id){
	mob *cMob = g->mobdb;
	while(cMob){
		if (cMob->mob_id == mob_id)
			return 1;
		cMob = cMob->next_mob;
	}
	return 0;
}

mob* spawnMonster(game *g, int mob_id){
	mob *monster, *spawn;
	monster = g->mobdb;
	
	if ((spawn = malloc(sizeof(mob))) == NULL){
		p("There was not enough memory to spawn a new mob instance.");
		SPACE; PAUSE;
		exit(1);
	}

	while(monster->mob_id < mob_id)
		monster = monster->next_mob;
	
	/* Set details */
	spawn->mob_id = mob_id;
	spawn->HP = monster->HP;
	strcpy(spawn->mob_name, monster->mob_name);
	strcpy(spawn->mob_description,monster->mob_description);
	spawn->weakness = monster->weakness;
	spawn->immune = monster->immune;
	spawn->rate = monster->rate;
	spawn->next_mob = NULL;
	
	if (MOB_SPAWN_DATA){
		SPACE; p("New mob instance [%s] spawned.", spawn->mob_name);
		NL;
	}	
	return spawn;
}


int getMobDamage(game *g, int mob_id){
	mob *monster = g->mobdb;
	while(monster->mob_id < mob_id){
		monster = monster->next_mob;
	}
	return monster->damage;
}
