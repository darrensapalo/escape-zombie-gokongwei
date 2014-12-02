
#ifndef RETURN_TO_EDITOR
	#define RETURN_TO_EDITOR display("You will now return to the Developer Menu."); SPACE; PAUSE; return;
#endif

#define NO_LOCATION_DB display("You have not yet populated the location database."); RETURN_TO_EDITOR
#define LOCATION_NOT_FOUND display("The location cannot be found."); RETURN_TO_EDITOR
#define NO_INTEREST_IN_LOCATION_DB display("You have not yet populated the interests of the selected location."); RETURN_TO_EDITOR

void edit_viewMap(game *g, int loc_id){
	location *cLoc = getLocation(g, loc_id);
	LongString message;
	if (!cLoc){
		sprintf(message,"Invalid location [%s]", loc_id);
		frame(message);
		display_("The location could not be found in the database.");
		RETURN_TO_EDITOR;
	}
	sprintf(message,"Currently viewing location [%s]", cLoc->location_name);
	frame(message);
	
	sprintf(message,"Name: %s", cLoc->location_name);
	display(message);
	
	sprintf(message,"Description: %s", cLoc->description);
	display(message);
	
	sprintf(message,"Where Description: %s", cLoc->where);
	display(message); 
	
	display_("Mob spawns:");
	viewMobSpawnList(cLoc->mob); NL;
	
	display_("Items attached to location:");
	viewItemListOfLocation(g, cLoc); NL;
	
	display_("Interest points found:");
	viewInterestList(cLoc->interests); NL;
	
	sprintf(message, "%s is connected to the following maps: ", cLoc->location_name);
	display_(message);
	viewConnections(g, cLoc->location_id); NL;
	}

void edit_createMap(game *g){
	int location_id;
	String name, pre_choice;
	LongString description;
	LongString where;
	Boolean err = true;
	char choice;
	location *last_location;
	do{
		frame("Map creation");
		display("You are now creating a new location.");
		
		display_("Please input name of location: ");
		grabs(name, 32);
		
		display_("Please input description of location: ");
		grabs(description, 128);
		
		display_("Please input string of 'where' description of connections of the location: ");
		grabs(where, 128);
		
		
		
		do{
			CLEAN_SCREEN;
			frame("Map creation");
			display_("Name: ");
			display_(name); NL;
			
			display_("Description: ");
			display_(description); NL;
			
			display_("Connections: ");
			display_(where); NL;
			
			display_("Is this data correct? [yes or no]");
			
			grabs(pre_choice, 10);
			strtolower(pre_choice);
		}while(strcmp(pre_choice, "yes") && strcmp(pre_choice, "no"));
	
	
		if (!strcmp(pre_choice, "no")){
			RETURN_TO_EDITOR;
		}else if (!strcmp(pre_choice, "yes"))
			err = false;
		
	}while(err);
	
	last_location = getLastLocation(g);
	if (last_location)
		location_id = last_location->location_id + 1;
	
	construct_location(g, location_id, name, description, where);
	display_("The location is now created.");
}

void edit_connectMaps(game *g){
	int from, dest;
	location *current = g->loc;
	LongString message;
	String choice;
	
	frame("Map connection");
	display_("Please select two maps to connect with each other.");
	display_("Please note that the two maps will allow access from both points (biconditionally).");
	display("Map list: ");
	
	sprintf(message, "%-5s %s", "ID", "Name of Location");
	display(message);
	while(current != NULL){
		sprintf(message, "%-5d %s", current->location_id, current->location_name); 
		display_(message);
		current = current->next_loc;
	}
	NL;

	display_("Please input first location ID.");
	from = grabi(choice, 10); NL;
	display_("Please input second location ID.");
	dest = grabi(choice, 10); NL;

	if (!dest || !from){
		display_("You input an invalid location.");
		RETURN_TO_EDITOR;
	}
	
	sprintf(message, "Are you sure you wish to connect [%s] to [%s]? [yes or no]", getLocationName(g, from), getLocationName(g, dest));
	display_(message);
	grabs(choice, 10);
	if (!strcmp(choice,"yes")){
		connectLocation(g, from, dest);
		connectLocation(g, dest, from);
		display_("The two locations are now connected.");
		return;
	}
	display("You will now return to the map editor.");
}

void edit_removeInterestPoint(location *loc, char* interest){

}

