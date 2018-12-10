/*
 * Deduplify.cpp
 * Author: Jack Dempsey
 * Date: 10/14/2018
 * Description: This program will take in at least two arguements
 * The source of a directory of pictures and a name for the destination
 * of a new directory to house a copy of the source, except each 
 * photo that appears more than once has been made to only apear once.
 * 
 * In the end you will have a new directory and a file containing statistics
 * on your source and new directories.
 * 
 * If the function seems to fail, then three arguements can be given 
 * first two are the same, and the third is a large prime number.
 * 
 */


#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <vector>
#include <unistd.h>
#include <list>
#include <math.h>
#include <fstream>
#include <string.h>
#include "Deduplify.h"

using namespace std;


//vector<vector<string>> hTable;
int Hash::hashFunc(string photo, Hash *h){//key should be 521
	//need to add file expanse, that is add the hash table then do  modulo+prime
	//dynamical allocation
	int  ret=0;
	double GR = (sqrt(5.0)+1.0)/(2.0);
	for(int i=0;i<photo.length();i++){
		//ret = (ret+photo[i]*2741);
		ret =  (ret+photo[i]*GR);
	}
	return (ret % h->key);
}

Hash::Hash(int  key) 
{ //initialization
	//plus one here, the last box is for a pointer to 
	//the next table if needed to buffer
	Hash::key = key;
    table = new list<string>[key]; 
    Hash::actFiles = 0;
    Hash::deepest = 0;
    Hash::check_deepest =0;
    Hash::files = 0;
    Hash::largestDir = 0;
    Hash::check_largestDir = 0;
    Hash::unqFiles = 0;
} 
Hash::~Hash(){
	delete []table;
	//cout<<"cleaned up if "<<table[0].size()<<" = 0"<<endl;

}



int Hash::insertItem(string photo,Hash *h) { 
	//returns length of chain if greater than 1 than duplicate
    int index = hashFunc(photo, h);
    //getting a wierd error here when front is null
    list<string>::iterator begin = table[index].begin();
//need to figure out how to represent a null iterator
    if(begin == table[index].end()){
		//first and unique entry
		h->files+=1;
		h->unqFiles+=1;
		table[index].push_back(photo);
		return table[index].size();
	}
    else if (*begin != photo){//collision handling, if not same photo
		//first collision so the buffer needs be one
	    for(int i=0;i<photo.length();i++){
		photo[i] = photo[i]*313;//some prime 
	}
		insertItem(photo, h);
	}
	//not unique file 
	h->files+=1;
	table[index].push_back(photo);
	return table[index].size();
}


//this function also searches for photos
vector <string> findDirs(string dirname,string newDir,Hash *h){


	DIR *dr = opendir(&dirname[0]);
	struct dirent *de;  // Pointer for directory entry 

    vector<string> dirs;//heap of directories
    vector<int> keys;
    h->check_largestDir =0;
	while ((de = readdir(dr)) ){ 
		
		//printf("%s type: %hhu \n", de->d_name,de->d_type);//d_type may not work in other systems
		string name =de->d_name;
		int size=name.length();
		string filetype = &de->d_name[size-4];
		
		if(de->d_type == 4 && de->d_name[0] != '.' ){ //is a directory and does not start with .
			string newwestDir = newDir +"/"+string(de->d_name);
			int dirStatus;
			dirStatus = mkdir(&newwestDir[0], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);//
			dirs.push_back(de->d_name);
			
		}
		
		else if(de->d_type == 8 && de->d_name[0] !='.'){ 
			if(filetype == ".jpg"){
				int key = h->hashFunc(&de->d_name[0], h);
				int len = h->insertItem(de->d_name,h);
				if(len >1){
					//is duplicate
					//frist time it hit max has not been allocated 
					if(h->max.front() <  len){
						//if less than want to delete all and make new;
						h->max.clear();
						h->maxName.clear();
						h->max.push_back(len);
						h->maxName.push_back(de->d_name);
					}
					else if(h->max.front() == len){
						//if two pictures have the same max
						h->max.push_back(len);
						h->maxName.push_back(de->d_name);					
					}

				}
				else{//add file to the directory
					string oldDir = dirname+"/"+string(de->d_name);
					string picdir = newDir; 
					string command = "cp "+oldDir+" "+newDir;
					system(&command[0]);
					h->check_largestDir+=1;
					h->actFiles+=1; 
					if(h->largestDir < h->check_largestDir){
						h->largestDir = h->check_largestDir;
						h->ldname = newDir;
					}
				}
			}
			else{//is file however not a jpg, still want to move over to new directory
				//does not check to see if it is a repeat or not
					string oldDir = dirname+"/"+string(de->d_name);
					string picdir = newDir; 
					string command = "cp "+oldDir+" "+newDir;
					system(&command[0]);
					h->actFiles +=1;
			}
		}
	}

	closedir(dr);
	return dirs;
	
}

