GAME FINAL
	Use the mouse to choose where to place towers
	Left-Click to place a tower at a valid location
	Towers will shoot whenever an enemy gets in range
	Towers have a reload time once they've shot a certain number of bullets
	Enemies will lose health whenever a bullet hits them
	You get points for killing enemies
	
	Press F5 to Save the state of the game
	Press F9 to Load back to the most recent save
	
	Press 1 to play the small map
	Press 2 to play the medium map
	Press 3 to play the large map
	
GameplayElement
	Towers cost score to place.
	Score persists between maps
	Gameplay would be to play small maps, then work way up to larger maps
	If you have less than the starting score when switching to a new map, your score goes to the default
	
	
Polish
	Screen resizes whenever it's dragged
	Score display doesn't need to rebuild VBO each time it changes, all code is in shader
	Pathfinding uses a graph with nodes and edges, allows for custom edge cost calculations
		This was to be used for adding walls, but I never got around to that