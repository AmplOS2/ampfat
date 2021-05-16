# ampfat

An open source FAT16/32 filesystem driver for new operating systems.
This library is licensed under a BSD license, you may use it under the terms found in COPYING.

## Structure

The filesystem driver itself is contained within the a single source file ``ampfat.cc``.  This
relies upon a generic block driver which can provide a 512 byte block from the volume containing
the filesystem.  This driver may for example request a block from an SD card.  See ``block.hh`` for
the common function definitions that must be provided by the block driver.

There is one example block drivers in the ``src/block_driver`` folder: ``block_pc.c`` is an
implementation that uses a local file. It also contains some tools to snapshot and generate MD5
hashes for testing.

TODO: Document necessary steps for porting.

There is also a handler for MBR type primary partition tables in ``partition.c`` which can be used
in an embedded system to identify partitions within a volume.

## History

Gristle has been developed for the [OggBox](http://oggbox.nathandumont.com) project and as such 
has focused on fast file reading and has some unusual optimisations (seeking backwards in a file is 
particularly fast for example) which are to do with opperations on media files (in that case finding
the play time of an ogg file).  Some effort has been made to do unit testing using the PC driver
which is the basis of the writing routines, but this is still sparse.

ampfat is a port of Gristle to C++ for use in [AmplOS](https://github.com/AmplOS2/AmplOS2).
