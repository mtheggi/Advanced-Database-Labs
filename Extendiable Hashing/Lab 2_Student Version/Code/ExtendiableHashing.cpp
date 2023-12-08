
#include "ExtendiableHashing.h"
#include <bitset>
#include <iostream>
#include <cmath>
#include <string>
using namespace std;


void displayNotFound(int key) {
	std::cout << "\t\t\t key:  " << key << " \t value : \t not found" << std::endl;
}

//Display one record entry, please don't change this function
void displayItem(DataItem* dataItem) {
	if (dataItem != 0 && dataItem->valid)
	{
		std::cout << "\t\t\t key: " << std::bitset<8>(dataItem->key) << "\t value:\t" << dataItem->data << std::endl;
	}
	else {
		std::cout << "\t\t\t key:  ============ \t value:\t empty data" << std::endl;
	}
}

void displayBucket(Bucket& currentBucket, string& depths, string& values, int verbose) {
	depths.append(std::to_string(currentBucket.localDepth));
	depths.append(",");
	values.append("[");
	if (verbose)
		std::cout << "\t\tBucket:\t local depth:" << currentBucket.localDepth << std::endl;
	if (currentBucket.currentEntries == 0)
	{
		if (verbose)
			std::cout << "\t\t\tNo Data yet\n";
		for (int i = 0; i < RECORDSPERBUCKET; i++)
		{
			values.append("null");
			values.append(",");
		}
	}
	else {
		for (int i = 0; i < RECORDSPERBUCKET; i++)
		{
			if (currentBucket.dataItem[i].valid)
			{
				values.append(std::to_string(currentBucket.dataItem[i].data));
				values.append(",");
				if (verbose)
					std::cout << "\t\t\t key: " << std::bitset<8>(currentBucket.dataItem[i].key) << "\t value:\t" << currentBucket.dataItem[i].data << std::endl;
			}
			else {
				values.append("null");
				values.append(",");
				if (verbose)
					std::cout << "\t\t\t key:  ============ \t value:\t empty data" << std::endl;
			}

		}
	}
	values.pop_back();
	values.append("]");

}

void displayDirectory(GlobalDirectory& globaldirectory, Bucket& currentBucket, int verbose) {
	std::cout << "Directory:\t global depth:" << globaldirectory.globalDepth << std::endl;
	string values = "(";
	string depths = "(";
	int count = 0;
	//string locations = "(";
	if (globaldirectory.length == 0)
	{
		count++;
		std::cout << "\tNo Directory yet\n";
		displayBucket(currentBucket, depths, values, verbose);
	}
	else {

		for (int i = 0; i < globaldirectory.length; i++)
		{
			if (i == 0)
			{
				count++;
			}
			else {
				if (globaldirectory.entry[i - 1] != globaldirectory.entry[i])
					count++;
			}
			if (verbose)
				std::cout << "\t key: " << std::bitset<8>(i) << "\t value:\t" << globaldirectory.entry[i] << std::endl;
			displayBucket(*globaldirectory.entry[i], depths, values, verbose);
			if (verbose)
				std::cout << "-----------------------------------------------\n\n";
		}
		//values.pop_back();
		depths.pop_back();

	}

	values.append(")");
	depths.append(")");
	std::cout << " buckets:\t" << count << "/" << globaldirectory.length << endl;
	std::cout << "values:\t" << values << endl;
	std::cout << "depths:\t" << depths << endl;
	std::cout << "=========================\n";
	//std::cout << "Press any key to continue\n";
	char t[100];
	//std::cin >> t;


}



//Hashing function and getting directory Index, please don't change this function
int getCurrentHash(int key, int depth) {
	// int hashedKey = (key & MAXKEYVALUE) >> (MAXKEYLENGTH - depth);
	int hashedkey =key & ((1<<depth)-1);	
	return hashedkey;
}

//TODO1: Implement this function, Don't change the interface please
// functionlity: try to insert item into a bucket
// return:   1 if succedded
//			 0 if failed
// input:   currentBucket, dataItem to be inserted (check ExtendiableHashing.h for the content of the file)
// Hint1: don't forget to update currentEntries upon insertion, you will need it later

