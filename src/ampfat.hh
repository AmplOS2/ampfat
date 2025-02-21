#pragma once

#include <stdint.h>
#include "block.hh"
#include "dirent.hh"

#define FAT_BAD_PATH 255

#define FAT_MAX_OPEN_FILES 4
#define FAT_MAX_PATH_LEN 256

#define FAT_ERROR_CLUSTER 1
#define FAT_END_OF_FILE 2

/* FAT file attribute bit masks */
#define FAT_ATT_RO  0x01
#define FAT_ATT_HID 0x02
#define FAT_ATT_SYS 0x04
#define FAT_ATT_VOL 0x08
#define FAT_ATT_SUBDIR 0x10
#define FAT_ATT_ARC 0x20
#define FAT_ATT_DEV 0x40

struct fat_info {
  uint8_t   read_only;
  uint8_t   fat_entry_len;
  uint32_t  end_cluster_marker;
  uint8_t   sectors_per_cluster;
  uint32_t  cluster0;
  uint32_t  active_fat_start;
  uint32_t  sectors_per_fat;
  uint32_t  root_len;
  uint32_t  root_start;
  uint32_t  root_cluster;
  uint8_t   type;               // type of filesystem (FAT16 or FAT32)
  blockno_t part_start;         // start of partition containing filesystem
  uint32_t  total_sectors;
  uint8_t   sysbuf[512];
};

typedef struct {
  uint8_t   jump[3];
  char      name[8];
  uint16_t  sector_size;
  uint8_t   cluster_size;
  uint16_t  reserved_sectors;
  uint8_t   num_fats;
  uint16_t  root_entries;
  uint16_t  total_sectors;
  uint8_t   media_descriptor;
  uint16_t  sectors_per_fat;
  uint16_t  sectors_per_track;
  uint16_t  number_of_heads;
  uint32_t  partition_start;
  uint32_t  big_total_sectors;
  uint8_t   drive_number;
  uint8_t   current_head;
  uint8_t   boot_sig;
  uint32_t  volume_id;
  char      volume_label[11];
  char      fs_label[8];
} __attribute__((__packed__)) boot_sector_fat16;

typedef struct {
  uint8_t   jump[3];                   /*    0 */
  char      name[8];                   /*    3 */
  uint16_t  sector_size;               /*    B */
  uint8_t   cluster_size;              /*    D */
  uint16_t  reserved_sectors;          /*    E */
  uint8_t   num_fats;                  /*   10 */
  uint16_t  root_entries;              /*   11 */
  uint16_t  total_sectors;             /*   13 */
  uint8_t   media_descriptor;          /*   15 */
  uint16_t  short_sectors_per_fat;     /*   16 */
  uint16_t  sectors_per_track;         /*   18 */
  uint16_t  number_of_heads;           /*   1A */
  uint32_t  partition_start;           /*   1C */
  uint32_t  big_total_sectors;         /*   20 */
  uint32_t  sectors_per_fat;           /*   24 */
  uint16_t  fat_flags;                 /*   28 */
  uint16_t  version;                   /*   2A */
  uint32_t  root_start;                /*   2C */
  uint16_t  fs_info_start;             /*   30 */
  uint16_t  boot_copy;                 /*   32 */
  char      reserved[12];              /*   34 */
  uint8_t   drive_number;              /*   40 */
  uint8_t   current_head;              /*   41 */
  uint8_t   boot_sig;                  /*   42 */
  uint32_t  volume_id;                 /*   43 */
  char      volume_label[11];          /*   47 */
  char      fs_label[8];               /*   52 */
} __attribute__((__packed__)) boot_sector_fat32;

#define FS_INFO_SIG1 0x0000
#define FS_INFO_SIG2 0x01E4
#define FREE_CLUSTERS 0x01E8
#define LAST_ALLOCATED 0x01EC

typedef struct {
  char      filename[8];
  char      extension[3];
  uint8_t   attributes;
  uint8_t   reserved;
  uint8_t   create_time_fine;
  uint16_t  create_time;
  
  uint16_t  create_date;
  uint16_t  access_date;
  uint16_t  high_first_cluster;
  uint16_t  modified_time;
  uint16_t  modified_date;
  uint16_t  first_cluster;
  uint32_t  size;
} __attribute__((__packed__)) direntS;

typedef struct {
  uint8_t   flags;
  uint8_t   buffer[512];
  uint32_t  sector;
  uint32_t  cluster;
  uint8_t   sectors_left;
  uint16_t  cursor;
  uint8_t   error;
  char      filename[8];
  char      extension[3];
  uint8_t   attributes;
  size_t    size;
  uint32_t  full_first_cluster;
  uint32_t  entry_sector;
  uint8_t   entry_number;
  uint32_t  parent_cluster;
  uint32_t  file_sector;
  time_t    created;
  time_t    modified;
  time_t    accessed;
} FileS;

// flag values for FileS
#define FAT_FLAG_OPEN 1
#define FAT_FLAG_READ 2
#define FAT_FLAG_WRITE 4
#define FAT_FLAG_APPEND 8
#define FAT_FLAG_DIRTY 16
#define FAT_FLAG_FS_DIRTY 32

#define FAT_INTERNAL_CALL 4242

// int sdfat_lookup_path(int, const char *);
// int sdfat_next_sector(int fd);

int str_to_fatname(char *url, char *dosname);

int fat_mount(blockno_t start, blockno_t volume_size, uint8_t part_type_hint);

// POSIX open, but with an `rerrno` parameter for getting errors
int fat_open(const char *name, int flags, int mode, int *rerrno);

int fat_close(int fd, int *rerrno);
int fat_read(int, void *, size_t, int *);
int fat_write(int, const void *, size_t, int *);
int fat_fstat(int, struct stat *, int *);
int fat_lseek(int, int, int, int *);
int fat_get_next_dirent(int, struct dirent *, int *rerrno);

int fat_unlink(const char *path, int *rerrno);
int fat_rmdir(const char *path, int *rerrno);
int fat_mkdir(const char *path, int mode, int *rerrno);
