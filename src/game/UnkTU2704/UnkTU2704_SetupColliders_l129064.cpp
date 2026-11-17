// CSpawnPointComponent's unit (tools/tu_map.py unit 2704): after the base setup, creates the component's
// collider (named after the owner, configured from `shape`) and a second collision object that is
// registered with the tracker. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/global/CRntVector.h"
#include "base/reflection/CClass.h"
#include "game/CSpawnPointComponent.h"
#include "game/UnkTU2506.h"
#include "game/UnkTU2704.h"
#include "game/UnkTU365.h"
#include "game/logic/collision/CCollider.h"
#include <new>
#include "game/UnkBroadphaseObject.h"
#include "types.h"

struct UnkManager_7100863a8c;
void fn_7100d46804(UnkManager_7100863a8c* self);

struct UnkSpawnCollider_l129064 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30(bool on);
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60(u32 mode);
};

struct UnkSpawnOwner_l129064 {
    u8 pad_0x0[0x30];
    base::global::CStrId name;  // 0x30
};

// The collider as this function uses it.
static UnkSpawnCollider_l129064* Collider(CSpawnPointComponent* self) {
    return static_cast<UnkSpawnCollider_l129064*>(self->field_0x168);
}

struct UnkStruct_71007e44b8;
struct UnkObj_71007e44b8;
UnkObj_71007e44b8* fn_71007e44b8(UnkStruct_71007e44b8* self, void* owner, const base::global::CStrId* name, u32 index);
struct UnkTriggerCollider_b0058;
using base::reflection::UnkReflObject;
struct UnkTracker_71008618cc;
void fn_7100d59994(UnkTracker_71008618cc*, UnkReflObject*);

static inline UnkStruct_71007e44b8* Manager() {
    UnkManager_7100863a8c* m = g_unk_71726a4788;
    if (!m) {
        m = static_cast<UnkManager_7100863a8c*>(fn_71000839a0(0x6b0, 0x3f));
        fn_7100d46804(m);
        g_unk_71726a4788 = m;
    }
    return reinterpret_cast<UnkStruct_71007e44b8*>(m);
}

void fn_7101129064(CSpawnPointComponent* self, void* shape) {
    fn_71011265c0(self);
    alignas(8) u8 name_storage[sizeof(base::global::CStrId)];
    {
        UnkStruct_71007e44b8* manager = Manager();
        base::global::CStrId* name =
            new (name_storage) base::global::CStrId(self->Owner<UnkSpawnOwner_l129064>()->name);
        self->field_0x168 = fn_71007e44b8(manager, self, name, 0);
        name->~CStrId();
    }
    Collider(self)->vf_0x30(true);
    fn_7100d36d30(static_cast<UnkTriggerCollider_b0058*>(self->field_0x168), shape);
    Collider(self)->vf_0x60(2);
    fn_7100d35a98(static_cast<UnkEntry*>(self->field_0x168), true);
    {
        UnkStruct_71007e44b8* manager = Manager();
        base::global::CStrId name(self->Owner<UnkSpawnOwner_l129064>()->name);
        self->field_0x1b0 = fn_7100d475ec(manager, &name, 1);
    }
    fn_7100d59994(static_cast<UnkTracker_71008618cc*>(self->field_0x1b0),
                  static_cast<UnkReflObject*>(self->field_0x168));
}
