// CCharClassIgnoreCollisionMaskTrack's unit (tools/tu_map.py): two setters of a string id (+0x18 and
// +0x20) that bump the new name's use count and cache the kind fn_7100d4bc00 finds for its text
// (+0x28 and +0x2c). Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

u32 fn_7100d4bc00(const CRntString*);

// A string id's data as the name read sees it: the text at +0x10 and a use count at +0x44 (a union
// member: the original reloads the id after the increment).
struct UnkStrIdText_ovb0092 {
    u8 pad_0x0[0x10];
    const char* text;
    u8 pad_0x18[0x2c];
    union {
        s32 field_0x44;
    };
};

struct UnkMaskTrack_ovb0092 {
    u8 pad_0x0[0x18];
    CStrId name_a;  // 0x18
    CStrId name_b;  // 0x20
    u32 kind_a;     // 0x28
    u32 kind_b;     // 0x2c
};

static inline void CacheKind(const CStrId& id, u32* kind) {
    const char* text = reinterpret_cast<UnkStrIdText_ovb0092*>(id.data)->text;
    CRntString str(text ? text : g_unk_710162e2ed);
    *kind = fn_7100d4bc00(&str);
}

void fn_7100c29fa0(UnkMaskTrack_ovb0092* self, const CStrId* name) {
    self->name_a = *name;
    ++reinterpret_cast<UnkStrIdText_ovb0092*>(self->name_a.data)->field_0x44;
    CacheKind(self->name_a, &self->kind_a);
}

void fn_7100c2a114(UnkMaskTrack_ovb0092* self, const CStrId* name) {
    self->name_b = *name;
    ++reinterpret_cast<UnkStrIdText_ovb0092*>(self->name_b.data)->field_0x44;
    CacheKind(self->name_b, &self->kind_b);
}
