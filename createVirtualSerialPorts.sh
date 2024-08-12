#!/bin/bash

# This script will create /dev/pts/3 and /dev/pts/4. 
# These are the virtual serial ports that can be used for sender and receiver programs.
socat -d -d pty,raw,echo=0 pty,raw,echo=0 > logs/socat.log 2>&1 &

echo ""
echo "Check in logs/socat.log which virtual devices have been created e.g. /dev/pts/8 and /dev/pts/9"
echo ""
echo "Then run the following commands with pts value from the log entry to create soft links to /dev/ttyV0 and /dev/ttyV1"
echo "sudo ln -s -f /dev/pts/8 /dev/ttyV0"
echo "sudo ln -s -f /dev/pts/9 /dev/ttyV1"
echo ""
echo "Showing logs/socat.log below:"
sleep 3
cat logs/socat.log
echo ""