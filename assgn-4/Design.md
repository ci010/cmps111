
## FAT image strcut desgin


| Bytes          | Usage                                                        |
| -------------- | ------------------------------------------------------------ |
| 4              | magic                                                        |
| 4              | number of block                                              |
| 4              | number of FAT                                                |
| 4              | block size                                                   |
| 4              | start block at (root)                                        |
| 4076           | empty                                                        |
| 4              | first FAT block; this 4 byte represents the super block (-1) |
| 4096 * k - 4   | rest of FAT blocks (-1); this stands for rest of FAT table   |
| 24             | name of root dir-entry ('.')                                 |
| 8              | ctime of root dir-entry                                      |
| 8              | mtime of root dir-entry                                      |
| 8              | atime of root dir-entry                                      |
| 4              | file len in bytes                                            |
| 4              | start block                                                  |
| 4              | flags; least-sign bit represent is it a directory            |
| 4              | used byte                                                    |
| 64             | data for '..'; same struct like '.'                          |
| 4096 - 128     | initially empty reserved space for root dir block            |
| (N - 1) * 4096 | rest of data blocks                                          |

## Helper functions

| function | type                             | usage                             |
| -------- | -------------------------------- | --------------------------------- |
| fat_seek | (device: int, nblock: int): void | seek block by 4096 * block_number |