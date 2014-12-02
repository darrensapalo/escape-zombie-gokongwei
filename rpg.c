/*0
 * Darren Sapalo, S20A, 11127821
 * Current Progress: 85.00%
 *
 * Do the following: 
 *			Login accounts for playing
 *			Interest points that are not yet discovered are not... yet discovered? why did you do this?
 *
 *		Map Editor Development
 *			Do not allow user to view interest points that are not yet explored... (future me: so how do you explore it then?)
 *
 *		Gameplay Development
 *			Um. Populate the game first.
 *			
 *		Story Line Development
 *			What happened in La Salle? There is a zombie outbreak right now. Contagion? Some bacteria grew from the chemistry lab.
 *
 *		Database Development
 *			Populate database with items and mobs
 *			Populate locations with mobs
 *			Populate locations with interest points
 *
 *		Follow up To Do List
 *			Sorter for databases (location, item, mob)
 *			Saving game to binary file
 *			Populate random chat messages
 *			Populate random commands
 *
 */
 
/* Debugging constants */
#define DEBUG (0)
/* Allow mob encounter? */
#define MOB_ENCOUNTER (1)

#if DEBUG == 1
	#define MOB_CHANCE (1)
	#define MOB_SPAWN_DATA (1)
	#define LOCATION_CREATION (1)
	#define MOB_CREATION (1)
	#define ITEM_CREATION (1)
	#define LOCATION_CONNECTION (1)
	#define ITEM_TO_INTEREST (1)
	#define INTEREST_TO_LOCATION (1)
	#define VIEW_ALL_INTERESTS (0)
	#define SCREEN_FAST (1)
#else
	#define MOB_CHANCE (0)
	#define MOB_SPAWN_DATA (0)
	#define LOCATION_CREATION (0)
	#define MOB_CREATION (0)
	#define ITEM_CREATION (0)
	#define LOCATION_CONNECTION (0)
	#define ITEM_TO_INTEREST (0)
	#define INTEREST_TO_LOCATION (0)
	#define VIEW_ALL_INTERESTS (0)
	#define SCREEN_FAST (0)
#endif

/* Speaking speed of the computer */
#define SPEAK_SPEED 0

#include "init.h"
int main(){
	int gameStatus;
	/* Initialize srand */
	srand(time(NULL));
	prepareGameScreen();

	/* Initialize the game */
	game *g;

	/* Initialize file-saved database */
	g = initGame();

	/* Menu */
	   if (gameMenu(g)){
		
		startGame(g); /* Tutorial */
		
		switch(playGame(g)){ /* Play the game itself */
			case -1: /* You died */
				gameOver(g);
			break;
			
			case 1: /* You won */
				gameSuccess(g);
			break;
		}
	}
	
	/* End of Program */
	quitGame();
    return 0;
}

