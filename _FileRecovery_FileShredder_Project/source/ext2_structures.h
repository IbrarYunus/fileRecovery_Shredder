
#ifndef __my_ext2_structures_h
#define __my_ext2_structures_h


#define EXT2_NDIR_BLOCKS                12
#define EXT2_IND_BLOCK                  EXT2_NDIR_BLOCKS
#define EXT2_DIND_BLOCK                 (EXT2_IND_BLOCK + 1)
#define EXT2_TIND_BLOCK                 (EXT2_DIND_BLOCK + 1)
#define EXT2_N_BLOCKS                   (EXT2_TIND_BLOCK + 1)


#define LINUX_S_IFMT   00170000
#define LINUX_S_IFSOCK 0140000
#define LINUX_S_IFLNK  0120000
#define LINUX_S_IFREG  0100000
#define LINUX_S_IFBLK  0060000
#define LINUX_S_IFDIR  0040000
#define LINUX_S_IFCHR  0020000
#define LINUX_S_IFIFO  0010000
#define LINUX_S_ISUID  0004000
#define LINUX_S_ISGID  0002000
#define LINUX_S_ISVTX  0001000

#define LINUX_S_IRWXU 00700
#define LINUX_S_IRUSR 00400
#define LINUX_S_IWUSR 00200
#define LINUX_S_IXUSR 00100

#define LINUX_S_IRWXG 00070
#define LINUX_S_IRGRP 00040
#define LINUX_S_IWGRP 00020
#define LINUX_S_IXGRP 00010

#define LINUX_S_IRWXO 00007
#define LINUX_S_IROTH 00004
#define LINUX_S_IWOTH 00002
#define LINUX_S_IXOTH 00001


#define EXT2_BAD_INO             1      /* Bad blocks inode */
#define EXT2_ROOT_INO            2      /* Root inode */
#define EXT2_ACL_IDX_INO         3      /* ACL inode */
#define EXT2_ACL_DATA_INO        4      /* ACL inode */
#define EXT2_BOOT_LOADER_INO     5      /* Boot loader inode */
#define EXT2_UNDEL_DIR_INO       6      /* Undelete directory inode */
#define EXT2_RESIZE_INO          7      /* Reserved group descriptors inode */
#define EXT2_JOURNAL_INO         8      /* Journal inode */



#define LINUX_S_ISLNK(m)        (((m) & LINUX_S_IFMT) == LINUX_S_IFLNK)
#define LINUX_S_ISREG(m)        (((m) & LINUX_S_IFMT) == LINUX_S_IFREG)
#define LINUX_S_ISDIR(m)        (((m) & LINUX_S_IFMT) == LINUX_S_IFDIR)
#define LINUX_S_ISCHR(m)        (((m) & LINUX_S_IFMT) == LINUX_S_IFCHR)
#define LINUX_S_ISBLK(m)        (((m) & LINUX_S_IFMT) == LINUX_S_IFBLK)
#define LINUX_S_ISFIFO(m)       (((m) & LINUX_S_IFMT) == LINUX_S_IFIFO)
#define LINUX_S_ISSOCK(m)       (((m) & LINUX_S_IFMT) == LINUX_S_IFSOCK)


/* ***********************************
 * Inode type
 * ***********************************/

#define EXT2_FT_UNKNOWN         0
#define EXT2_FT_REG_FILE        1
#define EXT2_FT_DIR             2
#define EXT2_FT_CHRDEV          3
#define EXT2_FT_BLKDEV          4
#define EXT2_FT_FIFO            5
#define EXT2_FT_SOCK            6
#define EXT2_FT_SYMLINK         7
#define EXT2_FT_MAX             8




#define EXT2_DIR_PAD                    4
#define EXT2_DIR_ROUND                  (EXT2_DIR_PAD - 1)
#define EXT2_DIR_REC_LEN(name_len)      (((name_len) + 8 + EXT2_DIR_ROUND) & ~EXT2_DIR_ROUND)



// #define EXT2_I_SIZE(i)  ((i)->i_size | ((__u64) (i)->i_size_high << 32))




struct ext2_group_desc
{
    __u32   bg_block_bitmap;        /* Blocks bitmap block */
    __u32   bg_inode_bitmap;        /* Inodes bitmap block */
    __u32   bg_inode_table;         /* Inodes table block */
    __u16   bg_free_blocks_count;   /* Free blocks count */
    __u16   bg_free_inodes_count;   /* Free inodes count */
    __u16   bg_used_dirs_count;     /* Directories count */
    __u16   bg_flags;
    __u32   bg_reserved[2];
    __u16   bg_itable_unused;       /* Unused inodes count */
    __u16   bg_checksum;            /* crc16(s_uuid+grouo_num+group_desc)*/
};




