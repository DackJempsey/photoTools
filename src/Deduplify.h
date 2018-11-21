//#ifndef Deduplify
#define Deduplify

using namespace std;

class Hash 
{ 
	
    int BUCKET;    // No. of buckets 
      
    list<string> *table;

    public: 
		Hash(int key);  // Constructor 
		vector <int> max;//max number of duplicates, is vector if all have the same number of duplicates 
		vector <string> maxName; //name of most duplicated
		int files; //total files
		int unqFiles; //total unique fiels
		int actFiles; //total actual files in tree after deduplication
		string ldname;//llargest directory name
		int check_largestDir; 
		int largestDir;//number of files within the largest dir
		//anymore stats?
		int check_deepest;
		int deepest;//how deep does the tree go?
		int key;//is the key of the table
		// inserts a key into hash table 
		int insertItem(string x, Hash *h); 
		// hash function to map values to key 
		int hashFunc(string photo, Hash *h);
  
}; 

//finds directory within given directory and returns a vector of 
//the numbers which corespond to the directory numbers in the given directory
vector <string> findDirs(string Dirname, string newDir, Hash *h);
//find directories in a directory
//void traverse(string dir,string newDirs,Hash *h);
//is given directory name and 
void traverseRec(char *files, string newDir,Hash *h);//does traersal recursively

