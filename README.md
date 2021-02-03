# Alpha CubeSat Flight Code

## Getting Started

* Install [VSCode](https://code.visualstudio.com/download)
* Install [Git](https://www.atlassian.com/git/tutorials/install-git)
    * Make a GitHub account if you do not have one already
* In VSCode, click on "Extensions" on the left hand toolbar. Search for and install PlatformIO IDE.
* On the top toolbar, click "Terminal" and then "New Terminal". This should open a terminal at the bottom of your screen if it was not open already.
* In the terminal, type "git clone" https://github.com/Alpha-CubeSat/oop-flight-code.git
* Now at the bottom of your screen in the blue toolbar you should see a check, arrow, and serial monitor icon (it looks like an electrical cord).
* If you would just like to compile code but not upload to the teensy, press the check.
* If you would like to upload to the teensy, press the arrow.
* To view the serial monitor press the electrical cord icon.

## Code Overview

This code is structed based on [Lodestar](https://github.com/shihaocao/lodestar), a small scale electric demonstrator for the belly-flop and tail-sitting control algorithms necessary for SpaceX's Starship.

### main.cpp
This file is comparable to a .ino file you would see in the Arduino IDE (notice setup and loop are exactly the same). 

### MainControlLoop
This file initializes and executes every monitor and control task.

### MissionManager
The CubeSat has 4 different modes with distinct entrance and exit conditions
* Standby 
* Safe
* Low Power
* Deployment

![mission modes](https://github.com/Alpha-CubeSat/oop-flight-code/blob/main/flight_code.png "mission modes")




