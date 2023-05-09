// On-disk file system format.
// Both the kernel and user programs use this header file.


#define ROOTINO  1   // root i-number
#define BSIZE 1024  // block size

// Disk layout:
// [ boot block | super block | log | inode blocks |
//                                          free bit map | data blocks]
//
// mkfs computes the super block and builds an initial file system. The
// super block describes the disk layout:
struct superblock {
  uint magic;        // Must be FSMAGIC
  uint size;         // Size of file system image (blocks)
  uint nblocks;      // Number of data blocks
  uint ninodes;      // Number of inodes.
  uint nlog;         // Number of log blocks
  uint logstart;     // Block number of first log block
  uint inodestart;   // Block number of first inode block
  uint bmapstart;    // Block number of first free map block
};

#define FSMAGIC 0x10203040

#define NDIRECT 12
// 二阶索引块中包含的inode数量
#define NINDIRECT (BSIZE / sizeof(uint))
//12个一阶索引加1个二阶索引, MAXFILE is max number of the biggest file
#define MAXFILE (NDIRECT + NINDIRECT)

// On-disk inode structure
struct dinode {
  short type;           // File type
  short major;          // Major device number, represent type of device (T_DEVICE only)
  short minor;          // Minor device number, represent which device (T_DEVICE only)
  short nlink;          // Number of links to inode in file system, hard links
  uint size;            // Size of file (bytes)
  uint addrs[NDIRECT+1];   // Data block addresses
};

// Inodes per block (一块所能包含的inode数)
#define IPB           (BSIZE / sizeof(struct dinode))

// Block containing inode i(包含inode i的块)
#define IBLOCK(i, sb)     ((i) / IPB + sb.inodestart)

// Bitmap bits per block (一块所能包含的bit数)
#define BPB           (BSIZE*8)

// Block of free map containing bit for block b (第b个data块所在的bit map)
#define BBLOCK(b, sb) ((b)/BPB + sb.bmapstart)

// Directory is a file containing a sequence of dirent structures.(目录名的最大上限)
#define DIRSIZ 14

// 目录项的结构 16byte对齐
struct dirent {
  ushort inum;
  char name[DIRSIZ];
};

