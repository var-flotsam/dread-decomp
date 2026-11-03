// Unit 113 of tools/tu_map.py (no class hint): unpacks a gzip stream into `out` (its size read
// from `out_size`) with a work buffer on the stack (nn::util::DecompressGzip). Names are
// placeholders.
#include "types.h"

namespace nn::util {
bool DecompressGzip(void* out, u64 out_size, const void* data, u64 size, void* work, u64 work_size);
}  // namespace nn::util

bool fn_71001409a0(u64 size, const void* data, const u64* out_size, void* out) {
    u8 work[0x1bf8];
    return nn::util::DecompressGzip(out, *out_size, data, size, work, sizeof(work));
}
