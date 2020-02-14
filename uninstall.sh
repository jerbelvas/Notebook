#!/bin/bash

# Remove installation directory
INSTALL_DIRECTORY=~/notebook/
echo "Installation directory: "$INSTALL_DIRECTORY
rm -rf $INSTALL_DIRECTORY


# Remove notebook command
sudo rm -f /usr/bin/notebook


# Done
echo
echo "Done."