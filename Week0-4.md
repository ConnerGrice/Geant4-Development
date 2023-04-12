# Development timeline for the first 4 weeks of the project

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
C (3 HCIs)|151,834|75.7%
B (2 HCIs)|153,856|76.7%


After adding a stacking action to delete all particles except the initial protons (removing secondary particles so they arent counted if they hit the detector multiple times as shown above) the efficiency changes.

|Layer     |Hits   |Efficiency|
 ------- | ----- | -------- 
D (4 HCIs)|133,279|66.5%
C (3 HCIs)|144,275|72.0%
B (2 HCIs)|148,442|74.0%


This makes sense because the number of hits isn't being inflated by the extra hits by electrons. Though, there is still the issue of the original protons scattering and hitting the detector multiple times.

## Getting total system efficiency

To get the total efficiency, what constitutes a valid and invalid event must be defined rather than just counting the number of hits on each detector. If either particle misses more than 1 layer the whole event is invalid. However, since there are 3 layers, there are different levels of success for each valid event. These levels were defined the experiment was run to calculate the total efficiency of the system.

 - Total Efficiency: 51.9%
 
|Level of Success|Definition|Percentage of Valid Events|
|---|---|---|
|Good|C + B _or_ C + B + D|90.4%|
|Alright|C + D|8.7%|
|Bad|B + D|0.9%|
