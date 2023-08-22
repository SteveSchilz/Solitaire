Qt Solitaire

Opening Vector Art:
    Inkscape -> open file -> vectorstock_38016577.ai -> Cairo Import_

# Date: 


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
       
    