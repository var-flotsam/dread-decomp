// CCharClassUsableComponent's unit (tools/tu_map.py): the destructor. Restores the vtable, releases the two
// file paths, the string id and the nineteen string ids last to first, destroys the three strings, then
// runs the char-class component base's destructor (fn_71011acd74). Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CCharClassComponent.h"
#include "game/CCharClassUsableComponent.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;

struct UnkUsable_la7b7e0 {
    void** vtable;
    u8 pad_0x8[0x58 - 0x8];
    CRntString field_0x58;
    CRntString field_0x88;
    CRntString field_0xb8;
    CStrId field_0xe8[19];
    CStrId field_0x180;
    CFilePathStrId field_0x188;
    CFilePathStrId field_0x190;
};
static_assert(__builtin_offsetof(UnkUsable_la7b7e0, field_0x190) == 0x190, "layout");

void fn_7100a7b7e0(char* p) {
    UnkUsable_la7b7e0* self = reinterpret_cast<UnkUsable_la7b7e0*>(p);
    self->vtable = g_unk_7101a90b80 + 2;
    self->field_0x190.~CFilePathStrId();
    self->field_0x188.~CFilePathStrId();
    self->field_0x180.~CStrId();
#pragma clang loop unroll(full)
    for (int i = 19; i-- > 0;)
        self->field_0xe8[i].~CStrId();
    CRntString_Destroy(&self->field_0xb8);
    CRntString_Destroy(&self->field_0x88);
    CRntString_Destroy(&self->field_0x58);
    fn_71011acd74(reinterpret_cast<UnkIdObject*>(self));
}
