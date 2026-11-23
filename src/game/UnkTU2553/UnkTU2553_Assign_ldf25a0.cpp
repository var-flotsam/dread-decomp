// Unit 2553 (near CGameManager::CTunableGameManager): the copy assignment of a record holding a weak
// handle, four strings, four string ids and plain values. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkRecord_df25a0 {
    u64 field_0x0;
    UnkWeakHandle* owner;  // 0x8
    bool field_0x10;
    CRntString field_0x18;
    CRntString field_0x48;
    CRntString field_0x78;
    CStrId field_0xa8;
    CStrId field_0xb0;
    CStrId field_0xb8;
    CStrId field_0xc0;
    u32 field_0xc8;
    u32 field_0xcc;
    u32 field_0xd0;
    u32 field_0xd4;
    u16 field_0xd8;
    CRntString field_0xe0;
    u32 field_0x110;
    u32 field_0x114;
    u64 field_0x118;
    u32 field_0x120;
};
static_assert(__builtin_offsetof(UnkRecord_df25a0, field_0xe0) == 0xe0, "field_0xe0");
static_assert(__builtin_offsetof(UnkRecord_df25a0, field_0x120) == 0x120, "field_0x120");

UnkRecord_df25a0* fn_7100df25a0(UnkRecord_df25a0* self, const UnkRecord_df25a0* other) {
    self->field_0x0 = other->field_0x0;
    if (self != other)
        CopyWeakRef(&self->owner, other->owner);
    self->field_0x10 = other->field_0x10;
    CRntString_Assign(&self->field_0x18, &other->field_0x18);
    CRntString_Assign(&self->field_0x48, &other->field_0x48);
    CRntString_Assign(&self->field_0x78, &other->field_0x78);
    self->field_0xa8 = other->field_0xa8;
    self->field_0xb0 = other->field_0xb0;
    self->field_0xb8 = other->field_0xb8;
    self->field_0xc0 = other->field_0xc0;
    self->field_0xc8 = other->field_0xc8;
    self->field_0xcc = other->field_0xcc;
    self->field_0xd0 = other->field_0xd0;
    u32 d4 = other->field_0xd4;
    u16 d8 = other->field_0xd8;
    self->field_0xd8 = d8;
    self->field_0xd4 = d4;
    CRntString_Assign(&self->field_0xe0, &other->field_0xe0);
    self->field_0x110 = other->field_0x110;
    self->field_0x114 = other->field_0x114;
    u64 v118 = other->field_0x118;
    u32 v120 = other->field_0x120;
    self->field_0x120 = v120;
    self->field_0x118 = v118;
    return self;
}
