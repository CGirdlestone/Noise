# Noise
A layered Perlin Noise implementation for generating noise maps. Here's an example map using 8 octaves, a frequency of 0.005 and a persistence (rate of amplitude decrease) of 0.45.

![image](https://user-images.githubusercontent.com/45768379/109888785-06fd8900-7c7c-11eb-955c-4bb9f81b5358.png)

The Perlin class covers 1D, 2D and 3D perlin noise implementations.

Noise values can get obtained by calling noise1D, noise 2D or noise 3D, depending on the dimensions of the noise required. This will give you a single octave of noise at the specified spatial point. If you want to layer the octaves, there are octaves1D, octaves 2D and octaves3D. These simply loop over the number the octaves and add the contribution to the noise value (increasing frequency and decreasing amplitude). The values are normalised to lie within the range 0 - 1, although the majority of the noise values lie within 0.3 - 0.7 with anything outside of these bounds rarely appearing. 
