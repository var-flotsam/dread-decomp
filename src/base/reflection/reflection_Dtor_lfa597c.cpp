// base::reflection's unit (tools/tu_map.py, unit 2610): the destructor of the 0xd0-byte object
// fn_7100fa45a4 constructs: the two lists, the nine ids, the four leading ids, then the base destructor
// fn_7101043934. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CGameObject.h"
#include "game/UnkTU2577.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntVector;
using base::global::CStrId;

extern char g_unk_7101cab428[];  // the object's vtable

struct UnkObject_lfa597c {
    void** vtable;
    u8 unk_0x8[0x8];
    CStrId field_0x10;
    CStrId field_0x18;
    CFilePathStrId field_0x20;
    CStrId field_0x28;
    CStrId field_0x30[9];  // destroyed together (fn_7100ee1548)
    CRntVector<CStrId> field_0x78;
    CRntVector<CFilePathStrId> field_0xa0;
    f32 field_0xc8;
    bool field_0xcc;
    bool field_0xcd;
};
static_assert(__builtin_offsetof(UnkObject_lfa597c, field_0x78) == 0x78, "");
static_assert(__builtin_offsetof(UnkObject_lfa597c, field_0xc8) == 0xc8, "");

void fn_7100fa597c(UnkObject_lfa597c* self) {
    self->vtable = reinterpret_cast<void**>(g_unk_7101cab428 + 0x10);
    self->field_0xa0.Free();
    self->field_0x78.Free();
    fn_7100ee1548(self->field_0x30);
    self->field_0x28.~CStrId();
    self->field_0x20.~CFilePathStrId();
    self->field_0x18.~CStrId();
    self->field_0x10.~CStrId();
    fn_7101043934(reinterpret_cast<UnkCameraCtrl_ovb0049*>(self));
}
