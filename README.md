# What's this

Library allows to convert RGB to Y'UV formats at high speed using platform SIMD acceleration and appropriate
approximation level.

Also contains a lot of convenient methods, a very fast gaussian blur with very good speed included.

## Supported YUV matrix

Almost all YUV formats supported, everything you might need :)

| YUV      | 444 | 422 | 420 | 411 | 410 | 400 | NV12/NV21 | NV16/NV61 | NV24/NV42 |
|----------|-----|-----|-----|-----|-----|-----|-----------|-----------|-----------|
| YCbCr    | ✅   | ✅   | ✅   | ✅   | ✅   | ✅   | ✅         | ✅         | ✅         |
| YcCbcCrc | ✅   | ✅   | ✅   | ❌   | ❌   | ❌   | N/A       | N/A       | N/A       |
| YCgCo    | ✅   | ✅   | ✅   | ❌   | ❌   | ❌   | N/A       | N/A       | N/A       |
| YCgCo-Ro | ✅   | ✅   | ✅   | ❌   | ❌   | ❌   | N/A       | N/A       | N/A       |
| YCgCo-Re | ✅   | ✅   | ✅   | ❌   | ❌   | ❌   | N/A       | N/A       | N/A       |
| YDzDx    | ✅   | ✅   | ✅   | ❌   | ❌   | ❌   | N/A       | N/A       | N/A       |
| YIQ      | ✅   | ✅   | ✅   | ❌   | ❌   | ❌   | N/A       | N/A       | N/A       |
| YDbDr    | ✅   | ✅   | ✅   | ❌   | ❌   | ❌   | N/A       | N/A       | N/A       |

When encoding to NV12/NV21, NV12/N61, 420, 422 for chroma subsampling bi-linear scaling is automatically applied. There
is no option to turn this off. Bi-linear scaling probably may be not so good as libsharpyuv, however also good.
Due to nature of this transformation in those cases it is exceptionally fast.

All NV, YUV444 (4:4:4), YUV422(4:2:2), YUV420(4:2:2), YUV411 (4:1:1), YUV410 (4:1:0) do not accept nullable U and V
planes, if you need to use 4:0:0 chroma subsample, please, do use 4:0:0 when it's available

### Usage example

