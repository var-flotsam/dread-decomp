// animtree::CSamusLegsBoneControlDef unit (tools/tu_map.py): copy constructors of the bone control
// definition (fn_71002e9378 and fn_71002e93e8 are the same code): the base (vtable
// g_unk_7101a21718, string id at +0x8), then this class's vtable (g_unk_7101a221d0) and the plain
// fields at +0x10..+0x33 (see fn_71002e9464.cpp for the assignment). Names and layouts are
// placeholders.
#include <new>

#include "base/global/CStrId.h"

using base::global::CStrId;

extern void* g_unk_7101a21718[];
extern char g_unk_7101a221d0[];

struct UnkBoneControlDefFields {
    u64 field_0x10;
    u64 field_0x18;
    u64 field_0x20;
    u64 field_0x28;
    u32 field_0x30;
};

struct UnkSamusLegsBoneControlDef_b48 {
    char* vtable;
    CStrId name;                     // 0x8
    UnkBoneControlDefFields fields;  // 0x10
};

static inline void CopyConstruct(UnkSamusLegsBoneControlDef_b48* self, const UnkSamusLegsBoneControlDef_b48* other) {
    self->vtable = reinterpret_cast<char*>(g_unk_7101a21718) + 0x10;
    new (&self->name) CStrId(other->name);
    self->vtable = g_unk_7101a221d0 + 0x10;
    __builtin_memcpy(&self->fields, &other->fields, 0x24);
}

void fn_71002e9378(UnkSamusLegsBoneControlDef_b48* self, const UnkSamusLegsBoneControlDef_b48* other) {
    CopyConstruct(self, other);
}

void fn_71002e93e8(UnkSamusLegsBoneControlDef_b48* self, const UnkSamusLegsBoneControlDef_b48* other) {
    CopyConstruct(self, other);
}