int insertItemIntoBucket(Bucket& currentBucket, DataItem data)
{
	if (currentBucket.currentEntries == RECORDSPERBUCKET){
		puts("FULL");
		return 0;
	}
		 
	else {

		for (int i = 0; i < RECORDSPERBUCKET; i++) {
			if (currentBucket.dataItem[i].valid == 0) {
				currentBucket.dataItem[i] = data;
				currentBucket.currentEntries= currentBucket.currentEntries + 1 ;
				return 1;
			}
		}
	}
		
}

//TODO 2: Implement this function, Don't change the interface please
// functionlity: look for an item in a bucket using key, if found call displayItem(..), if not found call displayNotFound()
// return:   nothing
// input:   currentBucket, key value to search for
// Hint: use displayNotFound & displayItem functions

void findItemInBucket(Bucket& currentBucket, int key)
{
	for ( int i =0 ;i < RECORDSPERBUCKET ; i++) {
		if ( currentBucket.dataItem[i].valid ==1  && currentBucket.dataItem[i].key == key)
			{
				displayItem(&currentBucket.dataItem[i]);
				return;
			}
	}

	displayNotFound(key);
}


//TODO3: Implement this function, Don't change the interface please
// functionlity: try to Delete item based on a key value from a bucket
// return:   1 if succedded
//			 0 if failed (when does it fail to delete??)
// input:   currentBucket, key to be inserted (check ExtendiableHashing.h)
// Hint1:   don't forget to update currentEntries upon deletion, you will need it later

int deleteItemFromBucket(Bucket& currentBucket, int key)
{
	for (int i = 0; i < RECORDSPERBUCKET ; i++) {
		if (currentBucket.dataItem[i].valid == 1 && currentBucket.dataItem[i].key == key)
		{
			currentBucket.dataItem[i].valid = 0; // delete 
			currentBucket.currentEntries= currentBucket.currentEntries - 1 ; // update currentEntries 
			return 1; // success
		}
	}
	return 0;
}

//TODO4: Implement this function, Don't change the interface please
// functionlity: try to insert item in the file, if the bucket is full, extend the directory,
//				  if you extended the directory five times but it still doesn't work, return 0 
// return:   1 if succedded
//			 0 if failed (when does it fail to insert??)
// input:   data:--> dataitem to be inserted, currentBucket --> initialBucket before creating director , globaldirectory
// Hint: use insertItemIntoBucket,getCurrentHash, extendDirectory functions
// Hint1:   don't forget to check for corner cases, for example if several entries points to the same bucket and you are going to split it
// Hint2:   a bucket could be split without expanding the directory (think when this will happen?)
// Hint3:   don't forget to delete extra data

void SplitBucket(GlobalDirectory& globaldirectory , int splitIndex){
	int localdepth = globaldirectory.entry[splitIndex]->localDepth; // number of bits;  
	int newlocaldepth = localdepth + 1;
	globaldirectory.entry[splitIndex]->localDepth = newlocaldepth; // update the local depth of the bucket
	int firstIndx = 0 ;
	for (int bit  =0 ;bit < newlocaldepth; bit++) 
		firstIndx = (firstIndx | (1<<bit & splitIndex)); // set the bit to 1
	
	Bucket * newBucket = new Bucket(newlocaldepth); 
	int increments= 1<<(newlocaldepth); 
	for(int i = firstIndx;  i < globaldirectory.length ; i+=increments){
		globaldirectory.entry[i] = newBucket;
	}
}



