#include	"main.h"


void scan_super_block(struct ext2_filsys *fs)
{
    scan_init_super(fs);
    scan_read_super_1KB(fs);
    scan_print_super_block(fs);
    scan_sanity_check(fs);

}

// init the superblock structure that will hold
// information about the superblock
void scan_init_super(struct ext2_filsys *fs)
{
    fs->sb = malloc(sizeof(struct ext2_super_block));
    if (!fs->sb) {
	printf("  ERROR: Can't alloc sb  \n");
	exit(1);
    }
}

// read the superblock
void scan_read_super_1KB(struct ext2_filsys *fs)
{
    ext2_off_t loc;
    ext2_off_t rloc;
    int retval;
    struct ext2_super_block *sb = fs->sb;
    
    // the superblock is always located at byte offset 1024
    // from the beginning of the file image
    loc = (ext2_off_t)(BLOCK_NUMBER_IN_BYTES_SUPER_BLOCK);   // BLOCK_NUMBER_IN_BYTES_SUPER_BLOCK is 1024

    // seek 
    rloc = lseek64(devfd, loc, SEEK_SET);
    if (rloc < 0) {
	printf("  ERROR: can't lseek64 fd=%d loc=%d \n", 
	       devfd, BLOCK_NUMBER_IN_BYTES_SUPER_BLOCK);
	close(devfd);
	exit(1);
    }
    
    // read the superblock, and store it in fs->sb
    retval = read(devfd, sb, SIZE_SUPERBLOCK);
    if (retval < 0) {
	printf("  ERROR: Can't read superblock, error = %d \n", errno);
	close(devfd);
	exit(1);
    }
}



// sanity check, just to make sure that we get the correct
// superblock
void scan_sanity_check(struct ext2_filsys *fs)
{
    struct ext2_super_block *sb = fs->sb;
    
    if (sb->s_inode_size != sizeof(struct ext2_inode)) {
	printf("  ERROR: inode size does not match \n");
	exit(0);
    }
}

// print important information
// some information are not available in superblock,
// hence we need to perform some computation to get
// the information we want
void scan_print_super_block(struct ext2_filsys *fs)
{
    struct ext2_super_block *sb = fs->sb;
    int i = 0;

    // the file system block size (should be 1024 bytes)
    fs->block_size = 1 << (10 + sb->s_log_block_size);

    // how many data pointers (block numbers), we can put in
    // an indirect block. Should be 1024 / 4 = 256 pointers
    fs->data_pointers_per_block = fs->block_size / sizeof(__u32);
    
    // how many block/cylinder groups exist in the system
    fs->gds_count = ((sb->s_blocks_count - sb->s_first_data_block) / 
		     sb->s_blocks_per_group) + 1;
    
    // how many group descriptor structures are stored in a single block
    fs->gds_per_block = fs->block_size / sizeof(struct ext2_group_desc);
    
    // how many blocks are used to store all group descriptors
    fs->gd_blocks_count = (fs->gds_count / fs->gds_per_block) + 1;
    
    // how many inodes stored in an inode table
    // (i.e. how many inodes stored in a block group)
    // you need this to scan the inodes in the inode table
    fs->inodes_per_group = sb->s_inodes_per_group;

    // the size of the inode (should be 128 bytes)
    fs->inode_size = sb->s_inode_size;

    // the total number of inodes in the file system image
    fs->inodes_count = sb->s_inodes_count;

    // how many inode blocks needed to store an inode table in a group.
    // you need this to read an inode table from the image.
    // note that an inode table can span hundreds of blocks.
    fs->inode_blocks_per_group = (fs->inodes_per_group * fs->inode_size / fs->block_size);
    
    // the total number of blocks in the file system image
    fs->blocks_count = sb->s_blocks_count;
    
    
    printf("\t-----------------------------------\n");    
    printf("\tImportant FS Information \n");    
    printf("\t-----------------------------------\n");
    printf("\t  Block size           : %u \n", fs->block_size);
    printf("\t  Data pointers/block  : %u \n", fs->data_pointers_per_block);
    printf("\t  GDs count            : %u \n", fs->gds_count);
    printf("\t  GD-blocks count      : %u \n", fs->gd_blocks_count);    
    printf("\t  Inodes / group       : %u \n", fs->inodes_per_group);
    printf("\t  Inode size           : %d \n", fs->inode_size);    
    printf("\t  Inodes count         : %u \n", fs->inodes_count);    
    printf("\t  Inode blks / group   : %u \n", fs->inode_blocks_per_group);    
    printf("\t  Total blocks         : %u \n", fs->blocks_count);
    printf("\t-----------------------------------\n");
    printf("\n");
}

