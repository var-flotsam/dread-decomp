// nvn_blocklinear_check
//
// Checks the block-linear layout (src/blocklinear.cpp) without a GPU or game data:
//  - GobOffset is a permutation of 0..511 and puts the TRM's sample bytes where it says;
//  - BlockLinearOffset against a byte-by-byte model written separately from the description in
//    include/nvn_shim/blocklinear.h (block index, then GOB in block, then GobOffset), for every
//    byte of surfaces of several shapes: widths that are not whole GOBs, every block height,
//    3D blocks;
//  - Swizzle then Deswizzle gives the linear rows back, Deswizzle refuses a short source;
//  - sizes of known surfaces, and the mip block heights.
// Exit status 0 when every check passes.

#include <cstdint>
#include <cstdio>
#include <vector>

#include "nvn_shim/blocklinear.h"

using namespace dread::nvn;

namespace {

int g_failures = 0;

void Expect(bool ok, const char* what) {
    if (!ok) {
        std::printf("FAIL: %s\n", what);
        ++g_failures;
    }
}

// The model: walk blocks in surface order and the bytes of each in layout order, recording where
// each (x, y, z) lands. Independent of BlockLinearOffset's arithmetic.
std::vector<std::int64_t> Model(const BlockLinearSurface& s) {
    const std::uint32_t bh = 1u << s.block_height_log2, bd = 1u << s.block_depth_log2;
    const std::uint32_t wg = (s.row_bytes + 63) / 64, hb = (s.rows + 8 * bh - 1) / (8 * bh);
    const std::uint32_t db = (s.depth + bd - 1) / bd;
    std::vector<std::int64_t> at(static_cast<std::size_t>(s.row_bytes) * s.rows * s.depth, -1);
    std::int64_t offset = 0;
    for (std::uint32_t bz = 0; bz < db; ++bz)
        for (std::uint32_t by = 0; by < hb; ++by)
            for (std::uint32_t bx = 0; bx < wg; ++bx)
                for (std::uint32_t gz = 0; gz < bd; ++gz)
                    for (std::uint32_t gy = 0; gy < bh; ++gy)
                        for (std::uint32_t i = 0; i < 512; ++i, ++offset) {
                            // byte i of a GOB, from its bits: x[3:0] y[0] x[4] y[2:1] x[5]
                            const std::uint32_t lx = (i & 0xf) | (((i >> 5) & 1) << 4) | (((i >> 8) & 1) << 5);
                            const std::uint32_t ly = ((i >> 4) & 1) | (((i >> 6) & 3) << 1);
                            const std::uint32_t x = bx * 64 + lx, y = (by * bh + gy) * 8 + ly, z = bz * bd + gz;
                            if (x < s.row_bytes && y < s.rows && z < s.depth)
                                at[(static_cast<std::size_t>(z) * s.rows + y) * s.row_bytes + x] = offset;
                        }
    return at;
}

void CheckSurface(const BlockLinearSurface& s) {
    char what[160];
    std::snprintf(what, sizeof what, "surface %u bytes x %u rows x %u, block height 2^%u, depth 2^%u", s.row_bytes,
                  s.rows, s.depth, s.block_height_log2, s.block_depth_log2);
    const std::vector<std::int64_t> model = Model(s);
    bool same = true;
    for (std::uint32_t z = 0; z < s.depth && same; ++z)
        for (std::uint32_t y = 0; y < s.rows && same; ++y)
            for (std::uint32_t x = 0; x < s.row_bytes && same; ++x)
                same = static_cast<std::int64_t>(BlockLinearOffset(s, x, y, z)) ==
                       model[(static_cast<std::size_t>(z) * s.rows + y) * s.row_bytes + x];
    Expect(same, what);

    const std::uint64_t size = BlockLinearSize(s);
    std::vector<std::uint8_t> linear(static_cast<std::size_t>(s.row_bytes) * s.rows * s.depth), back(linear.size());
    std::uint32_t seed = 12345;
    for (std::uint8_t& b : linear) {
        seed = seed * 1103515245u + 12345u;
        b = static_cast<std::uint8_t>(seed >> 16);
    }
    std::vector<std::uint8_t> tiled(size, 0);
    Expect(Swizzle(s, linear.data(), s.row_bytes, tiled.data(), tiled.size()), what);
    Expect(Deswizzle(s, tiled.data(), tiled.size(), back.data(), s.row_bytes) && back == linear, what);
    Expect(!Deswizzle(s, tiled.data(), tiled.size() - 1, back.data(), s.row_bytes), "a short source is refused");
}

}  // namespace

int main() {
    std::vector<int> seen(512, 0);
    for (std::uint32_t y = 0; y < 8; ++y)
        for (std::uint32_t x = 0; x < 64; ++x)
            ++seen[GobOffset(x, y)];
    bool permutation = true;
    for (int n : seen)
        permutation = permutation && n == 1;
    Expect(permutation, "GobOffset is a permutation of 0..511");
    Expect(GobOffset(15, 0) == 15 && GobOffset(0, 1) == 16 && GobOffset(16, 0) == 32 && GobOffset(0, 2) == 64 &&
               GobOffset(32, 0) == 256 && GobOffset(63, 7) == 511,
           "GobOffset's sample bytes");

    const BlockLinearSurface shapes[] = {
        {64, 8, 1, 0, 0},     {256, 64, 1, 3, 0},  {100, 37, 1, 2, 0}, {1024, 256, 1, 4, 0},
        {48, 300, 1, 5, 0},   {16, 4, 1, 1, 0},    {128, 16, 5, 1, 2}, {200, 20, 3, 0, 1},
    };
    for (const BlockLinearSurface& s : shapes)
        CheckSurface(s);

    Expect(BlockLinearSize({1024, 256, 1, 4, 0}) == 256u * 256u * 4u, "256x256 RGBA8, block height 16: no padding");
    Expect(BlockLinearSize({16 * 16, 64, 1, 4, 0}) == 4u * 1u * 512u * 16u, "128x256 BC1 (256-byte rows, 64 rows of blocks) pads to one block row of 128");
    Expect(BlockLinearSize({}) == 0, "an empty surface has no size");

    Expect(MipBlockLog2(4, 512, 8) == 4, "mip: 64 GOB rows keep a block height of 16");
    Expect(MipBlockLog2(4, 64, 8) == 3, "mip: 8 GOB rows take a block height of 8");
    Expect(MipBlockLog2(4, 9, 8) == 1, "mip: 2 GOB rows take a block height of 2");
    Expect(MipBlockLog2(4, 8, 8) == 0, "mip: 1 GOB row takes a block height of 1");
    Expect(MipBlockLog2(2, 3, 1) == 2, "mip depth: 3 slices keep a block depth of 4");
    Expect(MipBlockLog2(2, 2, 1) == 1, "mip depth: 2 slices take a block depth of 2");

    if (g_failures == 0)
        std::printf("nvn_blocklinear_check: all checks passed\n");
    return g_failures == 0 ? 0 : 1;
}
