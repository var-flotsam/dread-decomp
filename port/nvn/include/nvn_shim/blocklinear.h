#pragma once

// The console GPU's block-linear ("GOB") memory layout, which packaged (.bctex) texture data is
// stored in, and its conversion to and from linear rows for the texel upload step. Written from the
// public description of the Tegra X1 layout (NVIDIA's Tegra X1 TRM, "Block Linear"), not from any
// emulator's code.
//
// The layout works on bytes, so a compressed format is described in blocks: one row is a row of
// 4x4 blocks (width in blocks times bytes per block) and the row count is the height in blocks.
//  - A GOB is 64 bytes by 8 rows (512 bytes). Inside it, byte (x, y) is at
//    x[3:0] | y[0] << 4 | x[4] << 5 | y[2:1] << 6 | x[5] << 8.
//  - A block is one GOB wide, 2^block_height_log2 GOBs tall and 2^block_depth_log2 GOBs deep, GOBs
//    in a block ordered by row, then by slice.
//  - Blocks are ordered across a row of the surface, then down, then through the slices; the surface
//    is padded to whole blocks on every side.
// A smaller mip level keeps the layout but its block height (and depth) shrinks while half of it
// would still cover the level: MipBlockLog2 (the usual rule, not yet confirmed on the game's own
// packaged layouts). Where each level and array layer starts in the
// packaged data, and how a layer's size is aligned, are the packaged layout's business, not this
// file's: the upload step takes them from the texture's packaged layout once that is read.

#include <cstddef>
#include <cstdint>

namespace dread::nvn {

struct BlockLinearSurface {
    std::uint32_t row_bytes = 0;          // bytes in one row (of texels, or of 4x4 blocks)
    std::uint32_t rows = 0;               // rows (texel rows, or rows of blocks)
    std::uint32_t depth = 1;              // slices
    std::uint32_t block_height_log2 = 0;  // GOBs per block vertically, log2: 0 to 5
    std::uint32_t block_depth_log2 = 0;   // GOBs per block in depth, log2: 0 to 5
};

// Where byte (x, y) of a GOB is, 0 to 511.
std::uint32_t GobOffset(std::uint32_t x, std::uint32_t y);
// Bytes the tiled surface occupies, padded to whole blocks.
std::uint64_t BlockLinearSize(const BlockLinearSurface& s);
// Where byte `x` of row `y` of slice `z` is in the tiled surface.
std::uint64_t BlockLinearOffset(const BlockLinearSurface& s, std::uint32_t x, std::uint32_t y,
                                std::uint32_t z);
// The block height (or depth) log2 a mip level uses, from the base level's and the level's own
// extent in that direction (rows of texels or blocks for the height, slices for the depth; `gob_extent`
// is 8 for the height, 1 for the depth).
std::uint32_t MipBlockLog2(std::uint32_t base_log2, std::uint32_t extent, std::uint32_t gob_extent);

// Tiled -> linear: every slice's rows into `dst`, `dst_pitch` bytes apart (at least row_bytes),
// slices `dst_pitch * rows` apart. False, writing nothing, when `src_size` is smaller than
// BlockLinearSize or the surface is empty or malformed.
bool Deswizzle(const BlockLinearSurface& s, const std::uint8_t* src, std::size_t src_size,
               std::uint8_t* dst, std::size_t dst_pitch);
// Linear -> tiled, the inverse (the padding is left as it was). Same conditions on `dst_size`.
bool Swizzle(const BlockLinearSurface& s, const std::uint8_t* src, std::size_t src_pitch,
             std::uint8_t* dst, std::size_t dst_size);

}  // namespace dread::nvn
