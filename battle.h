/*
 * simulateFieldCombat
 *		Returns mob ID (int) of the monster that you encountered. Will return 0 if there is no encounter.
 */
int simulateFieldCombat(location *loc){
	int chance;
	mob *current_mob = loc->mob;
	LongString message;
	
	if (!MOB_ENCOUNTER)
		return 0;
	
	chance = rand_(10000);
	while(current_mob != NULL){
		if (current_mob->rate > chance){
			system("CLS");
			NL; frame("A zombie appeared!");
			if (MOB_CHANCE){
				sprintf(message, "Safety chance: %.2f%% below the rate of encounter %.2f%%.", chance / 100.0, current_mob->rate / 100.0);
				display_(message);
			}
			sprintf(message, "You were ambushed by a %s.", current_mob->mob_name);
			display_(message);
			return current_mob->mob_id;
		}
		if (MOB_CHANCE){
			sprintf(message, "Current mob %s of rate %.2f %%.", current_mob->mob_name, current_mob->rate / 100.0); 
			SPACE; p(message); NL;
		}
		
		current_mob = current_mob->next_mob;
	}
	if (MOB_CHANCE)
		NL;
	
	
	
	return 0;
}

inventory *weaponSelection(game *g){
	player *pl = g->pl;
	inventory *weapon;
	weapon = selectWeapon(g);
	LongString message;
	if (weapon != NULL){
		pl->weapon = weapon->item_id;
		pl->weapon_durability = weapon->durability;

		sprintf(message, "You chose to use your %s.", getItemName(g, weapon->item_id));
		display_(message); 
		
	}else{
		display("Let's hope you know some hand-to-hand combat!"); 
		return NULL;
	}
	return weapon;
}


void battle(game *g, int mob_id){

	/* Weapon data */
	inventory *weapon;
	
	/* Monster data */
	mob *monster = spawnMonster(g, mob_id);
	
	/* Player data */
	player *pl = g->pl;

	/* Frame data */
	LongString message;

	/* Battle flags */
	Boolean triumph = false;
	Boolean hand_to_hand = false;

	/* Damage assessment per turn */
	int player_weapon;
	int player_dps;
	int mob_dps;
	sprintf(message, "You prepare to fight %s with currently %3d%% HP left!", getMobName(g, mob_id), pl->HP);
	SPACE; speak(message); NL; NL;
	SPACE; PAUSE;
	pl = g->pl;
	weapon = weaponSelection(g);
	/* Select a weapon phase */
	
	if (weapon == NULL)
		hand_to_hand = true;

		/* If the target zombie is weak against your weapon, then you win automatically. */
	if (monster->weakness == pl->weapon){
		triumph = true;
		SPACE; speak("Your weapon was super effective against the zombie."); NL; NL;
	}

	/* If the target zombie is immune to your weapon, then your damage is halfed.
	 * Pray that your weapon damage is at least two, or else you'll be like 
	 * hitting the enemy with air (integral division).
	 */
	if (monster->immune == pl->weapon){
		player_dps /= 2;
		SPACE; speak("Your weapon seems weak against this kind of zombie."); NL; NL;
	}

	player_weapon = pl->weapon;
	player_dps = getItemDamage(g, pl->weapon);
	
	SPACE; PAUSE; 
	sprintf(message,"Battle against %s", monster->mob_name);
	frame(message);
	mob_dps = getMobDamage(g, mob_id);
	
	
	/* As long as nobody has won yet, and at least one of you is alive */
	while(!triumph && (pl->HP > 0 && monster->HP > 0) ){
	
		if (hand_to_hand){
			player_dps = rand() % 2;
			if (player_dps == 0)
				strcpy(message, "You threw sissy punches that hit nothing but air.");
			else
				sprintf(message, "You dealt %d damage.", player_dps);
		}else{
			sprintf(message, "You dealt %d damage.", player_dps);
		}
	
		/* Player damages enemy first always */
		monster->HP -= player_dps;
		
		SPACE; speak(message); NL;
		/* If monster is still alive, then get damaged in this turn */
		if (monster->HP > 0){
			pl->HP -= mob_dps;
			sprintf(message, "The zombie dealt %d damage.", mob_dps);
			SPACE; speak(message); NL;
			}
	}
	/* You are still standing */
	if (pl->HP > 0){
		sprintf(message, "You were victorious. You were left with %d%% HP left.", pl->HP);
		SPACE; speak(message); NL; NL;
		
	/* Zombie is left standing */
	}else if (monster->HP > 0){
		SPACE; speak("You unfortunately died."); NL;
	}
	
	/* Decrease weapon durability */
	pl->weapon_durability -= 1;
	
	if (weapon != NULL)
		weapon->durability -= 1;
	
	free(monster);
	SPACE; PAUSE;
}
