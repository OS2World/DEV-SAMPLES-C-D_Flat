 - DFlat++ V3 for OS/2          - 05dec93

This is the second release of an OS/2 port of DFlat++. The enhancements are 
thanks to Jim Riblett [71774,21], and greatly speed up screen painting by 
writing directly to the Logical Video Buffer. A latent bug in the textbox 
was also fixed.

The modified files since the last relese are:

 screen.cpp    screen.h
 paint.cpp
 textbox.cpp   textbox.h
 dfwindow.h

Thanks again to Jim for doing a fine job.


---------------------  Previous release notes ------------------------------

 - DFlat++ V3 for OS/2          - 20nov93

This is the first release of an OS/2 port of DFlat++ I've been working on. It 
compiles with the Borland C++ for OS/2 1.0 tools.

The source files which have needed major modifications are:

  cursor.cpp       cursor.h
  keyboard.cpp     keyboard.h
  mouse.cpp        mouse.h
  screen.cpp       screen.h
  speaker.cpp      speaker.h
  timer.cpp        timer.h    (renamed dtimer.cpp etc)

popdown.cpp and strings.cpp also required minor fixes which caused 
protection violations.

I will be documenting the port in a forthcoming DDJ article, and would be 
interested in any comments anyone has about the code.

Regards,  Jon [71333,2641]
