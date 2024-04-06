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
- YDbDr required very high precision matrix for decoding, however low precision approximation is used, some color info loss is highly possible especially in TV range

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
Load Average: 3.61, 3.07, 2.71
-----------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations
-----------------------------------------------------------------------
SparkyuvYCbCr444P10ToRGBA10      550366 ns       549948 ns         1287
LibYuvYCbCr444P10ToRGBA8        1308003 ns      1307043 ns          538
SparkyuvYCbCr422P10ToRGBA10      571971 ns       571742 ns         1271
LibYuvYCbCr422P10ToRGBA8        1099869 ns      1099643 ns          639
SparkyuvYCbCr420P10ToRGBA10      575550 ns       575350 ns         1267
LibYuvYCbCr420P10ToRGBA8        1127683 ns      1124824 ns          618
SparkyuvRGBAP10ToYCbCr420P10     615994 ns       614896 ns         1129
SparkyuvRGBA10ToYCbCr422P10      725135 ns       718817 ns          960
SparkyuvRGBA10ToYCbCr444P10      613594 ns       611047 ns         1125
SparkyuvYCbCr444ToRGBA8          465757 ns       462832 ns         1557
LibYuvYCbCr444ToRGBA8            404306 ns       402583 ns         1774
SparkyuvYCbCr422ToRGBA8          505272 ns       502037 ns         1507
LibYuvYCbCr422ToRGBA8            470946 ns       469183 ns         1341
SparkyuvYCbCr420ToRGBA8          465562 ns       464248 ns         1503
LibYuvYCbCr420ToRGBA8            470852 ns       469671 ns         1473
SparkyuvRGBA8ToYCbCr420          523110 ns       521630 ns         1348
LibyuvRGBA8ToYCbCr420            277892 ns       277030 ns         2510
SparkyuvRGBA8ToYCbCr422          735605 ns       733550 ns          949
LibYuvRGBA8ToYCbCr422            404793 ns       404317 ns         1714
SparkyuvRGBA8ToYCbCr444          608959 ns       608084 ns         1152
SparkyuvYCgCoR444ToRGBA8         410194 ns       408953 ns         1760
SparkyuvYCgCoR422ToRGBA8         407927 ns       406654 ns         1720
SparkyuvYCgCoR420ToRGBA8         407004 ns       406035 ns         1717
SparkyuvRGBA8ToYCgCoR420         255195 ns       254620 ns         2743
SparkyuvRGBA8ToYCgCoR422         324746 ns       324075 ns         2153
SparkyuvRGBA8ToYCgCoR444         257719 ns       257132 ns         2684
SparkyuvYCgCo444ToRGBA8          468750 ns       467536 ns         1516
SparkyuvYCgCo422ToRGBA8          484087 ns       482108 ns         1451
SparkyuvYCgCo420ToRGBA8          486016 ns       484576 ns         1453
SparkyuvRGBA8ToYCgCo420          370847 ns       369858 ns         1904
SparkyuvRGBA8ToYCgCo422          548260 ns       546906 ns         1282
SparkyuvRGBA8ToYCgCo444          374832 ns       373820 ns         1849
SparkyuvYcCbcCrc444ToRGBA8     16107218 ns     16083767 ns           43
SparkyuvYcCbcCrc422ToRGBA8     15951363 ns     15924636 ns           44
SparkyuvYcCbcCrc420ToRGBA8     16165950 ns     16137545 ns           44
SparkyuvRGBA8ToYcCbcCrc420     18620914 ns     18564395 ns           38
SparkyuvRGBA8ToYcCbcCrc422     19361869 ns     19301865 ns           37
SparkyuvRGBA8ToYcCbcCrc444     18771207 ns     18714474 ns           38
SparkyuvYIQ444ToRGBA8            626179 ns       623892 ns         1116
SparkyuvYIQ422ToRGBA8            691582 ns       689509 ns         1042
SparkyuvYIQ420ToRGBA8            681508 ns       679841 ns         1027
SparkyuvRGBA8ToYIQ420            632046 ns       630068 ns         1113
SparkyuvRGBA8ToYIQ422            715291 ns       713345 ns          995
SparkyuvRGBA8ToYIQ444            630726 ns       629078 ns         1134
SparkyuvYDzDx444ToRGBA8          735837 ns       733462 ns          940
SparkyuvYDzDx422ToRGBA8          784505 ns       782142 ns          906
SparkyuvYDzDx420ToRGBA8          788515 ns       780979 ns          897
SparkyuvRGBA8ToYDzDx420         1037394 ns      1032611 ns          689
SparkyuvRGBA8ToYDzDx422         1132408 ns      1129809 ns          608
SparkyuvRGBA8ToYDzDx444          238964 ns       238226 ns         2901
SparkyuvYDbDr444ToRGBA8          672767 ns       671066 ns         1056
SparkyuvYDbDr422ToRGBA8          736621 ns       734111 ns          975
SparkyuvYDbDr420ToRGBA8          731772 ns       730075 ns          964
SparkyuvRGBA8ToYDbDr420          620523 ns       619666 ns         1132
SparkyuvRGBA8ToYDbDr422          712573 ns       710845 ns          993
SparkyuvRGBA8ToYDbDr444          629323 ns       627746 ns         1136
LibyuvPremultiply                213606 ns       212369 ns         3289
SparkyuvPremultiply              317568 ns       316792 ns         2183
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