This project is the MacOS client of F-OCR, you can launch it with your own IDE by selecting the .pro file.

Explanation for some source files:
qtdependency.h is where Qt libs needed for this project are listed. 

imageLoader is the main window of this program.Where you can either open the picture for process through system resource manager, or by built-in screenshot or by dragging file onto the window.

imageProcessor is the dialog to process the picture you just load. Where you can display the binaryzed picture by clicking Binaryzation button or transform it into LaTex script by clicking Translate button(It will do the binaryzation automatically). The script will show up on the textfield below.

Functions for dividing characters form pixmap are located in Matrix. Recognization is added to identify each character divided from the whole picture. Then the logical relation part will transform the character locations and their types into corresponding LaTex script.