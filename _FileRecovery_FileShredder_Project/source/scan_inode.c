#include	"main.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void _scan_read_directories(struct ext2_filsys *fs, __u32 blknum, char * output_dir);
void scan_print_file(int inode_number, struct ext2_filsys *fs, struct ext2_inode *  my_inode, char* output_dir);
void ParseDirectory(__u32 blknum,struct ext2_filsys *fs);
void FileDuplicator(char inodeNum[], char name[]);


////////////TEMP STRUCTURES//////
struct ext2_dir_entry_2 *entry;
unsigned int size;
///////////TEMP STRUCTURES//////


typedef struct Directory
{
   char  d_filename[1024];
   int   d_inodeNum;
   int   d_size;
};

struct Directory DirectoryInformation[1000];


void _scan_read_inode(struct ext2_filsys *fs, __u32 blknum, char * output_dir)
{
    char *buffer;
    ext2_off_t loc;
    ext2_off_t rloc;
    int retval;
    size_t size;
    int is_jpg;

	int InodeTrav =  fs->block_size/fs->inode_size;   //8
    struct ext2_inode **my_inode =  malloc(InodeTrav* sizeof(struct ext2_inode));

    int k;
    for(k = 0; k < InodeTrav; ++k)
  			my_inode[k] = NULL;

  	for(k = 0; k < InodeTrav; ++k)
	  my_inode[k] = malloc(sizeof(struct ext2_inode));	


    // size is always 1 KB
    size = fs->block_size;
    
    // allocate 1-KB buffer in the heap
   // printf("Reserving Memory for Block\n");


   // printf("Size of inode is: %d\n", sizeof(struct ext2_inode));
   // printf("Number of Inodes in Block> %d \n", 1024/sizeof(struct ext2_inode));


    // set the actual byte offset 
    // actual byte offset = blknum * block size
    loc = (ext2_off_t) blknum * (ext2_off_t)(fs->block_size);
    
    // seek to the byte offset
    rloc = lseek64(devfd, loc, SEEK_SET);
    if (rloc < 0) {
	//printf("  ERROR: can't lseek64 fd=%d loc=%d \n", devfd, loc);
	//exit(1);
    }
    // read 1-KB from the offset, and put it in buffer
    int i;
    for (i =0; i < InodeTrav; i++)
    {
    	retval = read(devfd, my_inode[i], fs->inode_size);
	}


    char *bufPointer;

    if (retval < 0 || retval != fs->inode_size) {
	//printf("  ERROR: Can't read inode table, error = %d \n", errno);
	//exit(1);
    }    
    int j;
	//printf("In block> %d\n",blknum );
	//printf("______________\n");

	int o;
	static int inode_number = 0;
	for( k = 0; k< InodeTrav; k++)
	{
		inode_number++;

	    //if( my_inode[k]->i_size == 0)
	    //	printf(": empty!\n");
	    if( my_inode[k]->i_size > 0)
	    {

			printf("Inode> %d", inode_number);
	    	printf(": has something! --------------");
	    	printf("Reading First Block> %u", my_inode[k]->i_block[0]);
	    	bufPointer = scan_read_block(fs,my_inode[k]->i_block[0]);
	    	int is_jpg = 0;

	    	/* Checking for JPEG */
			if (bufPointer[0] == (char)0xff && bufPointer[1] == (char)0xd8 && bufPointer[2] == (char)0xff && (bufPointer[3] == (char)0xe0 ||  bufPointer[3] == (char)0xe1)) 
			{
				 is_jpg = 1;
				 printf("%c[1;36m", 27);
				 printf("-----Found a JPEG OMG!\n");
				 printf("%c[0m", 27);
				 printf("%c[3;36m", 27);	
				 printf("			Blocks Are\n");
				 for(o = 0; o < 12; o++)
				 	printf("			%d> DIRECT> %u  \n",o, my_inode[k]->i_block[o]);
				 printf("			%d> INDIRECT> %u  \n",12, my_inode[k]->i_block[12]);
				 printf("			%d> D-INDIRECT> %u  \n",13, my_inode[k]->i_block[13]);
				 printf("			%d> T-DIRECT> %u  \n",14, my_inode[k]->i_block[14]);
				 printf("%c[0m", 27);
				 scan_print_file(inode_number, fs, my_inode[k], output_dir);
			}
			////////////Temporary code
			/* Checking for Directory */
			//////////Temporary Code
			printf("\n");
	    }	   
	}
	//printf("\n");
}


