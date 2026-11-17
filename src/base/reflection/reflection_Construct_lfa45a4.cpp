// base::reflection's unit (tools/tu_map.py, unit 2610): the constructor of a 0xd0-byte object (the size the
// unit's CSubareaInfo is reflected with) on the base fn_7101043904: copies of the default string and path
// ids, nine empty ids, two empty lists (flags 0xf), -1 and two cleared flags. Names and layouts are placeholders.
#include "base/UnkResPathVec.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CGameObject.h"
#include "types.h"

#include <new>

using base::global::CFilePathStrId;
using base::global::CRntVector;
using base::global::CStrId;

extern char g_unk_7101cab428[];  // the object's vtable

struct UnkEntry_lfa45a4;

struct UnkObject_lfa45a4 {
    void** vtable;
    u8 unk_0x8[0x8];
    CStrId field_0x10;
    CStrId field_0x18;
    CFilePathStrId field_0x20;
    CStrId field_0x28;
    CStrId field_0x30;  // nine ids, assigned together (fn_7100fa4b64)
    CStrId field_0x38;
    CStrId field_0x40;
    CStrId field_0x48;
    CStrId field_0x50;
    CStrId field_0x58;
    CStrId field_0x60;
    CStrId field_0x68;
    CStrId field_0x70;
    CRntVector<UnkEntry_lfa45a4> field_0x78;
    UnkResPathVec field_0xa0;
    f32 field_0xc8;
    bool field_0xcc;
    bool field_0xcd;
};
static_assert(__builtin_offsetof(UnkObject_lfa45a4, field_0x78) == 0x78, "");
static_assert(__builtin_offsetof(UnkObject_lfa45a4, field_0xc8) == 0xc8, "");

// An empty id, creating the pool first when there is none.
static inline void MakeEmpty(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

void fn_7100fa45a4(UnkObject_lfa45a4* self) {
    fn_7101043904(self);
    self->vtable = reinterpret_cast<void**>(g_unk_7101cab428 + 0x10);
    new (&self->field_0x10) CStrId(g_unk_7101d470e0);
    new (&self->field_0x18) CStrId(g_unk_7101d470e0);
    new (&self->field_0x20) CFilePathStrId(g_unk_7101d470e8);
    new (&self->field_0x28) CStrId(g_unk_7101d470e0);
    MakeEmpty(&self->field_0x30);
    MakeEmpty(&self->field_0x38);
    MakeEmpty(&self->field_0x40);
    MakeEmpty(&self->field_0x48);
    MakeEmpty(&self->field_0x50);
    MakeEmpty(&self->field_0x58);
    MakeEmpty(&self->field_0x60);
    MakeEmpty(&self->field_0x68);
    MakeEmpty(&self->field_0x70);
    new (&self->field_0x78) CRntVector<UnkEntry_lfa45a4>(0xf);
    new (&self->field_0xa0) UnkResPathVec(0xf);
    self->field_0xc8 = -1.0f;
    self->field_0xcc = false;
    self->field_0xcd = false;
}
