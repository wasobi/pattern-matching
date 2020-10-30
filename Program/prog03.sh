#!/bin/bash
# Received help from Christopher McCooey and Professor Hervé

INPUT_DIR_PATH="Prog03/Data/Imagesimage2.img"
TEMP_DIR_PATH="Prog03/Output"
PROG_PATH="./prog03"
echo "+ input path: $INPUT_DIR_PATH"
echo "+ output path: $OUTPUT_DIR_PATH"

NAME = prog03
if [-f$NAME];then
	rm -f$NAME
fi
	fileName=${filePath##*/}
	echo "file path: $filePath"
	echo "file name: $fileName"
	
	if [[ -e $outFilePath ]]; then
		echo "File already $fileName exists in output directory"
		exit 1
	fi

	while read fileLine
	do
		for patternPath in "$PATTERN_DIR_PATH"/*;
			do
			 gcc -g main.c startup.c image.c pattern_matching.c -o $NAME
			done
		echo ${fileLine} | tr -d '\r'
	done > "${outFilePath}"
done