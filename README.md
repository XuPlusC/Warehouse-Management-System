# Warehouse-Management-System
A warehouse management system, written as the database course design in HUST. Using Microsoft SqlServer as DBMS, and made the GUI on Qt.  

## Repo structure:  
The QtProject folder includes all Qt files(qss, cpp, h, ui). Build.sql is for building and resetting the database in SQLServer.


## Fetures:  

* Support 3 different identities: admin, salesman, buyer
* Salesmans and buyers fill in import/outport form and submit them to the admin, then the admin has the right to pass it or fail it
* Submitted forms are fully editable unless passed
* Using database triggers to automatically generate goods lack log, and update lack log's status
* 3 qss skin files in the repo, change the qss path in the main.cpp to switch skin
