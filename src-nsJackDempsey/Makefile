#======================================================================================/
#**    Author: Jack Dempsey                                             	           /
#**    Date:   10/19/2018                                                              /
#**    Description: MakeFile for Deduplify and JsonBlob                                /
#**    After running this makefile the user will be able to run Deduplify and
#**    JsonBlob
#======================================================================================/
C++  = g++
FLAGS = -std=c++11
C++VERSION 	= ${C++} -dumpversion

RUN:	Deduplify.o exif.o jsonBlob.o Complete.o
	


Deduplify.o:
	@printf "\n=========================================\n"
	@printf "compiling Deduplify\n "
	@${C++} ${FLAGS} -o Deduplify.exe ${PWD}/Deduplify.cpp

jsonBlob.o:
	@printf "\n=========================================\n"
	@printf "compiling jsonBlob\n "
	@${C++} ${FLAGS} -o jsonBlob.exe ${PWD}/exif.o ${PWD}/jsonBlob.cpp

	
exif.o: exif.cpp
	@printf "\n=========================================\n"
	@printf "compiling exfi\n"
	@${C++} ${FLAGS} -c exif.cpp	
	
Complete.o:
	@printf "\n========COMPILATION COMPLETE=============\n"
	
clean: 
		@rm -f *.o Deduplify.exe jsonBlob.exe  
