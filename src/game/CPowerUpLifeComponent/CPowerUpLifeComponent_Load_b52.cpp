// CPowerUpLifeComponent unit (unit 1381 of tools/tu_map.py): loads the actor definition the string
// id at +0x3d0 names into the reference at +0x3c8, fn_71008e4e40, unless one is held already that
// is ready (+0x10). The id's use count (+0x44 of its data) is raised, the actordef factory
// (g_unk_71726a0220) looks the entry up under the type id g_unk_7172a0ba48, fn_7100232864 is run
// on it and fn_7100232f18 gives the object referenced (+8 counts its references; vtable slot 0x10
// frees it). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU341.h"
#include "game/UnkTU344.h"
#include "types.h"

using base::global::CStrId;

struct UnkActorDefFactory;
struct UnkEntityState;

// Counters raised through accesses that may alias anything: the original reloads the pointers it
// reached them through.
typedef u32 __attribute__((may_alias)) UnkAliasU32_b52;

struct UnkPowerUpLifeName_b52 {
    u8 unk_0x0[0x10];
    char text[0x34];
    u32 field_0x44;
};

struct UnkPowerUpLifeRef_b52 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    u32 ref_count;
    u8 unk_0xc[4];
    void* field_0x10;
};

struct UnkPowerUpLifeEntry_b52;

// The type id the entry is looked up under.
struct alignas(8) UnkPowerUpLifeType_b52 {
    u32 id;
};

struct UnkPowerUpLife_b52 {
    u8 unk_0x0[0x3c8];
    UnkPowerUpLifeRef_b52* ref;
    CStrId name;
};

void fn_71008e4e40(UnkPowerUpLife_b52* self) {
    if (self->ref && self->ref->field_0x10)
        return;
    UnkPowerUpLifeName_b52* name = reinterpret_cast<UnkPowerUpLifeName_b52*>(self->name.data);
    if (self->name.data == &g_unk_7101d47108->empty)
        return;
    UnkActorDefFactory* factory = g_unk_71726a0220;
    u32 id = g_unk_7172a0ba48;
    ++*reinterpret_cast<UnkAliasU32_b52*>(&name->field_0x44);
    UnkPowerUpLifeEntry_b52* entry = fn_710119438c(
        factory, reinterpret_cast<UnkPowerUpLifeName_b52*>(self->name.data)->text, &self->name, UnkPowerUpLifeType_b52{id}, nullptr);
    if (!entry)
        return;
    fn_7100232864(reinterpret_cast<UnkEntityState*>(entry), false);
    UnkPowerUpLifeRef_b52* ref = fn_7100232f18(entry);
    if (self->ref == ref)
        return;
    if (ref)
        ++*reinterpret_cast<UnkAliasU32_b52*>(&ref->ref_count);
    if (UnkPowerUpLifeRef_b52* old = self->ref) {
        if (--old->ref_count == 0)
            old->vf_0x10();
    }
    self->ref = ref;
}
