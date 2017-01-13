I want it to store what pitches they tend to pitch our guys at certain counts and predict
Stores it and prints the percentage of the pitch pitched
Ex.
----------------------------------	
	
	MAIN MENU

	1) New Game
	2) List Stats
	3) Search Player Stats
	*4) Remove Player
	*5) Add Player
	6) Quit

----------------------------------

	NEW GAME

	1) Enter Lineup
	2) Select Batter
	3) Back to Main Menu

----------------------------------

	SELECT BATTER: 

	At Bat: #12 CF Johnny Lewis 
	Prediction: Highly Likely, Likely, Fairly Likely to see this pitch at this count (printing the return value of the function)
	Suggestion: Look for this pitch on this count (printing the return value of the function)

	1) Pitch
	2) Result
	3) Back

----------------------------------

	Pitch

	Enter Count?
	0-0
	Pitch?
	1

	Select:
	1) Next pitch
	2) Result

----------------------------------

	Next pitch -> back to the pitch screen (screen above)

----------------------------------
	Result

	Hit = 1, Walk = 2, or Out = 3?
	(User Input)

	If 1,
		hit counter on that pitch and count ++ and at bat counter ++
		batter array ++

	If 2, 
		batter array++
	
	If 3,
		at bat counter ++
		batter array ++

----------------------------------


	Santa Clara Broncos

	At Bat: Johnny Lewis
	Pitch Percentage on 0-0 count:
		Fastball: 89%
		Curveball: 9%
		Slider: 1%
		Change Up: 0%

	Batting Percentage on 0-0 count:
		Fastball: .333 (1/3)
		Curveball: .166 (1/6)
		Slider: .200 (1/5)
		Change Up: .000	(0/0) 

	Prediction: Fastball

------------------------------------
What is the criteria for a prediction?

* Tracks that team and overall trends that teams are pitching this batter

* Recognizes patterns:
	second time up the pitcher has started the inning or second batter with curveball

Storage Option:
	Place for the teams signs.
	Place for the lineup
		Array of pointers to central node of each player
			!PROBLEMS WITH BENCH PLAYERS!
	Players - pointers pointing to each player
		each player has a node list (log of games)
		central node with "career stats"
			Need percetnage for each pitch on each at bat and each count
				list of struct nodes about 6 of them for each at bat
					inside:
						Player name
						2D array:

							vertical Ptiches
							horizonal count
								0-0 = 0
								0-1 = 1
								0-2 = 2
								1-0 = 3
								1-1 = 4
								1-2 = 5
								2-0 = 6
								2-1 = 7
								2-2 = 8
								3-0 = 9
								3-1 = 10
								3-2 = 11

								If ball = 0
									array position = strike
								if ball = 1 
									array position = 3+strike
								if ball = 2
									array position = 6+strike
								if ball = 3
									array position = 9+strike
								if ball = 4
									gets out and goes to main menu new batter option

							If pitch:
								then fill colunm with zeros then put 1 in the pitch


						Check with [0][blah] of game isnt empty then put into central node
	


			How do you get this?
				End game? Yes
				Automatically puts the last games stats into central node
				Or update as you go..?!?!
		create a new node each game
			Struct nodes:
				String opponent
				create 2d array for game with each at bat (verical) and put pitch (horizontal) into the array (1 fast, 2 curve...)


Flags for current games:
	During game flag = 1
	End game? Yes ... flag 0
	Want to create when flag = 0? Sir would you like to start a new game? yes.. flag = 1


Functions:

	Create Lineup: Array of pointers
	Create Each player: cenral node
	Create game
		Auto for all players

IN THE SELECT BATTER SWITCH ********
-----------------------------------------------------

	*** Select Batter ***

	Batting: CF #2 Smith

	Prediction: Smith is likely to see a 0-2 curveball and 2-1 fastball (75% probability).

	Suggestion: Look for a fastball on 0-0 because you are batting .667 on that pitch.

	ENTER PITCH THROWN
	
	Suggestion Function:
		Search for the max of suggestion value (pitch percentage)




Revisions / Updates:
	- Take into account the total of the pitches when finding predictions
	- Location of the pitch	
		new array with counts and pitch
		inside the array:
			1 = top away
			2 = top middle
			3 = top inside
			4 = belt away
			5 = down the pipe
			6 = middle in
			7 = low away
			8 = low middle
			9 = low in
	- Export to excel document & Import excel documents of the opponent
		export to .txt and go from .txt to .xls
		







