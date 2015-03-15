#include "main.h"
#include "ext2_structures.h"
#include <assert.h>
#include <stdlib.h>

#define DEBUG 0

static struct ext2_filsys *fs = NULL;


static char *output_dir = NULL;

static void debug(char *message, ...);

void scan_open_simple(int argc, char **argv);


int main(int argc, char **argv)
{

	char *bufPointer;
    if (argc < 3) {
        printf("Usage: %s <image-file> <output-dir>\n", argv[0]);
        exit(1);
    }

    output_dir = argv[2];
    assert(strlen(output_dir) > 0);

   //if (opendir(output_dir) != NULL) {
   //     printf("Please specify an empty output directory\n");
   //     exit(1);
   //}

   // assert(mkdir(output_dir, 0777) == 0);
    
    // init the scanner, all important file system information
    // will be stored in fs
    fs = scan_init();
    
    // open the image file
    scan_open(argc, argv);

    // start scanning the image file
    scan_scan(fs);

    /*
    __u32 a;
	a=0;
	int counter = 0;
    printf("\n\n Reading the block\n");
    bufPointer = scan_read_block(fs,a);
    fwrite(bufPointer, 1, 1024, stdout);

    free(bufPointer);
	*/
    printf("\n closing\n");

    
    /*
    int h = gds->bg_inode_table+(56-1)*sizeof(struct ext2_inode);
	char buf[1024];
	lseek64(devfd,h , SEEK_SET);
	read(devfd, buf, sizeof(struct ext2_inode));

	printf("/test Code END/\n");
	*/
	int p = 0;
	printf("%c[1;33m", 27);
	printf("First Pass - Reading All Directory Information\n");
  printf("%c[0m", 27);
  int q;
  int y;
  int t;
  int Counter = 0;
  for(q = 0; q<fs->gds_count; q++)      //////Switches between groups
  {
    //printf("Parsing> Group %d\n", q+1 );

    for( y = fs->inodeTableLocations[q]; y < (fs->inode_blocks_per_group + fs->inodeTableLocations[q] +1) ; y++) //////Switches between blocks in a group
    {
        _scan_read_directories(fs,y, output_dir); 
    }

  }

 //  for( p = 0; p< 3; p++)
	// {
	// 	_scan_read_directories(fs,261+p, output_dir);	
	// }




  printf("%c[1;33m", 27);  
	printf("Second Pass - Reading for JPEG Files\n");
  printf("%c[0m", 27);
	
		
  for(q = 0; q<fs->gds_count; q++)      //////Switches between groups
  {
    for( y = fs->inodeTableLocations[q]; y < (fs->inode_blocks_per_group + fs->inodeTableLocations[q] +1) ; y++) //////Switches between blocks in a group
    {
        _scan_read_inode(fs,y, output_dir); 
    }
  }
	

    scan_close();

	//scan_open_simple(argc, argv);
	

}

static void debug(char *message, ...) {
    if (DEBUG) {
        va_list ap;
        va_start(ap, message);
        vprintf(message, ap);
    }

}


////////////////////////My Fnctions






void scan_open_simple(int argc, char **argv)
{

int counter1024 = 0;
int numCounter = 0;
int  number = 0;

printf("enter total blocks to print(as string)> ");
scanf("%d", &number);


int c;
FILE *file;
file = fopen(argv[1], "r");
	if(file == NULL)
			{
				printf("ERR: Could Not Open for Simple Reading!\n");
				exit(0);
			}


if (file) 
	{
    while ((c = getc(file)) != EOF)
    	{
       		putchar(c);
       		counter1024++ ;
       			if(numCounter >= number)
       				break;

       			if (counter1024 >= 1024)
       			{
       				printf("\n\n");
       				printf("Block> %d Total Characters Read> %d \n", numCounter+1, counter1024);
       				printf("\n\n");
       				numCounter++;
       				counter1024 = 0;
       			}
       			
    	}
    fclose(file);
	}
	//__u32 a = "Xd";
	printf("Hex Test!\n");
	printf("0X%02x  \n", "Xd");


}