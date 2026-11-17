// CWeaponMovement unit (tools/tu_map.py unit 1624): the movement's set-up. After the base set-up it
// caches its character class (a CCharClassWeaponMovement), fills the damage description from the class
// and copies it into the last-damage description. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCentralUnitCannonBeamMovementComponent.h"
#include "game/CCharClassAttackComponent.h"
#include "game/UnkTU1370.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;
using base::reflection::UnkReflObject;

bool fn_71010d0b00(char* s);

struct UnkWeaponCharClass_lac73c8 {
    u8 pad_0x0[0x68];
    CRntString damage_name;  // 0x68
    u8 pad_0x98[0x30];
    u32 field_0xc8;
    u32 field_0xcc;
    u32 field_0xd0;
    u8 pad_0xd4[0x14];
    u32 field_0xe8;
};
static_assert(__builtin_offsetof(UnkWeaponCharClass_lac73c8, field_0xe8) == 0xe8, "UnkWeaponCharClass_lac73c8 layout");

struct UnkWeaponDamage_lac73c8 {
    u8 pad_0x0[0x1c];
    u32 field_0x1c;
    u8 pad_0x20[0x90];
    u32 field_0xb0;
    u32 field_0xb4;
    CStrId name;  // 0xb8
    u8 pad_0xc0[0x8];
    u32 field_0xc8;
    u8 pad_0xcc[0x34];
};
static_assert(sizeof(UnkWeaponDamage_lac73c8) == 0x100, "UnkWeaponDamage_lac73c8 layout");

struct UnkWeaponMovement_lac73c8 {
    u8 pad_0x0[0x10];
    UnkReflObject* char_class_object;  // 0x10
    u8 pad_0x18[0x230];
    UnkWeaponDamage_lac73c8 damage;       // 0x248
    UnkWeaponDamage_lac73c8 last_damage;  // 0x348
    u8 pad_0x448[0x30];
    UnkWeaponCharClass_lac73c8* char_class;  // 0x478
};
static_assert(__builtin_offsetof(UnkWeaponMovement_lac73c8, char_class) == 0x478, "UnkWeaponMovement_lac73c8 layout");

// A string id built from a string's text, out of line (fn_7100000850).
struct UnkTextStrId_lac73c8 {
    UnkStringData* data;

    explicit UnkTextStrId_lac73c8(const CRntString& text) {
        fn_7100000850(reinterpret_cast<base::global::UnkStrIdBase*>(this), text, true);
    }
    ~UnkTextStrId_lac73c8() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

// A string id built from a string's text and handed straight to `dst`: what it leaves behind is the
// pool's empty id, which needs no release.
static inline void AssignText(CStrId& dst, const CRntString& text) {
    struct {
        UnkStringData* data;
    } id;
    fn_7100000850(reinterpret_cast<base::global::UnkStrIdBase*>(&id), text, true);
    UnkStringPool* pool = g_unk_7101d47108;
    if (pool) {
        if (dst.data && dst.data != &pool->empty) {
            --dst.data->ref_count;
            if (dst.data->ref_count == 0) {
                fn_71000803a4(pool, dst.data);
                pool = g_unk_7101d47108;
            }
        }
    }
    dst.data = id.data;
    id.data = &pool->empty;
}

static inline UnkReflObject* AsWeaponCharClass(UnkReflObject* obj) {
    return obj && fn_710009d50c(obj->vf_0x0(), fn_7100ab25ec()) ? obj : nullptr;
}

bool fn_7100ac73c8(UnkWeaponMovement_lac73c8* self) {
    bool result = fn_71010d0b00(reinterpret_cast<char*>(self));
    self->char_class = reinterpret_cast<UnkWeaponCharClass_lac73c8*>(AsWeaponCharClass(self->char_class_object));
    const CRntString& name = self->char_class->damage_name;
    if (name.data && name.data[0]) {
        UnkWeaponDamage_lac73c8* damage = &self->damage;
        {
            UnkTextStrId_lac73c8 id(name);
            fn_71008c1538(reinterpret_cast<UnkRequest*>(damage), fn_7100789f5c(reinterpret_cast<const CStrId*>(&id)));
        }
        AssignText(self->damage.name, self->char_class->damage_name);
    }
    UnkWeaponCharClass_lac73c8* char_class = self->char_class;
    self->damage.field_0xb4 = char_class->field_0xc8;
    self->damage.field_0xb0 = char_class->field_0xcc;
    self->damage.field_0xc8 = char_class->field_0xd0;
    self->damage.field_0x1c = char_class->field_0xe8;
    fn_7100400364(reinterpret_cast<UnkWeaponDamage_ovb0054*>(&self->last_damage),
                  reinterpret_cast<const UnkWeaponDamage_ovb0054*>(&self->damage));
    return result;
}
