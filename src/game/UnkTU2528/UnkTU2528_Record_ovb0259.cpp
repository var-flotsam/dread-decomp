// Unit 2528's record (tools/tu_map.py; no class hint): two words, two string ids, six words, two
// vectors, three words and three flags. Its default constructor (the ids built empty, the
// vectors cleared) and its move assignment. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "types.h"

using base::global::CStrId;

struct UnkRecord_ovb0259 {
    u32 field_0x0;
    u32 field_0x4;
    CStrId id_0x8;
    CStrId id_0x10;
    f32 field_0x18[6];
    base::math::CVector4D vec_0x30;
    base::math::CVector4D vec_0x40;
    u32 field_0x50;
    u32 field_0x54;
    u32 field_0x58;
    bool flag_0x5c;
    bool flag_0x5d;
    bool flag_0x5e;
    bool flag_0x5f;
};
static_assert(__builtin_offsetof(UnkRecord_ovb0259, flag_0x5f) == 0x5f, "UnkRecord_ovb0259 layout");

void fn_7100d9bffc(UnkRecord_ovb0259* self) {
    self->id_0x8.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->id_0x8.data = &g_unk_7101d47108->empty;
    self->id_0x10.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->id_0x10.data = &g_unk_7101d47108->empty;
    self->flag_0x5f = false;
    __builtin_memset(&self->field_0x18[0], 0, 0x38);
}

UnkRecord_ovb0259* fn_7100d9b27c(UnkRecord_ovb0259* self, UnkRecord_ovb0259* other) {
    self->id_0x8.ReleaseAndTake(other->id_0x8);
    __builtin_memcpy(&self->field_0x0, &other->field_0x0, 4);
    __builtin_memcpy(&self->field_0x4, &other->field_0x4, 4);
    self->id_0x10.ReleaseAndTake(other->id_0x10);
    self->field_0x18[0] = other->field_0x18[0];
    self->field_0x18[1] = other->field_0x18[1];
    self->field_0x18[2] = other->field_0x18[2];
    self->field_0x18[3] = other->field_0x18[3];
    self->field_0x18[4] = other->field_0x18[4];
    self->field_0x18[5] = other->field_0x18[5];
    fn_710007b518(&self->vec_0x30, &other->vec_0x30);
    const base::math::CVector4D* from = &other->vec_0x40;
    fn_710007b518(&self->vec_0x40, from);
    self->field_0x50 = other->field_0x50;
    self->field_0x54 = other->field_0x54;
    self->field_0x58 = other->field_0x58;
    self->flag_0x5c = other->flag_0x5c;
    self->flag_0x5d = other->flag_0x5d;
    self->flag_0x5e = other->flag_0x5e;
    return self;
}
