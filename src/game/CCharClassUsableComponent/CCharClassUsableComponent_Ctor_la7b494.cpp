// CCharClassUsableComponent's unit (tools/tu_map.py): the constructor. On the char-class component base
// (fn_71011acd10): the vtable, three empty strings, nineteen string ids built from the empty string, an
// empty string id, two empty file paths and the defaults (5, then 1, 1, 1, 0.01 and 1). Names and layouts
// are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CCharClassComponent.h"
#include "game/CCharClassUsableComponent.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;

struct UnkUsable_la7b494 {
    void** vtable;
    u8 pad_0x8[0x58 - 0x8];
    CRntString field_0x58;
    CRntString field_0x88;
    CRntString field_0xb8;
    CStrId field_0xe8[19];
    CStrId field_0x180;
    CFilePathStrId field_0x188;
    CFilePathStrId field_0x190;
    f32 field_0x198;
    u32 field_0x19c;
    f32 field_0x1a0;
    f32 field_0x1a4;
    f32 field_0x1a8;
    f32 field_0x1ac;
    f32 field_0x1b0;
};
static_assert(__builtin_offsetof(UnkUsable_la7b494, field_0x1b0) == 0x1b0, "layout");

static inline void InitEmpty(base::global::UnkStrIdBase* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

void fn_7100a7b494(UnkShipRechargeDef* def) {
    UnkUsable_la7b494* self = reinterpret_cast<UnkUsable_la7b494*>(def);
    fn_71011acd10(reinterpret_cast<UnkStruct_71008bca30*>(self));
    self->vtable = g_unk_7101a90b80 + 2;
    fn_710007e6c4(&self->field_0x58);
    fn_710007e6c4(&self->field_0x88);
    fn_710007e6c4(&self->field_0xb8);
    for (CStrId& id : self->field_0xe8)
        fn_7100000850(&id, g_unk_7101d58d10, true);
    InitEmpty(&self->field_0x180);
    new (&self->field_0x188) CFilePathStrId();
    new (&self->field_0x190) CFilePathStrId();
    self->field_0x198 = 5.0f;
    self->field_0x19c = 0;
    self->field_0x1a0 = 1.0f;
    self->field_0x1a4 = 1.0f;
    self->field_0x1a8 = 1.0f;
    self->field_0x1ac = 0.01f;
    self->field_0x1b0 = 1.0f;
}
