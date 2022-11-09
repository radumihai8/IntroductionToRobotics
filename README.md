# Introduction to Robotics (2022 - 2023)

Introduction to Robotics laboratory homeworks, taken in the 3rd year at the Faculty of Mathematics and Computer Science, University of Bucharest. Each homework includes requirements, implementation details, code and image files.

## Homework 1
##### Components: 
1. RBG  LED  (1  minimum)
2. Potentiometers  (3  minimum)
3. Resistors and wires (per logic)

##### Technical Task:
Use a separate potentiometer in controlling each of the color of the RGB led.  The control must be done with digital electronics, you must read the value of the potentiometer with Arduino, and write a mapped value to each of the pins connected to the led.

##### Setup pictures:
https://imgur.com/a/9RVxIL8

##### Video URL:
https://youtube.com/shorts/4J-0wRpqHRw

## Homework 2
##### Components: 
1. 5 LEDs
2. 1 button
3. 1 buzzer
4. Resistors and wires (per logic)

##### Technical Task:
Building  the  traffic  lights  for  a  crosswalk.  You will use 2 LEDs to represent the traffic lights for people (red and green) and 3 LEDs to represent the traffic lights for cars (red, yellow and green).

The system has the following states:
1. **State 1** (default, reinstated after state 4 ends):  green light for cars,red  light  for  people,  no  sounds.   Duration:  indefinite,  changed  bypressing the button.
2. **State 2** (initiated by counting down 8 seconds after a button press):the  light  should  be  yellow  for  cars,  red  for  people  and  no  sounds.Duration: 3 seconds.
3. **State 3** (initiated after state 2 ends):  red for cars, green for peopleand a beeping sound from the buzzer at a constant interval. Duration: 8 seconds.
4. **State 4** (initiated after state 3 ends):  red for cars,blinking greenfor people and a beeping sound from the buzzer,  at a constant in-terval,  faster than the beeping in state 3.  This state should last 4 seconds.

##### Setup pictures:
https://imgur.com/Yjkr31X

##### Video URL:
https://youtu.be/LLe9BP7FNYA

## Homework 3
##### Components: 
1. 1 7-Segment display
2. 1 Joystick
3. Resistors and wires (per logic)

##### Technical Task:
Use the joystick to control the position ofthe segment and ”draw” on the display.  The movement between segmentsshould be natural (meaning they should jump from the current positiononly to neighbors, but without passing through ”walls”.

The system has the following states:
1. **State 1** (default,  but also initiated after a  button press in State2): Current  position blinking.Can use the joystick to  move from one position to neighbors. Short pressing the  button toggles  state 2. Long pressing the button in state 1 resets the entire display by turning all the segments OFF and moving the current position to the decimal point.
2. **State 2** (initiated  after a button press in State  1): The current segment stops  blinking, adopting the state of the  segment before selection (ON or OFF). Toggling the X (or Y, you chose) axis should change  the  segment  state  from  ON  to OFF or from OFF to ON. Clicking the joystick should save the segment state and exit back to state 1.

##### Setup pictures:
https://imgur.com/UGH8V1O

##### Video URL:
https://youtu.be/8JOLg_aSXKA