```c++
sparkyuv::YCbCr422BT601ToRGBA(reinterpret_cast<uint16_t *>(rgba16Data.data()),
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

### Things to note

- YCgCo-Ro/YCgCo-Re 8-bit cannot be represented in 8-bit uint storage, so YCgCo-Ro/YCgCo-Re 8-bit requires a storage
  buffer to be at least twice widen ( 16-bit storage type )
- YCgCo-Ro/YCgCo-Re cannot be in limited YUV range at the moment, since it not clear how to this range reduction with
  dynamic bit-depth. For now, it always in full PC range.
- YCgCo current implementation allows to do a range reduction for TV range. That approximation have significant slowdown
  against full range transform. If that important range reduction may be removed and potential speed up about 30-60% is
  expected. 10/12-bit transformations may experience about 100-200% slowdown due to range reductions.
- YcCbcCrc ( YUV constant light ) primarily intended to be used in BT.2020 CL ( BT.2020 constant light ) color space,
  however ITU-R provides implementation for any possible kr, kb.
- YcCbcCrc is direct transformation due to its nature, so expect it to be at least 1000% slower, than any approximation
  matrices. It contains especially good acceleration for arm64-v8a with full FP16 support, however it still 1000% slower
  than other approximations. Against naive implementation current transformation about 400-600% faster.
- YDbDr should be computed from linearized components, however library expect that content already linearized and won't
  do that
- YDbDr requires very high precision matrix for decoding, however low precision approximation is used, some color info
  loss is highly possible especially in TV range
- YUV (4:1:1), YUV (4:1:0) does only box scaling, it's not very good, however it's only one available at the moment to
  keep performance in line

## Performance

#### Compare to libyuv:

All tests performed on Apple M3 Pro.

Not all the conversion path exists in libyuv so not everything can be benchmarked.

- Since very close approach to libyuv is used for YCbCr in general performance of decoding 8 bit is very close to
  libyuv, it is faster on arm64-v8a ( NEON ) on other platform is should be considered same.
- Encoding of YCbCr 8-bit faster in libyuv.
- 10/12 bit YCbCr in the library faster than libyuv.
- YcCbcCrc very slow transformation.
- YCgCo-Re/YCgCo-Ro fastest transformations available.
- YCgCo maybe reworked up to 100% performance gain for full range transform
- Some additional optimizations made for NEON, it is expected to be slightly better on arm64-v8a

#### Benchmark

```bash
Run on (12 X 24 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB
  L1 Instruction 128 KiB
  L2 Unified 4096 KiB (x12)
Load Average: 6.41, 4.63, 3.87
-----------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations
-----------------------------------------------------------------------
SparkyuvYCbCr444P10ToRGBA10      620223 ns       619880 ns         1135
LibYuvYCbCr444P10ToRGBA8        1314001 ns      1312990 ns          523
SparkyuvYCbCr422P10ToRGBA10      552089 ns       551719 ns         1274
LibYuvYCbCr422P10ToRGBA8        1092268 ns      1091448 ns          639
SparkyuvYCbCr420P10ToRGBA10      569507 ns       568057 ns         1272
LibYuvYCbCr420P10ToRGBA8        1090866 ns      1089950 ns          639
SparkyuvRGBAP10ToYCbCr420P10     494450 ns       494201 ns         1394
SparkyuvRGBA10ToYCbCr422P10      551609 ns       551187 ns         1275
SparkyuvRGBA10ToYCbCr444P10      493970 ns       493355 ns         1424
SparkyuvYCbCr444ToRGBA8          332122 ns       331858 ns         2106
LibYuvYCbCr444ToRGBA8            365939 ns       365582 ns         1909
SparkyuvYCbCr422ToRGBA8          346422 ns       346065 ns         2022
LibYuvYCbCr422ToRGBA8            458754 ns       458291 ns         1506
SparkyuvYCbCr420ToRGBA8          347903 ns       347500 ns         2020
LibYuvYCbCr420ToRGBA8            458823 ns       458356 ns         1504
SparkyuvRGBA8ToYCbCr420          543371 ns       542838 ns         1291
LibyuvRGBA8ToYCbCr420            270961 ns       270760 ns         2575
LibYuvRGBA8ToYCbCr422            402977 ns       402710 ns         1735
SparkyuvRGBA8ToYCbCr444          519188 ns       518637 ns         1354
SparkyuvRGBA8ToYCbCr422          552492 ns       551868 ns         1269
SparkyuvRGBA8ToYCbCr411          585180 ns       584749 ns         1216
SparkyuvYCbCr411ToRGBA8          412131 ns       411310 ns         1688
SparkyuvRGBA8ToYCbCr410          343129 ns       342826 ns         2041
SparkyuvYCbCr410ToRGBA8          410149 ns       409832 ns         1634
SparkyuvRGBA8ToYCbCr400          241941 ns       241749 ns         2873
SparkyuvYCbCr400ToRGBA8          174510 ns       173868 ns         4416
SparkyuvRGBA8ToNV21              605454 ns       604053 ns         1183
SparkyuvNV21ToRGBA8              394745 ns       393337 ns         1721
LibyuvNV21ToRGBA8                407932 ns       405722 ns         1744
SparkyuvRGBA8ToNV12              602741 ns       601062 ns         1134
SparkyuvNV12ToRGBA8              386739 ns       385713 ns         1775
LibyuvNV12ToRGBA8                387258 ns       386217 ns         1781
SparkyuvRGBA8ToNV16              630768 ns       629137 ns         1135
SparkyuvNV16ToRGBA8              391858 ns       390851 ns         1749
SparkyuvRGBA8ToNV24              542312 ns       541239 ns         1245
SparkyuvNV24ToRGBA8              402250 ns       400533 ns         1699
SparkyuvYCgCoR444ToRGBA8         401596 ns       400683 ns         1704
SparkyuvYCgCoR422ToRGBA8         397587 ns       396831 ns         1760
SparkyuvYCgCoR420ToRGBA8         398611 ns       398151 ns         1759
SparkyuvRGBA8ToYCgCoR420         237379 ns       236989 ns         2997
SparkyuvRGBA8ToYCgCoR422         295757 ns       295151 ns         2391
SparkyuvRGBA8ToYCgCoR444         257157 ns       256537 ns         2762
SparkyuvYCgCo444ToRGBA8          463513 ns       462416 ns         1504
SparkyuvYCgCo422ToRGBA8          474461 ns       473481 ns         1390
SparkyuvYCgCo420ToRGBA8          492716 ns       490531 ns         1421
SparkyuvRGBA8ToYCgCo420          368884 ns       367747 ns         1910
SparkyuvRGBA8ToYCgCo422          504085 ns       502893 ns         1000
SparkyuvRGBA8ToYCgCo444          374294 ns       373273 ns         1863
SparkyuvYcCbcCrc444ToRGBA8     16413192 ns     16376605 ns           43
SparkyuvYcCbcCrc422ToRGBA8     16129819 ns     16099205 ns           44
SparkyuvYcCbcCrc420ToRGBA8     16051337 ns     16028295 ns           44
SparkyuvRGBA8ToYcCbcCrc420     18415296 ns     18357973 ns           37
SparkyuvRGBA8ToYcCbcCrc422     18768154 ns     18751189 ns           37
SparkyuvRGBA8ToYcCbcCrc444     18134075 ns     18117923 ns           39
SparkyuvYIQ444ToRGBA8            661008 ns       660297 ns         1061
SparkyuvYIQ422ToRGBA8            718039 ns       717432 ns          976
SparkyuvYIQ420ToRGBA8            719200 ns       718537 ns          976
SparkyuvRGBA8ToYIQ420            516908 ns       515223 ns         1410
SparkyuvRGBA8ToYIQ422            689602 ns       683831 ns         1044
SparkyuvRGBA8ToYIQ444            637150 ns       632302 ns         1051
SparkyuvYDzDx444ToRGBA8          664280 ns       659891 ns         1134
SparkyuvYDzDx422ToRGBA8          694884 ns       691275 ns          994
SparkyuvYDzDx420ToRGBA8          680278 ns       679061 ns         1045
SparkyuvRGBA8ToYDzDx420         1014901 ns      1013896 ns          689
SparkyuvRGBA8ToYDzDx422         1158669 ns      1155639 ns          617
SparkyuvRGBA8ToYDzDx444          235243 ns       234500 ns         3005
SparkyuvYDbDr444ToRGBA8          683540 ns       681427 ns         1027
SparkyuvYDbDr422ToRGBA8          733433 ns       731622 ns          924
SparkyuvYDbDr420ToRGBA8          758845 ns       755145 ns          976
SparkyuvRGBA8ToYDbDr420          501362 ns       500499 ns         1401
SparkyuvRGBA8ToYDbDr422          686498 ns       684271 ns         1052
SparkyuvRGBA8ToYDbDr444          617821 ns       616719 ns         1134
SparkyuvRGB10BitToF16            376059 ns       375126 ns         1869
LibyuvPremultiply                213810 ns       213188 ns         3219
SparkyuvPremultiply              259198 ns       258596 ns         2720
SparkyuvUnpremultiply            895136 ns       893093 ns          797
LibyuvUnpremultiply             1382779 ns      1380055 ns          492
SparkyuvWide8To10Fixed           251359 ns       250905 ns         2765
SparkyuvWide8To10Dynamic         303078 ns       302300 ns         2254
SparkyuvSaturate10To8Fixed       307243 ns       306721 ns         2316
SparkyuvSaturate10To8Dynamic     317039 ns       316536 ns         2233
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