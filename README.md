# Development Timeline

This will be a timeline to outline the development process.

Timeline section:

1. [Week 0-4](./Week0-4.md)
2. [Week 5-8](./Week5-8.md)

## 12/04/2023

### Recording measured position

The ability to record the digitised positions has been added and the first test has been done.

|Stave B| Stave C| Stave D|
|-------|--------|--------|
|![bx](Results/StaveB_xPos.png)|![cx](Results/StaveC_xPos.png)|![dx](Results/StaveD_xPos.png)|
|![by](Results/StaveB_yPos.png)|![cy](Results/StaveC_yPos.png)|![dy](Results/StaveD_yPos.png)|
|![bz](Results/StaveB_zPos.png)|![cz](Results/StaveC_zPos.png)|![dz](Results/StaveD_zPos.png)|

X position data follows a trend. There are dinstinct blocks the represent the HIC units. The shallower blocks are due to the HIC units that are on the diagonal of the hexagon and have a seperation beween them due to the passive 1.2mm chip section not detecting particles. There is a gap in the centre due to the passive chip section as well as the padding on either side of the cold plate. Finally, the peaks at either side of the plot are due to the vertical HIC units since they will always give a similar x value compared to the HICs that are diagonal.

Y position data also follows a similar trend by showning different sections. The shallower sections represent the vertical HIC units since they will have the larger y spread. The taller sections are the diagonal sections that span a smaller y range. The gaps are also due to the passive chip sections similar to the x positions.

The Z position data shows a smoother curve. For staves further from the centre, the curve is stretched in the x axis direction, which makes sense since the solid angle spanned increases the further from the centre the stave is.

### Averaging firing pixels

Since adding the pixels to the geometry, a single particle can fire multiple pixels in a single HIC unit as it passes through. This means we can take the average value of all of the fired pixels to get the position of the particle with a better resolution. This calculation has been added.

THe resulting position data measured is very similar to that above (when not averaged) however, each distinct section has become slightly more convex in shape. Take the y positions in stave C

|Measured|Averaged|
|---|---|
|![cm](Results/StaveC_yPos.png)|![ca](Results/StaveC_yAvg.png)|

A few things to note in the averaged plot. I have split the data up by the incident particle in order to get a valid average value. Events where one of the particles did not hit a HIC unit are also discarded. Therefore, the number of entries has reduced by 90%. However, if all events were valid there would only be a maximum of 100240 entires, therefore, the total efficiency is sitting at around 60%.

### Averaging digitised error

I have also took this oppotunity to test how these values compare to the averaged exact value. There plots are very different compared to the original error plots.

|X|Y|Z|
|---|---|---|
|![xe](Results/AvgError_x.png)|![ye](Results/AvgError_y.png)|![ze](Results/AvgError_z.png)|

