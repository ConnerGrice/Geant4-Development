# Development timeline from week 5 to 8 of the project

## 21/3/2023

### Material definitions

At this point the materials used have been placeholders. Each layer was made up of carbon, while the empty space was a vaccum. Trying to make the simluation closer to the correct materials, I have modifed them

```cpp
void DevDetectorConstruction::DefineMaterials(){
	//Defining materials
	G4NistManager* pNist = G4NistManager::Instance();
	G4Material* C = pNist->FindOrBuildMaterial("G4_C");
	G4Material* Si = pNist->FindOrBuildMaterial("G4_Si");
	G4Material* Al = pNist->FindOrBuildMaterial("G4_Al");
	G4Material* kapton = pNist->FindOrBuildMaterial("G4_KAPTON");

	//Defining volume materials
	pWorldMat = pNist->FindOrBuildMaterial("G4_Galactic");
	pPlateMat = C;
	pFleeceMat = C;
	pGlue = C;
	pChips = Si;
	pSolder = C;
	pConducting = Al;
	pSubstrate = kapton;
}
```
Now, the only material that is not correct is `pSolder`. This is an epoxy chemical compound, $C_2H_2$.

After running the experiments again, the efficiency is pretty much the same. This was expected as the thickness of the materials is smaller and the energy of the protons are high so complete absorbtion is unlikely to occur.

### Improving efficiency by moving HCI unit

When trying to increase the efficiency of the system, one method to do this is to increase the solid angle swept by the detector plates. There are a few ways to do this.

The first is to move the HCI unit forward away from the origin of the particles. This means the units will cover more of the cone in which the 
particles can move. 

![hci-shift-image](screenshots/HCI-Unit-Shift.png)

These are the results:

![moving-hci](./Results/Eff_HCI.svg)

### Imporoving efficiency by chaning the amount of padding by the cold plate

For the same reasons, the padding on either side of the HCI unit can also be reduced to lower the space between the detectors that the particles can pass through.

![hci-padding](screenshots/HCI-Unit-padding.png)

The results show that the efficiency was greatly increased if the padding was non-existent (~80%). However, this may become an issue when building the physical detector.  

### Improving efficiency by moving the particle origin point

For the same reason as the changes above, the particle source can be moved to the end of the detector. This means that the entire HCI unit will be used. Before this change (particle are generated from the centre), half of the unit was never hit because the particles will always be travelling forward when they are generated.

![particle-shift](screenshots/Particle-Origin-Shift.png)

I carried out an experiment with these parameters:

- Particles are generated 10mm from the back of the detector
- The HCI units are shifted 10mm towards to front of the detector
- There is 3mm of padding on wither side of the cold plates

Here are the results:

- Total Efficiency: 66.7%%
 
|Level of Success|Percentage of Valid Events|
|---|---|
|Good|70.2%|
|Alright|29.0%|
|Bad|0.8%|

Though the total efficiency has increased, the proportion of those that are considered _Good_ is reduced. This makes sense since the particles that are now captured after moving the origin back will be those that are travelling at a shallower angle. Therefore, there is a higher chance of missing the last layer, resulting in a _Alright_ event.

## 23/03/2023
### Testing affect of padding on efficiency if particle source is pushed down the detector

To keep my testing consistent and as close to the main simulation as possible, I have changed the position of the particle source to be where the effective area of the detector starts, e.g. Not at the very end of the detector, but at the point where all the HCI units begin. 

I then carried out the experiment changing the amount of padding and here are the results:

![padding](Results/Eff_padding_new.svg)

These results show that with no padding at all, the system could be more than 90% efficient. However, this may be impractiacal. The accpeted efficiency is 70% or higher. This is the case when the padding is at most 3.5mm.

To get a better view on the levels of success, a final test was done where the padding was set to 3mm in order to get the efficiency above 70%. Here are the results:

- Total Efficiency: 73.6%
 
|Level of Success|Percentage of Valid Events|
|---|---|
|Good|89.4%|
|Alright|9.4%|
|Bad|1.2%|

## 24/03/2023
### Adding Liquid Hydrogren target

In the real detector, the protons will be produced by aiming a particle beam at a liquid hydrogen target. This was added at the over the top of where the particles are generated.

The target as a diameter of 3cm and a length of 3cm.

