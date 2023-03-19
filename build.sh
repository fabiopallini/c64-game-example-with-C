FILE=program.prg
if [ -f "$FILE" ]; then
	rm $FILE 
fi

cl65 $1 -o program.prg -t c64
x64sc program.prg
