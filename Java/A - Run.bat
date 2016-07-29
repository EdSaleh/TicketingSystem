@echo off
setlocal EnableDelayedExpansion

cd ..
copy /Y CPlusPlus\EventsFile.txt Java\EventsFile.txt
copy /Y CPlusPlus\TransactionFile.txt Java\TransactionFile.txt
copy /Y CPlusPlus\UsersFile.txt Java\UsersFile.txt
cd Java

cls

javac BackEnd.java
java BackEnd