// CSamusMovement unit (tools/tu_map.py, unit 1501): set-up after the base (fn_71010d7a64). Every state
// in the list at +0x2d18 runs its slot 0x48; the link at +0x5240 is moved to the owner's object at
// +0x1d8 (+0xc70); the three bone controls registered in the owner's +0x220 under the keys at
// 0x7172741000/08/10 are kept (+0x5460 a CCallbackBoneControl, +0x5468/+0x5470 CSamusLegsBoneControl)
// and given a callback each (fn_71002e3bd8); then the legs update runs once (fn_71009e0160). Names
// and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU399.h"
#include "types.h"

using base::reflection::CClass;

struct UnkSamusMovement;
struct UnkLinkList;
void fn_71002aeb9c(UnkLinkList* list, UnkLinkList** slot);
void fn_71002af070(UnkLinkList*, UnkLinkList**);

struct UnkEntry;

struct UnkBoneControl_B71a6 {
    virtual const CClass* vf_0x0();
};

struct UnkBoneCallback_B71a6 {
    void* object;
    void (*fn)();
};

void fn_71002e3bd8(UnkBoneControl_B71a6* control, const UnkBoneCallback_B71a6* callback);
void fn_71009f1154();
void fn_71009f1158();

extern __attribute__((visibility("hidden"))) char g_unk_7172741000[];
extern __attribute__((visibility("hidden"))) char g_unk_7172741008[];
extern __attribute__((visibility("hidden"))) char g_unk_7172741010[];

struct UnkInitMoveState_B71a6 {
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
};

struct UnkInitRegistryOwner_B71a6 {
    u8 pad_0x0[0x40];
    void* registry;
};

struct UnkInitLinkOwner_B71a6 {
    u8 pad_0x0[0xc70];
    u8 list[8];
};

struct UnkInitOwner_B71a6 {
    u8 pad_0x0[0x1d8];
    UnkInitLinkOwner_B71a6* link_owner;
    u8 pad_0x1e0[0x220 - 0x1e0];
    UnkInitRegistryOwner_B71a6* registry_owner;
};

#pragma pack(push, 4)
struct UnkInitMove_B71a6 {
    u8 pad_0x0[0x30];
    UnkInitOwner_B71a6* owner;
    u8 pad_0x38[0x2d18 - 0x38];
    u32 state_count;
    UnkInitMoveState_B71a6* states[1];  // 0x2d1c, 4-byte aligned
    u8 pad_0x2d24[0x5240 - 0x2d24];
    UnkLinkList* link;
    u8 pad_0x5248[0x5460 - 0x5248];
    UnkBoneControl_B71a6* callback_control;
    UnkBoneControl_B71a6* legs_a;
    UnkBoneControl_B71a6* legs_b;
};
#pragma pack(pop)

void fn_71010d7a64(UnkInitMove_B71a6* self);

static inline UnkBoneControl_B71a6* FindControl(UnkInitMove_B71a6* self, const void* key, const CClass* (*cls)()) {
    auto* c = reinterpret_cast<UnkBoneControl_B71a6*>(fn_71002d57ac(self->owner->registry_owner->registry, key));
    if (!c)
        return nullptr;
    return fn_710009d50c(c->vf_0x0(), cls()) ? c : nullptr;
}

void fn_71009dffa8(UnkInitMove_B71a6* self) {
    fn_71010d7a64(self);
    UnkInitMoveState_B71a6** it = self->states;
    u64 bytes = (u64)self->state_count * sizeof(*it);
    asm("" : "+r"(bytes));
    for (; bytes != 0; ++it, bytes -= sizeof(*it))
        (*it)->vf_0x48();
    if (UnkInitLinkOwner_B71a6* lo = self->owner->link_owner) {
        if (self->link) {
            fn_71002aeb9c(self->link, &self->link);
            self->link = nullptr;
        }
        self->link = reinterpret_cast<UnkLinkList*>(lo->list);
        fn_71002af070(reinterpret_cast<UnkLinkList*>(lo->list), &self->link);
    }
    UnkBoneCallback_B71a6 cb_a = {self, &fn_71009f1154};
    UnkBoneCallback_B71a6 cb_b = {self, &fn_71009f1158};
    UnkBoneControl_B71a6* c = FindControl(self, g_unk_7172741000, fn_71002e38c4);
    self->callback_control = c;
    fn_71002e3bd8(c, &cb_a);
    c = FindControl(self, g_unk_7172741008, fn_71002e85e4);
    self->legs_a = c;
    fn_71002e3bd8(c, &cb_b);
    c = FindControl(self, g_unk_7172741010, fn_71002e85e4);
    self->legs_b = c;
    fn_71002e3bd8(c, &cb_b);
    fn_71009e0160(reinterpret_cast<UnkSamusMovement*>(self), true, 0.0f);
}
