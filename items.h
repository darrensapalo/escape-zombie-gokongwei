itemdb* itemBefore(itemdb* item_db, itemdb* current){
	if (item_db == current)
		return NULL;
	while(item_db){
		if(item_db->next_item == current)
			return item_db;
		item_db = item_db->next_item;
	}
	return NULL;
}

itemdb* sortIntoItemDB(itemdb *item_db, itemdb *toBeSorted){
	int item_id = toBeSorted->item_id;
	itemdb *current = item_db;
	itemdb *before_element = itemBefore(item_db, toBeSorted);
	itemdb *after_element = toBeSorted->next_item;
	
	/* Remove -toBeSorted- from the linked list */
	if (before_element)
		before_element->next_item = after_element;
	
	/* Starting from the beginning find out if the item's 
	 * item ID is greater than the item you wish to
	 * stop loop then re-insert into the database */
	while(current){
		if (current->item_id > item_id)
			break;
		current = current->next_item;
	}
	
	before_element = itemBefore(item_db, current);
	
    if (before_element){
		after_element = before_element->next_item;
		before_element->next_item = toBeSorted;
		toBeSorted->next_item = after_element;
	}else{
		toBeSorted->next_item = item_db;
		item_db = toBeSorted;
	}
	
	return item_db;
}

itemdb* sortItemDatabase(game *g){
	itemdb *item_db = g->itemdb;
	itemdb *current = item_db;
	
	while(current){
		item_db = sortIntoItemDB(item_db, current);
		current = current->next_item;
	}
	g->itemdb = item_db;
	return item_db;
}

inventory* toInvEnd(inventory* start){

	inventory* current = start;
	
	while(current->next_item != NULL){
	current = current->next_item;
	}
	return current;
}

int isInInventory(game *g, int item_id){
	inventory *inv = g->pl->inv;
	while(inv){
		if (inv->item_id == item_id)
			return true;
		inv = inv->next_item;
	}
	
	return false;
}

int isWeapon(game *g, int item_id){
	itemdb *item_db = g->itemdb;
	
	while(item_db){
		if (item_db->item_id == item_id)
			break;
		item_db = item_db->next_item;
	}
	if (item_db->damage > 0)
		return true;
	
	return false;
}

inventory* addToInventory(game *g, inventory *item){
	/* To the end of the inventory */
	inventory *cInv = g->pl->inv;
	if (!cInv){
         g->pl->inv = item;
		return item;
	}
	
	
	while(cInv->next_item){
		/* Found a same item in your inventory. Add amount and durability */
		if (cInv->item_id == item->item_id){
			cInv->amount += item->amount;
			cInv->durability += item->durability * item->amount;
			free(item);
			return cInv;
		}
		cInv = cInv->next_item;
	}
	item->isDiscovered = true;
	cInv->next_item = item;

	return item;
}

inventory* invAddAfter(inventory* start, inventory* item){
	inventory *current;
	
	/* Save previous end of item */
	current = start->next_item;
	
	/* Set next item to currently new made item */
	start->next_item = item;
	
	
	/* Point new item to the previously added item */
	item->next_item = current;
	
	return item;
}

char* getItemDescription(game *g, int item_id){
	itemdb *item_db = g->itemdb;
	do{
		if (item_db->item_id == item_id)break;
	}while(item_db->next_item != NULL);
	
	return item_db->description;
}

char* getItemName(game *g, int item_id){
	Boolean err = true;
	itemdb *current = g->itemdb;
	
	while(current){
		if (current->item_id == item_id)
			return current->item_name;
		else
			current = current->next_item;
		}
	return "None";
}

int getItemDurability(game *g, int item_id){
	itemdb *item_db = g->itemdb;
	while(item_db){
		if (item_db->item_id == item_id)
			return item_db->default_durability;
		item_db = item_db->next_item;
		}
	return 0;
}

int getInvDurability(game *g, int item_id){
	inventory *cInv = g->pl->inv;
	int dur = 0;
	while(cInv){
		if (cInv->item_id == item_id)
			if (cInv->durability > 0)
				return cInv->durability;
		
		cInv = cInv->next_item;
	}
	return 0;
}