void traverseRec(char *files, string newDir,Hash *h){
	
	vector <string> dirs = findDirs(files,newDir,h);
	
	if(dirs.size() <1){
		//end of hole, check if max dirs equals 
		if(h->deepest < h->check_deepest){//then this is the deepest 
			h->deepest = h->check_deepest;
			h->check_deepest = 0;//reset the counter
		}
		return;
	}

	//want to go into searching photos here as well
	for(int i =0;i<dirs.size();i++){
		h->check_deepest +=1;
		string holdDir = newDir +"/" +string(&dirs[i][0]);
	
		string oldDir=  string(&files[0])+"/"+string(&dirs[i][0]);

		traverseRec(&oldDir[0], holdDir,h);
	}
	
}

void writeStats(Hash *h, string newDir){
		string fileName = newDir+"/Deduplify_Stats.txt";
		ofstream file(fileName);
		cout<<"\nStats Can be found in Deduplify_Stats.txt in the new directory\n"<<endl;
		file<<"\nListing of the Most duplicates in the Directories:"<<endl;
		for(int i=0;i<h->max.size();i++){
			file<<"Name: "<<h->maxName[i]<<"Size: "<<h->max[i]<<endl;
		}
		file<<"\nTotal Files Before Deduplication: "<<h->files<<endl;
		file<<"\nUnique Images: "<<h->unqFiles<<endl;
		file<<"\nTotal Actual Files after Deduplication: "<<h->actFiles<<endl;
		file<<"\nDirectory with the most pictures: "<<h->ldname<<" with "<<h->largestDir<<endl;
		//anymore stats?
		file<<"\nThe structure is "<<(h->deepest-1)<<" directories deep"<<endl;
		file.close();
}



int main(int argc, char **argv)
{
	if(argc < 3){
		//have erno handling if time permit
		cout<<"Not Enough Arguments, need at least input directory and path to output directory\n";
		return 0;
	}
	else if(argc == 3){
		//if wanted could make the key another user input
		int key = 521;
		Hash h(key);//key should be 521, set in class
		h.max.push_back(0);
		h.maxName.push_back(""); //set these so that first entry is not null
		int dirStatus;
		dirStatus = mkdir(argv[2], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);//makes the directoy out of input 2
		traverseRec(argv[1], argv[2],&h);
		writeStats(&h, argv[2]);
	}
	else if(argc == 4){
		int key = stoi(argv[3]);
		//cout<<key<<endl;
		//int key =3786;
		if(key < 700){
			cout<<"Please enter larger number, here are some big primes to use: 1009, 2741, 3559"<<endl;
			cin>>key;
		}
		Hash h(key);
		h.max.push_back(0);
		h.maxName.push_back(""); //set these so that first entry is not null
		int dirStatus;
		dirStatus = mkdir(argv[2], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);//makes the directoy out of input 2
		traverseRec(argv[1], argv[2],&h);
		//write to stats file here:
		h.actFiles+=1 ;//stats file adds one
		writeStats(&h, argv[2]);
	}
	return 0;
}

/*print tabel:
	for (auto x : table[index]) {
      cout << " --> " << x;
	}
	* */ 
