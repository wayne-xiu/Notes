# Modern C++ for Computer Vision and Image Processing

[toc]

https://www.youtube.com/watch?v=F_vIB3yjxaM&list=PLgnQpQtFTOGR50iIOtO36nK6aNPtVq98C

2018 and 2020 versions

http://www.ipb.uni-bonn.de/teaching/modern-cpp/

https://www.ipb.uni-bonn.de/teaching/cpp-2020/

## Course Introduction and Hello World

> Talk is cheap. Show me the code. - Linus Torvalds

Linux directory tree

![linuxDirectoryTree](../Media/linuxDirectoryTree.png)

- Tree organization starting with root: /
- there are no volume letters, e.g. C:, D:
- user can only access his/her own folder

Understanding files and folders

- Folders end with / e.g. /path/folder/
- Everything else are files, e.g. /path/file
- Absolute paths start with / while all other paths are relative
  - /home/folder/  - absolute
  - folder/filer  - relative
- Paths are case sensitive: filename is different from FileName
- extension is part of a name

Linux terminal

- Terminal is faster

- Terminal is always in some folder

- pwd

- cd <dir>

- ls <dir>

- special folders:

  - / - root folder
  - ~ - home folder
  - . - current folder
  - .. - parent folder

- --help

- mkdir

- rm -[r]f: remove [recursive] [force]

- cp [-r] <source><dest> - copy

- mv <source> <dest> - move

- Using placeholders: can be used with most terminal commands: ls, rm, mv

  - \* : Any set of characters

  - ? : Any single character

  - [a-f]: characters in [abcdef]

  - [^a-c]: characters not in [abc]

Standard input/output channels

- stdin
- stdout
- stderr

Working with files

- cat <filename>  -- cool
  - print the contents of the file in the terminal
- grep <what> <where>  -- insanely powerful (probably the most)
  - search for a string <what> in a file <where>

![linuxGrepDemo](../Media/linuxGrepDemo.png)



