// Copies of fn_7100493cd8 (src/game_s2/fn_7100493cd8.cpp) for other types, with their own callees and data (the names, strings and comments below are the original's): fn_7100644f24, fn_7100644f78, fn_7100646314, fn_7100646368.
// Copy constructor of a layered class.
#include "types.h"

extern void* g_unk_7101a24060[];
extern void* g_unk_7101a3c6c8[];

void fn_7100644f24(char* self, const char* other) {
    *reinterpret_cast<void***>(self) = &g_unk_7101a24060[2];
    __builtin_memcpy(self + 0x8, other + 0x8, 32);
    *reinterpret_cast<void***>(self) = &g_unk_7101a3c6c8[2];
    *reinterpret_cast<u64*>(self + 0x28) = *reinterpret_cast<const u64*>(other + 0x28);
    *reinterpret_cast<u32*>(self + 0x30) = *reinterpret_cast<const u32*>(other + 0x30);
    *reinterpret_cast<u32*>(self + 0x34) = *reinterpret_cast<const u32*>(other + 0x34);
    *reinterpret_cast<u32*>(self + 0x38) = *reinterpret_cast<const u32*>(other + 0x38);
}

void fn_7100644f78(char* self, const char* other) {
    *reinterpret_cast<void***>(self) = &g_unk_7101a24060[2];
    __builtin_memcpy(self + 0x8, other + 0x8, 32);
    *reinterpret_cast<void***>(self) = &g_unk_7101a3c6c8[2];
    *reinterpret_cast<u64*>(self + 0x28) = *reinterpret_cast<const u64*>(other + 0x28);
    *reinterpret_cast<u32*>(self + 0x30) = *reinterpret_cast<const u32*>(other + 0x30);
    *reinterpret_cast<u32*>(self + 0x34) = *reinterpret_cast<const u32*>(other + 0x34);
    *reinterpret_cast<u32*>(self + 0x38) = *reinterpret_cast<const u32*>(other + 0x38);
}

void fn_7100646314(char* self, const char* other) {
    *reinterpret_cast<void***>(self) = &g_unk_7101a24060[2];
    __builtin_memcpy(self + 0x8, other + 0x8, 32);
    *reinterpret_cast<void***>(self) = &g_unk_7101a3c6c8[2];
    *reinterpret_cast<u64*>(self + 0x28) = *reinterpret_cast<const u64*>(other + 0x28);
    *reinterpret_cast<u32*>(self + 0x30) = *reinterpret_cast<const u32*>(other + 0x30);
    *reinterpret_cast<u32*>(self + 0x34) = *reinterpret_cast<const u32*>(other + 0x34);
    *reinterpret_cast<u32*>(self + 0x38) = *reinterpret_cast<const u32*>(other + 0x38);
}

void fn_7100646368(char* self, const char* other) {
    *reinterpret_cast<void***>(self) = &g_unk_7101a24060[2];
    __builtin_memcpy(self + 0x8, other + 0x8, 32);
    *reinterpret_cast<void***>(self) = &g_unk_7101a3c6c8[2];
    *reinterpret_cast<u64*>(self + 0x28) = *reinterpret_cast<const u64*>(other + 0x28);
    *reinterpret_cast<u32*>(self + 0x30) = *reinterpret_cast<const u32*>(other + 0x30);
    *reinterpret_cast<u32*>(self + 0x34) = *reinterpret_cast<const u32*>(other + 0x34);
    *reinterpret_cast<u32*>(self + 0x38) = *reinterpret_cast<const u32*>(other + 0x38);
}

