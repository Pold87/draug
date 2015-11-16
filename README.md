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


