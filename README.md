# Development Timeline

This will be a timeline desplaying screenshots taken to outline the development process.

## The cold plates were initially generated using an assembly volume.

![barrel-view](screenshots/staveC-D-barrel.png)

![full-view](screenshots/staveC-D-barrel-2.png)

## The thickness was fixed and all 3 layers were added

![barrel-thin](screenshots/3Layers-FixedThickness.png)

## Orientation was fixed and a scale was shown

![scaled](screenshots/3Layers-Scales.png)

## Assembly volume was scrapped and mother volumes where used to create the HCI unit ontop of the cold plate

![layers](screenshots/HCI-Unit-CrossSection.png)

![segments](screenshots/HCI-Unit-Segments.png)

## Multiple, alternating HCI units were placed on the plate

![alternating](screenshots/HCI-Unit-Alternating.png)

## Stave C was generated

![staveC](screenshots/StaveC-Single.png)

## All staves were added in the correct shape

![complete](screenshots/Complete-Staves.png)

![complete-barrel](screenshots/Complete-Staves-Barrel.png)

## Added sensitive detectors ("Chips" layers only)

![sensdets](screenshots/SensitiveDet-copyNumbers.png)

## NOTE the printout showing each layers is distinct from eachother

![sensdets-layers](screenshots/SensitiveDet-layers.png)

## Implimented the ability for the program to count the number of hits on each sensitive detector throughout the run and calculate the efficiency

![sensdet-counting](screenshots/SensitiveDet-Counting.png)

However, sometimes a secondary particle is generated and scatters multiple times within the sensitive detectors, causing multiple counts. Notice the count of 15 hits on the stave C layer. 

![sensdet-electron](screenshots/SensitiveDet-Electron.png)

## First efficiency outcome

After a full run simply counting the number of particles that hit each layer without taking scattering, or secondary particles into account the efficiency was calculated. A 100% efficiency would mean the layer has been hit 200480 times since there are 2 particles generated for each of the 100240 events.

|Layer     |Hits   |Efficiency|
 ------- | ----- | -------- 
D (4 HCIs)|139,147|69.4%
B (3 HCIs)|153,856|76.7%
C (2 HCIs)|151,834|75.7%


