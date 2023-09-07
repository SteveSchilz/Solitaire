Qt Solitaire

Opening Vector Art:
    Inkscape -> open file -> vectorstock_38016577.ai -> Cairo Import_

TODO: 
  * Add more paint text to non-face cards. 
  * Add zoom game? 
   

# Date: 09/07/23 (Thurs)
  * split game constructor up into a bunch of methods.  Passing parameters to these methods
    makes the constructor more readable and will make it more possible to test them, 
    but it required the use of double pointers, so, more complex code overall.  
  * Rename "Suite" to "Suit".  This was a typo in initial design.
    
  
# Date: 09/05/23 (Tues)
  * Add ability to drag stacks of cards between different playfield stacks
  * Add Double-click to move cards to next destination.
     
# Date: 08/31/23 (Thurs)
  * Revamp drop events so we don't need friends
    * new cardStack methods addCard and TakeCard move access to cards inside the stack where
      they belon 
    * rename existing RandomStack->takeCard() -> TakeTop
    * Adding takeCard to sorted stack fixed issues when flipping waste pile over.
  * fixup dragging from RandomStack and SortedStack casses to the DescendingStacks
  * Add "clicked" signal to CardStack, wire up mHand.clicked() to make it recover waste pile
  * Remove unused "Card-Back" image from RandomStack (we draw outlines if empty).
  
# Date: 08/30/23 (Weds)
  * Write a chatGPT prompt to tell it to write a solitaire game, based on the following changes 
    from my design: 
    a) Core Classes are C++ only, no knowledge of Qt
    b) CardStacks ARE stacks (instead of having stacks)
    c) iGui methods are used by core code where needed. 
    
    THe main thing I was hoping to learn was how to do the dependency inversion, and hook
    all that together.  I don't know that I'm much further along 
    
    What i DID learn was:
    * it gave some good advice, but was far from producing a working program
    * dragging stacks of cards involves methods that accept LISTs of cards. 
    * conflict between list and stack doesn't seem to be too much of a problem.
          
# Date: 08/28/23(Tues)
  * Implement click on the mHand pile to flip top card to waste pile 
  * Changed a bunch of card references to pointers in addCard/TakeCard
  * used "Friend Classes" to remove cards from original stack in drop event
    - This is kinda ugly but it works
    - Alternative is probably to serialize list into mime data. 

# Date: 08/28/23(Mon)

 * Implemented "Deal" to spread cards along playfield
   * Cards in mHand need to be faceDown.
 * I realized that I am not dealing with ownership correctly
   * Once I drag a card OUT of a DescendingStack, it never becomes empty again. 
   
 * Changed addCard/TakeCard stuff to use ugly raw pointers. 
 * Looked some more at fanning cards animation - still does not work. 

# Date 08/27/23 (Sun) 
  * Create DescendingStack, initialize playfield with 7 empty stacks.
  * Add ability for card to flip front and back

# Date 08/24/23 (Fri) 
  * Implment Deck::shuffle()
  * Add Deck::fanCards 

# Date 08/23/23    1.0 hr
  * Fix animating back to original position when drop rejected

# Date  08/22/23   6.0 hrs Stub in Deck, Add CardClick, add ClickableTextItem
  * Add Deck object with shuffle and deal methods
  * Add Card::click() signal, connected to Game::onCardClicked()
  * Add ClickableGraphicsTextItem, create three clickable items on bottom of screen 
  
  
    
