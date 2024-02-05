This is the final working screenshotting module. 
It runs in two threads that work sychnronously to capture the actions of the user, the mouse clicks and the window screenshots.


To compile the code install mingw-x64 from this link: 

https://www.mingw-w64.org/downloads/#winlibscom 
ie https://winlibs.com/

Once installed, run this code at the location of main.cpp to compile the exe file. 
```
g++ main.cpp -o done.exe -lgdiplus -luser32 -lgdi32 -static-libgcc -static-libstdc++ -mwindows -mconsole
```
Ensure that the folder "Counter_location", "Keyboard", "Screenshots", "Meta" and "status.txt" are on the same level as the compiled exe. 

remove the  -mconsole flag to stop the terminal from displaying