void edit_plantItemToInterestPoint(game *g, location *cLoc){
	int item_id, amount, loc_id, decision = 1;
	String interest_point_name, item_name, choice;
	LongString message;
	inventory *inv = NULL;
	interest *interest = NULL;
	itemdb *itemdb = g->itemdb;
	Boolean notSelected, repeat = true;
	
	/* Selecting what item and amount */
	frame("Attaching item to interest point");
	
	if (!itemdb){
		NO_LOCATION_DB;
	}
	
	display_("Please select an item you wish to attach."); NL;
	viewItemDatabase(g);
	notSelected = true;
	do{
		display_("Please enter the item id of the item you wish to attach: ");
		item_id = grabi(message, 10);

		display_("Please enter the amount of the item you wish to attach: ");
		amount = grabi(message, 10);

		if (inv = spawnItem(g, item_id, DEFAULT_DURABILITY, amount)){
			notSelected = false;
			strcpy(item_name, getItemName(g, inv->item_id));
		}else{
			display_("You input an incorrect item id or amount.");
			RETURN_TO_EDITOR;
		}
	}while(notSelected);

	if (!g->loc){
		NO_LOCATION_DB;
	}
	
	interest = cLoc->interests;
	
	if (!interest){
		display("You have not yet populated the location database.");
		return;
	}
	
	/* Selecting which interest point */
	display_("Please select an interest point found in that location: "); NL;
	viewInterestList(interest);
	notSelected = true;
	
	do{
		display_("Please enter the exact name of the interest point: ");
		grabs(interest_point_name, 32);
		if (interest = findInterestPoint(g, cLoc, interest_point_name))
			notSelected = false;
		else
			display_("Incorrect interest point name given.");
	}while(notSelected);

	notSelected = true;
	do{
		CLEAN_SCREEN;
		sprintf(message, "Current items attached to interest point [%s]", interest->name);
		frame(message);
		viewItemListOfInterest(g, interest); NL;
		frame("Spawning new item to interest point");
		sprintf(message, "Item Name: %s", item_name);
		display(message);
		sprintf(message, "Item Amount: %d", amount);
		display(message);
		sprintf(message, "Attach to interest point [%s] of location [%s]", interest->name, cLoc->location_name);
		display(message); 
		
		display_("Is the above data correct? [yes or no]");
		grabs(message, 10); NL;
		strtolower(message);
		
		if (!strcmp(message, "no")){
			decision = 0;
			display("You will now return to the map editor.");
			return;
		}else if (!strcmp(message, "yes")){
			display_("The item was successfully attached to the interest point."); NL;
			notSelected = false;
		}
	}while(notSelected);
	
	if (decision)
		attachItemToInterestPoint(g, inv, interest);
	return;
}

void edit_createInterest(game *g, location *cLoc){
	LongString interest_description;
	String interest_name, choice, message;
	int item_req, isDiscovered;
	interest *current;
	
	frame("Creating a new interest point");
	display_("Please input name of new interest point:");
	grabs(interest_name, 32); NL;
	
	display_("Please input description of new interest point:");
	grabs(interest_description, 128);NL;
	
	display_("Please select item requirement: "); 
	display("You may leave this blank."); 
	viewItemDatabase(g);
	item_req = grabi(choice, 10);NL;
	
	if (item_req > 0 && !itemIsValid(g, item_req)){
		display_("The selected required item is not valid.");
		return;
	}
	
	display_("Is this interest point, by default, explored? [yes or no]"); 
	grabs(choice, 10); NL;
	strtolower(choice);
	if (!strcmp(choice, "yes"))
		isDiscovered = 1;
	else
		isDiscovered = 0;
	
	
	sprintf(message, "Are you sure you want to create interest point [%s] in location [%s]? [yes or no]", interest_name, cLoc->location_name);
	display(message);
	grabs(choice, 10);
	if (!strcmp(choice,"yes")){
		current = construct_interest(g, cLoc->location_id, interest_name, interest_description, item_req, isDiscovered);
		display("The interest point was successfully attached to the location.");
	}else{
		display("You will now be returned to the interest point editor.");
	}
}

