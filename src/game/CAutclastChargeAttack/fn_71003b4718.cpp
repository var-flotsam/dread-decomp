// Constructor tail of an Autclast charge attack's component: runs the base (fn_71003606c8), takes the
// current navigation mesh of the navmesh manager (game/UnkNavMeshManager.h; created first when there
// is none) and, given an owner's object, stores it cast to the attack class (null when it is not one).
#include "base/UnkVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkNavMeshManager.h"
#include "types.h"

using base::reflection::CClass;

struct UnkActor {
    void** vtable;
    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkActor*)>(vtable[0])(this); }
};

struct UnkSelf {
    u8 pad_0x0[0x28];
    UnkActor* field_0x28;
    u8 pad_0x30[0x28];
    UnkNavMesh* field_0x58;
};

void fn_71003606c8(UnkSelf* self, UnkActor* actor);

void fn_71003b4718(UnkSelf* self, UnkActor* actor) {
    fn_71003606c8(self, actor);
    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    self->field_0x58 = manager->navmesh;
    if (actor) {
        UnkActor* attack = nullptr;
        if (fn_710009d50c(actor->vf_0x0(), fn_71003b815c()))
            attack = actor;
        self->field_0x28 = attack;
    }
}
