# draug - Data augmentation

draug is a tool for generating many different views of one single
image. It is intended for simulating the views of a planar target a
drone would see during flight. But it is of course also useful for
general data augmentation.

** Compiling:

`g++ -g draug.cpp -o draug \`pkg-config --cflags --libs opencv\` -std=c++11 -lboost_filesystem -lboost_system`
I'll add a makefile later on.

** Usage

`draug gui` for an interactive view
`drauf rnd [int]`, for example, `draug rnd 1000` for generating random samples (random x, y, z, roll, pitch, and yaw) of the views


