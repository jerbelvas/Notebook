#!/bin/bash

# Create installation directory
INSTALL_DIRECTORY=~/notebook
mkdir -p $INSTALL_DIRECTORY
echo "Installation directory: "$INSTALL_DIRECTORY
echo


# Copy dependencies
mkdir -p $INSTALL_DIRECTORY
echo "Installing template and styles..."
cp -r template $INSTALL_DIRECTORY

echo "Installing Prism..."
cp -r prism $INSTALL_DIRECTORY

echo "Installing MathJax... (may take a while)"
cp -r mathjax $INSTALL_DIRECTORY


# Build and copy Notebook binary
echo "Building and installing binary..."
# Build binary
g++ -std=c++11 -Wall main.cpp parser.cpp -o main
cp main $INSTALL_DIRECTORY
chmod +x $INSTALL_DIRECTORY/main
# Clean binary and .o files
rm -rf *.o *~ main

# Install the notebook command
cp notebook.sh $INSTALL_DIRECTORY/notebook
chmod +x $INSTALL_DIRECTORY/notebook
sudo cp notebook.sh /usr/bin/notebook
sudo chmod +x /usr/bin/notebook


# Done
echo
echo "Done."