![target](screenshots/Target.png)

![target-barrel](screenshots/Target-Barrel.png)

### Starting to digitize

The next goal is to set up digitisation in order to gather results that are more realistic to what the detector would output. To do this I have added the ability to split the Individual HCI segments into smaller pixel sections.

In the real simulation the pixels will be 30x30 $\mu m^2$. In the image below is not to scale and is just for ease of viewing.

![HCI-pixels](screenshots/HCIPixels.png)

## 27/03/2023
### HIC Unit seperation

Due to physical constraints, the individual HIC segments cannot be touching and have a 0.2m gap inbetween each one. I have added this feature into the HICs. Another feature that will be present in the real detector is the addition of a passive periphery circuit along the bottom of the unit that is 1.2mm wide. 

![HIC-gap](screenshots/HIC-Gap.png)

The actual chips going to be used will be the [ALPIDE chips](https://indico.cern.ch/event/863068/contributions/3752479/attachments/1996261/3330551/20200302-Aglieri-ALPIDE-Overview.pdf).

These chips have a resolution of 512x1024 and are 15mmx30mm. An attempt will be made to get our model as close to this as possible.

## 28/03/2023
### Fixing Target

It was brought to my attention that the target had the wrong dimensions. The new target as a radius of 3cm and a length of 1cm

![target-new](screenshots/Target-New.png)

![target-barrel-new](screenshots/Target-Barrel-new.png)

### Attempt to make detector unit

I set up a parameterisation to define the pixels and the passive circuit board into the HIC unit. However, whole trying to generate a single 15mmx30mm, the entirty of my 16GB of RAM was used up. Therefore, this method is not usable in a practical sense.

### Second Attempt to make detector unit

The more memory efficient approach was to use the PVReplica class twice to make a grid of pixels. This method allows me to run the program, though the visualiser takes much longer to start up. However, due to the large number of pixels, it is not practical to render each one, since the visualiser lags. Therefore, only the 9 segments will be visualised.

### Creating chip active area

Due to the way the replica class works, I must first generate the active area of the chips and add the passive circuit board with another set of replicas. 

First, I have made the active area the correct size, so that it can contain 460x1000 px.

![ActiveChip](screenshots/ALPIDE-Chip-Active.png)

### Creating chip passive area

I have now generated the passive chip area along the side of each chip, shown in the dark green.

![Chip-Top](screenshots/ALPIDE-Chip-Top.png)

![Chip-Side](screenshots/ALPIDE-Chips-Side.png)

### Testing with pixel array

After running a single test, I found that not a single event was classed as valid. This is because, a valid event was determined by each particle coming into contact with a single senstive detector of each layer, e.g. each layer picks up 2 detections. However, since adding the pixels, each particle is passing through multiple pixels on each layer, therefore, making the event invalid.

For example, the last event of the simluation shows the following pixels within stave C hit:

```cmd
G4WT0 > StaveC: 8 (490,491,492,308,309,310,310,311)
```

8 different pixels were hit by the particles. Though the pixels are clise together indicated by their similar copy numbers.

I think I am going to try and set up the digitisation before fixing this bug. I think this can be fixed easily by just considering the first pixel hit by each particle at each layer.

## 30/03/2023

### Digitisation problem

When trying to digitise the detector, the goal it to find out which pixel the particle hit, then find the position the centre of that particle. When setting up the geometry, different parts of the detector are given copy numbers. These copy numbers are the way of identifying which part of the sensitive detector was hit. 

An overview on how the copy numbers are set up are below,

#### Each stave is given a number

![stave](screenshots/Stave-Numbers.png)

#### Each HCI strip is given a number, relative to the stave it is on

The range of these numbers depend on the layer e.g. stave D contains 4 strips so they will be numbered between 0 -> 3.

![strip](screenshots/Strip-Numbers.png)

#### Each segment of the strip is given a number

![segments](screenshots/SensitiveDet-copyNumbers.png)

#### Each segment is split into a pixel array

- The back top corner represents (0,0)
- Increase in positive z direction until 999
- Increases in negative y driection until 459 (relative to stave 0 or 5)

### First Digitisation Test

After setting up what I thought was correct to get the 3D coordinates of the pixel that was hit, I did a test firing a single particle along the x axis from the origin. This resulted in the particle missing both B and D staves but passing directly though a unit in stave C.

Results:

- Exact Position (mm): 		(44.5373,-1.0764e-05,-4.07475e-05)
- Digitised Position (mm): 	(44.5123,13.785,-9.195)

This indicates that by calulations for the X value is correct, only being off by 25um. This is correct because the exact point is actaully the point at which the particle exists the pixel, which is 50um thick. Therefore, the centre of the pixel would actaully be at 25um.

### Progress

I have found that I can get the position of a volume relative to its mother volume. This means I can loop through the tree of volumes, adding up their position vectors as I go to get a final position vector that will be the pixels absolute position in the world. 

I have tried implimenting this but results are varied. Which make be think that I have made an error somewhere.

For example, having done a test where the particle passes through each layer, the error in the exact position and the digitised position increases for layers further from the origin. However, in tests where a particle is fired along the x-axis, the results are quite accurate.

More investigation is needed.

## 01/04/2023

### Digitiser Tweaking

I found that the position given by the volume does not take rotation into account. Since in the program, the staves are generated horizontally then rotated into a vertical position, all the positions are as if the stave was still horizontal.

I am able to get the rotation of the volumes and apply them to the position vectors while adding them together. This has given results that are much more accurate. I have done a test firing 6 particles, each is hits a stave at its normal angle.

Here are the results:

```cmd
G4WT0 > EVENT START===================================
G4WT0 > Digit: (22.2432,-38.5563,0.005)
G4WT0 > Exact: (22.2686,-38.5704,-4.07475e-05)
G4WT0 > Diff : (0.0254811,-0.014156,-0.00504075)
G4WT0 > =============================================
G4WT0 > Digit: (-15.543,-27.2913,0.005)
G4WT0 > Exact: (-15.7735,-27.3204,5.46514e-08)
G4WT0 > Diff : (-0.23049,-0.0291508,-0.00499995)
G4WT0 > =============================================
G4WT0 > Digit: (-22.0641,-38.5563,0.005)
G4WT0 > Exact: (-22.2687,-38.5704,6.50483e-05)
G4WT0 > Diff : (-0.204545,-0.0141302,-0.00493495)
G4WT0 > =============================================
G4WT0 > Digit: (-44.1023,-0.015,0.005)
G4WT0 > Exact: (-44.5373,4.20583e-05,4.99024e-05)
G4WT0 > Diff : (-0.435,0.0150421,-0.0049501)
G4WT0 > =============================================
G4WT0 > Digit: (-15.5689,27.3063,0.005)
G4WT0 > Exact: (-15.7735,27.3204,2.22299e-07)
G4WT0 > Diff : (-0.20451,0.0141505,-0.00499978)
G4WT0 > =============================================
G4WT0 > Digit: (-22.0641,38.5563,0.005)
G4WT0 > Exact: (-22.2687,38.5704,3.78402e-05)
G4WT0 > Diff : (-0.204594,0.0141018,-0.00496216)
G4WT0 > =============================================
G4WT0 > Digit: (15.748,27.3063,0.005)
G4WT0 > Exact: (15.7735,27.3204,-4.1168e-08)
G4WT0 > Diff : (0.0254903,0.0141507,-0.00500004)
G4WT0 > =============================================
G4WT0 > Digit: (22.2691,38.5413,0.005)
G4WT0 > Exact: (22.2687,38.5704,1.6705e-05)
G4WT0 > Diff : (-0.000487288,0.0291488,-0.0049833)
G4WT0 > =============================================
G4WT0 > Digit: (44.5123,-0.015,0.005)
G4WT0 > Exact: (44.5373,-1.14702e-05,-7.27118e-06)
G4WT0 > Diff : (0.025,0.0149885,-0.00500727)
G4WT0 > =============================================
```
As seen, the results are still not perfect but progress is being made.

## 03/04/2023

### Further Digitiser Testing

I did a test using the first entry of `quasi.root`. In this run, one particle goes through stave 0 (right-most horizontal) and the other went through stave 3 (right-most horizontal).

From stave 0:

```cmd
G4WT0 > Pixel: 0(312,79)
G4WT0 > Digit: (31.5219,4.425,-93.385)
G4WT0 > Exact: (31.4969,4.4189,-93.4273)
G4WT0 > Diff : (-0.025,-0.00610462,-0.0423365)
G4WT0 > =============================================
G4WT0 > Pixel: 0(312,80)
G4WT0 > Digit: (31.5219,4.425,-93.385)
G4WT0 > Exact: (31.5237,4.42302,-93.4)
G4WT0 > Diff : (0.00181556,-0.00197683,-0.015)
G4WT0 > =============================================
```

These results are very good but I noticed that even though the pixel that was activated is different, `(312,79)` and `(312,80)` the digitised coordinate is exactly the same, in this case, the z value for both should be slightly different. 

Some results from stave 3 are:

```cmd
G4WT0 > Pixel: 3(374,469)
G4WT0 > Digit: (-31.1119,2.565,-81.685)
G4WT0 > Exact: (-31.4969,-2.56587,-81.7101)
G4WT0 > Diff : (-0.385,-5.13087,-0.0251173)
G4WT0 > =============================================
G4WT0 > Pixel: 3(374,470)
G4WT0 > Digit: (-31.1119,2.565,-81.655)
G4WT0 > Exact: (-31.5045,-2.56639,-81.7)
G4WT0 > Diff : (-0.392547,-5.13139,-0.045)
G4WT0 > =============================================
G4WT0 > Pixel: 3(374,471)
G4WT0 > Digit: (-31.1119,2.565,-81.655)
G4WT0 > Exact: (-31.5268,-2.56792,-81.67)
G4WT0 > Diff : (-0.414914,-5.13292,-0.015)
G4WT0 > =============================================
```

The `(376,470)` and `(374,471)` pixels show the same issue as stave 0, however, `(374,469)` seems to change the z value correctly. This leads me to think something is wrong with the pixel volume coordinates.

Another thing to notice is the fact that the y value for the digitised coordinate is the negative of what it should be. This is probably something to do with the angles I am rotating the position vectors by. This coul be what has also lead to the bigger difference in the x value as well. More tests will be carried out with this stave.

### Angle Problem

I have found that the issue with the incorrect negative values only occurs for the staves of the left side of the detector (2,3,4).

On the right side of the detector, the HIC copy numbers increases from bottom to top. However, since I generated the staves by rotating them by $\frac{\pi}{3}$ at a time, copy number of the staves of the left side increase in the opposite direction (from top to bottom). This is what is causing the issues. 

### Digitiser Solved (Almost)

I have fixed the negative value problem. The issue was with how Geant4 deals with angles. After some experimentation, I found that if the HIC that was triggered was on a stave on the negative x side of the detector, instead of adding the angles of each object, I needed to subtract them. Now the digitiser gives coordinates that are much more accurate for all staves. However, the issue with the z being exactly the same between some concurrent pixels along the z axis is still not fixed.

## 04/04/2023

### Digitiser Error

I have added the ability to record the difference between the preStepPoint and the digitied point for every hit in the simluation and have generated some plots showing this error.

#### X Error

![x-error](Results/X_Error.png)

The 2 tall sections are +/- 0.025mm, the reason for this is that the preStepPoint is the point as the particle leaves the pixel and the pixel is 0.05mm thick, meaning that the centre point is 0.025mm away from the back of the pixel, giving this result. However, since the pixel is 0.03x0.03x0.05mm, I would have also expected some peaks around +/- 0.015mm but these aren't present.

There is still some results that gave an error slightly larger than 0.025mm, I am not sure what is causing this since the furthest point from the centre of each particle is about 0.0328mm.

#### Y Error

![y-error](Results/Y_Error.png)

The peaks shown in this plot are at 0.015mm, which is what I would expect because of the side length in that. However, it seems like that error can get as high as 0.036mm, which is more than the furthest point in the pixel.

#### Z Error

![z-error](Results/Z_Error.png)

NOt much can be seen in this plot apart from the large number of points close to 0. The mean is largest than the other points by quite a significant amount. This could be due to the error where that some pixels that are close to eachother are giving the same z value when they should not be. The plot was also quite zoomed out when it was generated, this probably means that there is an outlier somewhere, this could also be changing the mean considerably. 

### Diagnosing Error in Z Direction

I have ran a single test where a particle was straight down the sensitive detector and output which pixel (z-coordinate) that was hit.

The last few results are:

```cmd
G4WT0 > CChipsPixel(993): 4.71239 (0,-0,14.835)
G4WT0 > Pixel: 0(210,993)
G4WT0 > CChipsPixel(994): 4.71239 (0,-0,14.865)
G4WT0 > Pixel: 0(210,994)
G4WT0 > CChipsPixel(995): 4.71239 (0,-0,14.895)
G4WT0 > Pixel: 0(210,995)
G4WT0 > CChipsPixel(996): 4.71239 (0,-0,14.895)
G4WT0 > Pixel: 0(210,996)
```

It showns that the last 2 pixels give the same z value. I could not find any more instances of this in this experiment.

I also did a test after angling the beam up slighltly. The last few results:

```cmd
G4WT1 > DChipsPixel(982): 4.71239 (0,-0,14.505)
G4WT1 > Pixel: 0(1,982)
G4WT1 > DChipsPixel(983): 4.71239 (0,-0,14.535)
G4WT1 > Pixel: 0(1,983)
G4WT1 > DChipsPixel(984): 4.71239 (0,-0,14.565)
G4WT1 > Pixel: 0(1,984)
G4WT1 > DChipsPixel(985): 4.71239 (0,-0,14.595)
G4WT1 > Pixel: 0(1,985)
G4WT1 > DChipsPixel(986): 4.71239 (0,-0,14.625)
G4WT1 > Pixel: 0(1,986)
G4WT1 > DChipsPixel(987): 4.71239 (0,-0,14.625)
G4WT1 > Pixel: 0(1,987)
G4WT1 > DChipsPixel(987): 4.71239 (0,-0,14.655)
G4WT1 > Pixel: 0(0,987)
G4WT1 > DChipsPixel(988): 4.71239 (0,-0,14.685)
G4WT1 > Pixel: 0(0,988)
G4WT1 > DChipsPixel(989): 4.71239 (0,-0,14.715)
G4WT1 > Pixel: 0(0,989)
G4WT1 > DChipsPixel(990): 4.71239 (0,-0,14.745)
G4WT1 > Pixel: 0(0,990)
G4WT1 > DChipsPixel(991): 4.71239 (0,-0,14.775)
G4WT1 > Pixel: 0(0,991)
G4WT1 > DChipsPixel(992): 4.71239 (0,-0,14.775)
G4WT1 > Pixel: 0(0,992)

```

This shows that the error appears much more often. The areas that are wrong are:


```cmd
G4WT1 > DChipsPixel(986): 4.71239 (0,-0,14.625)
G4WT1 > Pixel: 0(1,986)
G4WT1 > DChipsPixel(987): 4.71239 (0,-0,14.625)
G4WT1 > Pixel: 0(1,987)
G4WT1 > DChipsPixel(987): 4.71239 (0,-0,14.655)
G4WT1 > Pixel: 0(0,987)

...

G4WT1 > DChipsPixel(991): 4.71239 (0,-0,14.775)
G4WT1 > Pixel: 0(0,991)
G4WT1 > DChipsPixel(992): 4.71239 (0,-0,14.775)
G4WT1 > Pixel: 0(0,992)
```

Showing that the error is only occuring when a new pixel strip is activated by the particle e.g. `Pixel: 0(y,xxx)`, where `y` is the strip in the y direction and `xxx` is the pixel in that strip. I don't think this is due to the actual positions of the pixels since the error looks like it can occur on any pair of pixels, as long as they are the first 2 of the strip that are detected. Therefore, it is probably an issue of the method used to digitise.

### Fix Found*

I realised that I had the wrong idea about the preStepPoint. In reality, the preStepPoint gets the position of the particle as it enters the volume instead of as it exists. The volume attached to this point is therefore the volume that the particle is leaving.

I have now changed to using the postStepPoint, which gets the particle as it is leaving, and the volume that it is leaving. This fixes the error completely. However, now when I run more than a single run I get a segmentation fault. This is due to this change.

### Reason For Segmentation Fault

The issue happens when I get the volume that the particle that was inside of using the postStepPoint. If I use the preStepPoint, the error does not happen, but the original error returns.

### Digging Deeper

The fault seems to happen when the particles leave the active area. This is because I am now taking the postStepPoint, therefore, the volume given will not be one of the pixel volumes. This causes the segmentation fault because these volumes do not have as many mothers as the pixel volumes. 

- Pixels: Pixel -> Pixel Strip -> HIC Segment -> HIC Strip -> HIC Unit -> Stave -> World
- Another HIC layer: HIC Segment -> HIC Strip -> HIC Unit -> Stave ->  World

Since I am looping through each volume in the tree of volumes to get the position relative to the world volume, I am assuming this volume will have exactly 6 layers (since the pixels normally have this many layers). However, if the volume is not a pixel, there will be less than 6 layers, meaning that I am trying to get a volume that doesn't exist. Giving me the segmentation fault.

### Solution

To bypass this error, I have added a condition that will decide if the preStepPoint or postStepPoint should be used. This means if the particle is leaving an active volume and going into a passive volume, it will use the preStepPoint and therefore, the volume of the pixel it was leaving. If it is just moving between active volumes, it will use the postStepPoint and get the volume it is moving in to.

Here are the new results:

#### X Error

![x-error-2](Results/X_Error_mk2.png)

The peaks at +/- 0.025mm have disappeared and the width of the distribution has shrunk but the mean still remains very close to zero.

#### Y Error

![y-error-2](Results/Y_Error_mk2.png)

The peaks remain in place, but more of the point lie inside of their boundaries. This has lead to a thinner plot, similar to that of the X errors.

#### Z Error

![z-error-2](Results/Z_Error.png)

THis plot is still vary similar to the first z error plot. The next idea I am going to try is to ignore the particles that are enter volumes that are not active.

### Ignoring Particles

After ignoring particles that move into passive areas as they leave active areas gives the same shape results in the x and y directions, however, the z error changes:

![z-error-3](Results/Z_Error_mk3.png)

Doing this has removed the outlier points present in the plots above. The mean is also closer to the centre. As a result of excluding particles, the number of recorded particles was also reduced by a large amount.

The particles that are causing this error are the ones that collide with the active area first, before moving through the rest of the detector. I think a better solution must be found to preserve the number of recorded particles.   

## 05/04/2023

### New Driection

Since this issue cannot be sovled by using the postStepPoint, I need to take a different approach. The issue may be the way I generate the geometry. I am using an object just `PVReplicaSlice` which divides the volume into sections but it has the ability to create a gap in between those sections. However, when doing this I think it will squish the section volume. Since my pixels are within the volume that gets squished they might either be over-lapping or are not the size I am assuming them to be, causing issues in the digitiser.

TO get around this I am going to experiment with just placing the sections down manually.

### Issues with PVReplicaSlices

it so happens that `PVReplicaSlices` will also shorten the volume at the ends by the same amount as the distance between each section. This means that the entire volume is quite a bit shorter than I thought.

I have added the chips manually to make sure they are all the same size. The issue with the replace length is much more apparent in the passive chip volume along the top. The axes is 271.6mm long, the length the strip should be.

![replace](screenshots/ReplicaSlice.png)  

For the sake of accuracy and trying to fix my current issue, I will generate all layers of the HIC manually.

### Digitiser testing

After redoing the geometry generation I run another experiment. These tests were done using the preStepPoint:

#### X Error

![x-error-4](Results/X_Error_mk4.png)

Compared to the results from the other test that used the preStepPoints this distribution cuts off faster. The largest error is around 0.026mm. There are also less points that has a smaller error.

#### Y Error

![y-error-4](Results/Y_Error_mk4.png)

Similar to the X error results, there are no errors with a magnitude larger than 0.039mm. The results look similar to that produced by the test where the postStepPoint was used, however, there are a much larger number of entires since there are no particles being disregarded.

#### Z Error

![z-error-4](Results/Z_Error_mk4.png)

These results are more slightly better than they were before. There are no outliers like there were in the last test wherer the preStepPoint was used. Like the y error, the plot looks very similar to the postStepPoint experiment, however, there are many more entries, therefore, the results that gave errors less than -0.015 can be seen. 

I am starting to think that this error may be due to the fact that particles in the test are generally moving in the negative z direction, therefore, will almost always hit the side of the pixel that is facing in the positive z direction. The x and y errors are more symmetrical since the pixels are rotated about the z axis.