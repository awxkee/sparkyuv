# What's this

Library allows to convert RGB to Y'UV formats at high speed using platform SIMD acceleration and appropriate approximation level

## Supported YUV formats and layouts

| YUV      | 444 | 422 | 420 | 400 | NV12/NV21 | NV16/NV61 | NV24/NV42 |
|----------|-----|-----|-----|-----|-----------|-----------|-----------|
| YCbCr    | ✅   | ✅   | ✅   | ❌   | ✅         | ✅         | ✅         |
| YcCbcCrc | ✅   | ✅   | ✅   | ❌   | N/A       | N/A       | N/A       |
| YCgCo    | ✅   | ✅   | ✅   | ❌   | N/A       | N/A       | N/A       |
| YCgCo-Ro | ✅   | ✅   | ✅   | ❌   | N/A       | N/A       | N/A       |
| YCgCo-Re | ✅   | ✅   | ✅   | ❌   | N/A       | N/A       | N/A       |
| YDzDx    | ✅   | ✅   | ✅   | ❌   | N/A       | N/A       | N/A       |
| YIQ      | ✅   | ✅   | ✅   | ❌   | N/A       | N/A       | N/A       |

When encoding to NV12/NV21, NV12/N61, 420, 422 for chroma subsampling bi-linear scaling is automatically applied. There
is no option to turn this off.
Due to nature of this transformation in those cases it is exceptionally fast.

### Things to note

- YCgCo-Ro/YCgCo-Re 8-bit cannot be represented in 8-bit uint storage, so YCgCo-Ro/YCgCo-Re 8-bit requires a storage buffer to be at least twice widen ( 16-bit storage type )
- YCgCo-Ro/YCgCo-Re cannot be in limited YUV range at the moment, since it not clear how to this range reduction with dynamic bit-depth. For now, it always in full PC range.
- YCgCo current implementation allows to do a range reduction for TV range. That approximation have significant slowdown against full range transform. If that important range reduction may be removed and potential speed up about 30-60% is expected. 10/12-bit transformations may experience about 100-200% slowdown due to range reductions.
- YcCbcCrc ( YUV constant light ) primarily intended to be used in BT.2020 CL ( BT.2020 constant light ) color space, however ITU-R provides implementation for any possible kr, kb.
- YcCbcCrc is direct transformation due to its nature, so expect it to be at least 1000% slower, than any approximation matrices. It contains especially good acceleration for arm64-v8a with full FP16 support, however it still 1000% slower than other approximations. Against naive implementation current transformation about 400-600% faster.

## Performance

#### Compare to libyuv:
All tests performed on Apple M3 Pro.

Not all the conversion path exists in libyuv so not everything can be benchmarked.

- Since very close approach to libyuv is used for YCbCr in general performance of decoding 8 bit is very close to libyuv.
- Encoding of YCbCr 8-bit faster in libyuv.
- 10/12 bit YCbCr in the library faster than libyuv.
- YcCbcCrc very slow transformation.
- YCgCo-Re/YCgCo-Ro fastest transformations available.
- YCgCo maybe reworked up to 100% performance gain for full range transform

#### Benchmark

