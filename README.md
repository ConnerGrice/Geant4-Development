# Development Timeline

This will be a timeline to outline the development process.

Timeline section:

1. [Week 0-4](./Week0-4.md)
2. [Week 5-8](./Week5-8.md)
3. [Week 9-12](./Week9-12.md)

## 23/05/2023

### Modifying detector shape

A new octagonal detector is going to be tested. Even though this design will increase the amount of passive area, the CALIFA will have passive area that follows an octagon.

This is the new detector shape as for now:

![octagon](screenshots/Octagon_mk1.png)

### Adding offset

To make the system match more closley to the plans, I have added an small angular offset to the entire detector. Here is the new orientation:

![octagon2](screenshots/Octagon_mk2.png)

### First test

I have ran the first test to see the distribution of the particles that it could detect. There are some significant changes. 

The first difference is the x and y values that it can record:

|Octagon|Hexagon|
|---|---|
|![oct-x](Results/StaveB_xPos_Oct.png)|![hex-x](Results/StaveB_xPos.png)|
|![oct-y](Results/StaveB_yPos_Oct.png)|![hrx-y](Results/StaveB_yPos.png)|

Unlike the hexagon configuration, this pattern was the same for all layers (2,3, or 4 HICs per stave). The z plot was very similar.

The next difference, which I think is a not as intended and should be investigated is with the error in the position measurements in the first place

|Octagon|Hexagon|
|---|---|
|![oct-x-diff](Results/AvgError_x_Oct.png)|![hex-x-diff](Results/AvgError_x.png)|
|![oct-y-diff](Results/AvgError_y_Oct.png)|![hex-y-diff](Results/AvgError_y.png)|
|![oct-z-diff](Results/AvgError_z_Oct.png)|![hex-y-diff](Results/AvgError_z.png)|

The error in the z direction seems to be pretty much unchaged, while the x and y directions have a few much larger errors. I want to reduce these outlier errors as much as possible. It must have something to do with the method that is being used to get the points of the pixels via the volume tree. This will be investigated.

