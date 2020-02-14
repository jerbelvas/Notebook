#!/bin/bash


### Command arguments
INIT=0      # bool:     Prepare directory and copy dependencies
PARSE=""    # string:   The file path to parse

while [[ "$#" -gt 0 ]]; do
    case $1 in
        -i|--init)  INIT=1; shift;;
        -p|--parse) PARSE="$2"; shift;;
        *) >&2 echo "Invalid argument: $1"; exit 1;;
    esac;
    shift;
done



INSTALL_DIRECTORY=~/notebook



if [[ $INIT -eq 1 ]]; then
    # Copy dependencies
    echo "Preparing directory with dependencies..."

    mkdir -p "images"
    mkdir -p "dependencies"
    
    echo "Copying template..."
    cp $INSTALL_DIRECTORY"/template/template.html" "./dependencies/template.html"
    
    echo "Copying styles..."
    cp $INSTALL_DIRECTORY"/template/style.css" "./dependencies/style.css"
    
    echo "Copying Prism..."
    mkdir -p "dependencies/prism"
    cp -r $INSTALL_DIRECTORY"/prism" "./dependencies"

    echo "Copying MathJax... (may take a while)"
    mkdir -p "dependencies/mathjax"
    cp -r $INSTALL_DIRECTORY"/mathjax" "./dependencies"

    echo -e "\nDone."
fi

if [[ -n $PARSE ]]; then
    TEMPLATE="dependencies/template.html"
    OUTPUT=$(echo $PARSE | sed 's/\(.*\)\..*/\1/').html
    $INSTALL_DIRECTORY/main $PARSE $TEMPLATE $OUTPUT
fi