void edit_interestPoint(game *g){
	LongString message, interest_description;
	String choice, interest_name, item_req_s;
	int loc_id, item_req;
	location *cLoc = NULL;
	Boolean keepEditing = true;
	interest *current;
	for(;;){
		if (!cLoc){
			frame("Interest point manipulation");
			display_("Leave blank if you wish to return to the Developer Menu."); 
			display_("Please select the location whose interests you wish to edit:"); NL;
			viewLocationDatabase(g);
			loc_id = grabi(choice, 10);
			
			cLoc = getLocation(g, loc_id);
			
			if (!cLoc){
				LOCATION_NOT_FOUND;
			}
			CLEAN_SCREEN;
		}
		
		frame("Interest point manipulation");
		display_("Below are the current interest points found in the location."); NL;
		viewInterestList(cLoc->interests); NL;
		
		display_("[add], [remove], [edit], or [view] interest point");
		display_("[plant] item on interest point");
		display_("[map] selection");
		display("[exit] to map editor");
		display_("What do you want to do?"); 
		grabs(choice, 10);
		strtolower(choice);
		
		
		if (!strcmp(choice,"exit")) {
			RETURN_TO_EDITOR;
		}else if (!strcmp(choice,"add")) {
			CLEAN_SCREEN;
			edit_createInterest(g, cLoc);
		}else if (!strcmp(choice,"edit")) {
			display_("Which interest point would you like to edit? (interest name)");
			grabs(interest_name, 32);
			if (current = findInterestPoint(g, cLoc, interest_name)){
				CLEAN_SCREEN;
				frame("Current interest point information");
				sprintf(message, "Name: %s", current->name);
				display(message); 
				sprintf(message, "Description: %s", current->description);
				display(message); 
				sprintf(message, "Item requirement: %s", (getItemName(g, current->item_requirement)) ? getItemName(g, current->item_requirement) : "None");
				display(message); 
				
				if (current->next_item){
					display_("Current items attached to interest point: "); NL;
					viewItemListOfInterest(g, current); NL;
				}
				frame("New interest point information");
				display_("Please input name of new interest point:");
				grabs(interest_name, 32); NL;
				
				display_("Please input description of new interest point:");
				grabs(interest_description, 128); NL;
				
				display_("Please select item requirement: "); 
				display_("You may leave this blank if you wish."); NL;
				viewItemDatabase(g);
				
				item_req = grabi(item_req_s, 10);
				
				if (item_req > 0 && !itemIsValid(g, item_req)){
					display_("The selected required item is not valid.");
					SPACE; PAUSE;
					continue;
				}
				
				CLEAN_SCREEN;
				frame("Current interest point information");
				sprintf(message, "Previous Name: %s", current->name);
				display_(message); 
				sprintf(message, "New Name: %s", interest_name);
				display(message); 
				sprintf(message, "Previous Description: %s", current->description);
				display_(message); 
				sprintf(message, "New Description: %s", interest_description);
				display(message); 
				sprintf(message, "Previous Item requirement: %s", (getItemName(g, current->item_requirement)) ? getItemName(g, current->item_requirement) : "None");
				display_(message); 
				sprintf(message, "New Item requirement: %s", (getItemName(g, item_req)) ? getItemName(g, item_req) : "None");
				display(message); NL;
				
				display_("Do you wish to update the interest point? [yes, no]");
				grabs(choice, 10);
				if (!strcmp(choice,"yes")){
					display_("The interest point was successfully edited.");
					edit_interest(current, interest_name, interest_description, item_req);
				}else{
					display("You will now return to the inerest point menu.");
					SPACE; PAUSE;
					continue;
				}
				
			}else{
				display_("The interest point could not be found.");
			}
			
		}else if (!strcmp(choice,"remove")) {
			
			CLEAN_SCREEN;
			frame("Interest point removal");
			display("This feature is not yet implemented.");
			
		}else if (!strcmp(choice,"view")) {
			display_("Which interest point would you like to edit? (interest name)");
			grabs(interest_name, 32);
			if (current = findInterestPoint(g, cLoc, interest_name)){
				
				CLEAN_SCREEN;
				frame("Interest point details");
				sprintf(message, "Name: %s", current->name);
				display(message); 
				sprintf(message, "Description: %s", current->description);
				display(message); 
				sprintf(message, "Item requirement: %s", (getItemName(g, current->item_requirement)) ? getItemName(g, current->item_requirement) : "None");
				display(message); 
				sprintf(message, "By default, this interest point %s discovered.", (current->isDiscovered) ? "is":"is not");
				display(message); 
				
				if (current->next_item){
					display_("Current items attached to interest point: "); NL;
					viewItemListOfInterest(g, current); NL;
				}
			}else{
				display_("The interest point could not be found.");
			}
			
		}else if (!strcmp(choice,"plant")){
			CLEAN_SCREEN;
			edit_plantItemToInterestPoint(g, cLoc);
		}else if (!strcmp(choice,"map")) {
			cLoc = NULL;
			CLEAN_SCREEN;
			continue;
		}else{
			display_("Unknown command.");
		}
		SPACE; PAUSE;
	}
}