int insertItem(DataItem data, Bucket& currentBucket, GlobalDirectory& globaldirectory)
{
	//First corner case is done for you: if no directory yet
	if (globaldirectory.globalDepth == 0) {
		if (insertItemIntoBucket(currentBucket, data) == 0)
		{
			createFirstTimeDirectory(globaldirectory, currentBucket);
		}
		else {
			return 1; //successfully inserted;
		}
	}
	// complete the rest of the function ....
	int bucketindex = getCurrentHash(data.key, globaldirectory.globalDepth);
	printf("Want to insert %d \n",data.data);
	if (insertItemIntoBucket(*globaldirectory.entry[bucketindex], data) == 0){
		// extending 
		if (globaldirectory.entry[bucketindex]->localDepth == globaldirectory.globalDepth) {
			int extendcount =0 ; 
			extendDirectory(globaldirectory, bucketindex);
			return insertItem(data, currentBucket , globaldirectory); 
			// while(extendcount <=5 && insertItemIntoBucket(*globaldirectory.entry[bucketindex], data) == 0) {
			// 	extendDirectory(globaldirectory, bucketindex);
			// 	extendcount++;
			// }
			// if (extendcount > 5)
			// 	return 0; // failed to insert 
			// else {
			// 	return 1; // success in inserting 
			// }
		}
		else { // splitting the bucket  
				SplitBucket(globaldirectory, bucketindex);
				for(int i =0 ;i < RECORDSPERBUCKET ; i++) {	
					DataItem temp = globaldirectory.entry[bucketindex]->dataItem[i];
					globaldirectory.entry[bucketindex]->dataItem[i].valid = 0;
					globaldirectory.entry[bucketindex]->currentEntries-=1; // delete
					int bucketKey = getCurrentHash(globaldirectory.entry[bucketindex]->dataItem[i].key, globaldirectory.globalDepth);
					insertItem(temp ,currentBucket , globaldirectory); // insert the item in the new bucket
				}
				int newdataKey = getCurrentHash(data.key, globaldirectory.globalDepth);
				return insertItem(data ,currentBucket , globaldirectory); // insert the item in the new bucket
		}
	}
	else {
		return 1; //successfully inserted;
	}
}

// TODO5: Implement this function, Don't change the interface please
// functionlity: search the directory for an item using the key
// return:   nothing
// input:   key to be searched for , currentBucket --> initialBucket before creating directory,  globaldirectory
// Hint1:   use findItemInBucket & getCurrentHash functions

void searchItem(int key, Bucket& currentBucket, GlobalDirectory& globaldirectory) {
	if(globaldirectory.globalDepth==0){
		findItemInBucket(currentBucket,key);
	}
	else{
		int HashedKey=getCurrentHash(key,globaldirectory.globalDepth); //get the new key
		Bucket * CurrBucket=globaldirectory.entry[HashedKey];          //get bucket pointer
		findItemInBucket(*CurrBucket,key);
	}
}

//TODO6: Implement this function, Don't change the interface please
// functionlity: search on an item based on the key and delete it.
// return:   1 if succedded
//			 0 if failed (when does it fail to delete??)
// input:   key to be searched for , currentBucket --> initialBucket before creating directory,  globaldirectory
// Hint:    use deleteItemFromBucket & getCurrentHash & checkDirectoryMinimization functions
// Hint1:   in case the whole bucket is empty, the bucket should be merged again and the pointer should point to the peer bucket
// Hint2:   in how many steps do we reach our goal?
// Hint3:   in case of delete success don't forget to call checkDirectoryMinimization to compress directory if needed.
// Hint4: You might want to loop on checkDirectoryMinimization, not just call it once to continue merging

int deleteItem(int key, Bucket& currentBucket, GlobalDirectory& globaldirectory) {
	if(globaldirectory.globalDepth==0){
		if(deleteItemFromBucket(currentBucket,key)==1){
			return 1;
		}
		else{
			return 0;
		}
	}
	else{
		int HashedKey=getCurrentHash(key,globaldirectory.globalDepth); //get the new key
		Bucket * CurrBucket=globaldirectory.entry[HashedKey];          //get bucket pointer
		if(deleteItemFromBucket(*CurrBucket,key)==1){
			checkDirectoryMinimization(globaldirectory);
			return 1;
		}
		else{
			return 0;
		}
	}
	return 0;
}





