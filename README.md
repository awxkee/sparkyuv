# What's this

Library allows to convert RGB to Y'UV formats at high speed using platform SIMD acceleration and appropriate approximation level

## Supported YUV formats and layouts

Almost all YUV formats supported, everything you might need :)

| YUV      | 444 | 422 | 420 | 411 | 400 | NV12/NV21 | NV16/NV61 | NV24/NV42 |
|----------|-----|-----|-----|-----|-----|-----------|-----------|-----------|
| YCbCr    | ✅   | ✅   | ✅   | ✅   | ✅   | ✅         | ✅         | ✅         |
| YcCbcCrc | ✅   | ✅   | ✅   | ❌   | ❌   | N/A       | N/A       | N/A       |
| YCgCo    | ✅   | ✅   | ✅   | ❌   | ❌   | N/A       | N/A       | N/A       |
| YCgCo-Ro | ✅   | ✅   | ✅   | ❌   | ❌   | N/A       | N/A       | N/A       |
| YCgCo-Re | ✅   | ✅   | ✅   | ❌   | ❌   | N/A       | N/A       | N/A       |
| YDzDx    | ✅   | ✅   | ✅   | ❌   | ❌   | N/A       | N/A       | N/A       |
| YIQ      | ✅   | ✅   | ✅   | ❌   | ❌   | N/A       | N/A       | N/A       |
| YDbDr    | ✅   | ✅   | ✅   | ❌   | ❌   | N/A       | N/A       | N/A       |