int getItemDamage(game *g, int item_id){
	itemdb *item_db = g->itemdb;
	while(item_db){
		if (item_db->item_id == item_id)
			return item_db->damage;
		item_db = item_db->next_item;
	}
	return 0;
}

int itemIsValid(game*g, int item_id){
	itemdb *current = g->itemdb;
	while(current){
		if (current->item_id == item_id)
			return 1;
		current = current->next_item;
	}
	return 0;
}

inventory* spawnItem(game *g, int item_id, int item_durability, int amount){
	/* Create new item */
	
	if (item_id <= 0 || amount <= 0)
		return NULL;
	
	if (!itemIsValid(g, item_id))
		return NULL;
	
	inventory *newInventory;
	if ((newInventory = malloc(sizeof(inventory))) == NULL){
		p("There was not enough memory to create a new inventory instance.");
		SPACE; PAUSE;
		exit(1);
	}
	newInventory->amount = amount;
	newInventory->item_id = item_id;
	newInventory->isDiscovered = false;

	if (item_durability > 0)
		newInventory->durability = item_durability;
	else
		newInventory->durability = getItemDurability(g, item_id);
	
	newInventory->next_item = NULL;
		
	return newInventory;
}

/*
typedef struct inventoryStructure {
	int item_id;
	int durability;
	LongString description;
	struct inventoryStructure *next_item;
}inventory;


typedef struct itemDatabase {
	int item_id;
	String item_name;
	LongString description;
	int default_durability;
	struct itemDatabase *next_item;
}itemdb;


*/

int hasItems(game *g){
	inventory *inv = g->pl->inv;
	if (inv == NULL || inv->item_id < 0)
		return false;
	else
		return true;
}

void displayInventory(game *g){
	inventory *inv = g->pl->inv;
	Boolean view_inventory = true;
	
	frame("Viewing inventory...");
	
	if (!hasItems(g)){
		SPACE; p("You currently have no items."); NL; NL;
		view_inventory = false;
	}else{
		SPACE; p("%-3s %-7s %-32s %-12s %-9s", "ID", "Amount", "Item Name", "Durability", "Damage"); NL;
		while (view_inventory){
			SPACE; p("%-3d %-7d %-32s %-12d %-9d", inv->item_id, inv->amount, getItemName(g, inv->item_id), inv->durability, getItemDamage(g, inv->item_id)); NL;
			if (inv->next_item == NULL)
				view_inventory = false;
			else
				inv = inv->next_item;
		}
		NL;
	}
}


inventory* getInventory(game *g, int item_id){
	inventory *cInv = g->pl->inv;
	while(cInv){
		if(cInv->item_id == item_id)
			return cInv;
		cInv = cInv->next_item;
	}
	return cInv;
}

inventory* selectWeapon(game *g){
	itemdb *item_db = g->itemdb;
	inventory *inv = g->pl->inv;
	String choice;
	int item_id;
	Boolean err = true;

	/* Does not have items, therefore hand to hand combat */

	/* Show list of items */
	displayInventory(g);
	
	do{
		display_("Leave blank to use your hands.");
		display_("What weapon would you like to use? (item id)"); 
		item_id = grabi(choice, 5); NL;
		
		inv = getInventory(g, item_id);
		
		if (!hasItems(g) || !item_id){
			CLEAN_SCREEN;
			frame("Open-hand combat. Practicing kung-fu...");
			display("You decide to use your fists. You will have 50% chance to deal 1 damage to any zombie. "); 
			g->pl->weapon = 0;
			return NULL;
		}
		
		if (!isInInventory(g, item_id)){
			display("The item is not found on your inventory."); 
		}else if (!isWeapon(g, item_id)){
			display("The item is not considered a weapon."); 
		}else if(getInvDurability(g, item_id) <= 0){
			display("The item has been worn out."); 
		}else{
			err = false;
		}

	}while(err);
	return getInventory(g, item_id);
}

void viewItemDatabase(game *g){
	itemdb *itemdb = g->itemdb;
	
	SPACE; p("%-8s %-6s", "Item ID", "Name"); NL;
	if (!itemdb)
		display_("There were no items in the item database.");

	while(itemdb){
		SPACE; p("%-8d %-6s", itemdb->item_id, itemdb->item_name); NL;
		itemdb = itemdb->next_item;
	}
}
