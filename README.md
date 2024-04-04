### Supported YUV formats table

| YUV      | 444 | 422 | 420 | 400 | NV12/NV21 | NV16/NV61 | NV24/NV42 |
|----------|-----|-----|-----|-----|-----------|-----------|-----------|
| YCbCr    | ✅   | ✅   | ✅   | ❌   | ✅         | ✅         | ✅         |
| YcCbcCrc | ✅   | ✅   | ✅   | ❌   | N/A       | N/A       | N/A       |
| YCgCo    | ✅   | ✅   | ✅   | ❌   | N/A       | N/A       | N/A       |
| YCgCo    | ✅   | ✅   | ✅   | ❌   | N/A       | N/A       | N/A       |
When encoding to NV12/NV21, NV12/N61, 420, 422 for chroma subsampling bi-linear scaling is automatically applied. There is no option to turn this off.
Due to nature of this transformation in those cases it is exceptionally fast.

### Targets
Sparkyuv uses libhwy as backend, so that means on all supported platforms it's uses SIMD for acceleration

Sparkyuv (the same as libhwy) supports 22 targets, listed in alphabetical order of platform:

-   Any: `EMU128`, `SCALAR`;
-   Arm: `NEON` (Armv7+), `SVE`, `SVE2`, `SVE_256`, `SVE2_128`;
-   IBM Z: `Z14`, `Z15`;
-   POWER: `PPC8` (v2.07), `PPC9` (v3.0), `PPC10` (v3.1B, not yet supported
    due to compiler bugs, see #1207; also requires QEMU 7.2);
-   RISC-V: `RVV` (1.0);
-   WebAssembly: `WASM`, `WASM_EMU256` (a 2x unrolled version of wasm128,
    enabled if `HWY_WANT_WASM2` is defined. This will remain supported until it
    is potentially superseded by a future version of WASM.);
-   x86:
    -   `SSE2`
    -   `SSSE3` (~Intel Core)
    -   `SSE4` (~Nehalem, also includes AES + CLMUL).
    -   `AVX2` (~Haswell, also includes BMI2 + F16 + FMA)
    -   `AVX3` (~Skylake, AVX-512F/BW/CD/DQ/VL)
    -   `AVX3_DL` (~Icelake, includes BitAlg + CLMUL + GFNI + VAES + VBMI +
        VBMI2 + VNNI + VPOPCNT; requires opt-in by defining `HWY_WANT_AVX3_DL`
        unless compiling for static dispatch),
    -   `AVX3_ZEN4` (like AVX3_DL but optimized for AMD Zen4; requires opt-in by
        defining `HWY_WANT_AVX3_ZEN4` if compiling for static dispatch)
    -   `AVX3_SPR` (~Sapphire Rapids, includes AVX-512FP16)
