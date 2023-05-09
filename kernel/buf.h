struct buf {
  int valid;   // has data been read from disk?
  int disk;    // does disk "own" buf? if disk = 1, dick is using buf.
  uint dev;
  uint blockno; //which block is?
  struct sleeplock lock;
  uint refcnt;
  struct buf *prev; // LRU cache list
  struct buf *next;
  uchar data[BSIZE];
};