When encoding to NV12/NV21, NV12/N61, 420, 422 for chroma subsampling bi-linear scaling is automatically applied. There
is no option to turn this off. Bi-linear scaling probably may be not so good as libsharpyuv, however also good.
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
Load Average: 3.30, 3.71, 4.90
-----------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations
-----------------------------------------------------------------------
SparkyuvYCbCr444P10ToRGBA10      609800 ns       607643 ns         1063
LibYuvYCbCr444P10ToRGBA8        1344075 ns      1339559 ns          540
SparkyuvYCbCr422P10ToRGBA10      693780 ns       683084 ns         1275
LibYuvYCbCr422P10ToRGBA8        1118818 ns      1116711 ns          592
SparkyuvYCbCr420P10ToRGBA10      586061 ns       584138 ns         1178
LibYuvYCbCr420P10ToRGBA8        1119826 ns      1117162 ns          630
SparkyuvRGBAP10ToYCbCr420P10     511709 ns       510207 ns         1423
SparkyuvRGBA10ToYCbCr422P10      584649 ns       582449 ns         1210
SparkyuvRGBA10ToYCbCr444P10      519780 ns       516551 ns         1327
SparkyuvYCbCr444ToRGBA8          345842 ns       344921 ns         1977
LibYuvYCbCr444ToRGBA8            381061 ns       379909 ns         1884
SparkyuvYCbCr422ToRGBA8          363049 ns       362152 ns         1920
LibYuvYCbCr422ToRGBA8            467196 ns       466491 ns         1450
SparkyuvYCbCr420ToRGBA8          356870 ns       356170 ns         1970
LibYuvYCbCr420ToRGBA8            477062 ns       476148 ns         1483
SparkyuvRGBA8ToYCbCr420          548881 ns       548173 ns         1285
LibyuvRGBA8ToYCbCr420            272835 ns       272609 ns         2553
LibYuvRGBA8ToYCbCr422            410960 ns       410343 ns         1733
SparkyuvRGBA8ToYCbCr444          541296 ns       538422 ns         1354
SparkyuvRGBA8ToNV21              637339 ns       631729 ns         1137
SparkyuvNV21ToRGBA8              392456 ns       390590 ns         1833
LibyuvNV21ToRGBA8                772765 ns       436924 ns         1782
SparkyuvRGBA8ToNV12              637842 ns       629930 ns         1109
SparkyuvNV12ToRGBA8              385766 ns       385014 ns         1776
LibyuvNV12ToRGBA8                410236 ns       408528 ns         1832
SparkyuvRGBA8ToNV16              651538 ns       649222 ns         1022
SparkyuvNV16ToRGBA8              380074 ns       379075 ns         1743
SparkyuvRGBA8ToNV24              532233 ns       531790 ns         1321
SparkyuvNV24ToRGBA8              379270 ns       379073 ns         1844
SparkyuvYCgCoR444ToRGBA8         391539 ns       391269 ns         1789
SparkyuvYCgCoR422ToRGBA8         393832 ns       393533 ns         1787
SparkyuvYCgCoR420ToRGBA8         403814 ns       402959 ns         1789
SparkyuvRGBA8ToYCgCoR420         237720 ns       237261 ns         2944
SparkyuvRGBA8ToYCgCoR422         291581 ns       291124 ns         2438
SparkyuvRGBA8ToYCgCoR444         258335 ns       257853 ns         2804
SparkyuvYCgCo444ToRGBA8          486887 ns       485087 ns         1387
SparkyuvYCgCo422ToRGBA8          479746 ns       478732 ns         1464
SparkyuvYCgCo420ToRGBA8          486307 ns       484605 ns         1455
SparkyuvRGBA8ToYCgCo420          396058 ns       393833 ns         1825
SparkyuvRGBA8ToYCgCo422          515246 ns       513669 ns         1294
SparkyuvRGBA8ToYCgCo444          387669 ns       385781 ns         1783
SparkyuvYcCbcCrc444ToRGBA8     16604034 ns     16556625 ns           40
SparkyuvYcCbcCrc422ToRGBA8     16192556 ns     16167279 ns           43
SparkyuvYcCbcCrc420ToRGBA8     16457130 ns     16429119 ns           42
SparkyuvRGBA8ToYcCbcCrc420     18353658 ns     18327921 ns           38
SparkyuvRGBA8ToYcCbcCrc422     19498998 ns     19441853 ns           34
SparkyuvRGBA8ToYcCbcCrc444     18720504 ns     18689789 ns           38
SparkyuvYIQ444ToRGBA8            689786 ns       687931 ns         1051
SparkyuvYIQ422ToRGBA8            751729 ns       749262 ns          975
SparkyuvYIQ420ToRGBA8            718858 ns       718264 ns          976
SparkyuvRGBA8ToYIQ420            506654 ns       505567 ns         1380
SparkyuvRGBA8ToYIQ422            687676 ns       685889 ns         1037
SparkyuvRGBA8ToYIQ444            648154 ns       645391 ns         1152
SparkyuvYDzDx444ToRGBA8          692211 ns       688293 ns          977
SparkyuvYDzDx422ToRGBA8          696440 ns       694803 ns         1012
SparkyuvYDzDx420ToRGBA8          687772 ns       686455 ns         1044
SparkyuvRGBA8ToYDzDx420         1041240 ns      1039100 ns          688
SparkyuvRGBA8ToYDzDx422         1175739 ns      1172213 ns          616
SparkyuvRGBA8ToYDzDx444          242779 ns       242067 ns         2889
SparkyuvYDbDr444ToRGBA8          698071 ns       695953 ns         1061
SparkyuvYDbDr422ToRGBA8          732089 ns       730239 ns          916
SparkyuvYDbDr420ToRGBA8          747276 ns       744525 ns          976
SparkyuvRGBA8ToYDbDr420          523672 ns       520410 ns         1309
SparkyuvRGBA8ToYDbDr422          702100 ns       699818 ns          997
SparkyuvRGBA8ToYDbDr444          625860 ns       624225 ns         1143
SparkyuvRGB10BitToF16            386778 ns       385704 ns         1738
LibyuvPremultiply                213954 ns       213497 ns         3291
SparkyuvPremultiply              258468 ns       257823 ns         2729
SparkyuvUnpremultiply            929987 ns       927071 ns          801
LibyuvUnpremultiply             1395256 ns      1388599 ns          484
SparkyuvWide8To10Fixed           249801 ns       249301 ns         2801
SparkyuvWide8To10Dynamic         317945 ns       316749 ns         2320
SparkyuvSaturate10To8Fixed       311601 ns       310780 ns         2322
SparkyuvSaturate10To8Dynamic     317014 ns       316063 ns         2238
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