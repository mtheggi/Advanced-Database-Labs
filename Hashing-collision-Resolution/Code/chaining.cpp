#include "readfile.h"

/* Hash function to choose bucket
 * Input: key used to calculate the hash
 * Output: HashValue;
 */
int hashCode(int key)
{
   return key % MBUCKETS;
}



int FileSize=FILESIZE;


int CreateExtraBucket(int fd)
{
	FileSize+=BUCKETSIZE;
	//size_t result = lseek(fd, BUCKETSIZE-1, SEEK_SET);
	size_t result = lseek(fd, FileSize-1, SEEK_SET);
    if (result == -1)
	 {
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
    	    fd = open("chaining", O_RDWR  , (mode_t)0600);
    	    if (fd == -1) {
    		perror("Error opening file for writing");
    	        return -1;
    	}
    }
}

int insertItemChainig(int fd,DataItem item)
{
   //TODO: implement this function
   	// printf ("Key: %d \n",item.key);
	// printf("data: %d \n",item.data);
	int hashIndex=hashCode(item.key);
	int startingOffset = hashIndex*sizeof(Bucket);
	int Offset = startingOffset;
	Bucket bData; // bucket of data
	int NumberOfAccesses=0;
	RESEEK:
		NumberOfAccesses++;
		ssize_t result = pread(fd,&bData,sizeof(Bucket), Offset);
		//printf(" valid buckt data or not 1:%d    2:%d  ",bData.dataItem[0].valid, bData.dataItem[1].valid);
		if(result<=0)
		{
			printf("error:\n");
			return -1;
		}
		else if( bData.dataItem[0].valid==0 || bData.dataItem[1].valid==0)    // here check if the valid =0 thent not occupied then the new item will occupiey this space
		{	
			//printf ("enterd else if \n");

			if (bData.dataItem[0].valid==0)
			{
			item.valid=1;
			bData.dataItem[0]=item;
			result = pwrite(fd,&bData,sizeof(Bucket),Offset);
			//printf ("enterd else if \n");
			return NumberOfAccesses;
			}
			else if (bData.dataItem[1].valid==0)
			{
			item.valid=1;
			bData.dataItem[1]=item;
			result = pwrite(fd,&bData,sizeof(Bucket),Offset);
			return NumberOfAccesses;
			}
		}
		else 
		{
			// printf ("enterd else vale of bPTR: %d  \n",bData.Bptr);
			// printf("offset before creating : %d\n",Offset);
			if(bData.Bptr==0)
			{
				//printf ("enterd if to create bucket  \n");

				//crating new bucket
				bData.Bptr=FileSize;
				result = pwrite(fd,&bData,sizeof(Bucket),Offset);
				Offset=bData.Bptr;
				//printf("offset after creating : %d \n",Offset);
				//printf ("enterd if to create bucket  bData.Bptr: %d \n",bData.Bptr);
				CreateExtraBucket(fd);
				goto RESEEK;
			}
			else
			{
				Offset=bData.Bptr;
				goto RESEEK;
			}
		}		
   return NumberOfAccesses;
}


int searchItem(int fd,struct DataItem *ptr,int *count)
{
	int hashIndex=hashCode(ptr->key);
	int startingOffset = hashIndex*sizeof(Bucket);
	int Offset = startingOffset;
	Bucket bData; // bucket of data

	RESEEK:
		(*count)++;
		//printf("counbt: %d \n",*count);
		ssize_t result = pread(fd,&bData,sizeof(Bucket), Offset);
		//printf(" valid buckt data or not 1:%d    2:%d  ",bData.dataItem[0].valid, bData.dataItem[1].valid);
		if(result<=0)
		{
			printf("error:\n");
			return -1;
		}

		else if( bData.dataItem[0].key==ptr->key )    
		{	
			//printf ("enterd else if \n");

			return Offset;
		}
		else if (  bData.dataItem[1].key==ptr->key)
		{
			return Offset +sizeof(DataItem);
		}
		else 
		{

			if(bData.Bptr==0)
			{

				return -1;
			}
			else
			{
				Offset=bData.Bptr;
				goto RESEEK;
			}
		}		
   return -1;
}


int DisplayFile(int fd){

	struct Bucket  data;
	int count = 0;
	int Offset = 0;
	for(Offset =0; Offset< FileSize;Offset += sizeof(Bucket))
	{
		ssize_t result = pread(fd,&data,sizeof(Bucket), Offset);
		if(result < 0)
		{ 	  perror("some error occurred in pread");
			  return -1;
		} 
		else if (data.dataItem[0].valid !=0)
		{
			printf("Bucket: %d, Offset: %d, Data: %d, key: %d  \n",Offset/BUCKETSIZE,Offset,data.dataItem[0].data,data.dataItem[0].key);
		}
		else 
		{
			printf("Bucket: %d, Offset: %d ~ \n",Offset/BUCKETSIZE,Offset);
		}
		 if (data.dataItem[1].valid !=0)
		{
			printf("Bucket: %d, Offset: %d, Data: %d, key: %d  \n",Offset/BUCKETSIZE,Offset,data.dataItem[1].data,data.dataItem[1].key);
		}	
		else 
		{
			printf("Bucket: %d, Offset: %d ~ \n",Offset/BUCKETSIZE,Offset);
		}
		
	}
	return count;
}

int deleteOffset(int fd, int Offset)
{
	struct DataItem dummyItem;
	dummyItem.valid = 0;
	dummyItem.key = -1;
	dummyItem.data = 0;
	int result = pwrite(fd,&dummyItem,sizeof(DataItem), Offset);
	return result;
}