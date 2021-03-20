#!/bin/bash
window=$(xdotool getactivewindow)
#window=$(xdotool search  --onlyvisible --class "gedit") #--class
echo $window
xdotool windowactivate $window keydown Shift+Insert
sleep 0.5
xdotool windowactivate $window keyup Shift+Insert 
sleep 0.5
xdotool windowactivate $window key Insert

