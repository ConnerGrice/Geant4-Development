#!/bin/bash

suffix=0
for value in $(seq 0.0 4.88 24.4);
do
	cd ..
	sed -i 's/const G4double HCIZ = .*/const G4double HCIZ = '"$value"'*mm;/g' classes/parameters.h
	cd build
	make
	./Dev run.mac
	echo $suffix
	((suffix++))
done

echo ALL done