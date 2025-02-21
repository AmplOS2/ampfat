#pragma once
#include "block.hh"
#include <stdint.h>

/**
 * \defgroup PARTITION_TABLE_ADDRESSES Offset into the volume for each partition table entry
 * @{
 **/
#define PARTITION0START 0x1BE
#define PARTITION1START 0x1CE
#define PARTITION2START 0x1DE
#define PARTITION3START 0x1EE
/**
 * @}
 **/

/**
 * \defgroup PARTITION_TYPES List of partition type codes
 * @{
 **/
/** FAT16 partitions should be labelled as 0x06 */
#define PART_TYPE_FAT16 0x06
/** FAT32 partitions should be labelled as 0x0B */
#define PART_TYPE_FAT32 0x0B

/**
 * @}
 **/

// an MBR entry structure, because this is based on actual on-disk structure, it must be packed
/**
 * \brief The structure of a Master Boot Record partition table entry
 * 
 * This is defined as part of a DOS formatted disk but is common on memory cards, USB memory sticks
 * and hard disks alike.  There are 4 of these partition definitions in the record describing the
 * four primary partitions on the disk.  Note that in most modern systems only logical block
 * addressing (LBA) is used so the cylinder/head/sector addresses are ignored.  The block size for
 * LBA addressed volumes must match #BLOCK_SIZE in your block driver.
 * 
 * Because this is an on-disk structure the __packed__ attribute must be set to stop the compiler
 * re-arranging the structure components to align them with your architecture's memory alignment.
 **/
typedef struct {
  uint8_t  bootable;    /** 0x80 means bootable, 0x00 means un-bootable all others invalid */
  uint8_t  chs[3];      /** Cylinder/head/sector address of partition (not used in modern disks) */
  uint8_t  type;        /** Partition type, 0x0B = FAT32 or 0x06 = FAT16 others not understood */
  uint8_t  chs_end[3];  /** Cylinder/head/sector address for end of partition */
  uint32_t lba_start;   /** Logical Block Address of the start of the partition */
  uint32_t length;      /** Number of blocks in the partition */
} __attribute__((__packed__)) mbr_entry;

/**
 * \brief this struct has the essentials for in memory lists of partitions.
 * 
 * Since the partition table contains a lot of superfluous information and the structure is not
 * optimal for fast access on most systems a much smaller list of critical information is stored
 * in this struct for passing between functions.
 **/
struct partition {
  blockno_t start;
  blockno_t length;
  uint8_t type;
};

/**
 * \brief Reads and validates the partition table from an in-memory copy of the MBR
 * 
 * Returns a list of the partitions read from the MBR.  The MBR should already be in memory and is
 * expected to be a full 512 byte block.  The total size of the physical volume should be passed to
 * this function as it is used for validating the partitions read and can save trying to read or
 * write a corrupt partition.  A pointer to the list of partitions is returned but is only valid
 * until a subsequent call to read_partition_table().
 * 
 * \param mbr is a pointer to the first 512 bytes read from the volume in memory.
 * \param volume_size is the total size in blocks of the volume for validating the table.
 * \param retlist is a pointer to a list of struct partition that will be assigned before return
 * \returns the number of valid partitions found 0 = none, 4 = all four are valid
 **/
int read_partition_table(uint8_t *mbr, blockno_t volume_size, struct partition **retlist);
