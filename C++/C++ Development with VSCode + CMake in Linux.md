# C++ Development with VSCode + CMake in Linux

[toc]

![VSCodeCMakeLinux](../Media/VSCodeCMakeLinux.png)

## Linux System

Linux is open source operating system

Everything are files in Linux

### 1.1 directory structure

Linux file folder structures

### 1.2 command & options

Linux commands that can be input in the terminal

Command format:

- command (whitespace) [option] (whitespace) [object]
- option and object can be none or multiple

Important commands

```sh
# pwd - print current working directory
pwd
# ls - list directory contents; dir works as well
# ls relative path
ls
ls ./   # [current directory]
ls ../  # [parent directory]
# ls absolute path
ls /home  #
ls /      # root directory
# ls with options
ls -lah /home # l (list) a (all files including hidden) h(high 		visible format)
```

```sh
# cd - change directory
cd
cd ~  # these two equivalent; goes to home direcotry

# cd - relative path
cd ..   	# parent directory
cd ../bin/  # (sister) directory under parent directory
# cd - absolute path
cd /usr/local  # starts with /

# mkdir - make directory
mkdir src
mkdir -p a/b/c  # make multiple layers of non-existent folders
mkdir a b c     # make multiple folders under current directory
```

```sh
# touch - change file timestamps; create file
touch linux.txt
touch ../linux		# create linux file in parent directory
touch /home/wayne/myfile  # create file with absolute path
touch file file.txt	# create multiple files

# rm - remove files or directories
rm file.txt		# remove file.txt in current directory
rm /usr/file01	# remove file01 with absolute path
# remove directory
rm -rf myfolder	 # remove r(recursive) f(force); be cautious
rm -rf /usr/myfolder
```

```sh
# cp - copy files and directories
cp account.txt temp	# copy account.txt file to temp folder
cp -r /home/wayne/myfolder /  # -r recursive copy

# mv - move (rename) files
mv myfile /folder
mv myfolder /tempfolder
mv myfile myfile001		# rename myfile to myfile001
```

```sh
# man - an interface to the system reference manuals
man ls
man cd  # help cd
man man
```

```sh
# reboot - reboot the machine
reboot
# shutdown - power-off the machine
shutdown -h now
```



### 1.3 file editing

- Vim
  - Vim is the standard editor in Unix, Linux
  - Vim can be used for software development, as well as file editing
  - very powerful in terminal
- gedit
- nano



## Setup Development Environment

### 2.1 compiler, debugger, CMake installation

- Install GCC, GDB

```sh
sudo apt update
# install compiler and debugger
sudo apt install build-essential gdb
```

```sh
# confirm installation 
gcc --version
g++ --version
gdb --version
```

```sh
# clang
clang --version
```



- Install CMake

```sh
sudo apt install cmake
```

```sh
# confirm installation
cmake --version
```



## GCC Compiler

- GCC compiler support C++, Go etc.
- VSCode calls GCC compiler to compile C/C++ code
- gcc compiles C; g++ compiles C++

### 3.1 compiling process

1. Pre-processing

   ```sh
   # -E option instructs compiler to pre-process the input file
   g++ -E test.cpp -o test.i  #.i file
   # view - optional
   vi test.i
   ```

2. Compiling

   ```sh
   # -S compiler instructs g++ to stop compiling after generating assembly
   # the default extension for assembly is .s
   g++ -S test.i -o test.s
   ```

3. Assembling

   ```sh
   # -c option instructs g++ compiles assembly to objective code of the machine language
   # by default, the objective code created by g++ will have .o extension
   g++ -c test.s -o test.o  # -c or -C the same
   ```

4. Linking

   ```sh
   # -o option generates the executable code
   g++ test.o -o test
   ```

5. All steps can be combined

   ```sh
   g++ test.cpp -o test
   # or
   c++ test.cpp -o test  # use default c++ compiler
   # check all files
   ls -lah
   ```

### 3.2 g++ compiling arguments

1. -g compile executable with debugging information

   ```sh
   # -g option instructs GCC to create debugging information that could be used by GDB debugger
   g++ -g hello.cpp -o hello  # the hello file will be much bigger than the versionw without -g
   ```

2. -O[n]  optimize the code

   ```sh
   # optimize the source code durign compilation
   # generally will make the code run faster
   # -o reduces the code length and run-time, equivalent to -o1
   # -o0 no optimization
   # -o1 default optimization
   # -o2 extra optimization besides o1
   # -o3 extra optimization
   # option will make the compilation slower than -o but running faster
   g++ -o2 hello.cpp  # -o2 is ususally good enough
   # timing the efficiency
   time ./test_wtO
   time ./test_wO
   ```

3. -l, -L specify library file | library file path

   ```sh
   # -l argument (lower) is used to specify the library to link
   # the libraries in /lib, /usr/lib, /usr/local/lib can be linked direclty with -l
   
   # e.g. link glog lib
   g++ -lglog test.cpp
   
   # if the library files are not within the 3 directories listed above, need use -L (upper) to specify the libary file directory
   
   # e.g. link mytest library, libmytest.so is in /home/wayne/mytestlibfolder directory
   g++ -L/home/wayne/mytestlibfolder -lmytest test.cpp
   ```

4. -I specify header file search directory

   ```sh
   # -I
   # /usr/include directory not need to be specified, gcc will search there. If the header files # are not found there, we need specify it with -I/myincldue e.g.
   # -I argument can use relative path
   
   g++ -I/myclude test.cpp
   ```

5. -Wall print warning information

   ```sh
   # print gcc provided warning info
   g++ -Wall test.cpp
   ```

6. -w lose warning information

   ```sh
   # close all warning info
   g++ -w test.cpp
   ```

7. -std=c++11 setup compile standard

   ```sh
   # use c++11 standard
   g++ -std=c++11 test.cpp
   ```

8. -o specify output file name

   ```sh
   #specify the executable name being test
   g++ test.cpp -o test
   ```

9. -D define macro

   ```sh
   # define macro when using gcc/g++
   # commonly used
   # - DEBUG define DEBUG macro, use this to open or close the DEBUG info in the code
   ```

   ```c++
   // -Dname: define macro name, the default content is string "1"
   #include <iostream>
   
   int main() {
       #ifdef DEBUG
           std::cout << "DEBUG log:\n";
       #endif
       	std::cout << "hello there\n";
       return 0;
   }
   
   // to show the debug info, compile with g++ -DDEBUG test.cpp
   ```

notice we can use "man gcc" to check the usage manual

### 3.3 g++ in terminal

```sh
# directly compile and run
# include "swap.h" to use "swap.cpp"
g++ main.cpp src/swap.cpp -Iinclude -o test
./test
```

#### generate library file and linking to executable

link static library



## GDB Debugger

### 4.1 common debugging arguments

### 4.2 terminal debugging

## IDE - VSCode

### 5.1 interface

### 5.2 install plugin

### 5.3 shortcuts

### 5.4 practice

## CMake

### 6.1 cross-platform development

### 6.2 syntax

### 6.3 important commands and CMake values

### 6.4 CMake compile project

### 6.5 practice

## Project Development using VSCode

### 7.1 setup project directory

### 7.2 write source code

### 7.3 write CMakeLists.txt for compiling rules

### 7.4 compile CMake project

### 7.5 configure VSCode json file and debug project

