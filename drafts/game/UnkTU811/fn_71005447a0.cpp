// UnkTU811 (CEmmyAIComponent's unit): resets the E.M.M.I.'s AI state after the base reset: the saved
// record, the navigation-mesh listeners, the reach check, the pending flags and the listener on the
// owner's animation component. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU811.h"
#include "game/behaviortree/CEmmyPatrolAction.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkRecord;
struct UnkEmmyAI_l544fb8;
struct UnkCentralUnit_b49;
UnkRecord* fn_710054715c(UnkRecord* self, const UnkRecord* other);
bool fn_7100544fb8(UnkEmmyAI_l544fb8* self, bool force);
void fn_7100545138(UnkEmmyActor_b55* actor);
void fn_71007717c0(UnkCentralUnit_b49* self);
void fn_71003696d4(void* self);

struct UnkMeshListeners_l5447a0;
struct UnkMeshListener_l5447a0;
void fn_7100566080(UnkMeshListeners_l5447a0* list, UnkMeshListener_l5447a0* listener);
void fn_7100566210(UnkMeshListeners_l5447a0* list, UnkMeshListener_l5447a0* listener);
void fn_71005663c4();

struct UnkMeshListeners_l5447a0 {
    u8 pad_0x0[0x80];
};

// What the navigation mesh holds for its listeners: two lists.
struct UnkNavMeshView_l5447a0 {
    u8 pad_0x0[0x190];
    UnkMeshListeners_l5447a0 first;   // 0x190
    UnkMeshListeners_l5447a0 second;  // 0x210
};

struct UnkMeshListener_l5447a0 {
    UnkMeshListeners_l5447a0* list;
    u8 pad_0x8[0x10];

    void Attach(UnkMeshListeners_l5447a0* to) {
        if (list) {
            fn_7100566080(list, this);
            list = nullptr;
        }
        list = to;
        fn_7100566210(to, this);
    }
};

// A listener registered on a weak target, with the owner and the callback it runs.
struct UnkAnimListener_l5447a0 {
    u8 link[0x8];
    void* owner;            // 0x8
    void (*callback)();     // 0x10
    UnkWeakHandle* handle;  // 0x18
};

struct UnkFlagHolder_l5447a0 {
    u8 pad_0x0[0x30];
    bool field_0x30;
};

struct UnkStateSource_l5447a0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual UnkFlagHolder_l5447a0* vf_0xd8();
};

struct UnkSwitch_l5447a0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(bool on);

    u8 pad_0x8[0x3b8];
    u8 record[0x8];   // 0x3c0
    u32 recordTime;   // 0x3c8
};

struct UnkCentralSlot_l5447a0 {
    u8 pad_0x0[0x10];
    UnkCentralUnit_b49* unit;  // 0x10
};

struct UnkOwner_l5447a0 {
    u8 pad_0x0[0x220];
    UnkWeakTarget* animation;  // 0x220
};