void edit_itemToMap(game *g, location *cLoc){
	String choice;
	LongString message;
	int item_id, amt;
	sprintf(message, "New item in %s", cLoc->location_name);
	frame(message);
	viewItemDatabase(g);
	display_("Please enter the item ID you wish to attach:");
	item_id = grabi(choice, 10);
	
	display_("Please enter amount:");
	amt = grabi(choice, 10);
	CLEAN_SCREEN; NL;
	frame("Confirm item spawn");
	sprintf(message, "Are you sure you wish to spawn a %s in %s?(yes or no)", getItemName(g, item_id), cLoc->location_name);
	display_(message);
	
	grabs(choice, 10);
	strtolower(choice);
	
	if (!strcmp(choice,"yes")){
		attachItemToLocation(g, spawnItem(g, item_id, DEFAULT_DURABILITY, amt), cLoc);
		display("The item was successfully spawned.");
	}else{
		display("Returning to map editor.");
	}
}

void edit_spawnMob(game *g, location *cLoc){
	String choice;
	LongString message;
	mob *cMob = NULL;
	int loc_id = cLoc->location_id, mob_id;
	
	if (!cLoc){
		sprintf(message,"Invalid location.");
		frame(message);
		display_("The selected location could not be found");
		return;
	}
	
	for(;;){
		CLEAN_SCREEN;
		
		sprintf(message, "Creating new mob spawn in location [%s]", cLoc->location_name);
		frame(message);
		display_("Current mob spawns:");
		viewMobSpawnList(cLoc->mob); NL;
		display_("Mob Database:");
		viewMobSpawnList(g->mobdb); NL;
		display_("Please input mob ID of the mob you wish to spawn: ");
		mob_id = grabi(choice, 10); NL;
		
		CLEAN_SCREEN;
		if (!mobIsValid(g, mob_id)){
		frame("Invalid mob spawn");
		display("Cannot spawn mob. Invalid mob ID given.");
			SPACE; PAUSE;
			continue;
		}
		frame("Confirm mob spawn");
		display("You may type [exit] to end mob spawning in this location.");
		sprintf(message, "Are you sure you wish to spawn mob [%s] in location [%s]? [yes or no]", getMobName(g, mob_id), getLocationName(g, loc_id));
		display(message);
		grabs(choice, 10);
		if (!strcmp(choice,"yes")){
			display_("The mob spawn was successfully applied.");
			construct_mob_spawn(g, loc_id, mob_id);
			
			return;
		}else if (!strcmp(choice,"exit"))
			return;
	}
}

