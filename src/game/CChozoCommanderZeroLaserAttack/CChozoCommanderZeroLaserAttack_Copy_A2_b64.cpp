// CChozoCommanderZeroLaserAttack's unit: two identical copy constructors of a layered class (the three
// bases of src/game/CChozoCommanderAirAttack/fn_7100424c08.cpp, then the last layer: two words, a word,
// an array of 8-byte elements copied with its settings and reserved to the other's capacity, and four
// words). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

extern void* g_unk_7101a24060[];
extern void* g_unk_7101a35018[];
extern void* g_unk_7101a35248[];
extern void* g_unk_7101a364e8[];

static inline __attribute__((always_inline)) void CopyZeroLaser(char* self, const char* other) {
    *reinterpret_cast<void***>(self) = &g_unk_7101a24060[2];
    __builtin_memcpy(self + 0x8, other + 0x8, 32);
    *reinterpret_cast<void***>(self) = &g_unk_7101a35248[2];
    __builtin_memcpy(self + 0x28, other + 0x28, 32);
    *reinterpret_cast<void***>(self) = &g_unk_7101a35018[2];
    __builtin_memcpy(self + 0x48, other + 0x48, 8);
    *reinterpret_cast<void***>(self) = &g_unk_7101a364e8[2];
    __builtin_memcpy(self + 0x50, other + 0x50, 20);

    auto* v = reinterpret_cast<base::UnkVector<u64>*>(self + 0x68);
    auto* o = reinterpret_cast<const base::UnkVector<u64>*>(other + 0x68);
    v->data = nullptr;
    v->count = 0;
    v->capacity = 0;
    v->step = o->step;
    v->field_0x14 = o->field_0x14;
    v->flags = o->flags;
    v->allocator = o->allocator;
    v->Reserve(o->capacity);
    __builtin_memcpy(v->data, o->data, (u64)o->count * sizeof(u64));
    v->count = o->count;

    *reinterpret_cast<u32*>(self + 0x90) = *reinterpret_cast<const u32*>(other + 0x90);
    *reinterpret_cast<u32*>(self + 0x94) = *reinterpret_cast<const u32*>(other + 0x94);
    *reinterpret_cast<u32*>(self + 0x98) = *reinterpret_cast<const u32*>(other + 0x98);
    *reinterpret_cast<u32*>(self + 0x9c) = *reinterpret_cast<const u32*>(other + 0x9c);
}

void fn_71004458bc(char* self, const char* other) { CopyZeroLaser(self, other); }
void fn_7100445a28(char* self, const char* other) { CopyZeroLaser(self, other); }