```bash
Run on (12 X 24 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB
  L1 Instruction 128 KiB
  L2 Unified 4096 KiB (x12)
Load Average: 4.05, 3.49, 3.12
---------------------------------------------------------------------
Benchmark                           Time             CPU   Iterations
---------------------------------------------------------------------
CPU Caches:
  L1 Data 64 KiB
  L1 Instruction 128 KiB
  L2 Unified 4096 KiB (x12)
Load Average: 3.97, 3.17, 2.49
-----------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations
-----------------------------------------------------------------------
SparkyuvYCbCr444P10ToRGBA10      529913 ns       529228 ns         1356
LibYuvYCbCr444P10ToRGBA8        1240636 ns      1238238 ns          567
SparkyuvYCbCr422P10ToRGBA10      524934 ns       524014 ns         1343
LibYuvYCbCr422P10ToRGBA8        1027332 ns      1026172 ns          686
SparkyuvYCbCr420P10ToRGBA10      527574 ns       526863 ns         1326
LibYuvYCbCr420P10ToRGBA8        1028310 ns      1027050 ns          685
SparkyuvRGBAP10ToYCbCr420P10     559627 ns       559209 ns         1251
SparkyuvRGBA10ToYCbCr422P10      629020 ns       628380 ns         1117
SparkyuvRGBA10ToYCbCr444P10      551020 ns       550281 ns         1272
SparkyuvYCbCr444ToRGBA8          405062 ns       402525 ns         1771
LibYuvYCbCr444ToRGBA8            365472 ns       365111 ns         1917
SparkyuvYCbCr422ToRGBA8          433533 ns       433216 ns         1614
LibYuvYCbCr422ToRGBA8            435511 ns       435088 ns         1605
SparkyuvYCbCr420ToRGBA8          433548 ns       433306 ns         1613
LibYuvYCbCr420ToRGBA8            437906 ns       437572 ns         1602
SparkyuvRGBA8ToYCbCr420          479681 ns       479230 ns         1459
LibyuvRGBA8ToYCbCr420            265424 ns       265233 ns         2633
SparkyuvRGBA8ToYCbCr422          676895 ns       676320 ns         1037
LibYuvRGBA8ToYCbCr422            389598 ns       389285 ns         1797
SparkyuvRGBA8ToYCbCr444          566461 ns       565914 ns         1239
SparkyuvYCgCoR444ToRGBA8         380957 ns       380588 ns         1842
SparkyuvYCgCoR422ToRGBA8         377372 ns       376931 ns         1875
SparkyuvYCgCoR420ToRGBA8         383632 ns       383336 ns         1822
SparkyuvRGBA8ToYCgCoR420         240795 ns       240627 ns         2898
SparkyuvRGBA8ToYCgCoR422         310966 ns       310709 ns         2279
SparkyuvRGBA8ToYCgCoR444         237510 ns       237271 ns         2820
SparkyuvYCgCo444ToRGBA8          437727 ns       437380 ns         1595
SparkyuvYCgCo422ToRGBA8          450829 ns       450511 ns         1552
SparkyuvYCgCo420ToRGBA8          450551 ns       450213 ns         1554
SparkyuvRGBA8ToYCgCo420          350294 ns       350012 ns         1994
SparkyuvRGBA8ToYCgCo422          514462 ns       514013 ns         1379
SparkyuvRGBA8ToYCgCo444          346769 ns       346320 ns         1972
SparkyuvYcCbcCrc444ToRGBA8     15018564 ns     15002660 ns           47
SparkyuvYcCbcCrc422ToRGBA8     14947334 ns     14932830 ns           47
SparkyuvYcCbcCrc420ToRGBA8     14951980 ns     14937149 ns           47
SparkyuvRGBA8ToYcCbcCrc420     17118340 ns     17098024 ns           41
SparkyuvRGBA8ToYcCbcCrc422     17838106 ns     17807026 ns           39
SparkyuvRGBA8ToYcCbcCrc444     17155542 ns     17135805 ns           41
SparkyuvYIQ444ToRGBA8            577836 ns       577133 ns         1221
SparkyuvYIQ422ToRGBA8            627080 ns       626398 ns         1119
SparkyuvYIQ420ToRGBA8            630263 ns       629753 ns         1110
SparkyuvRGBA8ToYIQ420            594194 ns       591400 ns         1192
SparkyuvRGBA8ToYIQ422            656416 ns       655664 ns         1068
SparkyuvRGBA8ToYIQ444            576889 ns       576047 ns         1223
SparkyuvYDzDx444ToRGBA8          754052 ns       749348 ns         1034
SparkyuvYDzDx422ToRGBA8          795340 ns       790486 ns          878
SparkyuvYDzDx420ToRGBA8          791833 ns       787225 ns          916
SparkyuvRGBA8ToYDzDx420         1079805 ns      1074111 ns          633
SparkyuvRGBA8ToYDzDx422         1215429 ns      1208062 ns          617
SparkyuvRGBA8ToYDzDx444          239677 ns       238794 ns         2959
LibyuvPremultiply                221311 ns       219895 ns         3304
SparkyuvPremultiply              313820 ns       312474 ns         2224
```

##### YcCbcCrc 

Without `F16` Support

