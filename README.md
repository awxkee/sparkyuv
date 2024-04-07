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

- Since very close approach to libyuv is used for YCbCr in general performance of decoding 8 bit is very close to libyuv, sometimes faster however in general it should be considered as same.
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
Load Average: 3.43, 2.95, 2.99
-----------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations
-----------------------------------------------------------------------
SparkyuvYCbCr444P10ToRGBA10      552068 ns       550974 ns         1288
LibYuvYCbCr444P10ToRGBA8        1323677 ns      1321660 ns          532
SparkyuvYCbCr422P10ToRGBA10      554385 ns       553683 ns         1270
LibYuvYCbCr422P10ToRGBA8        1146141 ns      1140665 ns          639
SparkyuvYCbCr420P10ToRGBA10      577148 ns       575087 ns         1088
LibYuvYCbCr420P10ToRGBA8        1141446 ns      1137501 ns          641
SparkyuvRGBAP10ToYCbCr420P10     595141 ns       584473 ns         1377
SparkyuvRGBA10ToYCbCr422P10      614998 ns       607958 ns         1174
SparkyuvRGBA10ToYCbCr444P10      547730 ns       544443 ns         1258
SparkyuvYCbCr444ToRGBA8          382512 ns       381960 ns         1826
LibYuvYCbCr444ToRGBA8            374970 ns       374276 ns         1843
SparkyuvYCbCr422ToRGBA8          410954 ns       410578 ns         1701
LibYuvYCbCr422ToRGBA8            459863 ns       459478 ns         1516
SparkyuvYCbCr420ToRGBA8          410857 ns       410518 ns         1700
LibYuvYCbCr420ToRGBA8            461247 ns       460743 ns         1518
SparkyuvRGBA8ToYCbCr420          451737 ns       450874 ns         1550
LibyuvRGBA8ToYCbCr420            273577 ns       273188 ns         2560
SparkyuvRGBA8ToYCbCr422          570610 ns       569903 ns         1221
LibYuvRGBA8ToYCbCr422            402970 ns       402711 ns         1735
SparkyuvRGBA8ToYCbCr444          513079 ns       512371 ns         1363
SparkyuvYCgCoR444ToRGBA8         393244 ns       392874 ns         1777
SparkyuvYCgCoR422ToRGBA8         393980 ns       393595 ns         1772
SparkyuvYCgCoR420ToRGBA8         395363 ns       394800 ns         1777
SparkyuvRGBA8ToYCgCoR420         271534 ns       270054 ns         2703
SparkyuvRGBA8ToYCgCoR422         336881 ns       335309 ns         2039
SparkyuvRGBA8ToYCgCoR444         270773 ns       269246 ns         2626
SparkyuvYCgCo444ToRGBA8          494904 ns       492146 ns         1318
SparkyuvYCgCo422ToRGBA8          503000 ns       500609 ns         1000
SparkyuvYCgCo420ToRGBA8          489938 ns       488997 ns         1441
SparkyuvRGBA8ToYCgCo420          381438 ns       379971 ns         1804
SparkyuvRGBA8ToYCgCo422          578307 ns       575003 ns         1195
SparkyuvRGBA8ToYCgCo444          392821 ns       391046 ns         1774
SparkyuvYcCbcCrc444ToRGBA8     16930076 ns     16839268 ns           41
SparkyuvYcCbcCrc422ToRGBA8     16979860 ns     16881905 ns           42
SparkyuvYcCbcCrc420ToRGBA8     16439615 ns     16374744 ns           43
SparkyuvRGBA8ToYcCbcCrc420     18537978 ns     18484333 ns           39
SparkyuvRGBA8ToYcCbcCrc422     19906911 ns     19798229 ns           35
SparkyuvRGBA8ToYcCbcCrc444     19245489 ns     19154829 ns           35
SparkyuvYIQ444ToRGBA8            656856 ns       653369 ns         1036
SparkyuvYIQ422ToRGBA8            706572 ns       703642 ns          958
SparkyuvYIQ420ToRGBA8            702505 ns       700010 ns         1019
SparkyuvRGBA8ToYIQ420            579146 ns       576891 ns         1165
SparkyuvRGBA8ToYIQ422            623863 ns       621263 ns         1137
SparkyuvRGBA8ToYIQ444            584510 ns       580947 ns         1220
SparkyuvYDzDx444ToRGBA8          626611 ns       625307 ns         1023
SparkyuvYDzDx422ToRGBA8          675487 ns       674459 ns         1032
SparkyuvYDzDx420ToRGBA8          676196 ns       675182 ns         1039
SparkyuvRGBA8ToYDzDx420         1026178 ns      1023539 ns          683
SparkyuvRGBA8ToYDzDx422         1127812 ns      1126132 ns          622
SparkyuvRGBA8ToYDzDx444          234294 ns       233828 ns         2999
SparkyuvYDbDr444ToRGBA8          618782 ns       617931 ns         1135
SparkyuvYDbDr422ToRGBA8          671566 ns       670810 ns         1036
SparkyuvYDbDr420ToRGBA8          690405 ns       688193 ns         1047
SparkyuvRGBA8ToYDbDr420          592501 ns       589796 ns         1235
SparkyuvRGBA8ToYDbDr422          619320 ns       617306 ns         1151
SparkyuvRGBA8ToYDbDr444          547096 ns       546120 ns         1292
LibyuvPremultiply                221898 ns       221095 ns         3199
SparkyuvPremultiply              332122 ns       330568 ns         2001
SparkyuvWide8To10Fixed           260829 ns       260308 ns         2532
SparkyuvWide8To10Dynamic         316163 ns       315957 ns         2200
SparkyuvSaturate10To8Fixed       321728 ns       321187 ns         2181
SparkyuvSaturate10To8Dynamic     327312 ns       326248 ns         2154
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