void scan_print_file(int inode_number, struct ext2_filsys *fs, struct ext2_inode *  my_inode, char* output_dir)
{
	printf("			Printing to File!\n");
	int z = 0;
 	char tempBuf[20];
	int u = 0;
	while(output_dir[u] != 0)
	{
		tempBuf[u] = output_dir[u];
		u++;
	}

	int BlockEntrySize =  fs->block_size/sizeof(__u32); 

	//printf("TESTING BLOCK ENTRY SIZE> %d\n", BlockEntrySize);
	
	char str[15];
	sprintf(str, "%d", inode_number);
	char *bufPointer;

	FILE * FOUT;
	
	strcat(tempBuf,"/");
	strcat(tempBuf,str);

	FOUT = fopen (tempBuf, "wb");	
	if(FOUT == NULL)
	{
		printf("ERROR! Sector>> SCN_INODE > PRNT_FILE  ------Invalid Output Directory");
		exit(0);
	}
	printf("			Reading and Writing Direct Blocks!");
	int a;

	for(a = 0; a < 12; a++)
	{
		bufPointer = scan_read_block(fs,my_inode->i_block[a]);
		fwrite(bufPointer, fs->block_size,1,FOUT);
	}
	printf("\t------Done");						
	printf("\n");


	printf("			Reading and Writing Indirect Blocks!");
	//printf("				These are");
	__u32  *blockStorage;
	blockStorage = scan_read_block(fs, my_inode->i_block[12]);
	int y;

	for(y = 0; y< BlockEntrySize; y++)
	{
		///breaking condition
		if(*blockStorage == 0)
			break;
		bufPointer = scan_read_block(fs,*blockStorage);
		fwrite(bufPointer, fs->block_size,1,FOUT);

		blockStorage++;	
	}
	printf("\t------Done");	
	printf("\n");


	/////////////////////////////////
	printf("			Reading and Writing Double-Indirect Blocks!");
	__u32 *blockStorage2;
	__u32 *blockStorage3;
	int k;
	blockStorage2 = scan_read_block(fs, my_inode->i_block[13]);

	for ( y = 0; y<BlockEntrySize;y++)
	{ 
		if(*blockStorage2 == 0)
			break;

		blockStorage3 = scan_read_block(fs, *blockStorage2);
			for(k=0; k< BlockEntrySize; k++)
				{
					if(*blockStorage3 == 0)
						break;

					bufPointer = scan_read_block(fs, *blockStorage3);
					fwrite(bufPointer, fs->block_size, 1, FOUT);

					blockStorage3++;
				}

		blockStorage2++;
	}
	printf("\t------Done");	
	printf("\n");


	/////////////////////////////////
	printf("			Reading and Writing Triple-Indirect Blocks!");
	__u32 *blockStorage4;
	__u32 *blockStorage5;
	__u32 *blockStorage6;
	int p;
	blockStorage4 = scan_read_block(fs, my_inode->i_block[13]);

	for ( y = 0; y<BlockEntrySize;y++)
	{ 
		if(*blockStorage4 == 0)
			break;

		blockStorage5 = scan_read_block(fs, *blockStorage4);
			for(k=0; k< BlockEntrySize; k++)
				{
					if(*blockStorage5 == 0)
						break;

					blockStorage6 = scan_read_block(fs, *blockStorage5);
					for(p=0; p<BlockEntrySize;p++)
					{
							if(*blockStorage6 == 0)
								break;
							bufPointer = scan_read_block(fs, *blockStorage6);
							fwrite(bufPointer, fs->block_size,1,FOUT);

							blockStorage6++;
					}
					

					blockStorage5++;
				}

		blockStorage4++;
	}
	printf("\t------Done");	
	printf("\n");

	//printf("TEST INODE NUM %d\n",inode_number );
	int e;
	printf("			Searching File Name in Directory Structures\n");
	for(e = 0; e < 1000 ; e++)
	{
		if(strlen(DirectoryInformation[e].d_filename)==0)
			break;
		if(DirectoryInformation[e].d_inodeNum == inode_number)
			{	
			printf("%c[1;29m", 27);  
			printf("			Printing with File Name\n");
			FileDuplicator(tempBuf, DirectoryInformation[e].d_filename);
			printf("%c[0m", 27);
			}
	}

	fclose (FOUT);
}

