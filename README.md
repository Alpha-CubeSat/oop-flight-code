# Alpha CubeSat Flight Code

## Getting Started

* Install [VSCode](https://code.visualstudio.com/download)
* Install [Git](https://www.atlassian.com/git/tutorials/install-git)
    * Make a GitHub account if you do not have one already
* In VSCode, click on "Extensions" on the left hand toolbar. Search for and install PlatformIO IDE.
* On the top toolbar, click "Terminal" and then "New Terminal". This should open a terminal at the bottom of your screen if it was not open already.
* In the terminal, type "git clone https://github.com/Alpha-CubeSat/oop-flight-code.git"
* Now at the bottom of your screen in the blue toolbar you should see a check, arrow, and serial monitor icon (it looks like an electrical cord). If you do not see the blue toolbar make sure you have the cloned git repo folder open.
* If you would just like to compile code but not upload to the teensy, press the check.
* If you would like to upload to the teensy, press the arrow.
* You can also use `pio run -e debug -t upload` to upload code with debugging print statements, or `pio run -e teensy35 -t upload` to upload code without debugging print statements.
* Use `pio test` to run unit tests.
* To view the serial monitor press the electrical cord icon.

## Code Overview

This code is structured based on [Lodestar](https://github.com/shihaocao/lodestar), a small scale electric demonstrator for the belly-flop and tail-sitting control algorithms necessary for SpaceX's Starship.

Both ClockManager and TimedControlTask are from [PAN](https://github.com/pathfinder-for-autonomous-navigation).

### main.cpp
This file is comparable to a .ino file you would see in the Arduino IDE (notice setup and loop are exactly the same as they would be in an Arduino file). 

### MainControlLoop
The MainControlLoop initializes and executes every monitor and control task.

### MissionManager
The CubeSat has 4 different modes with distinct entrance and exit conditions
* Standby 
* Safe
* Low Power
* Deployment

![mission modes](https://github.com/Alpha-CubeSat/oop-flight-code/blob/main/flight_code.png "mission modes")

MissionManager sets which mode the CubeSat should be in based on these exit/entrance conditions.

### SFR
SFR stands for State Field Registry. It contains the mode of each system, all sensor values, and universal flags that should be available to the entire CubeSat. 

### Control Tasks
Control tasks are any file that actuates/performs a task based on a value in the SFR. 

### Monitors
Monitors simply read input from sensors and update the sensor values in the SFR.

### constants.hpp
Constants contains values that will never be changed. This prevents "magic numbers".




