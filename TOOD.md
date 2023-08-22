TODO.MD 

# Todos For Qt Solitaire. 

 1. Write specifications for the animations and stuff. 
 1. Investigate Unit Testing thinking through stuff. 
 
 1. Later: Deploy on Mac and Windows
    * OSX: Generate a dpkg?  Code signing may be very difficult these days
 1. Windows: generate an installer? 
 1. Handle multiple card decks.
 
# Ugly things to improve 
 1. Revisit incrementing class enums (testValue in SortedStack)
 1. Revisit qulonglong (non-portable) in QDataStream opeator++ in card.cpp
 
 #
# DONE  
 * change to class enums, add [iterator code from stack overflow](https://stackoverflow.com/questions/261963/how-can-i-iterate-over-an-enum)
 * Revisit the scaling of everything, and the boundary rects.  Try to understand it better. 


# DEPLOYMENT 
 * Licensing See [Stack Overflow] Does App Store accept Qt Linkede App LGPLv3](https://stackoverflow.com/questions/35068054/does-app-store-accept-qt-app-linked-with-qt-library-lgplv3)
 * Technical: App Store see [Uploading a Python App to the Mac App Store(October 22](https://github.com/nyavramov/python_app_mac_app_store)
   * [Notes on Apple Developer Forum]()