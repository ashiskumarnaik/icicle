# ICICLE Core

ICICLE Core is a library written in C++/CUDA. All the ICICLE primitives are implemented within ICICLE Core.

The Core is split into logical modules that can be compiled into static libraries using different [strategies](#compilation-strategies). You can then [link](#linking) these libraries with your C++ project or write your own [bindings](#writing-new-bindings-for-icicle) for other programming languages. If you want to use ICICLE with existing bindings please refer to the [Rust](./rust-bindings) or [Golang](./golang-bindings) bindings documentation.

## Supported curves, fields and operations

### Supported curves and operations

| Operation\Curve | [bn254](https://neuromancer.sk/std/bn/bn254) | [bls12-377](https://neuromancer.sk/std/bls/BLS12-377) | [bls12-381](https://neuromancer.sk/std/bls/BLS12-381) | [bw6-761](https://eprint.iacr.org/2020/351) | grumpkin |
| --- | :---: | :---: | :---: | :---: | :---: |
| [MSM][MSM_DOCS] | ✅ | ✅ | ✅ | ✅ | ✅ |
| G2  | ✅ | ✅ | ✅ | ✅ | ❌ |
| [NTT][NTT_DOCS] | ✅ | ✅ | ✅ | ✅ | ❌ |
| ECNTT | ✅ | ✅ | ✅ | ✅ | ❌ |
| [VecOps][VECOPS_CODE] | ✅ | ✅ | ✅ | ✅ | ✅ |
| [Polynomials][POLY_DOCS] | ✅ | ✅ | ✅ | ✅ | ❌ |
| [Poseidon](primitives/poseidon) | ✅ | ✅ | ✅ | ✅ | ✅ |
| [Merkle Tree](primitives/poseidon#the-tree-builder) | ✅ | ✅ | ✅ | ✅ | ✅ |

### Supported fields and operations

| Operation\Field | [babybear](https://eprint.iacr.org/2023/824.pdf) | [Stark252](https://docs.starknet.io/documentation/architecture_and_concepts/Cryptography/p-value/) |
| --- | :---: | :---: |
| [VecOps][VECOPS_CODE] | ✅ | ✅ |
| [Polynomials][POLY_DOCS] | ✅ | ✅ |
| [NTT][NTT_DOCS] | ✅ | ✅ |
| Extension Field | ✅ | ❌ |

### Supported hashes

| Hash | Sizes |
| --- | :---: |
| Keccak | 256, 512 |

## Compilation strategies

Most of the codebase is curve/field agnostic, which means it can be compiled for different curves and fields. When you build ICICLE Core you choose a single curve or field. If you need multiple curves or fields, you compile ICICLE once per curve or field that is needed. It's that simple. Currently, the following choices are supported:

- [Field mode][COMPILE_FIELD_MODE] - used for STARK fields like BabyBear / Mersenne / Goldilocks. Includes field arithmetic, NTT, Poseidon, Extension fields and other primitives.
- [Curve mode][COMPILE_CURVE_MODE] - used for SNARK curves like BN254 / BLS curves / Grumpkin / etc. Curve mode is built upon field mode, so it includes everything that field does It also includes curve operations / MSM / ECNTT / G2 and other curve-related primitives.

:::info

If you only want to use a curve's scalar or base field, you still need to use curve mode. You can disable MSM with [options](#compilation-options)

:::

### Compiling for a field

You can compile ICICLE for a field using this command:

```sh
cd icicle
mkdir -p build
cmake -DFIELD=<FIELD> -S . -B build
cmake --build build -j
```

This command will output `libingo_field_<FIELD>.a` into `build/lib`.

### Compiling for a curve

:::note

Field related primitives will be compiled for the scalar field of the curve

:::

You can compile ICICLE for a SNARK curve using this command:

```sh
cd icicle
mkdir -p build
cmake -DCURVE=<CURVE> -S . -B build
cmake --build build -j
```

Where `<CURVE>` can be one of `bn254`/`bls12_377`/`bls12_381`/`bw6_761`/`grumpkin`.

This command will output both `libingo_curve_<CURVE>.a` and `libingo_field_<CURVE>.a` into `build/lib`.

### Compilation options

There exist multiple options that allow you to customize your build or enable additional functionality.

#### EXT_FIELD

Used only in [field mode][COMPILE_FIELD_MODE] to add an Extension field. Adds all supported field operations for the extension field.

Default: `OFF`

Usage: `-DEXT_FIELD=ON`

#### G2

Used only in [curve mode][COMPILE_CURVE_MODE] to add G2 definitions. Also adds G2 MSM.

Default: `OFF`

Usage: `-DG2=ON`

#### ECNTT

Used only in [curve mode][COMPILE_CURVE_MODE] to add ECNTT function.

Default: `OFF`

Usage: `-DECNTT=ON`

#### MSM

Used only in [curve mode][COMPILE_CURVE_MODE] to add MSM function. As MSM takes a lot of time to build, you can disable it with this option to reduce compilation time.

Default: `ON`

Usage: `-DMSM=OFF`

#### BUILD_HASH

Can be used in any mode to build a hash library. Currently it only includes Keccak hash function, but more are coming.

Default: `OFF`

Usage: `-DBUILD_HASH=ON`

#### BUILD_TESTS

Can be used in any mode to include tests runner binary.

Default: `OFF`

USAGE: `-DBUILD_TESTS=ON`

#### BUILD_BENCHMARKS

Can be used in any mode to include benchmarks runner binary.

Default: `OFF`

USAGE: `-DBUILD_BENCHMARKS=ON`

#### DEVMODE

Can be used in any mode to include debug symbols in the build.

Default: `OFF`

USAGE: `-DEVMODE=ON`

## Linking

To link ICICLE with your project you first need to compile ICICLE with options of your choice. After that you can use CMake `target_link_libraries` to link with the generated static libraries and `target_include_directories` to include ICICLE headers (located in `icicle/include`).

Refer to our [c++ examples](https://github.com/ingonyama-zk/icicle/tree/main/examples/c%2B%2B) for more info. Take a look at this [CMakeLists.txt](https://github.com/ingonyama-zk/icicle/blob/main/examples/c%2B%2B/msm/CMakeLists.txt#L22)

## Writing new bindings for ICICLE

Since ICICLE Core is written in CUDA / C++ its really simple to generate static libraries. These static libraries can be installed on any system and called by higher level languages such as Golang.

Static libraries can be loaded into memory once and used by multiple programs, reducing memory usage and potentially improving performance. They also allow you to separate functionality into distinct modules so your static library may need to compile only specific features that you want to use.

Let's review the [Golang bindings][GOLANG_BINDINGS] since its a pretty verbose example (compared to rust which hides it pretty well) of using static libraries. Golang has a library named `CGO` which can be used to link static libraries. Here's a basic example on how you can use cgo to link these libraries:

```go
/*
#cgo LDFLAGS: -L/path/to/shared/libs -lbn254 -lbls12_381 -lbls12_377 -lbw6_671
#include "icicle.h" // make sure you use the correct header file(s)
*/
import "C"

func main() {
  // Now you can call the C functions from the ICICLE libraries.
  // Note that C function calls are prefixed with 'C.' in Go code.

  out := (*C.BN254_projective_t)(unsafe.Pointer(p))
  in := (*C.BN254_affine_t)(unsafe.Pointer(affine))

  C.projective_from_affine_bn254(out, in)
}
```

The comments on the first line tell `CGO` which libraries to import as well as which header files to include. You can then call methods which are part of the static library and defined in the header file, `C.projective_from_affine_bn254` is an example.

If you wish to create your own bindings for a language of your choice we suggest you start by investigating how you can call static libraries.

<!-- Begin Links -->
[GOLANG_BINDINGS]: golang-bindings.md
[COMPILE_CURVE_MODE]: #compiling-for-a-curve
[COMPILE_FIELD_MODE]: #compiling-for-a-field
[NTT_DOCS]: primitives/ntt
[MSM_DOCS]: primitives/msm
[POLY_DOCS]: polynomials/overview
[VECOPS_CODE]: https://github.com/ingonyama-zk/icicle/blob/main/icicle/include/vec_ops/vec_ops.cuh
<!-- End Links -->
