# sqlcpp


====SETUP for VISUAL STUDIO 2017=========
Making it Run with CHAI-3D
1. Copy DBManager.cpp / DBManager.h to the project folder from https://github.com/sandeepzgk/sqlcpp
2. Right Click the application > Click on Add > Add Existing Items and select the DBManager.cpp and DBManager.h files
3. Download SQLITE DLL and def from the link https://sqlite.org/download.html "Precompiled Binaries for Windows" get both x86 and win64 versions.
4. Create SQLITE import library from the downloaded sqlite3.def file. for both 32 and 64 bits (needed for different compiler options for chai3d / vs applications) Use "DEVELOPER COMMAND PROMPT FOR VS"
5. Ideally the the dll's should be extracted to a folder say "c:\sqlite" and then run the command  @@@@  lib /DEF:"C:\sqlite\sqlite-dll-win64\sqlite3.def" /OUT:"c:\sqlite\sqlite3-32.lib" @@@@  in the DEVELOPER COMMAND PROMPT FOR VS (the command is within the @@@@'s). This will generate the 32 bit lib files.
6. Make the 64 bit libs files using /MACHINE toggle like this @@@@ lib /DEF:"C:\sqlite\sqlite-dll-win64\sqlite3.def" /OUT:"c:\sqlite\sqlite3.lib" /MACHINE:X64 @@@@, Ensure the names are different for both the 32 bit and 64 bit.
7. To include sqlite.h in the Vs project download source code for sqlite3 from https://sqlite.org/download.html "sqlite-amalgamation-xxxxxxx.zip"
Extract the zip file and copy sqlite3.h to a directory say for eg:c:\sqlite
also copy generated sqlite3.lib (in step-2) and sqlite3.dll to the same directory.
8. In visual studio in your code add the appropriate folders
9. Set additional include directory to sqlite.h file directory.
for this ,in the Vs solution explorer take project properties and go to.
Configuration Properties–>C++–>General–>set additional directories
set it as the directory in which sqlite3.h copied for eg: "c:\sqlite"
10. Now set sqlite.lib as linker input. for this ,in the Vs solution explorer take project properties and go to.
Configuration Properties–>Linker–>Input and add sqlite3.lib to it. Ensure you have either 32 or 64 bit as you created and named.
11. Set sqlite3.lib library directory as additional library
for this in the Vs solution explorer take project properties and go to.
Configuration Properties–>Linker–>General–>Additional Library Directories
set it , for eg:"c:\sqlite"


======HOW TO USE THIS=========
1. In your main file, include the header file #include "DBManager.h"
2. Create an instance of DBManager by adding 	DBManager dbmanager;
3. There are two writes possible with this class,
3a. dbmanager.writeMainData(rand(), "P1", "A", rand() % 5, rand() % 5, "[1,2,3,4]", "Y");
3b. dbmanager.writeSRData("P1", rand() % 4, rand() % 40);
4. These writes will add values to the two different tables in the database. 

A working example of this code is available at https://github.com/sandeepzgk/chai3d-experiments/tree/SQLIntegration_Test SQLIntegration_Test as a part of the chai3d-experiements repository.


====ADDITIONAL IMPORTANT NOTES=====
* Ensure that you ***DO NOT*** instantiate more than 1 instance of DBManager. This does not have a singleton pattern and WILL cause some issues.
* The filename for the db is test.db which can be changed to anything else
* the initial startup of the code is delayed by two seconds to ensure that the db is ready with tables and everything is set to work
* this uses detachable threads to write data to the db
* the destructor will sleep the main thread to ensure that all data has been written to the tables
* the write call should immediately return control to the main thread, as it has already spawn a write thread to the DB which will kill itself after execution.

=====EXPLORING THE DB========
** Use https://sqlitebrowser.org/ this is a pretty good tool to explore the sqlite database 
** You will see two tables MAINDATA and SRDATA tables that contains the data entered by you.


=====SETUP for UBUNTU====
* sudo apt-get install libsqlite3-dev
* sudo apt-get install sqlite3
* use cmake to build the makefile and make that should get you the executable. 
https://github.com/sandeepzgk/sqlcpp 