void edit_location(game *g){
	String choice, map_name;
	LongString map_description, map_where, message;
	int loc_id;
	location *cLoc;
	for(;;){
		frame("Map manipulation");
		viewLocationDatabase(g); NL;
		
		display_("[create] a new map");
		display_("[view] map");
		display_("[edit] map");
		display_("[bridge] connection of two maps"); 
		display_("[spawn] a mob in a location"); 
		display_("[plant] an item in a location");
		display("[exit] to Developer Menu"); 
		
		display_("What would you like to do?"); 
		grabs(choice, 10); NL;
		if(!strcmp(choice,"create"))
			edit_createMap(g);
		else if (!strcmp(choice,"view")){
			display_("Which map would you like to view? [location id]");
			loc_id = grabi(choice, 32);
			if (!locationIsValid(g, loc_id)){
				display("Invalid location selected.");
			}else{
				CLEAN_SCREEN;
				edit_viewMap(g, loc_id);
			}
			
		}else if(!strcmp(choice,"bridge")){
				CLEAN_SCREEN;
				edit_connectMaps(g);
		}else if(!strcmp(choice,"plant")){
				display_("Which map would you like to plant an item in? [location id]");
				loc_id = grabi(choice, 32);
				if (!locationIsValid(g, loc_id)){
					display("Invalid location selected.");
				}else{
					CLEAN_SCREEN;
					edit_itemToMap(g, getLocation(g, loc_id));
				}
		}else if(!strcmp(choice,"edit")){
				display_("Which map would you like to edit? [location id]");
				loc_id = grabi(choice, 32);
				cLoc = getLocation(g, loc_id);
				if (!cLoc){
					sprintf(message, "The given location id was not found in the database.");
					display(message);
				}else{
					CLEAN_SCREEN;
					frame("Current map information");
					sprintf(message, "Name: %s", cLoc->location_name);
					display(message);
					sprintf(message, "Description: %s", cLoc->description);
					display(message);
					sprintf(message, "Where: %s", cLoc->where);
					display(message);
					display_("Mob spawns:");
					viewMobSpawnList(cLoc->mob); NL;
					display_("Interest points:");
					viewInterestList(cLoc->interests); NL;
					frame("New map information");
					display_("Please input new map name: ");
					grabs(map_name, 32); NL;
					display_("Please input new map descrption: ");
					grabs(map_description, 32); NL;
					display_("Please input new map where: ");
					grabs(map_where, 32); NL;
					
					
					CLEAN_SCREEN;
					frame("Confirm map changes");
					display_("Please confirm if you wish to apply the following changes.");
					sprintf(message, "Old name: %s", cLoc->location_name);
					display_(message);
					sprintf(message, "New name: %s", map_name);
					display(message); NL;
					sprintf(message, "Old description: %s", cLoc->description);
					display(message);
					sprintf(message, "New description: %s", map_description);
					display(message);
					sprintf(message, "Old where: %s", cLoc->where);
					display(message); 
					sprintf(message, "New where: %s", map_where);
					display(message); NL;
					
					
					display_("Do you wish to apply the changes? [yes or no]");
					grabs(choice, 10); NL;
					if (!strcmp(choice,"yes")){
						strcpy(cLoc->location_name,map_name);
						strcpy(cLoc->description,map_description);
						strcpy(cLoc->where,map_where);
						display("The map was successfully updated.");
					}else{
						display("Returning to map editor.");
					}
				}
		}else if(!strcmp(choice,"spawn")){
				display_("What map do you want to spawn a mob in? [location id]");
				loc_id = grabi(choice, 5);
				cLoc = getLocation(g, loc_id);
				if (!cLoc){
					display("Invalid location selected.");
				}else{
					CLEAN_SCREEN;
					edit_spawnMob(g, cLoc);
				}
		
		}else if (!strcmp(choice,"exit"))
			break;
		
		SPACE; PAUSE;
	}
}

void edit_createItem(game *g){
	String choice, item_name;
	LongString item_description;
	Text message;
	int item_id, damage, default_durability;

	frame("Current item database");
	viewItemDatabase(g); NL;
	SPACE; PAUSE;
	
	frame("Creating a new item structure");
	display_("Please enter new item ID:");
	item_id = grabi(choice, 5); NL;
	if (itemIsValid(g, item_id)){
		sprintf(message, "The selected item ID is already used by item [%s].", getItemName(g, item_id));
		display(message);
		SPACE; PAUSE;
		return;
	}
	display_("Please enter new item name:");
	grabs(item_name, 32); NL;
	
	display_("Please enter new item description:");
	grabs(item_description, 128);NL;
	
	display_("Please enter item's damage:");
	damage = grabi(choice, 5);NL;
	
	display_("Please enter item's default durability:");
	default_durability = grabi(choice, 5);NL;
	
	CLEAN_SCREEN;
	frame("Confirming item structure creation");
	sprintf(message, "Item ID: %d", item_id);
	display(message);
	sprintf(message, "Item Name: %s", item_name);
	display(message);
	sprintf(message, "Description: %s", item_description);
	display(message);
	sprintf(message, "Damage: %d", damage);
	display(message);
	sprintf(message, "Default durability: %d", default_durability);
	display(message); 
	
	display("Do you wish to create the new item structure? [yes or no]");
	grabs(choice, 10);
	
	if (!strcmp(choice,"yes")){
		display("Item was successfully created.");
		construct_item(g, item_id, item_name, item_description, damage, default_durability);
		/* Sort after creation */
		SPACE; PAUSE;
		return;
	}else{
		display("You will now return to the developer menu.");
		SPACE; PAUSE;
		return;
	}
}

