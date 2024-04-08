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
| YDbDr    | ✅   | ✅   | ✅   | ❌   | N/A       | N/A       | N/A       |

When encoding to NV12/NV21, NV12/N61, 420, 422 for chroma subsampling bi-linear scaling is automatically applied. There
is no option to turn this off.
Due to nature of this transformation in those cases it is exceptionally fast.

### Things to note

- YCgCo-Ro/YCgCo-Re 8-bit cannot be represented in 8-bit uint storage, so YCgCo-Ro/YCgCo-Re 8-bit requires a storage buffer to be at least twice widen ( 16-bit storage type )
- YCgCo-Ro/YCgCo-Re cannot be in limited YUV range at the moment, since it not clear how to this range reduction with dynamic bit-depth. For now, it always in full PC range.
- YCgCo current implementation allows to do a range reduction for TV range. That approximation have significant slowdown against full range transform. If that important range reduction may be removed and potential speed up about 30-60% is expected. 10/12-bit transformations may experience about 100-200% slowdown due to range reductions.
- YcCbcCrc ( YUV constant light ) primarily intended to be used in BT.2020 CL ( BT.2020 constant light ) color space, however ITU-R provides implementation for any possible kr, kb.
- YcCbcCrc is direct transformation due to its nature, so expect it to be at least 1000% slower, than any approximation matrices. It contains especially good acceleration for arm64-v8a with full FP16 support, however it still 1000% slower than other approximations. Against naive implementation current transformation about 400-600% faster.
- YDbDr should be computed from linearized components, however library expect that content already linearized and won't do that
- YDbDr requires very high precision matrix for decoding, however low precision approximation is used, some color info loss is highly possible especially in TV range

## Performance

#### Compare to libyuv:
All tests performed on Apple M3 Pro.

Not all the conversion path exists in libyuv so not everything can be benchmarked.

- Since very close approach to libyuv is used for YCbCr in general performance of decoding 8 bit is very close to libyuv, it is faster on arm64-v8a ( NEON ) on other platform is should be considered same.
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
Load Average: 2.82, 2.24, 2.22
-----------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations
-----------------------------------------------------------------------
SparkyuvYCbCr444P10ToRGBA10      550554 ns       549983 ns         1269
LibYuvYCbCr444P10ToRGBA8        1292287 ns      1291283 ns          544
SparkyuvYCbCr422P10ToRGBA10      548486 ns       548172 ns         1284
LibYuvYCbCr422P10ToRGBA8        1059847 ns      1059062 ns          663
SparkyuvYCbCr420P10ToRGBA10      549154 ns       548833 ns         1278
LibYuvYCbCr420P10ToRGBA8        1057210 ns      1056456 ns          664
SparkyuvRGBAP10ToYCbCr420P10     511638 ns       511268 ns         1390
SparkyuvRGBA10ToYCbCr422P10      547983 ns       547713 ns         1305
SparkyuvRGBA10ToYCbCr444P10      489071 ns       488796 ns         1444
SparkyuvYCbCr444ToRGBA8          337801 ns       337679 ns         2067
LibYuvYCbCr444ToRGBA8            367485 ns       367367 ns         1892
SparkyuvYCbCr422ToRGBA8          351926 ns       351797 ns         1978
LibYuvYCbCr422ToRGBA8            450554 ns       450327 ns         1548
SparkyuvYCbCr420ToRGBA8          342186 ns       342071 ns         2034
LibYuvYCbCr420ToRGBA8            457029 ns       455826 ns         1535
SparkyuvRGBA8ToYCbCr420          451406 ns       450388 ns         1535
LibyuvRGBA8ToYCbCr420            272514 ns       272350 ns         2533
SparkyuvRGBA8ToYCbCr422          537356 ns       536586 ns         1283
LibYuvRGBA8ToYCbCr422            405447 ns       404808 ns         1734
SparkyuvRGBA8ToYCbCr444          505435 ns       504381 ns         1397
SparkyuvYCgCoR444ToRGBA8         397158 ns       396426 ns         1767
SparkyuvYCgCoR422ToRGBA8         393266 ns       392875 ns         1769
SparkyuvYCgCoR420ToRGBA8         396207 ns       395480 ns         1755
SparkyuvRGBA8ToYCgCoR420         249840 ns       249308 ns         2802
SparkyuvRGBA8ToYCgCoR422         322877 ns       322131 ns         2168
SparkyuvRGBA8ToYCgCoR444         251333 ns       250869 ns         2789
SparkyuvYCgCo444ToRGBA8          455380 ns       455187 ns         1527
SparkyuvYCgCo422ToRGBA8          462062 ns       461275 ns         1516
SparkyuvYCgCo420ToRGBA8          458182 ns       457464 ns         1514
SparkyuvRGBA8ToYCgCo420          360785 ns       360187 ns         1954
SparkyuvRGBA8ToYCgCo422          523707 ns       523334 ns         1337
SparkyuvRGBA8ToYCgCo444          365308 ns       365173 ns         1912
SparkyuvYcCbcCrc444ToRGBA8     16047198 ns     16017533 ns           45
SparkyuvYcCbcCrc422ToRGBA8     15839813 ns     15818909 ns           44
SparkyuvYcCbcCrc420ToRGBA8     15824769 ns     15804705 ns           44
SparkyuvRGBA8ToYcCbcCrc420     18345577 ns     18335105 ns           38
SparkyuvRGBA8ToYcCbcCrc422     19018042 ns     18992649 ns           37
SparkyuvRGBA8ToYcCbcCrc444     17551917 ns     17533237 ns           38
SparkyuvYIQ444ToRGBA8            741603 ns       740356 ns          949
SparkyuvYIQ422ToRGBA8            783603 ns       782424 ns          899
SparkyuvYIQ420ToRGBA8            800779 ns       797819 ns          896
SparkyuvRGBA8ToYIQ420            535511 ns       533711 ns         1324
SparkyuvRGBA8ToYIQ422            694581 ns       693297 ns         1043
SparkyuvRGBA8ToYIQ444            640147 ns       639971 ns         1060
SparkyuvYDzDx444ToRGBA8          646714 ns       646269 ns         1049
SparkyuvYDzDx422ToRGBA8          699578 ns       698138 ns          978
SparkyuvYDzDx420ToRGBA8          687259 ns       686288 ns         1013
SparkyuvRGBA8ToYDzDx420         1022749 ns      1021451 ns          690
SparkyuvRGBA8ToYDzDx422         1142889 ns      1140935 ns          612
SparkyuvRGBA8ToYDzDx444          232698 ns       232486 ns         2913
SparkyuvYDbDr444ToRGBA8          746238 ns       744218 ns          937
SparkyuvYDbDr422ToRGBA8          787738 ns       786268 ns          891
SparkyuvYDbDr420ToRGBA8          791881 ns       790623 ns          886
SparkyuvRGBA8ToYDbDr420          516041 ns       515300 ns         1367
SparkyuvRGBA8ToYDbDr422          665152 ns       664149 ns         1063
SparkyuvRGBA8ToYDbDr444          623468 ns       621349 ns         1147
LibyuvPremultiply                209651 ns       209364 ns         3263
SparkyuvPremultiply              247114 ns       246728 ns         2806
SparkyuvWide8To10Fixed           249122 ns       248451 ns         2830
SparkyuvWide8To10Dynamic         307184 ns       306176 ns         2251
SparkyuvSaturate10To8Fixed       308111 ns       307230 ns         2265
SparkyuvSaturate10To8Dynamic     332026 ns       330561 ns         2114
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