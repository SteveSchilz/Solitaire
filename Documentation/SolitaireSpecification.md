# Solitaire Game Specifications 

This file describes program specifications for the desktop version of a standard Solitaire game implmented using the Qt Graphics View Framework


# Solitaire Rules: 
**Card Deck**
A standard deck of cards is used, having the following properties:

  * Cards have one of two colors { Red, Black } 
  * There are four "suits"(categories) of cards, { Hearts, Diamonds, Spades, Clovers}.
    * Hearts and Diamonds are red
    * Spades and Clovers are black
  * Each suit has thirteen cards of increasing value, named as: { Ace, 2...10, Jack, Queen, King}
  * Each card has a unique face image representing the color, suit, and value. 
  * All cards share the same reverse image.

**Objective** 
The objective of the game is to uncover all of the cards on the playing field, and move them into four sorted stacks, one for each suit, in ascending order.

**Game Start:** A standard deck of 52 cards (no Jokers) is shuffled to be random, and dealt onto the playing field in seven columns, one column at a time.  Each time through the columns the first card dealt is shifted to one column to the right, resulting in column[0] containing 1 card, column[1] containing 2 cards, and so on. 

After the cards are dealt, the remaining cards are placed face down in the "Hand" stack, and then the top card is turned over and placed on the  "Waste Pile".  

There are four empty slots where cards of each suit will be collected in order from Ace through King.  

**Game Play:**
User can drag any face up card and drop it on one of the available stacks on the playfield, or the sorted stack area, subject to the following rules. 

a) Sorted Stacks:  Only the next higher card of the same suit is allowed, starting from Ace and progressing through 2, 3, 4... up to the King.
b) Alternating (Playfield) Stacks Only the next lower card of an opposite color of the top card is allowed, with King being the highest and Ace the lowest.

After the completion of the move, the score is updated, and a check to see if the game has ended is performed. 

To begin the next move, the user can click on the face down stack to move the top card to the face up stack (turning it over).

After the last faceDown card has been moved, the user can click on the "faceUp" stack to flip it over back to the face down stack.

**Game Ending**
The game can have either a "Win" or "Loss" outcome, and the game will display a Modal Dialog indicating the status.   The dialog has two options "Quit", or "Deal" to restart the game.

**Win:**
The game is won when any of the following are true:
a) All cards in the playfield are uncovered.
b) All cards have been placed onto the "Sorted By Suit" stacks. 

**Loss**
The game is lost when:

* No legal moves are detected during two cycles of the "faceDown" cards.
