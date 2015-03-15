
#include "main.h"
struct ext2_group_desc *gds;
int totalDirectories;

// scan the group descriptor
void scan_group_descriptors(struct ext2_filsys *fs)
{
    
    __u32 cur_ino = 1, start_ino, end_ino;
    int i;

    // store the locations of the inode tables
    // there are fs->gds_count inode tables exist in the system
    // each group has an inode table
    fs->inodeTableLocations = malloc(sizeof(__u32) * fs->gds_count);
    
    // read the group desriptor from the disk
    gds = (struct ext2_group_desc*) scan_read_gds(fs);

    printf("---------------------------------------------------------\n");
    printf("  Group Descriptors                           \n");
    printf("---------------------------------------------------------\n");
    printf("            |   BlkBm   InoBm  InoTbl | InoStrt .. InoEnd .. Directories\n");
    printf("---------------------------------------------------------\n");

    // for each group descriptor, print the information
    // about where the inode table is (bg_inode_table)

    // you can ignore the 

    for (i = 0; i < fs->gds_count; i++) {
	// setup inode range
	start_ino = cur_ino;
	end_ino = cur_ino + fs->inodes_per_group - 1;
	
	cur_ino += fs->inodes_per_group;
	
	printf("  Group-%02d  |  ", i);
      	printf("%6u  %6u %6u  |  %6u .. %-6u  |  %hu\n",
	       gds[i].bg_block_bitmap,
	       gds[i].bg_inode_bitmap,
	       gds[i].bg_inode_table,
	       start_ino, end_ino, gds[i].bg_used_dirs_count);

    totalDirectories = gds[i].bg_used_dirs_count + totalDirectories;    

	// sanity check, to make sure we are reading 
	// the correct group descirptor
	if ((gds[i].bg_block_bitmap+1 != gds[i].bg_inode_bitmap) ||
	    (gds[i].bg_inode_bitmap+1 != gds[i].bg_inode_table)) {

	    printf("  ERROR: Most likely we are not reading the \n");
	    printf("  ERROR: correct group descriptors.\n");
	    printf("  ERROR: Email 537-help.\n");
	    exit(1);
	}


	// ************************************************
	// store the location of the inode table.
	fs->inodeTableLocations[i] = gds[i].bg_inode_table;
	// ************************************************


    }
    printf("\n\n");

    // free gds
    free(gds);

    printf("---------------------------------------------------------\n");
    printf("  Here is the locations of the  inode tables:            \n");    
    printf("---------------------------------------------------------\n");
    
    // now scan the inodes in the group
    start_ino = 1;
    for (i = 0; i < fs->gds_count; i++) {
	printf("  Group-%02d  %10d  \n",
	       i, fs->inodeTableLocations[i]);
    }
    printf("\n\n");    
    printf(" The rest is your job ... Good luck!\n");
    printf("\n\n");    


}
    

// read the group descriptor
char* scan_read_gds(struct ext2_filsys *fs)
{
    size_t size;
    char * buffer;
    ext2_off_t loc;
    ext2_off_t rloc;
    int retval;


    // size is how many blocks store the group descriptors (fs->gd_blocks_count)
    size = (fs->block_size * fs->gd_blocks_count); 

    // allocate buffer to read as many blocks as needed
    buffer = malloc(size);
    if (!buffer) {
	printf("  ERROR: can't allocate buffer for gd \n");
	exit(1);
    }
    
    // the location of group descriptors is in block #2
    loc = (ext2_off_t) BLOCK_NUMBER_FIRST_GROUP_DESC * 
	(ext2_off_t)(fs->block_size);

    // seek to block #2
    rloc = lseek64(devfd, loc, SEEK_SET);
    if (rloc < 0) {
	printf("  ERROR: can't lseek64 fd=%d loc=%d \n", devfd, loc);
	exit(1);
    }

    // read as many size-bytes
    retval = read(devfd, buffer, size);
    if (retval < 0 || retval != size) {
	printf("  ERROR: Can't read gds, error = %d \n", errno);
	exit(1);
    }
    
    // return the group-descriptors
    return buffer;
}