# Date  08/22/23   4.0 hrs Github, Readme, Game Object, RandomStack Object
  * Put project on GitHub (private, for now)
  * Add initial readme
   * Move instantiation of graphics view objects from mainWindow to new "Game" object
  * Stub in "RandomStack" object to be used for hand and waste pile 
  * Looked up some terminology here
  * [semicolon.com:Solitaire/Rules/Glossary](https://www.semicolon.com/Solitaire/Rules/Glossary.html)
  * [cards.fandom.com-Solitaire_terminology](https://cards.fandom.com/wiki/Solitaire_terminology)
  * Spent a bit of time reading about Deployment
    * [Qt For MacOS - Deployment](https://doc.qt.io/qt-6/macos-deployment.html)
    * Licensing See [Stack Overflow] Does App Store accept Qt Linkede App LGPLv3](https://stackoverflow.com/questions/35068054/does-app-store-accept-qt-app-linked-with-qt-library-lgplv3)
    * Technical: App Store see [Uploading a Python App to the Mac App Store(October 22](https://github.com/nyavramov/python_app_mac_app_store)
    * [Notes on Apple Developer Forum]()
 
# Date: 08/20/23   5.0 hrs Switched to SVG Renderer from QPixmap. 
  * **SVG Images** Found two sets of cards on [VectorStock]
    * [Fantasy Deck](https://www.vectorstock.com/royalty-free-vector/set-playing-cards-printable-poker-game-kit-vector-38016577)
    * [Template Deck](https://www.vectorstock.com/royalty-free-vector/playing-cards-set-template-printable-empty-vector-37595976)
    I notice that both of these are a bunch of cards in a single file, which is odd to me. 
    I spent a bunch of time separating out the face cards from the fantasy deck. 
    
    When I look at the docs for [QGraphicsSvgItem](https://doc.qt.io/qt-6/qgraphicssvgitem.html) I see that there is a way t o
    draw a subset of a big file by calling "setElementId" on the QGraphicsSvgItem.   Interseting. 
    
    * **Resources:**  The resource system does not seem to me to provide path independence. 
    To my thinking you should not see any path info in the source code, only in the resource 
    file.  Maybe I'm not grokking it. 

    * **CMAKE:** Using CMake with Qt is pretty non-obvious.  Other people seem to understand it. 
    How can I learn? RTFM? Manuals? 
    
    * **Unit Testing QT code**
    * Qt is such an all-encompassing system, that I have difficulty seeing how to unit test it. 
    For example, my "Card" class inherits from QGraphicsItem: the UI is really baked in to the
    class.  I saw an example in the Conway game [Michael Abrash-Conway's Game of Life](https://www.phatcode.net/res/224/files/html/ch17/17-01.html) 
    where they nicely split the UI from the game code, and did a bunch of profileing too. 
       
    **Qt Vs StdLib**
    One of my goals is to learn about the standard library, so QVector or std::vector?  
    QQueue vs std::queue?   I think in Qt code you should use Qt data structures..
    
    But you could make the case that you keep any rules engine separate from Qt entirely,
    and thus gain ability to reuse it.   So the stacks and rules could be entirely std C. 
    
       
# Date  08/18/23    3.5 Hours: Initial Development.
 
Began copying code out of "Drag and Drop Robot", converting the "ColorItem" value to 
a card. Notes:

 * tried to use "class Enum" - Iterating required a class from stack overflow. Postponed 
   See [Stack Overflow: How can I iterate over an enum](https://stackoverflow.com/questions/261963/how-can-i-iterate-over-an-enum)
 * Example uses "png" files, want to use SVG, because I think that will show better, and 
   take less space. 
 * Tried to use DALL-E to generate images.  It's total trash. 
 * Got cards to appear as circles with a letter for suite, with an image for j/Q/K. 
 

# Date: 08/16/2023    2.0 hrs Initial Specifications
 * Developed initial set of Solitaire rules and specifications, and a simple class diagram.
 * Did some thinking about which data structures to use as follows:
   * Want to create 52 cards and be able to move them among entities, as opposed to 
     copying them. May be difficult with Qt? 
   * There are several distinct stacks 
     * FaceDown/Faceup Stacks: contains randomly sorted cards, we TAKE only from here 
     * Sorted Stacks:  The four final destination stacks, must be sorted in increasing 
       order from Ace to King
     * AlternatingStacks: Contains both facedown cards and faceup cards. 
       FaceUp cards must be sorted in decreasing order, with alternating colors. 
       
    
