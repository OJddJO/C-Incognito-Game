# C-Incognito-Game

## Knights and Spy
Each player has one pawn (and only one) marked on its hidden side with a sign of allegiance to their castle. This pawn is referred to as the spy. The other N - 1 pawns of the player are knights who have pledged allegiance to the same lord.

## Objective of the Game
The objective of the game is to get your spy into the opponent's castle or to discover the enemy spy.

## Starting Position
At the beginning of the game, the board contains N black pawns and N white pawns placed in the opposite corners of the board, respectively protecting the castles. Players do not know the initial position of their spy or the opponent's spy. For this reason, each player's spy pawn is chosen at random from among their five pawns.

## Game Progression
The first player to play is chosen at random. Each player, in turn, can choose between one (and only one) of the two types of moves:
- move a piece (and only one) in any of the eight directions, straight or diagonal, as many squares as desired, without being able to jump over a piece whether it is friendly or enemy (this is the movement of a queen in chess). A piece also cannot move to an already occupied square.
- interrogate an enemy piece. This is only possible when two opposing pieces are orthogonally adjacent (for example a2-b2 or a2-a3, but not a2-b1). The player then interrogates the enemy piece with one of their own pieces. Several cases then arise:
  - if the interrogated piece is the opponent's spy, the questioning player wins;
  - if the interrogated piece is not the spy, the questioning player loses the piece with which they interrogated. If the lost piece was the player's spy, they lose the game.

This interrogation between two adjacent pieces is not mandatory and can only be done instead of moving a piece. A player cannot move one of their pieces into their own castle. If a player manages to get a piece that is not the spy into the opponent's castle, that piece is lost. Furthermore, one can only enter a castle in an orthogonal line (thus from one of the two sides to which the corner belongs).

## End of the Game
Two reasons can determine the winner.
1. A spy has been unmasked:
   - if a spy interrogates an opposing piece that is not a spy, then the questioning player loses.
   - if a piece interrogates an opposing piece that is a spy, then the questioning player wins.
2. A castle has been taken:
   - A player who manages to get their spy into the opponent's castle is declared the winner.
