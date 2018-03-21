# FAT File System

This program implements a easy version of FAT32 file system.

## FAT image strcut desgin

The design of FAT image is kind of minimized:

| Bytes                        | Usage                                                            |
| ---------------------------- | ---------------------------------------------------------------- |
| 4                            | magic                                                            |
| 4                            | number of block                                                  |
| 4                            | number of FAT                                                    |
| 4                            | block size                                                       |
| 4                            | start block at (root)                                            |
| 4076                         | empty                                                            |
| (k + 1) * 4                  | FAT blocks; these 4 byte represents the super block and FAT (-1) |
| 4096 * (k + 1) - (k + 1) * 4 | rest of FAT blocks (0)                                           |
| 24                           | name of root dir-entry ('.')                                     |
| 8                            | ctime of root dir-entry                                          |
| 8                            | mtime of root dir-entry                                          |
| 8                            | atime of root dir-entry                                          |
| 4                            | file len in bytes                                                |
| 4                            | start block                                                      |
| 4                            | flags; least-sign bit represent is it a directory                |
| 4                            | used byte                                                        |
| 64                           | data for '..'; same struct like '.'                              |
| 4096 - 128                   | initially empty reserved space for root dir block                |
| (N - 1) * 4096               | rest of data blocks                                              |

## Utils

Utilities provides basic functions to manipulate string path....

| function | type                                            | usage                                                                            |
| -------- | ----------------------------------------------- | -------------------------------------------------------------------------------- |
| last_idx | (path: char*): int                              | just an impl of last index of / (slash) in the input                             |
| path_par | (str: char*, parent: char*, child: char*): void | split the input str into parent and child, e.g. /a/b/c -> parent: /a/b, child: c |

## Block

The fs uses 4KB blocks. These functions uses the blocks number to calculate block actuall offset and then use lseek.

The program handle blocks by such opertions:

| function             | type                                                            | usage                                                                                      |
| -------------------- | --------------------------------------------------------------- | ------------------------------------------------------------------------------------------ |
| fat_block_read       | (block_num: int, buf: void*): void                              | seek and read the block                                                                    |
| fat_block_write      | (block_num: int, buf: void*, size: size_t, offset: off_t): void | seek and write block with buffer                                                           |
| fat_block_next       | (block_num: int): int                                           | find the next block of this block                                                          |
| fat_block_alloc      | (): int                                                         | allocated a free block and return it                                                       |
| fat_block_next_alloc | (block_number: int): int                                        | find the next block of this block; if current block is the last block, allocate one for it |

## Entry

The program use `struct fat_entry` as `fat_entry_t` to represent the dir entry.

| function         | type                                                              | usage                                                                                                                                                         |
| ---------------- | ----------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| fat_entry_alloc  | (name: char*, entry: fat_entry_t, flags: int): off_t              | allocate a new entry to the input entry, return allocated offset of new entry in disk image                                                                   |
| fat_entry_open   | (path: char*, dir: bool, alloc: bool): fat_entry_t *              | return the opened entry by path; the param "alloc" decides whether the entry should be allocated if it not exists; param dir decide if it's a dir to allocate |
| fat_entry_close  | (entry: fat_entry_t *): int                                       | close the opened entry                                                                                                                                        |
| fat_entry_write  | (entry: fat_entry_t *, buf: char*, size: size_t, off: off_t): int | write the content of buffer to the entry                                                                                                                      |
| fat_entry_read   | (entry: fat_entry_t *, buf: char*, size: size_t, off: off_t): int | read the content from entry to the buffer                                                                                                                     |
| fat_entry_delete | (entry: fat_entry_t *): bool                                      | clear the content of entry out and delete an entry                                                                                                            |
| fat_entry_rename | (block_num: int): int                                             | rename or move the entry to the other path                                                                                                                    |


## Fuse

The program hook fuse in fat_fuse.c. The operations in it are basically, call the entry level functions

