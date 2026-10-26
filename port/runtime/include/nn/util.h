#pragma once

// nn::util: small helpers. The system start-up fn_71001325d0 stores a GenerateUuid result in a
// global (a session id); the play report fn_7100ec32c0 formats with TSNPrintf; the network and
// debug code (fn_7101204810, fn_7101224074, fn_71012259a8) with SNPrintf; fn_7100140990 and
// fn_71001409a0 unpack gzip data.
//
// `main` imports (data/functions/): GenerateUuid, TSNPrintf, SNPrintf, GetGzipDecompressedSize,
// DecompressGzip, ReferSymbol. Declared below: all but DecompressGzip (it needs an inflater; its
// one caller is outside the boot slice) and ReferSymbol (a debug-symbol lookup with one caller,
// fn_710120a6bc, outside the slice).

#include "nn/nn_common.h"

namespace nn::util {

// 16 bytes, returned in registers (fn_71001325d0 stores x0 and x1).
struct Uuid {
    u8 data[16];
};
static_assert(sizeof(Uuid) == 16, "two registers");

Uuid GenerateUuid();
s32 TSNPrintf(char* buffer, u64 size, const char* format, ...);
s32 SNPrintf(char* buffer, u64 size, const char* format, ...);
// The size a gzip stream unpacks to: its trailer's last word.
u64 GetGzipDecompressedSize(const void* data, u64 size);

}  // namespace nn::util
