// CWeaponMovement unit (tools/tu_map.py unit 1624): the movement's constructor, destructor, reset
// and shot-list clear. Over the character movement base (fn_71010d0754 / fn_71010d092c, reset
// fn_71010d0b78) it adds a counted reference at +0x230, two 0x100-byte damage descriptions (+0x248,
// +0x348; constructed by fn_71008c0c74, destroyed by fn_71008c1558, the first copied into the second
// by fn_7100400364), a kind mask at +0x448 and an array of 0x18-byte shots at +0x450, each holding a
// counted reference. The reset points the first description's source at the owner's weak handle.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// Signatures as the other hand-written callers declare them.
struct UnkCharacterMovementFull_ovb0021;
void fn_71010d0754(UnkCharacterMovementFull_ovb0021*);
void fn_71010d092c(UnkCharacterMovementFull_ovb0021*);
struct UnkStruct_71008c0c74;
void fn_71008c0c74(UnkStruct_71008c0c74*);
struct UnkBase_71003cc474;
void fn_71008c1558(UnkBase_71003cc474*);
UnkWeakHandle* fn_7100d78378(UnkWeakTarget*);

extern char g_unk_7101a96ea8[];  // the CWeaponMovement vtable

struct UnkWeaponShotRef_ovb0054 {
    UnkRefObject* ptr;
    ~UnkWeaponShotRef_ovb0054() {
        if (UnkRefObject* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkWeaponShot_ovb0054 {
    UnkWeaponShotRef_ovb0054 ref;
    u64 field_0x8;
    u64 field_0x10;
};

struct UnkWeaponDamage_ovb0054 {
    u8 pad_0x0[0x8];
    UnkWeakHandle* source;  // 0x8
    u8 pad_0x10[0xf0];
};

struct UnkWeaponMovement_ovb0054 {
    char* vtable;
    u8 pad_0x8[0x28];
    UnkWeakTarget* owner;  // 0x30
    u8 pad_0x38[0x1f8];
    UnkRefObject* field_0x230;
    u64 field_0x238;
    u32 field_0x240;
    u8 pad_0x244[0x4];
    UnkWeaponDamage_ovb0054 damage;       // 0x248
    UnkWeaponDamage_ovb0054 last_damage;  // 0x348
    u32 kinds;                            // 0x448
    u8 pad_0x44c[0x4];
    base::UnkVector<UnkWeaponShot_ovb0054> shots;  // 0x450
    u64 field_0x478;
};
static_assert(__builtin_offsetof(UnkWeaponMovement_ovb0054, field_0x478) == 0x478, "UnkWeaponMovement_ovb0054 layout");

void fn_71010d0b78(UnkWeaponMovement_ovb0054* self);
void fn_7100400364(UnkWeaponDamage_ovb0054* dst, const UnkWeaponDamage_ovb0054* src);

static inline UnkStruct_71008c0c74* AsCtorArg(UnkWeaponDamage_ovb0054* d) {
    return reinterpret_cast<UnkStruct_71008c0c74*>(d);
}
static inline UnkBase_71003cc474* AsDtorArg(UnkWeaponDamage_ovb0054* d) {
    return reinterpret_cast<UnkBase_71003cc474*>(d);
}

void fn_7100ac7224(UnkWeaponMovement_ovb0054* self) {
    fn_71010d0754(reinterpret_cast<UnkCharacterMovementFull_ovb0021*>(self));
    self->vtable = g_unk_7101a96ea8 + 0x10;
    self->field_0x230 = nullptr;
    self->field_0x238 = 0;
    self->field_0x240 = 0;
    fn_71008c0c74(AsCtorArg(&self->damage));
    fn_71008c0c74(AsCtorArg(&self->last_damage));
    self->kinds = 1;
    new (&self->shots) base::UnkVector<UnkWeaponShot_ovb0054>(0xf);
    self->field_0x478 = 0;
}

void fn_7100ac72a4(UnkWeaponMovement_ovb0054* self) {
    self->vtable = g_unk_7101a96ea8 + 0x10;
    self->shots.Free();
    fn_71008c1558(AsDtorArg(&self->last_damage));
    fn_71008c1558(AsDtorArg(&self->damage));
    if (UnkRefObject* p = self->field_0x230) {
        if (--p->ref_count == 0)
            p->vf_0x10();
    }
    fn_71010d092c(reinterpret_cast<UnkCharacterMovementFull_ovb0021*>(self));
}

void fn_7100ac7628(UnkWeaponMovement_ovb0054* self) {
    fn_71010d0b78(self);
    self->shots.Clear();
    ResetRef(&self->field_0x230);
    self->field_0x238 = 0;
    self->field_0x240 = 0;
    UnkWeakHandle* source = fn_7100d78378(self->owner);
    UnkWeaponDamage_ovb0054* damage = &self->damage;
    AssignRef(&damage->source, source);
    fn_7100400364(&self->last_damage, damage);
}

void fn_7100ac773c(UnkWeaponMovement_ovb0054* self) {
    self->shots.Clear();
}
