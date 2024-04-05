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
Load Average: 3.56, 3.63, 3.55
---------------------------------------------------------------------
Benchmark                           Time             CPU   Iterations
---------------------------------------------------------------------
SparkyuvYUV444P10ToRGBA10      591267 ns       587107 ns         1279
LibYuvYUV444P10ToRGBA8        1443675 ns      1433590 ns          502
SparkyuvYUV422P10ToRGBA10      622430 ns       615820 ns         1183
LibYuvYUV422P10ToRGBA8        1188320 ns      1180468 ns          586
SparkyuvYUV420P10ToRGBA10      604754 ns       597465 ns         1113
LibYuvYUV420P10ToRGBA8        1108164 ns      1103271 ns          639
SparkyuvRGBAP10ToYUV420P10     582013 ns       581083 ns         1203
SparkyuvRGBA10ToYUV422P10      664671 ns       663213 ns         1064
SparkyuvRGBA10ToYUV444P10      593346 ns       591960 ns         1173
SparkyuvYUV444ToRGBA8          412952 ns       411853 ns         1704
LibYuvYUV444ToRGBA8            376012 ns       374945 ns         1869
SparkyuvYUV422ToRGBA8          480432 ns       478300 ns         1519
LibYuvYUV422ToRGBA8            479219 ns       477883 ns         1355
SparkyuvYUV420ToRGBA8          488677 ns       486058 ns         1453
LibYuvYUV420ToRGBA8            488411 ns       486123 ns         1441
SparkyuvRGBA8ToYUV420          550135 ns       546927 ns         1337
LibyuvRGBA8ToYUV420            276986 ns       276331 ns         2495
SparkyuvRGBA8ToYUV422          726279 ns       724597 ns          961
LibYuvRGBA8ToYUV422            403467 ns       403144 ns         1733
SparkyuvRGBA8ToYUV444          604744 ns       603920 ns         1155
SparkyuvYCgCoR444ToRGBA8       413621 ns       411799 ns         1745
SparkyuvYCgCoR422ToRGBA8       416174 ns       413427 ns         1744
SparkyuvYCgCoR420ToRGBA8       419775 ns       418379 ns         1629
SparkyuvRGBA8ToYCgCoR420       264105 ns       260870 ns         2598
SparkyuvRGBA8ToYCgCoR422       326052 ns       325486 ns         2138
SparkyuvRGBA8ToYCgCoR444       265649 ns       264311 ns         2624
SparkyuvYCgCo444ToRGBA8        473239 ns       472128 ns         1440
SparkyuvYCgCo422ToRGBA8        528657 ns       527626 ns         1202
SparkyuvYCgCo420ToRGBA8        519041 ns       518351 ns         1000
SparkyuvRGBA8ToYCgCo420        406396 ns       404329 ns         1767
SparkyuvRGBA8ToYCgCo422        602829 ns       600687 ns         1257
SparkyuvRGBA8ToYCgCo444        403337 ns       401746 ns         1729
SparkyuvYcCbcCrc444ToRGBA8   16209038 ns     16176341 ns           44
SparkyuvYcCbcCrc422ToRGBA8   16824628 ns     16749682 ns           44
SparkyuvYcCbcCrc420ToRGBA8   16397463 ns     16335024 ns           42
SparkyuvRGBA8ToYcCbcCrc420   19834527 ns     19706974 ns           38
SparkyuvRGBA8ToYcCbcCrc422   19211038 ns     19161583 ns           36
SparkyuvRGBA8ToYcCbcCrc444   18733719 ns     18688514 ns           35
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
