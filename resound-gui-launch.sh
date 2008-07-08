#!/bin/bash
# Script for installation on client / GUI machine
# This launches the GUI application with args supplied to this script
# then connects the GUI MIDI port to hardware MIDI port with aconnect.
# This script can be aliased on the client machine for convenience.

echo -e "Launching Resound GUI -i$1 -o$2 -s$3";
resound_gui -i$1 -o$2 -s$3 & sleep 15;

# Connect resound_gui MIDI port to hardware. Use aconnect -io to list devices
aconnect 20:0 128:0;
