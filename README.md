# SnMeta
perpetual exe file to extract periodic screenshots and other metadata for intelliassist

run master.cpp with the flags:

g++ master.cpp -o master.exe -lgdi32 -lgdiplus


How to use this system:
1. use codeblocks
2. snmeta is the folder that has been synchronized with git
3. snmetaa is the folder that actually contains all the code.
4. run main.cpp
5. the counter_location folder saves the current location of the pointer, i don't remember exactly but i think that i need to make a shift by one and consider that as the staring of the thingie. this is a global counter position that everyone will use to index from. just need to ensure that all of the ppl who use it are well synchronized so that when they do use this pointer, it isn't accurate because of terminating the program midway.
6. ignore bin, obj, SnMetaa.cbp, SnMetaa.depend, SnMetaa.layout for now
7. screenshots, stores all the images of the screenshots which are then indexed using the said pointer
8. meta folder contains the metadata of each screenshot that is being grabbed thus mapping them to each other. right now it just has the x and y coordinates one below the other. more to be added later
9. keyboard will probably contain all the keyboard inputs that have been made from the previous timestamp to the current time stamp. 

Stuff left to do:
1. run it in a headless format
2. clip the buffer to only 5 minutes
3. to prevent the irregular termination, need a pipelined system/ interrupt that when triggered, endd the current run and then exits the infinite loop thus ensuring that every stack is perfectly indexed
4. add keyboard key strokes extraction system
5. probably some other metadata extraction module that is left over
6. right now in the git pushes im also pushing the folders like meta, keyboard and screenshots, will remove these later. 
