FileSystem.out : Block.cpp Block.h FileSystem.cpp FileSystem.h FileSystemAPI.cpp FileSystemAPI.h Modules.h Modules.cpp Source.cpp VirtualDisk.cpp VirtualDisk.h
	g++ *.cpp -std=c++11 -o FileSystem.out