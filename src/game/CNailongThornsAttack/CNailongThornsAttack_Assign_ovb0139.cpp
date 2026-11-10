// CNailongThornsAttack's unit (tools/tu_map.py): a copy assignment of a record holding raw words, a weak reference
// (+0x48, re-taken through the target's own weak handle, game_s2/UnkWeakHandle.h), more words, a string id (+0x78) and a flag.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkNailongRecord_ovb0139 {
    void* vtable;
    u8 words_0x8[0x10];        // 0x8
    u8 words_0x18[0x10];       // 0x18
    u8 words_0x28[8];          // 0x28
    u8 pad_0x30[8];
    u8 words_0x38[0x10];       // 0x38
    UnkWeakHandle* target;     // 0x48
    u64 field_0x50;
    u8 words_0x58[0x20];       // 0x58
    CStrId id;                 // 0x78
    u8 words_0x80[0x10];       // 0x80
    u8 flag;                   // 0x90
};
static_assert(__builtin_offsetof(UnkNailongRecord_ovb0139, flag) == 0x90, "layout");

struct UnkWordPair_ovb0139 {
    u64 a, b;
};

UnkNailongRecord_ovb0139* fn_710060db40(UnkNailongRecord_ovb0139* self, const UnkNailongRecord_ovb0139* other) {
const u64* src = reinterpret_cast<const u64*>(other->words_0x8);    u64 a = src[0], b = src[1];    const u64* src2 = reinterpret_cast<const u64*>(other->words_0x18);    u64 c = src2[0], d = src2[1];    u64* dst2 = reinterpret_cast<u64*>(self->words_0x18);    dst2[0] = c;    dst2[1] = d;    u64* dst = reinterpret_cast<u64*>(self->words_0x8);    dst[0] = a;    dst[1] = b;
    __builtin_memcpy(self->words_0x28, other->words_0x28, 8);
    __builtin_memcpy(self->words_0x38, other->words_0x38, 0x10);
    if (self != other)
        CopyWeakRef(&self->target, other->target);
    self->field_0x50 = other->field_0x50;
    __builtin_memcpy(self->words_0x58, other->words_0x58, 0x20);
    self->id = other->id;
    u64 w80 = reinterpret_cast<const u64*>(other->words_0x80)[0];
    u64 w88 = reinterpret_cast<const u64*>(other->words_0x80)[1];
    u8 flag = other->flag;
    self->flag = flag;
    reinterpret_cast<u64*>(self->words_0x80)[0] = w80;
    reinterpret_cast<u64*>(self->words_0x80)[1] = w88;
    return self;
}