struct UnkEmmyAI_l5447a0 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkOwner_l5447a0* owner;  // 0x30
    u8 pad_0x38[0x14780 - 0x38];
    UnkSwitch_l5447a0 alert;  // 0x14780
    u8 pad_0x14b50[0x10];
    u8 savedRecord[0x20];     // 0x14b60
    u8 pad_0x14b80[0x14c68 - 0x14b80];
    UnkMeshListener_l5447a0 meshListeners[2];  // 0x14c68
    u8 pad_0x14c98[0x27720 - 0x14c98];
    bool patrolPending;       // 0x27720
    u8 pad_0x27721[0x29558 - 0x27721];
    UnkCentralSlot_l5447a0* central;  // 0x29558
    u8 pad_0x29560[0x29ee0 - 0x29560];
    UnkStateSource_l5447a0* state;    // 0x29ee0
    u8 pad_0x29ee8[0x4];
    u32 patrolTime;           // 0x29eec
    u8 pad_0x29ef0[0xc];
    u32 field_0x29efc;        // 0x29efc
    u32 field_0x29f00;        // 0x29f00
    u8 pad_0x29f04[0x20];
    u16 field_0x29f24;        // 0x29f24
    bool field_0x29f26;       // 0x29f26
    u8 pad_0x29f27[0x2a254 - 0x29f27];
    u32 field_0x2a254;        // 0x2a254
    u8 pad_0x2a258[0x2a428 - 0x2a258];
    UnkAnimListener_l5447a0 animListener;  // 0x2a428
    u8 pad_0x2a448[0x2a6b8 - 0x2a448];
    bool field_0x2a6b8;       // 0x2a6b8
    u8 pad_0x2a6b9[0x3];
    u32 field_0x2a6bc;        // 0x2a6bc

    void vf_0x7d0() { reinterpret_cast<void (*)(UnkEmmyAI_l5447a0*)>(vtable[0x7d0 / 8])(this); }
    bool vf_0x7d8() { return reinterpret_cast<bool (*)(UnkEmmyAI_l5447a0*)>(vtable[0x7d8 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkEmmyAI_l5447a0, savedRecord) == 0x14b60, "UnkEmmyAI_l5447a0 layout");
static_assert(__builtin_offsetof(UnkEmmyAI_l5447a0, field_0x29efc) == 0x29efc, "UnkEmmyAI_l5447a0 layout");
static_assert(__builtin_offsetof(UnkEmmyAI_l5447a0, animListener) == 0x2a428, "UnkEmmyAI_l5447a0 layout");
static_assert(__builtin_offsetof(UnkEmmyAI_l5447a0, field_0x2a6bc) == 0x2a6bc, "UnkEmmyAI_l5447a0 layout");

static inline UnkNavMeshManager* NavManager() {
    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    return manager;
}

void fn_71005447a0(UnkEmmyAI_l5447a0* self) {
    fn_71003696d4(self);
    if (self->patrolPending) {
        self->patrolTime = 0;
        fn_7100544a34(reinterpret_cast<UnkEmmyActor_b55*>(self), g_unk_71726ded60, true);
    }
    UnkSwitch_l5447a0* alert = &self->alert;
    fn_710054715c(reinterpret_cast<UnkRecord*>(self->savedRecord), reinterpret_cast<UnkRecord*>(alert->record));
    alert->recordTime = 0;
    self->vf_0x7d0();

    if (UnkNavMeshView_l5447a0* mesh = reinterpret_cast<UnkNavMeshView_l5447a0*>(NavManager()->navmesh)) {
        self->meshListeners[0].Attach(&mesh->first);
        self->meshListeners[1].Attach(&mesh->second);
    }

    fn_7100544fb8(reinterpret_cast<UnkEmmyAI_l544fb8*>(self), false);
    self->field_0x29efc = 0;
    self->field_0x29f00 = 0;
    self->field_0x29f24 = 0;
    fn_7100545138(reinterpret_cast<UnkEmmyActor_b55*>(self));
    self->field_0x29f26 = false;

    bool alerted = false;
    if (self->state->vf_0xd8()->field_0x30)
        alerted = !fn_71005436e8(reinterpret_cast<UnkEmmyAIComponent_b46a1*>(self)) && !self->vf_0x7d8();
    alert->vf_0x20(alerted);

    if (self->central && self->central->unit)
        fn_71007717c0(self->central->unit);
    self->field_0x2a254 = 0;

    if (UnkWeakTarget* animation = self->owner->animation) {
        UnkAnimListener_l5447a0* listener = &self->animListener;
        listener->owner = self;
        listener->callback = fn_71005663c4;
        if (UnkWeakHandle* handle = listener->handle) {
            if (handle->target)
                fn_710117b3f0(handle->target, listener);
            ResetRef(&listener->handle);
        }
        CAbilityGhostAura_AssignWeakHandleToRef(animation, &listener->handle);
        fn_710117b3e8(listener->handle ? listener->handle->target : nullptr, listener);
    }

    fn_7100540dd0(reinterpret_cast<char*>(self));
    self->field_0x2a6b8 = false;
    self->field_0x2a6bc = 0;
}
