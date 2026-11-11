// CCharClassProjectileMovement's unit (tools/tu_map.py): the destructor of a class there (vtable
// g_unk_7101a96758): its three string ids (+0x110, +0x108, +0xf0) are released, then its base's
// destructor (fn_7100589b94, src/game/CCharClassGoliathXBurstProjectionBombMovement/
// CCharClassGoliathXBurstProjectionBombMovement.cpp) runs inlined: that vtable, the string id at +0xd8,
// the two strings (+0x98, +0x68) and its own base destructor (fn_710097b294). Names and layouts are
// placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CCharClassMovementComponent.h"
#include "types.h"

using base::global::CRntString;
using base::global::UnkStringData;
using base::global::UnkStringPool;

extern void* g_unk_7101a4bcf8[];
extern void* g_unk_7101a96758[];

struct UnkProjectileMovementClass_ovb0133 {
    void** vtable;
    u8 unk_0x8[0x60];
    CRntString name_0x68;
    CRntString name_0x98;
    u8 unk_0xc8[0x10];
    UnkStringData* id;  // 0xd8
    u8 unk_0xe0[0x10];
    base::UnkStrIdMember id_0xf0;
    u8 unk_0xf8[0x10];
    base::UnkStrIdMember id_0x108;
    base::UnkStrIdMember id_0x110;
};
static_assert(__builtin_offsetof(UnkProjectileMovementClass_ovb0133, id_0x110) == 0x110, "layout");

void fn_7100ac1694(UnkProjectileMovementClass_ovb0133* self) {
    self->vtable = &g_unk_7101a96758[2];
    self->id_0x110.~UnkStrIdMemberOf();
    self->id_0x108.~UnkStrIdMemberOf();
    self->id_0xf0.~UnkStrIdMemberOf();
    self->vtable = &g_unk_7101a4bcf8[2];
    if (UnkStringPool* pool = g_unk_7101d47108) {
        if (self->id && self->id != &pool->empty) {
            --self->id->ref_count;
            if (self->id->ref_count == 0) {
                fn_71000803a4(pool, self->id);
                pool = g_unk_7101d47108;
            }
        }
        self->id = &pool->empty;
    }
    fn_710007ecd4(&self->name_0x98);
    fn_710007ecd4(&self->name_0x68);
    fn_710097b294(self);
}