//create  the first directory, this might help you to implement extendDirectory
int createFirstTimeDirectory(GlobalDirectory& globaldirectory, Bucket& currentBucket)
{
	globaldirectory.globalDepth = 1;
	globaldirectory.length = 2;
	globaldirectory.entry = new Bucket * [globaldirectory.length];
	globaldirectory.entry[0] = new Bucket(globaldirectory.globalDepth);
	globaldirectory.entry[1] = new Bucket(globaldirectory.globalDepth);
	for (int i = 0; i < RECORDSPERBUCKET; i++) {
		int newKey = getCurrentHash(currentBucket.dataItem[i].key, globaldirectory.globalDepth);
		if (newKey == -1)
		{
			return -1;
		}
		insertItemIntoBucket(*globaldirectory.entry[newKey], currentBucket.dataItem[i]);
	}
	return 1;
}


//TODO7: Implement this function, Don't change the interface please
// functionlity: this expands the directory because we can't find a space anymore in the file,
//               it also redistrubtes data over the split buckets
// return:   1 if succedded
//			 0 if failed (when does it fail to expand??)
// input:   Directory, hashKey(bucket index) at which the overflow occurs
// Hint1:   don't forget to delete unneeded pointers to avoid memory leakage
// Hint2:   what is the size of the new directory compared to old one? what is the new depth?
// Hint3:   some entries will point to the same bucket
int extendDirectory(GlobalDirectory &globaldirectory, int splitIndex) {
    int oldSize = globaldirectory.length;
    int newSize = oldSize * 2;

    // Create new directory with double size
    Bucket** newDirectory = new Bucket *[newSize];

    // Increase global depth
    globaldirectory.globalDepth++;

    // Redistribute entries
    for (int i = 0; i < oldSize; i++) {
        if (i == splitIndex) {
            // Rehash with new global depth
			globaldirectory.entry[i]->localDepth++;
            newDirectory[i] = globaldirectory.entry[i];
            newDirectory[i + oldSize] = new Bucket(globaldirectory.globalDepth); 		

		} else {
            // Keep the same bucket
            newDirectory[i] = globaldirectory.entry[i];
            newDirectory[i + oldSize] = globaldirectory.entry[i];
        }
    }
	 delete [] globaldirectory.entry;
    // Update directory and size

    globaldirectory.entry= newDirectory;
    globaldirectory.length = newSize;
	printf("SIZE NOW %d\n",globaldirectory.globalDepth);
		for(int j =0 ;j < RECORDSPERBUCKET ; j++) {
			DataItem temp = globaldirectory.entry[splitIndex]->dataItem[j];
			globaldirectory.entry[splitIndex]->dataItem[j].valid = 0; // delete
			globaldirectory.entry[splitIndex]->currentEntries-=1; // delete
			Bucket dummyBucket ; 
			int x = insertItem(temp ,dummyBucket ,globaldirectory)	;
		} 

    return 1;
}



//If all buckets have depth less than global depth,
// compress the directory depth by one 
// loop on this function as appropriate to compress the file back
int checkDirectoryMinimization(GlobalDirectory& globaldirectory) {

	for (int i = 0; i < globaldirectory.length; i++) {
		if (globaldirectory.entry[i]->localDepth == globaldirectory.globalDepth)
			return -1;

	}
	//if its depth =1, there is nothing more we can do, stop here
	if (globaldirectory.length == 1)
		return -1;
	// all are smaller than localdepth, initiate minimization
	// this is actually too stupid, it should be at least half empty to initiate minimization
	// but we will keep it like that for simplicity
	int oldGlobalDepth, oldLength;
	Bucket** oldEntry;

	oldGlobalDepth = globaldirectory.globalDepth;
	oldLength = globaldirectory.length;
	globaldirectory.globalDepth -= 1;
	globaldirectory.length /= 2;
	oldEntry = globaldirectory.entry;
	globaldirectory.entry = new Bucket * [globaldirectory.length];
	for (int i = 0; i < globaldirectory.length; i++) {
		globaldirectory.entry[i] = oldEntry[2*i];
	}
	
	delete [] oldEntry;
	//delete oldEntry;
	return 1;

}
