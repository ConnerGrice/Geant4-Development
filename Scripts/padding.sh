#!/bin/bash

suffix=0
for value in $(seq 0.0 1.0 5);
do
	cd ..
	sed -i 's/const G4double PADDING = .*/const G4double PADDING = '"$value"';/g' classes/parameters.h
	cd build
	make
	./Dev run.mac
	echo $suffix
	((suffix++))
done

mv ../Results/Eff.dat ../Results/Eff_padding.dat

echo ALL done