Projectname=Rlsnake
DNAME=build
all:
	@echo "==> Builiding $(Projectname)"
	cmake -G Ninja . && ninja
clean:
	rm -rf CMakeCache.txt  cmake_install.cmake build.ninja 
	rm -rf CMakeFiles/
	rm -rf RLsnake

