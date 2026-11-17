// CCharClassShootActivatorComponent unit: after the base setup, registers the component with the player's
// set, caches a pointer from the owner's model, resolves both link lists and collects each linked actor's
// target into the matching array, then notifies itself. Names and layouts are placeholders.
#include "base/UnkDeferredSet.h"
#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkItemLife_l8e0f30;
void fn_71008e0f30(UnkItemLife_l8e0f30* self);
void* fn_710082e510(void* key);

struct UnkShootTarget_l890ae4 {
    u8 pad_0x0[0x198];
    base::UnkDeferredSet<void> set;  // 0x198
};

struct UnkShootPlayer_l890ae4 {
    u8 pad_0x0[0x2a8];
    UnkShootTarget_l890ae4* field_0x2a8;
};

struct UnkShootLinkTarget_l890ae4 {
    u8 pad_0x0[0x10];
    void* actor;  // 0x10
};

struct UnkShootLink_l890ae4 {
    u8 pad_0x0[8];
    u8 member[0x28];  // 0x8
    s32 state;  // 0x30
    u8 pad_0x34[4];
    UnkShootLinkTarget_l890ae4* target;  // 0x38
};

struct UnkShootLinks_l890ae4 {
    u8 links[8];
    u32 count;  // 0x8
};

struct UnkShootModelD_l890ae4 {
    u8 pad_0x0[0x48];
    void** field_0x48;
};

struct UnkShootModelC_l890ae4 {
    u8 pad_0x0[0x78];
    UnkShootModelD_l890ae4* field_0x78;
};

struct UnkShootModelB_l890ae4 {
    u8 pad_0x0[0x138];
    UnkShootModelC_l890ae4* field_0x138;
};

struct UnkShootModel_l890ae4 {
    u8 pad_0x0[0x50];
    UnkShootModelB_l890ae4* field_0x50;
};

struct UnkShootActivator_l890ae4 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkStruct_7100233684* owner;  // 0x30
    u8 pad_0x38[0x388 - 0x38];
    UnkShootLinks_l890ae4 links_a;  // 0x388
    u8 pad_0x394[0x3b0 - 0x394];
    UnkShootLinks_l890ae4 links_b;  // 0x3b0
    u8 pad_0x3bc[0x3d8 - 0x3bc];
    base::global::CRntVector<void*> targets_a;  // 0x3d8
    base::global::CRntVector<void*> targets_b;  // 0x400
    base::UnkDeferredSet<void>* registered_in;  // 0x428
    u8 pad_0x430[0x448 - 0x430];
    void* field_0x448;
    u8 pad_0x450[0x4b8 - 0x450];
    u32 field_0x4b8;
    u32 field_0x4bc;

    void vf_0x140(CBlackboard* blackboard) {
        reinterpret_cast<void (*)(UnkShootActivator_l890ae4*, CBlackboard*)>(vtable[0x140 / 8])(this, blackboard);
    }
    void vf_0x1f0() { reinterpret_cast<void (*)(UnkShootActivator_l890ae4*)>(vtable[0x1f0 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkShootActivator_l890ae4, targets_a) == 0x3d8, "UnkShootActivator layout");
static_assert(__builtin_offsetof(UnkShootActivator_l890ae4, field_0x4bc) == 0x4bc, "UnkShootActivator layout");

static inline UnkShootPlayer_l890ae4* Player() {
    return reinterpret_cast<UnkShootPlayer_l890ae4*>(fn_7100dec788(g_unk_7101d47310));
}

// Moves the registration kept in `slot` (its own address is the item) out of the set it is in and into
// `target`'s set.
static inline void RegisterIn(base::UnkDeferredSet<void>** slot, UnkShootTarget_l890ae4* target) {
    if (*slot) {
        fn_71004f3ea4(*slot, slot);
        *slot = nullptr;
    }
    *slot = &target->set;
    fn_71004f3cf0(*slot, slot);
}

// The link at `index` of `links`, resolved to its actor first if that is still pending.
static inline UnkShootLink_l890ae4* ResolvedLink(UnkShootLinks_l890ae4& links, u32 index) {
    UnkShootLink_l890ae4* link = reinterpret_cast<UnkShootLink_l890ae4*>(
        fn_7100247340(reinterpret_cast<UnkCaterzillaLinks_ovb0054*>(links.links), index));
    if (link->state == 1) {
        {
            UnkRefPtr ref = fn_710023b580(link->member);
            fn_7100234f78(reinterpret_cast<base::UnkRefHandle*>(&link->target),
                          reinterpret_cast<const base::UnkRefHandle*>(&ref));
        }
        link->state = 2;
    }
    return link;
}

// The target of the actor `link` refers to, or null.
static inline void* LinkedTarget(UnkShootLink_l890ae4* link) {
    if (link->target && link->target->actor)
        return fn_710082e510(link->target->actor);
    return nullptr;
}

struct UnkHidrogigaShootActivator;

void fn_7100890ae4(UnkHidrogigaShootActivator* activator) {
    UnkShootActivator_l890ae4* self = reinterpret_cast<UnkShootActivator_l890ae4*>(activator);
    fn_71008e0f30(reinterpret_cast<UnkItemLife_l8e0f30*>(self));

    if (Player() && Player()->field_0x2a8)
        RegisterIn(&self->registered_in, Player()->field_0x2a8);

    UnkShootModel_l890ae4* model = nullptr;
    if (self->owner)
        model = reinterpret_cast<UnkShootModel_l890ae4*>(fn_7100233684(self->owner));
    if (model)
        self->field_0x448 = model->field_0x50 ? *model->field_0x50->field_0x138->field_0x78->field_0x48 : nullptr;
    else
        self->field_0x448 = nullptr;

    for (u32 i = 0; i < self->links_a.count; ++i) {
        if (void* target = LinkedTarget(ResolvedLink(self->links_a, i)))
            self->targets_a.PushBack(target);
    }
    for (u32 i = 0; i < self->links_b.count; ++i) {
        if (void* target = LinkedTarget(ResolvedLink(self->links_b, i)))
            self->targets_b.PushBack(target);
    }

    self->field_0x4bc = self->field_0x4b8;
    self->vf_0x140(&g_unk_7101d47310->field_0x968);
    self->vf_0x1f0();
}