```bash
Run on (12 X 24 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB
  L1 Instruction 128 KiB
  L2 Unified 4096 KiB (x12)
Load Average: 3.59, 2.77, 2.86
---------------------------------------------------------------------
Benchmark                           Time             CPU   Iterations
---------------------------------------------------------------------
SparkyuvYcCbcCrc444ToRGBA8   20296729 ns     20080429 ns           35
SparkyuvYcCbcCrc422ToRGBA8   19318374 ns     19229343 ns           35
SparkyuvYcCbcCrc420ToRGBA8   19421785 ns     19355111 ns           36
SparkyuvRGBA8ToYcCbcCrc420   19789903 ns     19645216 ns           37
SparkyuvRGBA8ToYcCbcCrc422   19822250 ns     19736371 ns           35
SparkyuvRGBA8ToYcCbcCrc444   19283523 ns     19162514 ns           35
```

With `F16` Support

```bash
Run on (12 X 24 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB
  L1 Instruction 128 KiB
  L2 Unified 4096 KiB (x12)
Load Average: 3.20, 2.79, 2.86
---------------------------------------------------------------------
Benchmark                           Time             CPU   Iterations
---------------------------------------------------------------------
SparkyuvYcCbcCrc444ToRGBA8   16379008 ns     16325651 ns           43
SparkyuvYcCbcCrc422ToRGBA8   16312538 ns     16259651 ns           43
SparkyuvYcCbcCrc420ToRGBA8   16300961 ns     16251651 ns           43
SparkyuvRGBA8ToYcCbcCrc420   18579207 ns     18525289 ns           38
SparkyuvRGBA8ToYcCbcCrc422   19774963 ns     19647667 ns           36
SparkyuvRGBA8ToYcCbcCrc444   18945549 ns     18874472 ns           36
```

### Targets

Sparkyuv uses libhwy as backend, so that means on all supported platforms it's uses SIMD for acceleration

Sparkyuv (the same as libhwy) supports 22 targets, listed in alphabetical order of platform:

- Any: `EMU128`, `SCALAR`;
- Arm: `NEON` (Armv7+), `SVE`, `SVE2`, `SVE_256`, `SVE2_128`;
- IBM Z: `Z14`, `Z15`;
- POWER: `PPC8` (v2.07), `PPC9` (v3.0), `PPC10` (v3.1B, not yet supported
  due to compiler bugs, see #1207; also requires QEMU 7.2);
- RISC-V: `RVV` (1.0);
- WebAssembly: `WASM`, `WASM_EMU256` (a 2x unrolled version of wasm128,
  enabled if `HWY_WANT_WASM2` is defined. This will remain supported until it
  is potentially superseded by a future version of WASM.);
- x86:
    - `SSE2`
    - `SSSE3` (~Intel Core)
    - `SSE4` (~Nehalem, also includes AES + CLMUL).
    - `AVX2` (~Haswell, also includes BMI2 + F16 + FMA)
    - `AVX3` (~Skylake, AVX-512F/BW/CD/DQ/VL)
    - `AVX3_DL` (~Icelake, includes BitAlg + CLMUL + GFNI + VAES + VBMI +
      VBMI2 + VNNI + VPOPCNT; requires opt-in by defining `HWY_WANT_AVX3_DL`
      unless compiling for static dispatch),
    - `AVX3_ZEN4` (like AVX3_DL but optimized for AMD Zen4; requires opt-in by
      defining `HWY_WANT_AVX3_ZEN4` if compiling for static dispatch)
    - `AVX3_SPR` (~Sapphire Rapids, includes AVX-512FP16)

### Usage example

```c++
sparkyuv::YUV444P10ToRGBA10(reinterpret_cast<uint16_t *>(rgba16Data.data()),
                            rgba16Stride,
                            inWidth, inHeight,
                            reinterpret_cast<uint16_t *>(yPlane.data()),
                            yPlaneStride,
                            reinterpret_cast<uint16_t *>(uPlane.data()),
                            uvPlaneStride,
                            reinterpret_cast<uint16_t *>(vPlane.data()),
                            uvPlaneStride,
                            0.299f, 0.114f,
                            sparkyuv::YUV_RANGE_TV);

sparkyuv::YCgCoR420P8ToRGBA8(rgbaData.data(), rgbaStride, inWidth, inHeight,
                             yPlane.data(), yPlaneStride,
                             uPlane.data(), uvPlaneStride,
                             vPlane.data(), uvPlaneStride, sparkyuv::YCGCO_RE);
```