void edit_createMob(game *g){
	String choice, mob_name;
	LongString mob_description;
	Text message;
	int mob_id, damage, HP, weakness, immune, rate;
	
	
	frame("Current mob database");
	viewMobSpawnList(g->mobdb); NL;
	SPACE; PAUSE;

	frame("Creating a new mob structure");
	display_("Please enter new mob ID:");
	mob_id = grabi(choice, 5); NL;
	if (!mob_id){
		display("Please input a valid mob id.");
		SPACE; PAUSE;
		return;
	}else if (mobIsValid(g, mob_id)){
		sprintf(message, "The selected mob ID is already used by mob [%s].", getMobName(g, mob_id));
		display(message);
		SPACE; PAUSE;
		return;
	}
	display_("Please enter new mob name:");
	grabs(mob_name, 32); NL;
	
	display_("Please enter new mob description:");
	grabs(mob_description, 128);NL;
	
	display_("Please enter the new mob's damage:");
	damage = grabi(choice, 5);NL;
	
	viewItemDatabase(g);NL;	
	
	display_("Please enter monster's (item) weakness (leave blank to ignore): ");
	weakness = grabi(choice, 5);NL;
	
	display_("Please enter monster's (item) immunity (leave blank to ignore): ");
	immune = grabi(choice, 5);NL;
	
	display_("Please enter monster's rate of encounter (100 = 1%, 10000 = 100%): ");
	rate = grabi(choice, 10);NL;
	
	
	if (weakness && !itemIsValid(g, weakness)){
		sprintf(message, "The selected mob weakness was invalid. item [%d].", weakness);
		display(message);
		SPACE; PAUSE;
		return;
	}else{
		weakness = -1;
	}
	
	if (immune && !itemIsValid(g, immune)){
		sprintf(message, "The selected mob immunity was invalid. item [%d].", immune);
		display(message);
		SPACE; PAUSE;
		return;
	}else{
		immune = -1;
	}
	
	if(rate < 0 || rate > 10000){
		sprintf(message, "The selected mob encounter rate was invalid. [%.2f%%].", rate / 100.00);
		display(message);
		SPACE; PAUSE;
		return;
	}
	
	
	
	CLEAN_SCREEN;
	frame("Confirming mob structure creation");
	sprintf(message, "Mob ID: %d", mob_id);
	display(message);
	sprintf(message, "Mob Name: %s", mob_name);
	display(message);
	sprintf(message, "Description: %s", mob_description);
	display(message);
	sprintf(message, "Damage: %d", damage);
	display(message);
	sprintf(message, "Weakness: %s", getItemName(g, weakness));
	display_(message);
	sprintf(message, "Immunity: %s", getItemName(g, immune));
	display_(message);
	sprintf(message, "Rate of encounter: %.2f%%", rate / 100.0);
	display(message); 
	
	display_("Do you wish to create the new mob structure? [yes or no]");
	grabs(choice, 10);
	
	if (!strcmp(choice,"yes")){
		display("Mob was successfully created.");
		construct_mob(g, mob_id, mob_name, mob_description, HP, damage, weakness, immune, rate);
		SPACE; PAUSE;
		return;
	}else{
		display("You will now return to the developer menu.");
		SPACE; PAUSE;
		return;
	}
}

void edit_export(game *g){
	frame("Exporting current database");
	display("This feature is not yet implemented.");
	SPACE; PAUSE;
}

void edit_mode(game *g){
	String choice;
	Boolean keepEditing = true;
	
	do{
		system("CLS");
		frame("Developer Menu");
		display("This developer menu allows you create new locations, new interest points, new items, new mobs and many more. This feature allows you to edit something beforehand and experience it in the Apocalypse engine.");
		display("What would you like to do?");
		display_("[map] database manpulation"); 
		display_("[interest] point database manipulation");
		display_("[item] database manipulation");
		display_("[mob] database manipulation"); 
		display("[export] game database"); 
		display("[exit] dev mode"); 
		
		grabs(choice, 10);
		strtolower(choice);
		
		if (!strcmp(choice,"map")){
			CLEAN_SCREEN;
			edit_location(g);
		} else if (!strcmp(choice,"interest")){
			CLEAN_SCREEN;
			edit_interestPoint(g);
		} else if (!strcmp(choice,"item")){
			CLEAN_SCREEN;
			edit_createItem(g);
		} else if (!strcmp(choice,"mob")){
			CLEAN_SCREEN;
			edit_createMob(g);
		} else if (!strcmp(choice,"export")){
			CLEAN_SCREEN;
			edit_export(g);
		} else if (!strcmp(choice,"exit")){
			keepEditing = false;
			display("Thank you for using the Game Editor mode.");
		}
	}while(keepEditing);
	SPACE; PAUSE;
}
