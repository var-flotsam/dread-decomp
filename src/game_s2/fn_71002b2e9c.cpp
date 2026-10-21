// Default constructor of a record: a null head, the default string id, an unset index, a sub-object and
// two growable arrays (growth step 10, flags 0x57), followed by cleared state words.
#include <new>

#include "base/UnkVector.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkElem {
    u8 bytes[8];
};

struct UnkSub;
void fn_710002f4e4(UnkSub* sub);

struct UnkRecord {
    u64 field_0x0;
    CStrId field_0x8;
    s32 field_0x10;
    u8 pad_0x14[4];
    UnkSub* field_0x18;
    u8 pad_0x20[0xa8];
    base::UnkVector<UnkElem> field_0xc8;
    base::UnkVector<UnkElem> field_0xf0;
    u64 field_0x118;
    u64 field_0x120;
    u8 pad_0x128[0xc0];
    u16 field_0x1e8;
    u8 field_0x1ea;
    u8 pad_0x1eb;
    u16 field_0x1ec;
    u8 field_0x1ee;
    u8 pad_0x1ef;
    u64 field_0x1f0;
    u64 field_0x1f8;
    u64 field_0x200;
};

void fn_71002b2e9c(UnkRecord* self) {
    self->field_0x0 = 0;
    new (&self->field_0x8) CStrId(g_unk_7101d470e0);
    self->field_0x10 = -1;
    fn_710002f4e4(reinterpret_cast<UnkSub*>(reinterpret_cast<char*>(self) + 0x18));
    new (&self->field_0xc8) base::UnkVector<UnkElem>(0x57);
    new (&self->field_0xf0) base::UnkVector<UnkElem>(0x57);
    self->field_0x118 = 0;
    self->field_0x120 = 0;
    self->field_0x1e8 = 0;
    self->field_0x1ea = 0;
    self->field_0x1ec = 0;
    self->field_0x1ee = 0xff;
    self->field_0x1f0 = 0;
    self->field_0x1f8 = 0;
    self->field_0x200 = 0;
}
