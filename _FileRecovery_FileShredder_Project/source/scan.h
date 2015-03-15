#ifndef __my_scan_h
#define __my_scan_h

#include "main.h"
#include "ext2_types.h"
#include "ext2_structures.h"
#include <sys/types.h>
#include <unistd.h> 

// parameters
#define BLOCK_NUMBER_IN_BYTES_SUPER_BLOCK 1024
#define BLOCK_NUMBER_FIRST_GROUP_DESC 2

#define SIZE_SUPERBLOCK 1024


typedef long long ext2_off_t;


/****************/
// remember that when using threads, I'm not using any of these data
// structures (such as gds_copies, etc.)
struct ext2_filsys
{
    struct ext2_super_block *sb;
    
    __u32   *inodeTableLocations;

    __u32   block_size;       // file system block size
    __u32   gd_blocks_count;  // number of group descriptor blocks
    __u32   gds_count;        // number of group descriptors total in the fs
    __u32   gds_per_block;    // number of group descriptors per block
    __u32   inodes_per_group; // number of valid inodes per group
    __u32   data_pointers_per_block; // number of data pointers per block
    __u32   inode_size;
    __u32   inodes_count;
    __u32   inode_blocks_per_group;
    __u32   blocks_count;
};



// main.c
struct ext2_filsys* scan_init();
void scan_close();
void scan_open(int argc, char **argv);
void scan_scan(struct ext2_filsys *fs);
char * scan_read_block(struct ext2_filsys *fs, __u32 blknum);
  
// super.c
void scan_super_block(struct ext2_filsys *fs);
void scan_init_super(struct ext2_filsys *fs);
void scan_read_super_1KB(struct ext2_filsys *fs);
void scan_sanity_check(struct ext2_filsys *fs);
void scan_print_super_block(struct ext2_filsys *fs);


// gds.c
void scan_group_descriptors(struct ext2_filsys *fs);
char* scan_read_gds(struct ext2_filsys *fs);


#endif


//
