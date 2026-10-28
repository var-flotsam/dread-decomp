// The block-linear layout (include/nvn_shim/blocklinear.h has the description it follows).

#include "nvn_shim/blocklinear.h"

#include <algorithm>
#include <cstring>

namespace dread::nvn {

namespace {

constexpr std::uint32_t kGobBytes = 64;  // width of a GOB in bytes
constexpr std::uint32_t kGobRows = 8;
constexpr std::uint32_t kGobSize = kGobBytes * kGobRows;

std::uint32_t DivUp(std::uint32_t a, std::uint32_t b) { return (a + b - 1) / b; }

bool Valid(const BlockLinearSurface& s) {
    return s.row_bytes > 0 && s.rows > 0 && s.depth > 0 && s.block_height_log2 <= 5 &&
           s.block_depth_log2 <= 5;
}

// Copies between linear rows and the tiled surface, 16 bytes at a time: inside a GOB the 16 bytes
// with the same x[5:4] and y are contiguous.
template <bool kToLinear>
void Copy(const BlockLinearSurface& s, std::uint8_t* tiled, std::uint8_t* linear, std::size_t pitch) {
    for (std::uint32_t z = 0; z < s.depth; ++z) {
        for (std::uint32_t y = 0; y < s.rows; ++y) {
            std::uint8_t* row = linear + (static_cast<std::size_t>(z) * s.rows + y) * pitch;
            for (std::uint32_t x = 0; x < s.row_bytes; x += 16) {
                const std::uint32_t n = std::min<std::uint32_t>(16, s.row_bytes - x);
                std::uint8_t* t = tiled + BlockLinearOffset(s, x, y, z);
                if (kToLinear)
                    std::memcpy(row + x, t, n);
                else
                    std::memcpy(t, row + x, n);
            }
        }
    }
}

}  // namespace

std::uint32_t GobOffset(std::uint32_t x, std::uint32_t y) {
    return (x & 0xf) | ((y & 1) << 4) | (((x >> 4) & 1) << 5) | (((y >> 1) & 3) << 6) | (((x >> 5) & 1) << 8);
}

std::uint64_t BlockLinearSize(const BlockLinearSurface& s) {
    if (!Valid(s))
        return 0;
    const std::uint32_t bh = 1u << s.block_height_log2, bd = 1u << s.block_depth_log2;
    const std::uint64_t width_gobs = DivUp(s.row_bytes, kGobBytes);
    const std::uint64_t height_blocks = DivUp(s.rows, kGobRows * bh);
    const std::uint64_t depth_blocks = DivUp(s.depth, bd);
    return width_gobs * height_blocks * depth_blocks * kGobSize * bh * bd;
}

std::uint64_t BlockLinearOffset(const BlockLinearSurface& s, std::uint32_t x, std::uint32_t y,
                                std::uint32_t z) {
    const std::uint32_t bh = 1u << s.block_height_log2, bd = 1u << s.block_depth_log2;
    const std::uint64_t width_gobs = DivUp(s.row_bytes, kGobBytes);
    const std::uint64_t height_blocks = DivUp(s.rows, kGobRows * bh);
    const std::uint64_t block_bytes = static_cast<std::uint64_t>(kGobSize) * bh * bd;
    const std::uint64_t block = (z / bd) * height_blocks * width_gobs + (y / (kGobRows * bh)) * width_gobs +
                                x / kGobBytes;
    const std::uint64_t gob_in_block = (z % bd) * bh + (y / kGobRows) % bh;
    return block * block_bytes + gob_in_block * kGobSize + GobOffset(x % kGobBytes, y % kGobRows);
}

std::uint32_t MipBlockLog2(std::uint32_t base_log2, std::uint32_t extent, std::uint32_t gob_extent) {
    const std::uint32_t gobs = DivUp(std::max<std::uint32_t>(extent, 1), gob_extent);
    std::uint32_t log2 = base_log2;
    while (log2 > 0 && gobs <= (1u << (log2 - 1)))
        --log2;
    return log2;
}

bool Deswizzle(const BlockLinearSurface& s, const std::uint8_t* src, std::size_t src_size,
               std::uint8_t* dst, std::size_t dst_pitch) {
    if (!Valid(s) || dst_pitch < s.row_bytes || src_size < BlockLinearSize(s))
        return false;
    Copy<true>(s, const_cast<std::uint8_t*>(src), dst, dst_pitch);
    return true;
}

bool Swizzle(const BlockLinearSurface& s, const std::uint8_t* src, std::size_t src_pitch,
             std::uint8_t* dst, std::size_t dst_size) {
    if (!Valid(s) || src_pitch < s.row_bytes || dst_size < BlockLinearSize(s))
        return false;
    Copy<false>(s, dst, const_cast<std::uint8_t*>(src), src_pitch);
    return true;
}

}  // namespace dread::nvn
