// animtree::CBoneControl unit (tools/tu_map.py): copies the bone handle (definition -> +0x10 ->
// +0x30) to the control and its target, then the definition's settings at +0x18, +0x1d and +0x1c
// to the target. The original reloads both pointers after each store. Names and layouts are
// placeholders.
#include "types.h"

// Stores as raw bytes: the original reloads both pointers after each one.
template <class T>
static inline void StoreRaw(T* dst, T value) {
    __builtin_memcpy(dst, &value, sizeof(T));
}

struct UnkBoneSource {
    u8 pad_0x0[0x30];
    u64 bone;  // 0x30
};

struct UnkBoneControlDefView {
    u8 pad_0x0[0x10];
    UnkBoneSource* source;  // 0x10
    u32 field_0x18;
    u8 field_0x1c;
    u8 field_0x1d;
};

struct UnkBoneTarget {
    u8 pad_0x0[0x58];
    u64 bone;  // 0x58
    u32 field_0x60;
    u8 field_0x64;
    u8 field_0x65;
};

struct UnkBoneControlView {
    u8 pad_0x0[0x10];
    UnkBoneControlDefView* def;  // 0x10
    u8 pad_0x18[0x10];
    UnkBoneTarget* target;       // 0x28
    u8 pad_0x30[0x20];
    u64 bone;            // 0x50
};

void fn_71002e2840(UnkBoneControlView* self) {
    u64 bone = self->def->source->bone;
    StoreRaw(&self->bone, bone);
    StoreRaw(&self->target->bone, bone);
    StoreRaw(&self->target->field_0x60, self->def->field_0x18);
    StoreRaw(&self->target->field_0x64, self->def->field_0x1d);
    StoreRaw(&self->target->field_0x65, self->def->field_0x1c);
}
