
#include	"main.h"

int devfd = 0 ;


void scan_scan(struct ext2_filsys *fs)
{

    // first scan the superblock
    scan_super_block(fs);
    
    // once, we get the superblock
    // we can scan the group descriptors
    scan_group_descriptors(fs);
}    




void scan_open(int argc, char **argv)
{
    int flags = O_RDONLY;
    char devname [100];

    if (argc >= 2) {
	strcpy(devname, argv[1]);
    }
    else {
	printf("  # of arg is not correct \n");
	exit(1);
    }

    // open the file system image
    devfd = open64(devname, flags);
    if (devfd < 0) {
	printf("  ERROR: can't open %s \n", devname);
	exit(1);
    }
    printf("\n");
    printf("  Opening disk image '%s' successful fd=%d \n", devname, devfd);
    printf("\n");
}

void scan_close()
{
    int retval;
    retval = close(devfd);
    if (retval < 0) {
	printf("  ERROR: close fails \n");
	exit(1);
    }
    devfd = -1;
}


struct ext2_filsys* scan_init()
{
    // allocate the filsys structure
    struct ext2_filsys *fs = malloc(sizeof(struct ext2_filsys));
    if (!fs) {
	printf("  ERROR: can't allocate fs \n");
	close(devfd);
	exit(1);
    }

    // zeroes out the fs content
    memset(fs, 0, sizeof(struct ext2_filsys));

    return fs;
}


// ***************************************************
// This is a nice utility to read a block from the file
// system image. 
// blknum = blknum is the block number that you want to read.
// This function, will only read 1 block. It will malloc
// 1-KB of memory and return this block to the caller.
// To prevent memory leak, the caller needs to free this block 
// once it's done using it
// ***************************************************
char * scan_read_block(struct ext2_filsys *fs, __u32 blknum)
{
    char *buffer;
    ext2_off_t loc;
    ext2_off_t rloc;
    int retval;
    size_t size;
    int is_jpg;
    
    // size is always 1 KB
    size = fs->block_size;
    
    // allocate 1-KB buffer in the heap
    buffer = malloc(size);
    if (!buffer) {
	printf("  ERROR: can't allocate buffer for first block \n");
	exit(1);
    }


    // set the actual byte offset 
    // actual byte offset = blknum * block size
    loc = (ext2_off_t) blknum * (ext2_off_t)(fs->block_size);
    
    // seek to the byte offset
    rloc = lseek64(devfd, loc, SEEK_SET);
    if (rloc < 0) {
	printf("  ERROR: can't lseek64 fd=%d loc=%d \n", devfd, loc);
	exit(1);
    }
    
    // read 1-KB from the offset, and put it in buffer
    retval = read(devfd, buffer, size);
    if (retval < 0 || retval != size) {
	//printf(" >>Block points outside the range of valid blocks - IGNORING\n", errno);
	//exit(1);
    }    
    
    // return buffer
    return buffer;

}

// 
char * scan_read_size(struct ext2_filsys *fs, __u32 blknum, size_t size)
{
    char *buffer;
    ext2_off_t rloc;
    ext2_off_t loc;
    int retval;
    
    // allocate 'size' buffer in the heap
    buffer = malloc(size);
    if (!buffer) {
	printf("  ERROR: can't allocate buffer for first block \n");
	exit(1);
    }

    loc = (ext2_off_t) blknum * (ext2_off_t)(fs->block_size);

    rloc = lseek64(devfd, loc, SEEK_SET);
    if (rloc < 0) {
        printf("  ERROR: can't lseek64 fd=%d loc=%d \n", devfd, loc);
	    exit(1);
    }
   
    // read 1-KB from the offset, and put it in buffer
    retval = read(devfd, buffer, size);
    if (retval < 0 || retval != size) {
	    printf("  ERROR: Can't read inode table, error = %d \n", errno);
    	exit(1);
    }    
    
    // return buffer
    return buffer;

}
