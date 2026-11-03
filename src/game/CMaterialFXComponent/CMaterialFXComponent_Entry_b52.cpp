// CMaterialFXComponent unit (tools/tu_map.py, the name from reflection): a record's constructor,
// fn_71009271d4. Two 16-byte values (colours or vectors, copied a word at a time) and a value are
// stored, and the string id is held by a member record that takes it by value (so a temporary copy
// is made and released around it). Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

typedef u32 __attribute__((may_alias)) UnkAliasU32_b52;

struct UnkMaterialFXVec4_b52 {
    f32 v[4];
    UnkMaterialFXVec4_b52(const UnkMaterialFXVec4_b52& o) {
        ((UnkAliasU32_b52*)this)[0] = ((const UnkAliasU32_b52*)&o)[0];
        ((UnkAliasU32_b52*)this)[1] = ((const UnkAliasU32_b52*)&o)[1];
        ((UnkAliasU32_b52*)this)[2] = ((const UnkAliasU32_b52*)&o)[2];
        ((UnkAliasU32_b52*)this)[3] = ((const UnkAliasU32_b52*)&o)[3];
    }
};

struct UnkMaterialFXName_b52 {
    u64 field_0x0;
    CStrId id;
    u32 field_0x10;
    u32 field_0x14;
    UnkMaterialFXName_b52(CStrId id_, u32 value) : field_0x0(0), id(id_), field_0x10(value), field_0x14(0) {}
};

struct UnkMaterialFXEntry_b52 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    UnkMaterialFXVec4_b52 field_0xc;
    UnkMaterialFXVec4_b52 field_0x1c;
    u8 unk_0x2c[4];
    UnkMaterialFXName_b52 name;  // 0x30
    u32 field_0x48;
    f32 field_0x4c;
    u32 field_0x50;

    UnkMaterialFXEntry_b52(const CStrId& id, u32 value, u32 kind, const UnkMaterialFXVec4_b52& a,
                           const UnkMaterialFXVec4_b52& b, f32 f)
        : field_0x0(0), field_0x4(kind), field_0x8(1), field_0xc(a), field_0x1c(b), name(id, value),
          field_0x48(0), field_0x4c(f), field_0x50(0) {}
};

void fn_71009271d4(UnkMaterialFXEntry_b52* self, const CStrId& id, u32 value, u32 kind,
                   const UnkMaterialFXVec4_b52& a, const UnkMaterialFXVec4_b52& b, f32 f) {
    new (self) UnkMaterialFXEntry_b52(id, value, kind, a, b, f);
}