struct ext2_super_block {
    __u32   s_inodes_count;         /* Inodes count */
    __u32   s_blocks_count;         /* Blocks count */
    __u32   s_r_blocks_count;       /* Reserved blocks count */
    __u32   s_free_blocks_count;    /* Free blocks count */
    __u32   s_free_inodes_count;    /* Free inodes count */
    __u32   s_first_data_block;     /* First Data Block */
    __u32   s_log_block_size;       /* Block size */
    __s32   s_log_frag_size;        /* Fragment size */
    __u32   s_blocks_per_group;     /* # Blocks per group */
    __u32   s_frags_per_group;      /* # Fragments per group */
    __u32   s_inodes_per_group;     /* # Inodes per group */
    __u32   s_mtime;                /* Mount time */
    __u32   s_wtime;                /* Write time */
    __u16   s_mnt_count;            /* Mount count */
    __s16   s_max_mnt_count;        /* Maximal mount count */
    __u16   s_magic;                /* Magic signature */
    __u16   s_state;                /* File system state */
    __u16   s_errors;               /* Behaviour when detecting errors */
    __u16   s_minor_rev_level;      /* minor revision level */
    __u32   s_lastcheck;            /* time of last check */
    __u32   s_checkinterval;        /* max. time between checks */
    __u32   s_creator_os;           /* OS */
    __u32   s_rev_level;            /* Revision level */
    __u16   s_def_resuid;           /* Default uid for reserved blocks */
    __u16   s_def_resgid;           /* Default gid for reserved blocks */
    __u32   s_first_ino;            /* First non-reserved inode */
    __u16   s_inode_size;           /* size of inode structure */
    __u16   s_block_group_nr;       /* block group # of this superblock */
    __u32   s_feature_compat;       /* compatible feature set */
    __u32   s_feature_incompat;     /* incompatible feature set */
    __u32   s_feature_ro_compat;    /* readonly-compatible feature set */
    __u8    s_uuid[16];             /* 128-bit uuid for volume */
    char    s_volume_name[16];      /* volume name */
    char    s_last_mounted[64];     /* directory where last mounted */
    __u32   s_algorithm_usage_bitmap; /* For compression */
    __u8    s_prealloc_blocks;      /* Nr of blocks to try to preallocate*/
    __u8    s_prealloc_dir_blocks;  /* Nr to preallocate for dirs */
    __u16   s_reserved_gdt_blocks;  /* Per group table for online growth */
    __u8    s_journal_uuid[16];     /* uuid of journal superblock */
    __u32   s_journal_inum;         /* inode number of journal file */
    __u32   s_journal_dev;          /* device number of journal file */
    __u32   s_last_orphan;          /* start of list of inodes to delete */
    __u32   s_hash_seed[4];         /* HTREE hash seed */
    __u8    s_def_hash_version;     /* Default hash version to use */
    __u8    s_jnl_backup_type;      /* Default type of journal backup */
    __u16   s_desc_size;            /* Group desc. size: INCOMPAT_64BIT */
    __u32   s_default_mount_opts;
    __u32   s_first_meta_bg;        /* First metablock group */
    __u32   s_mkfs_time;            /* When the filesystem was created */
    __u32   s_jnl_blocks[17];       /* Backup of the journal inode */
    __u32   s_blocks_count_hi;      /* Blocks count high 32bits */
    __u32   s_r_blocks_count_hi;    /* Reserved blocks count high 32 bits*/
    __u32   s_free_blocks_hi;       /* Free blocks count */
    __u16   s_min_extra_isize;      /* All inodes have at least # bytes */
    __u16   s_want_extra_isize;     /* New inodes should reserve # bytes */
    __u32   s_flags;                /* Miscellaneous flags */
    __u16   s_raid_stride;          /* RAID stride */
    __u16   s_mmp_interval;         /* # seconds to wait in MMP checking */
    __u64   s_mmp_block;            /* Block for multi-mount protection */
    __u32   s_raid_stripe_width;    /* blocks on all data disks (N*stride)*/
    __u32   s_reserved[163];        /* Padding to the end of the block */
};


struct ext2_inode {
    __u16   i_mode;         /* File mode */
    __u16   i_uid;          /* Low 16 bits of Owner Uid */
    __u32   i_size;         /* Size in bytes */
    __u32   i_atime;        /* Access time */
    __u32   i_ctime;        /* Inode change time */
    __u32   i_mtime;        /* Modification time */
    __u32   i_dtime;        /* Deletion Time */
    __u16   i_gid;          /* Low 16 bits of Group Id */
    __u16   i_links_count;  /* Links count */
    __u32   i_blocks;       /* Blocks count */
    __u32   i_flags;        /* File flags */
    union {
	struct {
	    __u32   l_i_version; /* was l_i_reserved1 */
	} linux1;
	struct {
	    __u32  h_i_translator;
	} hurd1;
	struct {
	    __u32  m_i_reserved1;
	} masix1;
    } osd1;                         /* OS dependent 1 */
    __u32   i_block[EXT2_N_BLOCKS];/* Pointers to blocks */
    __u32   i_generatin;   /* File version (for NFS) */
    __u32   i_file_acl;     /* File ACL */
    __u32   i_dir_acl;      /* Directory ACL */
    __u32   i_faddr;        /* Fragment address */
    union {
	struct {
	    __u8    l_i_frag;       /* Fragment number */
	    __u8    l_i_fsize;      /* Fragment size */
	    __u16   i_pad1;
	    __u16   l_i_uid_high;   /* these 2 fields    */
	    __u16   l_i_gid_high;   /* were reserved2[0] */
	    __u32   l_i_reserved2;
	} linux2;
	struct {
	    __u8    h_i_frag;       /* Fragment number */
	    __u8    h_i_fsize;      /* Fragment size */
	    __u16   h_i_mode_high;
	    __u16   h_i_uid_high;
	    __u16   h_i_gid_high;
	    __u32   h_i_author;
	} hurd2;
	struct {
	    __u8    m_i_frag;       /* Fragment number */
	    __u8    m_i_fsize;      /* Fragment size */
	    __u16   m_pad1;
	    __u32   m_i_reserved2[2];
	} masix2;
    } osd2;                         /* OS dependent 2 */
};


#define EXT2_NAME_LEN 255
struct ext2_dir_entry {
    __u32   inode;                  /* Inode number */
    __u16   rec_len;                /* Directory entry length */
    __u16   name_len;               /* Name length */
    char    name[EXT2_NAME_LEN];    /* File name */
};



#endif
