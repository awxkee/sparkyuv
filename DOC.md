# Spec

Contains some basic functions to work with planar images:

- GaussianBlur ( original gaussian no approximation used )
- FastGaussianBlur ( good quality and very fast approximation of gaussian based on binomial polynomials )
- Rotate
- Copy image
- Transpose
- Flip/Flop
- Change image bit depth from low to high
- Helper functions for RGB565
- Convert uint image to f16
- Good support for almost all conversion paths for f16


Fast Gaussian works at same speed or little faster as StackBlur however results is slightly better