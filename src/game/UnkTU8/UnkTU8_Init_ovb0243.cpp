// Unit 8 of tools/tu_map.py (no name hint): a static initialiser. It builds the first global, clears a
// block and five pairs of words, and registers the pairs' destructors with __cxa_atexit. Names and layouts are
// placeholders.
#include "base/UnkAtExit.h"
#include "types.h"

#define HIDDEN __attribute__((visibility("hidden")))

struct UnkTU8Pair {
    u64 field_0x0;
    u64 field_0x8;
};

struct UnkTU8Tail {
    UnkTU8Pair first;
    UnkTU8Pair second;
};

struct UnkTU8Block {
    UnkTU8Pair field_0x0;
    UnkTU8Pair field_0x10;
    UnkTU8Pair field_0x20;
    u8 field_0x30;
    u8 pad_0x31[7];
    UnkTU8Tail tail;  // 0x38
};

struct UnkTU8First;
void fn_7100001be8(UnkTU8First* self);
void fn_7100001ec0(void* self);
void fn_7100001ed0(void* self);

extern UnkTU8First g_unk_7101d47560;
extern UnkTU8Block g_unk_7101d476c0 HIDDEN;
extern UnkTU8Pair g_unk_7101d475f0;
extern UnkTU8Pair g_unk_7101d47600;
extern UnkTU8Pair g_unk_7101d47610;
extern UnkTU8Pair g_unk_7101d47620;
extern UnkTU8Pair g_unk_7101d47630;

void fn_7100001ee0() {
    fn_7100001be8(&g_unk_7101d47560);
    g_unk_7101d476c0.tail = UnkTU8Tail{};
    __builtin_memset(&g_unk_7101d476c0, 0, 0x31);
    g_unk_7101d475f0 = UnkTU8Pair{};
    fn_7100000250(fn_7100001ec0, &g_unk_7101d475f0, &g_unk_7101a19000);
    g_unk_7101d47600 = UnkTU8Pair{};
    fn_7100000250(fn_7100001ec0, &g_unk_7101d47600, &g_unk_7101a19000);
    g_unk_7101d47610 = UnkTU8Pair{};
    fn_7100000250(fn_7100001ed0, &g_unk_7101d47610, &g_unk_7101a19000);
    g_unk_7101d47620 = UnkTU8Pair{};
    fn_7100000250(fn_7100001ec0, &g_unk_7101d47620, &g_unk_7101a19000);
    g_unk_7101d47630 = UnkTU8Pair{};
    fn_7100000250(fn_7100001ed0, &g_unk_7101d47630, &g_unk_7101a19000);
}
