# Spec

Where possible approximation with SIMD used that makes almost everything very fast with some cost of quality. In the most cases quality changes is not noticeable

### YUV API:
## Notation:
- RGBA - denotes 8 bit planar image if bit depth is not specified
- RGB/RGBA/BGRA etc - uses word order so that mean on any endian system it is stored in following order
- RGBA1010102 - denotes AR30 little endian format ( Apple and probably some others uses always big endian it is not supported )
- RGBA10, RGBA12 - denotes 10/12 corresponding bit depth format stored in uint16
- RGBA16 - denotes 16 bit planar if bit depth is not specified, if bit depth specified then in uint16 stored image with gived bit depth
- YUVP10, YUVP12 - number after P specifies correspondin YUV bit depth
- YUV without P suffix is always 8 bit
- RGBX F16 - denotes Float16 image storage type

If you need not only RGBA and RGB, there is almost ready to go also BGR, BGRA, ARGB, ABGR almost for every function. You'll need to recompile whole library with a flag `-DSPARKYUV_FULL_CHANNELS=1` 

Some examples:

```c++
sparkyuv::NV21ToRGBA(rgbaData.data(),
                     rgbaStride,
                     nvWidth,
                     nvHeight,
                     yNVPlane.data(),
                     nvWidth,
                     uvPlane.data(),
                     ((nvWidth + 1) / 2) * 2);
```

```c++
sparkyuv::NV16ToRGBA(rgbaNVData.data(),
                     rgbaNVStride,
                     nvWidth,  nvHeight,
                     yNVPlane.data(), nvWidth,
                     uvPlane.data(), uvPlaneStride,
                     0.2126f, 0.0722f, sparkyuv::YUV_RANGE_TV);
```

```c++
sparkyuv::RGBAToYCbCr444(rgbaData.data(), rgbaStride, width, height,
                         yPlane.data(), yPlaneStride,
                         uPlane.data(), uvPlaneStride,
                         vPlane.data(), uvPlaneStride, 0.299f, 0.114f, sparkyuv::YUV_RANGE_TV);
```

```c++
sparkyuv::YCbCr444ToRGBA(rgbaData.data(), rgbaStride, width, height,
                         yPlane.data(), yPlaneStride,
                         uPlane.data(), uvPlaneStride,
                         vPlane.data(), uvPlaneStride, 0.299f, 0.114f, sparkyuv::YUV_RANGE_TV);
```

```c++
sparkyuv::RGBA10ToYCbCr422P10(reinterpret_cast<uint16_t *>(rgba16Data.data()),
                              rgba16Stride,
                              inWidth,
                              inHeight,
                              reinterpret_cast<uint16_t *>(yPlane.data()),
                              yPlaneStride,
                              reinterpret_cast<uint16_t *>(uPlane.data()),
                              uvPlaneStride,
                              reinterpret_cast<uint16_t *>(vPlane.data()),
                              uvPlaneStride,
                              0.299f,
                              0.114f,
                              sparkyuv::YUV_RANGE_TV);
```

## Helpers

Contains some basic functions to work with planar images:

- Gaussian Blur ( original gaussian with no approximation used )
- Fast Gaussian Blur ( good quality and very fast approximation of gaussian based on binomial polynomials )
- Rotate
- Copy image
- Transpose
- Flip/Flop
- Change image bit depth from low to high / from high to log
- Helper functions for RGB565
- Convert uint image to f16
- Good support for almost all conversion paths for f16
- Scale functions (Lanczos, Box, Bilinear, Catmull-Rom, Mitchell-Netravali, Cubic, BSpline, Nearest Neighbor, Hermite)
- Premultiply/Unpremultiply alpha

Fast Gaussian works at same speed or little faster as StackBlur however results is slightly better
Fast Gaussian Next is improved (next version) of fast gaussian with better results, for especially larger radius and slower processing time
Non gaussian methods will be detected if use FFT or any advanced analysis. Not recommended if you need anti-alias or smoothing.

Some usage examples:
```c++
sparkyuv::FastGaussianNextBlurRGBAF16(reinterpret_cast<uint16_t *>(f16Store.data()), width * 4 * sizeof(uint16_t), width, height, 15);
```

```c++
sparkyuv::FastGaussianBlurRGBAF16(reinterpret_cast<uint16_t *>(f16Store.data()), width * 4 * sizeof(uint16_t), width, height, 15);
```

```c++
sparkyuv::TransposeClockwiseRGBA(rgbaData.data(), rgbaStride, transposed.data(), trnsStride, width, height);
```