TODO.MD 

# Todos For Qt Solitaire. 

**Improvements - Appearance**

  * Make New Game chain together the following operations:
    * Collect cards, 
    * Shuffle 
    * Deal  
 * Animation/Appearance
   * When dragging stack show entire stack
   * Animate moving cards
   * Fix shuffle animation (Qt Bug? Try Windows)
  * Visibly Enable the undo/redo items based on the undo stack.  
    * Will require keeping a pointer to them, unless maybe I can change those to 
      inherit or own the QActions?
  * Show entire stack of cards when draging on playfield-to-playfield
     * Mime Image created in Card::MouseMoveEvent (only current card) 
     * DragPlayfieldToPlayfieldCommand - calls TakeCards(*mDroppedCard)
        * happens at the point of the drop. 
     * So, currently the point where the drag is initiated does not know if we are on stack. 

**Features**

  * Add scoring.
    * Write scoring spec
    * implement scoring
    
  * **Gamification** 
    * Write specifications
    * initial run shows options (Form?)
    * Number of games per sitting (1-5)
    * Number of sittings per day (1-4)
    * Quit after first when. 
    * If program is opened after that, displays a message: Next game in xx hours (03:00pm)    
  * Add zoom for game.
  * Write specifications for the animations. 

* Modify drawing to reference cards from a single vector art file 
* Allow selection of multiple card art decks.



# Ugly things to improve 
 1. Revisit incrementing class enums (testValue in SortedStack)
 1. Revisit qulonglong (non-portable) in QDataStream opeator++ in card.cpp


# DONE  
 * Highlight cards when mouseover 
 * change to class enums, add [iterator code from stack overflow](https://stackoverflow.com/questions/261963/how-can-i-iterate-over-an-enum)
 * Revisit the scaling of everything, and the boundary rects.  Try to understand it better. 


# DEPLOYMENT 
* Licensing See [Stack Overflow] Does App Store accept Qt Linkede App LGPLv3](https://stackoverflow.com/questions/35068054/does-app-store-accept-qt-app-linked-with-qt-library-lgplv3)
* Technical: App Store see [Uploading a Python App to the Mac App Store(October 22](https://github.com/nyavramov/python_app_mac_app_store)
   * [Notes on Apple Developer Forum]()
* OSX: Generate a dpkg?  Code signing.
* Windows: generate an installer- scott wild easy installer? Still viable? 