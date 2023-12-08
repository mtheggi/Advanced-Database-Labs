//============================================================================
// Name        : hashskeleton.cpp
// Author      : Faculty of Engineering, Cairo University
// Version     :
// Description : Hashing using Extendible hashing
//============================================================================


#include <iostream>
#include "ExtendiableHashing.h"




int main() {

    double score = 0.0;

    //Initialize File
    GlobalDirectory globalDirectory;
    Bucket initialFile;
	// Empty Directory
    displayDirectory(globalDirectory, initialFile, 1);
	//============================================================

	//test insertion without making extra directory
    insertItem(DataItem(13, 33), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);
    cout.flush();
	//============================================================
	//search 
    searchItem(13, initialFile, globalDirectory);
	// delete item
    deleteItem(13, initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);
	//============================================================
    cout << "--------------------test here" << endl; 
    cout.flush();
	////////////////////////////////////////////////////////////

    insertItem(DataItem(1, 20), initialFile, globalDirectory);
    insertItem(DataItem(2, 70), initialFile, globalDirectory);
    insertItem(DataItem(42, 11), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);
	cout.flush();
    //============================================================
    cout << "--------------------test here" << endl;    
    insertItem(DataItem(112, 54), initialFile, globalDirectory);
    insertItem(DataItem(240, 16), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);
    insertItem(DataItem(241, 99), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);
	cout.flush();
    //============================================================
	insertItem(DataItem(159, 30), initialFile, globalDirectory);
	displayDirectory(globalDirectory, initialFile, 0);
    cout.flush();
	//============================================================
    insertItem(DataItem(14, 45), initialFile, globalDirectory);
    insertItem(DataItem(13, 88), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);
	cout.flush();
    //============================================================
    insertItem(DataItem(37, 1), initialFile, globalDirectory);
    searchItem(13, initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);
	cout.flush();
    //============================================================
    deleteItem(14, initialFile, globalDirectory);
    deleteItem(13, initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);
	cout.flush();
    //============================================================
    insertItem(DataItem(158, 5), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);
    insertItem(DataItem(3, 9), initialFile, globalDirectory);
    displayDirectory(globalDirectory, initialFile, 0);
    cout.flush();


    return 0;



}


