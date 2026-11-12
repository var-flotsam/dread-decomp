#pragma once

// CThreadSafeDevice: functions of this translation unit that other files call (tools/tu_map.py unit 68,
// 0x710008c7d4-0x710008ef54). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkStrIdPathPair;
struct UnkEntry_0x30;
struct UnkPathPairTemp_b45a1;
struct UnkStrPair_0x10;
void fn_710008d4c0(UnkStrPair_0x10* s, const UnkEntry_0x30* e);
void fn_710008d680(UnkStrPair_0x10* s, const char* path);
void fn_710008d2a0(UnkPathPairTemp_b45a1* self);  // The empty pair's constructor.
UnkStrIdPathPair* fn_710008d88c(UnkStrIdPathPair* dst, const void* id, const void* path);
// Builds the id and path pair from a file-path id.
void fn_710008d2ec(UnkStrPair_0x10* self, const base::global::CFilePathStrId* path);
// Sets `id` and `file` from the file-path id `path`.
void fn_710008d99c(const base::global::CFilePathStrId* path, base::global::CStrId* id, base::global::CFilePathStrId* file);
