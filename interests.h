void edit_interest(interest* interest, char* name, char* description, int item_req){
	strcpy(interest->name, name);
	strcpy(interest->description, description);
	interest->item_requirement = item_req;
}

void viewInterestList(interest* interest){
	SPACE; p("%-20s %-45s", "Name", "Short Description"); NL;
	if (!interest)
		display_("There were no interest points found.");
	
	while(interest){
		SPACE; p("%-20s %-.45s...", interest->name, interest->description); NL;
		interest = interest->next_interest;
	}
}

void viewItemListOfInterest(game *g, interest* interest){
	inventory *current = interest->next_item;
	String message, item_name;
	SPACE; p("%-4s %-8s %-32s", "ID", "Amount", "Item Name"); NL;
	if (!current)
		display_("There were no items attached to this interest point.");
	
	while(current){
		sprintf(message, "%-4d %-8d %-32s", current->item_id, current->amount, getItemName(g, current->item_id)); 
		display_(message);
		current = current->next_item;
	}
}

interest* getInterest(game *g, int interest_id){
	int i = 0;
	interest *current;
	if (g->interests == NULL)
		return NULL;
	
	current = (interest*) g->interests;
		
	while(i < interest_id){
		current = current->next_interest;
		i++;
	}
	return current;
}

void viewInterests(game *g){
	location *cLocation;
	interest *cInterest;
	inventory *cInventory;
	cLocation = g->loc;
	if (cLocation != NULL)
		cInterest = cLocation->interests;
	else
		display_("There were no maps found in the map database.");
	
	while(cLocation != NULL){
		p("Now in location: %s whose interest is %p", cLocation->location_name, cLocation->interests); NL;
		cInterest = cLocation->interests;
		while(cInterest != NULL){
			p(          "Now at interest point: %s : %p", cInterest->name, cInterest); NL;
			cInventory = cInterest->next_item;
			while(cInventory != NULL){
				p("The current item (%s) is attached to [%s] found at [%s].", getItemName(g, cInventory->item_id), cInterest->name, cLocation->location_name); NL;
				cInventory = cInventory->next_item;
			}
			cInterest = cInterest->next_interest;
		}
		cLocation = cLocation->next_loc;
	}
}

interest* findInterestPoint(game *g, location *loc, char *param){
	interest *current_interest = loc->interests;
	Boolean interest_point_found = false;
	
	while(current_interest != NULL && interest_point_found == false){
		if (!strcmp(param, current_interest->name))
			break;
		current_interest = current_interest->next_interest;
	}
	
	if (current_interest != NULL){
		if (current_interest->item_requirement == UNDEFINED)
			return current_interest;
	}
	return current_interest;
}

int getInterestPointRequirement(interest *interestPoint){
	if (interestPoint != NULL)
		return interestPoint->item_requirement;
	return 0;
}

void interestPointRequires(game *g, interest *current_interest){
	LongString message;
	if (current_interest->item_requirement > 0){
		sprintf(message, "The interest point '%s' requires '%s'.", current_interest->name, getItemName(g, current_interest->item_requirement));
		display(message); 
	}
}

Boolean canViewInterestPoint(game *g, interest *current_interest){
	if (current_interest->item_requirement <= 0)
		return true;
	else if (!(isInInventory(g, current_interest->item_requirement)))
		return false;
	else
		return true;
}

int exploreInterestPoint(game *g, interest *interest){
	inventory *cInventory = interest->next_item;
	while(cInventory != NULL){
		cInventory->isDiscovered = true;
		cInventory = cInventory->next_item;
	}
	return 1;
}

int itemToInventory(game *g, char* param){
	location *cLoc = getLocation(g, g->pl->location);
	interest *interest;
	inventory *cInventory, *cNext, *cPrev = NULL;
	int item_id;
	String message;
	if (cLoc != NULL)
		interest = cLoc->interests;
	else
		return 0;

	cInventory = cLoc->items;
	while(cInventory){
		if (cInventory->isDiscovered){
			if(!strcmp(getItemName(g, cInventory->item_id),param)){
				/* Found the item */
				
				/* Get next interest */
				cNext = cInventory->next_item;
				
				/* Get the previous interest, unless it was the first */
				if (cPrev)
					cPrev->next_item = cNext;
				else
					interest->next_item = cNext;
				
				item_id = cInventory->item_id;
				
				addToInventory(g, cInventory);
				sprintf(message, "Item '%s' added to your inventory.", getItemName(g, item_id));
				display_(message);
				return 1;
			}
		}
		cPrev = cInventory;
		cInventory = cInventory->next_item;
	}
		
	while(interest){
		cInventory = interest->next_item;
		while(cInventory){
			if (cInventory->isDiscovered){
				if(!strcmp(getItemName(g, cInventory->item_id),param)){
					/* Found the item */
					
					/* Get next interest */
					cNext = cInventory->next_item;
					
					/* Get the previous interest, unless it was the first */
					if (cPrev)
						cPrev->next_item = cNext;
					else
						interest->next_item = cNext;
					
					item_id = cInventory->item_id;
					
					addToInventory(g, cInventory);
					sprintf(message, "Item '%s' added to your inventory.", getItemName(g, item_id));
					display_(message);
					return 1;
				}
			}
			cPrev = cInventory;
			cInventory = cInventory->next_item;
		}
		interest = interest->next_interest;
	}
	return 0;
}

int attachItemToInterestPoint(game *g, inventory *inv, interest *interest){
	inventory *current;

	assert(inv != NULL);
	assert(interest != NULL);
	
	current = interest->next_item;
	
	
	if (current == NULL){
		interest->next_item = inv;
		return 1;
	}else{
		while(current->next_item != NULL){
			current = current->next_item;
		}
		current->next_item = inv;
		return 1;
	}
	
	if (ITEM_TO_INTEREST){
		SPACE; p("item [%s] added to interest point [%s].", getItemName(g, inv->item_id), interest->name); NL;
	}
		
	return 0;
}

void attachInterestPointToLocation(game *g, interest *cInterest, location *loc){
	interest *current = loc->interests;
	
	if (current == NULL){
			loc->interests = cInterest;
	}else{
		while(current->next_interest != NULL){
			current = current->next_interest;
		}
		current->next_interest = cInterest;
	}
	
	if (INTEREST_TO_LOCATION){
		SPACE; p("interest point [%s] added to location [%s].", cInterest->name, loc->location_name); NL;
	}
}
