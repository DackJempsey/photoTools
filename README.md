Written by Jack Dempsey:Jack.n.Dempsey@colorado.edu
There are three sections to this README: 

Usage, Descriptions, and Implementation decisions.

# Usage 
Dependancies: g++ 4.2.1 or higher 
OS: These programs have been made to work on Linux(Debian/Ubuntu), and also work on macOS(High Sierra)
This program is written in c++, induced in the src directory is a Makefile
to compile both programs enter:
`
$make clean
`
then
`
$make
`
This is will compile both Deduplify and jsonBlob.
These are the two programs for the challenge.

To run Deduplify enter:
```
$./Deduplify.exe /full/path/to/old/duplified/dir /full/path/to/new/dir/name
```

Where /path/to/old/duplified/dir is the full path of the directory with duplified files, and /path/new/dir/name is the full path to the new directory. All directories in that path must exist except the last one(name) which will be created by Deduplify, and can be named anything.

If the directory structure has more than about 500 unique files then you may want to add a third argument that is a large prime(1009, 2741, 3559).
e.g. $./Deduplify.exe /path/to/old/duplified/dir /path/to/new/dir/name 2741

To run jsonBlob enter:
```
$./jsonBlob.exe  picture.jpg
```

Where picture.jpg is the name of a JPEG that you would like json metadata from.

# Description

## DESCRIPTION of Deduplify:
This program takes in two arguments, a path to a directory structure that has duplicate pictures within it, and creates a new directory structure that looks the same, however each duplicate picture is only seen once. Along with creating a new directory it will give you statistics on the directory structure. This file is called Deduplify_stats.txt, and can be found in the new directory. 
Included in Deduplify_stats.txt is:
-The picture with the most duplicates, and number of repeats in the directories, if many had the same max number then all are listed.
-Total files before Deduplication.
-Number of unique Images
-Total Actual Files after Deduplication(includes all file types)
-The directory with the most pictures, and the number of pictures
-How many directories deep the structure is


## DESCRIPTION of jsonBlob:
This program takes a fully qualified filename of a JPEG image and returns a json file of the pictures metadata. This file will be filename.json, and will be in the src directory. This code is dependent on the exif program, which has been included and is compiled with the make command. The exif is an open library used for parsing EXIF data within a JPEG image. The usage is covered under the BSD license for both personal and commercial usage.

# Implementation Decisions
Here I will talk about some of the decisions that I made for each program.

## Deduplify:
At the core of this program is tree traversal, and I decided to use a depth first, in-order traversal for this. I did this because I knew that I would need to visit every directory in the structure, and to search through every directory, so I knew that I would not get any faster than a O(n) time(and I tried to make sure to not add anymore to that). When I say in-order I mean the order of the director pointer assigned by the opendir() function, included in the dirent.h library. This implementation is created by the union between The traverseRec() and finders() functions. 

traverseRec() function takes 3 arguments: a pointer to a character which takes the current directory name, a string which takes the new directory name, and a pointer to an implementation of the Hash class which I created. Description of the Hash class below. This function does not return anything.

The findDirs function takes the same three arguments as traverseRec, however it returns a vector of strings, which is all of the directory names within the directory that it was given. 
This function essentially handles all of the contents of each directory, and does all the heavy lifting. If it is a directory then it adds it to the new directory structure. If it is a jpg photo then it uses a hash map to check if it is a duplicate or not and add it to the complimentary directory in the new structure. If it is a different type of file it will also add it to the complimentary directory so that other information is not lost. 

## Hash Description:
I decided to use a self made hash-map implementation with this program so that I can run through the old directory structure once, and keep the big-O time down to O(n). With a hash-map you can check with the use of a key in constant time, if the photo has already been added before. If the photo was already in the bucket then I would add it the chain, and in this way I could find how many repeats each photo had. If there is a collision of keys, but the photos are different then the program adds some randomness to that name and puts it in a different bucket. I chose a hash function of: multiplying each character in the photo name string by the golden ratio:((sqrt(5) +1)/2) and summing them up, then modulus by the hash key(521, or larger if given), which is a large prime to avoid unnecessary collisions. If the key is too small then the program asks for a larger key as a third argument, and provides some examples of good large primes, so that they key is still a prime. I decided to create my own Hash class for the hash map so that I can also easily keep track of all of the statistics without having to run through the directory structure anymore times. Within the Hash class there are all the spaces to keep the statistics.


## jsonBlob:
For this program I was lucky enough to find a library which parses all of the data within a jpg photo. This program easyexid: https://github.com/mayanklahiri/easyexif was created by Mayank Lahiri with the BSD open usage license for personal and commercial use. Finding this library speed up my research immensely. I knew that I wanted to access all of the data within a JPEG photo because there is so much there to unpack, however I do not know the exif standards and would have had to read and parse all the data myself. Instead I could use this library and then just transfer the data to the json format and write it all to a file. 


## Makefile:
I decided to use a makefile then I could make sure that the user compiled the dependent libraries in the right order. I have always found that when a codebase has a Makefile that it is so much easier for me to get going, instead of worrying about all the compiling problems. Then once the user makes the programs they can immediately use either one. 


Testing:
I did 95% of the coding on my personal laptop which runs macOS-High Sierra and g++4.2.1. So I knew that I should test on a linux system, although macOS I believe is a Unix-like system. I have a couple of raspberry pi's, one that runs Debian(linux), and another that runs Unix(linux). I did my first tests on the Debian system and I ran into some problems. The DIR structure which I use for traversRec() function actually did not have the same information as the macOS counterpart, however it was only the name length data, so I was able to get the same usage with a quick workaround. Then I ran into a problem with how the os dealt with iterators. In my original implementation of the Hash::insertItem() function I was able to use the .front() function for lists, however the way that linux dealt with empty strings, it gave me an error I had never seen before. So to identify where the problem was occurring I used a program called gdb to look at the call stack. Most times with my edits it would compile just fine, but then running the program would bring up either an error or seg-fault. So after some searching the internet I was able to use the .begin() iterator and .end() iterator to do the same check. The check was if the bucket was empty, then add the photo to that bucket. After dealing with that problem the program ran just as it was supposed to, on all three of the testing environments. 