void ParseDirectory(__u32 blknum,struct ext2_filsys *fs)
{
	static int indexCounter = 0;

    char block[fs->block_size];
    ext2_off_t loc;
    ext2_off_t rloc;
    int retval;
    unsigned int size1;
    size_t size;   
    struct ext2_dir_entry *entry=malloc(sizeof(struct ext2_dir_entry));   
    size = fs->block_size;    
    loc = (ext2_off_t) blknum * (ext2_off_t)(fs->block_size);   
    rloc = lseek64(devfd, loc, SEEK_SET);

    if (rloc < 0) 
    {
		printf("  ERROR: can't lseek64 fd=%d loc=%d \n", devfd, loc);
		exit(1);
    }
    
    // read 1-KB from the offset, and put it in buffer
    retval = read(devfd, block, fs->block_size);
     
    if (retval < 0 || retval != size) {
	printf("  ERROR: Can't read inode table, error = %d \n", errno);
	exit(1);
    }  
	int si=0;
	printf("\n");
	printf("			SAVING File LIST\n");
	
	if(indexCounter < 1000)
	{
		while(si< fs->block_size)
		{
			entry = (struct ext2_dir_entry*) & ( block[si] );
			int name_len = entry->name_len & 0xFF; // convert 2 bytes to 4 bytes properly
			char name [EXT2_NAME_LEN];
			strncpy(name, entry->name, name_len);
			name[name_len] = '\0';
			int inode_num= entry->inode;
			//printf("Index Counter is %d \n", indexCounter);

			strcpy(DirectoryInformation[indexCounter].d_filename, name);
			DirectoryInformation[indexCounter].d_inodeNum = inode_num;
			DirectoryInformation[indexCounter].d_size = entry->rec_len;
			indexCounter++;

			//printf("			1. ");
			//printf(" NAME: %s\n", name);
			//printf("			    SIZE: %u\n", entry->rec_len);
			
			//printf("			    INODE: %d\n", inode_num);
		  	si=si+entry->rec_len;   
		}
	}
	else
		printf("Directory Index is FULL\n");    
}




///////////////READING DIRECTORIES



void _scan_read_directories(struct ext2_filsys *fs, __u32 blknum, char * output_dir)
{

	static int dirCounter = 0;
	int InodeTrav =  fs->block_size/fs->inode_size;   //8
    if( dirCounter < totalDirectories)
    {
	    ext2_off_t loc;
	    ext2_off_t rloc;
	    int retval;
	    size_t size;

	    struct ext2_inode **my_inode =  malloc(InodeTrav * sizeof(struct ext2_inode));

	    int k;
	    for(k = 0; k < InodeTrav; ++k)
	  			my_inode[k] = NULL;

	  	for(k = 0; k < InodeTrav; ++k)
		  my_inode[k] = malloc(sizeof(struct ext2_inode));	


	    size = fs->block_size;

	    loc = (ext2_off_t) blknum * (ext2_off_t)(fs->block_size);
	    

	    rloc = lseek64(devfd, loc, SEEK_SET);
	    if (rloc < 0) {
		printf("  ERROR: can't lseek64 fd=%d loc=%d \n", devfd, loc);
		exit(1);
	    }
	    // read 1-KB from the offset, and put it in buffer
	    int i;
	    for (i =0; i < InodeTrav; i++)
	    {
	    	retval = read(devfd, my_inode[i], fs->inode_size);
		}

	    if (retval < 0 || retval != fs->inode_size) {
		printf("  ERROR: Can't read inode table, error = %d \n", errno);
		exit(1);
	    }    
	    int j;
		printf("In block> %d\n",blknum );
		printf("______________\n");
	    
		int o;
		static int inode_number = 0;

		for( k = 0; k< InodeTrav; k++)
		{
			inode_number++;
			if(dirCounter >= totalDirectories)
				break;

			
		    if( my_inode[k]->i_size > 0)
		    {

				/* Checking for Directory */
				if (LINUX_S_ISDIR(my_inode[k]->i_mode))
				{
					printf("Inode> %d", inode_number);
					printf("%c[1;31m", 27);
					printf("--is a Directory!");
					ParseDirectory(my_inode[k]->i_block[0], fs);
					dirCounter++;
					printf("%c[0m", 27);

				}
				//////////Temporary Code
				printf("\n");
	    }	   
		}
		printf("\n");
	}
}


void i_debug()
{
	printf("Printing Directories\n");
	int a = 0;
	while(a< 50)
	{
		if(strlen(DirectoryInformation[a].d_filename) > 0)
		{	printf("Num %d  ",a );
			printf("Name: %s\n", DirectoryInformation[a].d_filename);
			printf("Inode: %d\n", DirectoryInformation[a].d_inodeNum);
		}
		a++;
	}
}


void FileDuplicator(char inodeNum[], char name[])
{

	//char str[15];
   	//sprintf(str, "%d", inode_number);

   	char ch;
   	FILE *source, *target;
   	char tempBuf[1024];
 
   	source = fopen( inodeNum, "rb");
	
   	if( source == NULL )
	   {
	      printf("Cannot find Source File\n");
	      exit(0);
	   }
	char * DestinationHolder;
	DestinationHolder =  strtok(inodeNum, "/");
	strcat(DestinationHolder, "/");
	strcat(DestinationHolder, name);

	 
	   target = fopen(DestinationHolder, "wb");
	  
	   if( target == NULL )
	   {
	      fclose(source);
	      printf("Cannot Create Destination File\n");
	      exit(0);
	   }
	  int n = 0;
	  while ( ( n = fread( tempBuf, sizeof(tempBuf), 1 ,source) ) > 0 )
	  {
	     fwrite(tempBuf, sizeof(tempBuf),1,target);
	  }

	 
	   //printf("File copied successfully.\n");
	 
	   fclose(source);
	   fclose(target);
	 
	   return 0;
}