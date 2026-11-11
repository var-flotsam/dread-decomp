// Copies of fn_71006f4344 / fn_71006f538c (src/game/CAINavigationComponent/CAINavigationComponent_b39.cpp)
// in three more units (CAbilityGhostAura, CAbilityOpticCamouflage, CAutclastChargeAttack): a listener's
// destructor stores its vtable, detaches itself from the owner of the counted link at +0x18 and drops the
// link's reference; the second of each pair is the deleting form. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/CEnemyMovement.h"
#include "game/CGunComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern void* g_unk_7101a22d48[];
extern void* g_unk_7101a23280[];
extern void* g_unk_7101a2aef8[];

struct UnkLink_b40 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;  // 0x8
    void* owner;  // 0x10
};

struct UnkListener_b40 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkLink_b40* link;  // 0x18
};

static inline __attribute__((always_inline)) void Detach_b40(UnkListener_b40* self, void** vtable, int which) {
    self->vtable = vtable;
    UnkLink_b40* link = self->link;
    if (!link)
        return;
    if (void* owner = link->owner) {
        if (which == 0)
            fn_710117b3f0((UnkWeakTarget*)owner, self);
        else if (which == 1)
            fn_710083b0bc(owner, self);
        else
            fn_71010cb2fc(owner, self);
        link = self->link;
        if (!link)
            return;
    }
    if (--link->ref_count == 0)
        link->Release();
}

void fn_71003149a8(UnkListener_b40* self) { Detach_b40(self, &g_unk_7101a22d48[2], 0); }
void fn_7100314a1c(UnkListener_b40* self) {
    Detach_b40(self, &g_unk_7101a22d48[2], 0);
    fn_7100083c00(self);
}
void fn_7100324e14(UnkListener_b40* self) { Detach_b40(self, &g_unk_7101a23280[2], 1); }
void fn_7100324e88(UnkListener_b40* self) {
    Detach_b40(self, &g_unk_7101a23280[2], 1);
    fn_7100083c00(self);
}
void fn_71003b4680(UnkListener_b40* self) { Detach_b40(self, &g_unk_7101a2aef8[2], 2); }
void fn_71003b7878(UnkListener_b40* self) {
    Detach_b40(self, &g_unk_7101a2aef8[2], 2);
    fn_7100083c00(self);
}
