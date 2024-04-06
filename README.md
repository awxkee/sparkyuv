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
SparkyuvYUV444P10ToRGBA10      551118 ns       550404 ns         1221
LibYuvYUV444P10ToRGBA8        1306243 ns      1303076 ns          537
SparkyuvYUV422P10ToRGBA10      553445 ns       552051 ns         1269
LibYuvYUV422P10ToRGBA8        1101809 ns      1097574 ns          639
SparkyuvYUV420P10ToRGBA10      554892 ns       552300 ns         1263
LibYuvYUV420P10ToRGBA8        1103214 ns      1099146 ns          639
SparkyuvRGBAP10ToYUV420P10     584238 ns       583105 ns         1204
SparkyuvRGBA10ToYUV422P10      674540 ns       673739 ns         1041
SparkyuvRGBA10ToYUV444P10      585881 ns       585132 ns         1198
SparkyuvYUV444ToRGBA8          401705 ns       401151 ns         1744
LibYuvYUV444ToRGBA8            368429 ns       368086 ns         1902
SparkyuvYUV422ToRGBA8          455233 ns       454815 ns         1536
LibYuvYUV422ToRGBA8            463601 ns       462949 ns         1513
SparkyuvYUV420ToRGBA8          455347 ns       454899 ns         1536
LibYuvYUV420ToRGBA8            465197 ns       464310 ns         1511
SparkyuvRGBA8ToYUV420          515248 ns       514565 ns         1355
LibyuvRGBA8ToYUV420            272279 ns       272076 ns         2562
SparkyuvRGBA8ToYUV422          726667 ns       725647 ns          962
LibYuvRGBA8ToYUV422            403226 ns       402867 ns         1734
SparkyuvRGBA8ToYUV444          620285 ns       616267 ns         1155
SparkyuvYCgCoR444ToRGBA8       395872 ns       394793 ns         1767
SparkyuvYCgCoR422ToRGBA8       395195 ns       394794 ns         1770
SparkyuvYCgCoR420ToRGBA8       395537 ns       395018 ns         1771
SparkyuvRGBA8ToYCgCoR420       248834 ns       248530 ns         2810
SparkyuvRGBA8ToYCgCoR422       317993 ns       317633 ns         2201
SparkyuvRGBA8ToYCgCoR444       250690 ns       250395 ns         2709
SparkyuvYCgCo444ToRGBA8        456278 ns       455835 ns         1532
SparkyuvYCgCo422ToRGBA8        471102 ns       470494 ns         1487
SparkyuvYCgCo420ToRGBA8        470865 ns       470233 ns         1486
SparkyuvRGBA8ToYCgCo420        360018 ns       359658 ns         1941
SparkyuvRGBA8ToYCgCo422        541583 ns       540184 ns         1291
SparkyuvRGBA8ToYCgCo444        366007 ns       365610 ns         1907
SparkyuvYcCbcCrc444ToRGBA8   16105818 ns     16086000 ns           43
SparkyuvYcCbcCrc422ToRGBA8   16046212 ns     16024455 ns           44
SparkyuvYcCbcCrc420ToRGBA8   16073391 ns     16048614 ns           44
SparkyuvRGBA8ToYcCbcCrc420   18330530 ns     18305684 ns           38
SparkyuvRGBA8ToYcCbcCrc422   19055117 ns     19029757 ns           37
SparkyuvRGBA8ToYcCbcCrc444   18423327 ns     18399026 ns           38
SparkyuvYDzDx444ToRGBA8        719628 ns       718599 ns          975
SparkyuvYDzDx422ToRGBA8        764828 ns       763778 ns          918
SparkyuvYDzDx420ToRGBA8        764519 ns       763557 ns          917
SparkyuvRGBA8ToYDzDx420       1018691 ns      1017451 ns          690
SparkyuvRGBA8ToYDzDx422       1149731 ns      1148238 ns          609
SparkyuvRGBA8ToYDzDx444        231984 ns       231768 ns         3004
LibyuvPremultiply              208437 ns       208235 ns         3344
SparkyuvPremultiply            307503 ns       306999 ns         2275
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