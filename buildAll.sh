#!/bin/bash

for example in $( find ./ -maxdepth 1 -type d )
do

   	if [ ! -e "$example"/$(basename $example).xcodeproj ]; then
   	    echo "-----------------------------------------------------------------"
   	    echo no xcode project for $example
   	    continue
   	fi

    echo "-----------------------------------------------------------------"
    echo building $example Debug
    xcodebuild -configuration Debug -target $(basename $example) -project $example/$(basename $example).xcodeproj
    ret=$?
    if [ $ret -ne 0 ]; then
   	    echo failed building $example Debug
   	    exit
    fi

    echo "-----------------------------------------------------------------"
   	echo building $example Release
   	xcodebuild -configuration Release -target $(basename $example) -project $example/$(basename $example).xcodeproj
   	ret=$?
   	if [ $ret -ne 0 ]; then
   	    echo failed building $example Release
   	    exit
   	fi
   	echo "-----------------------------------------------------------------"
   	echo ""
done
