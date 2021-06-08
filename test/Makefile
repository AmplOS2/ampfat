CXX ?= c++
CXXFLAGS += -Wall -Wextra -g -Os -I. -I../src -I../block_drivers

all: test_ampfat test_embext show_info

test_ampfat: test_ampfat.cc hash.cc hash.hh ../block_drivers/block_pc.cc ../block_drivers/block_pc.hh \
             ../src/ampfat.cc ../src/ampfat.hh ../src/partition.cc ../src/partition.hh Makefile
	$(CXX) $(CXXFLAGS) test_ampfat.cc hash.cc ../block_drivers/block_pc.cc ../src/ampfat.cc ../src/partition.cc -o test_ampfat

test_embext: test_embext.cc ../src/embext.cc ../block_drivers/block_pc.cc hash.cc ../src/embext.hh \
             ../block_drivers/block_pc.hh hash.hh Makefile
	$(CXX) $(CXXFLAGS) -DEXT_DEBUG test_embext.cc ../src/embext.cc ../block_drivers/block_pc.cc hash.cc -o test_embext

show_info: show_info.cc ../src/block.hh ../block_drivers/block_pc.cc ../block_drivers/block_pc.hh ../src/ampfat.cc \
           ../src/ampfat.hh ../src/partition.cc ../src/partition.hh Makefile
	$(CXX) $(CXXFLAGS) show_info.cc hash.cc ../block_drivers/block_pc.cc ../src/ampfat.cc ../src/partition.cc -o show_info

