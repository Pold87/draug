# draug - Drone data augmentation

draug is a tool for generating many different views of one single
image. It is intended for simulating the views of a planar target that
a drone would see during flight.

It saves the genenerated images at the folder `genimgs/` and creates a
`target.csv` file in the folder where it was called. The `target.csv`
files contains roll, pitch, yaw, x, y, and z coordinates of the camera
for each image.

## Compiling:

run `make`

You need OpenCV and Boost for compiling.

## Usage

- `./draug gui` for an interactive view
- `./drauf rnd [int] [imagename]`,
for example, `draug rnd 1000 map.png` for generating 1000 random
samples (random x, y, z, roll, pitch, and yaw) of the views

## Example: 

### Original image

![Original image (map)](https://raw.githubusercontent.com/Pold87/draug/master/img/cyberzoo_small.png "Cyberzoo")

### Generated images

![Generated images](https://raw.githubusercontent.com/Pold87/draug/master/montage.png "Generated images")

### Targets file

|Pitch | Roll | Yaw | x | y | z |
|----|------|-----|---|---|---|
|94.483653|-88.704241|343.623298|3066.280596|1203.075478|239.348295|
|90.193006|-92.050808|98.881635|2689.806722|-1477.947856|413.675838|
|...|...|...|...|...|...|



