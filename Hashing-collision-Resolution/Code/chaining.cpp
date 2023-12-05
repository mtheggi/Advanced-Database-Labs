#include "readfile.h"

/* Hash function to choose bucket
 * Input: key used to calculate the hash
 * Output: HashValue;
 */
int hashCode(int key){
   return key % MBUCKETS;
}



int FileSize=FILESIZE;


int CreateExtraBucket(int fd){
	FileSize+=BUCKETSIZE;
	size_t result = lseek(fd, BUCKETSIZE-1, SEEK_SET);
    if (result == -1) {
	close(fd);
	perror("Error calling lseek() to 'stretch' the file");
        return 1;
    }
    /* write just one byte at the end */
    result = write(fd, "", 1);
    if (result < 0) {
		close(fd);
		perror("Error writing a byte at the end of the file");
			return -1;
    	}
    
    else{
    	    fd = open("openaddressing", O_RDWR  , (mode_t)0600);
    	    if (fd == -1) {
    		perror("Error opening file for writing");
    	        return -1;
    	}
    }
}

int insertItem(int fd,DataItem item){
   //TODO: implement this function
	int hashIndex=hashCode(item.key);
	int startingOffset = hashIndex*sizeof(Bucket);
	int rewind=0;
	int Offset = startingOffset;
	DataItem data;
	bool flag=false;
	int NumberOfAccesses=0;
	RESEEK:
		NumberOfAccesses++;
		ssize_t result = pread(fd,&data,sizeof(DataItem), Offset);
		if(result<=0){
			return -1;
		}
		else if(data.valid==0  ){
			item.valid=1;
			if(flag)
			item.Offset=data.Offset;
			result = pwrite(fd,&item,sizeof(item),Offset);
			return NumberOfAccesses;
		}
		else {
			flag=true;
			int oldOffsite=Offset;
			Offset=data.Offset;
			if(Offset!=-1 ){
				goto RESEEK;
			}
			else {
				Offset=FileSize;
				data.Offset=Offset;
				data.valid=1;
				result = pwrite(fd,&data,sizeof(data),oldOffsite);
				CreateExtraBucket(fd);
				item.valid=1;
				result = pwrite(fd,&item,sizeof(item),Offset);
			}
		}		
   return NumberOfAccesses;
}


int searchItem(int fd,struct DataItem* item,int *count)
{

	//Definitions
	struct DataItem data;   //a variable to read in it the records from the db
	*count = 0;				//No of accessed records
	int rewind = 0;			//A flag to start searching from the first bucket
	int hashIndex = hashCode(item->key);  				//calculate the Bucket index
	int startingOffset = hashIndex*sizeof(Bucket);		//calculate the starting address of the bucket
	int Offset = startingOffset;						//Offset variable which we will use to iterate on the db

	//Main Loop
	RESEEK:
	//on the linux terminal use man pread to check the function manual
	ssize_t result = pread(fd,&data,sizeof(DataItem), Offset);
	//one record accessed
	(*count)++;
	//check whether it is a valid record or not
    if(result <= 0) //either an error happened in the pread or it hit an unallocated space
	{ 	 // perror("some error occurred in pread");
		  return -1;
    }
    else if (data.valid == 1 && data.key == item->key) {
    	//I found the needed record
    			item->data = data.data ;
    			return Offset;

    } else { //not the record I am looking for
    		Offset =data.Offset;
    		if(Offset ==-1 )
    		 { 
				return -1;
    	     }
    		goto RESEEK;
    }
}


int DisplayFile(int fd){

	struct DataItem data;
	int count = 0;
	int Offset = 0;
	for(Offset =0; Offset< FileSize;Offset += sizeof(DataItem))
	{
		ssize_t result = pread(fd,&data,sizeof(DataItem), Offset);
		if(result < 0)
		{ 	  perror("some error occurred in pread");
			  return -1;
		} else if (result == 0 || data.valid == 0 ) { //empty space found or end of file
			printf("Bucket: %d, Offset %d:~\n",Offset/BUCKETSIZE,Offset);
		} else {
			pread(fd,&data,sizeof(DataItem), Offset);
			printf("Bucket: %d, Offset: %d, Data: %d, key: %d Offset %d\n",Offset/BUCKETSIZE,Offset,data.data,data.key,data.Offset);
					 count++;
		}
	}
	return count;
}

int deleteOffset(int fd, int Offset)
{
	DataItem data;
	ssize_t rt = pread(fd,&data,sizeof(DataItem), Offset);
	struct DataItem dummyItem;
	dummyItem.valid = 0;
	dummyItem.key = -1;
	dummyItem.data = 0;
	dummyItem.Offset=data.Offset;
	int result = pwrite(fd,&dummyItem,sizeof(DataItem), Offset);
	return result;
}