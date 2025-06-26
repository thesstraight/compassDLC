# External Compass DLC for Counter-Strike: Source v92 (6630498) on Linux

Compass DLC that can apply to any Source Engine made game for linux that is 32 bit/i386/i686 if the view angle offset is correct for the given game.

## Showcase

<img src="https://i.ibb.co/mrRwY6x9/showcase.gif"/>

## Installation

1. Clone this repository in ur terminal like so

`$ git clone https://github.com/thesstraight/compassDLC.git`

2. Create a folder outside src and name it include
3. Make 3 more folders in include named glad, GLT, libmem
4. Go to https://glad.dav1d.de/ and under API select gl Version 3.3

* under Profile select Core
* make sure under Options Generate a loader is selected
* download and xtract the zip file and put .h files in glad folder but glad.c in src folder

5. Go to https://github.com/vallentin/glText, download gltext.h and put it in the GLT folder

6. Go to https://github.com/rdbo/libmem/releases download and extract
`libmem-5.0.5-i686-linux-gnu-static.tar.gz` into libmem folder

7. Install glfw

* if u are on 64 bit systems
`$ sudo apt install libglfw3:i386 libglfw3-dev:i386`
* if u are on 32 bit systems
`$ sudo apt install libglfw3 libglfw3-dev`

8. Download the g++ compiler

`$ sudo apt install g++ g++-multilib`

9. Compile with g++

`g++ -o main main.cpp glad.c -L../include/libmem -I../include -std=c++20 -Wextra -Wall -pedantic -O2 -m32 -lGL -lglfw -llibmem`

10. Run the binary as sudo

`$ sudo ./main`

## The end
Hope u have tried it and enjoyed it if so leave a star!