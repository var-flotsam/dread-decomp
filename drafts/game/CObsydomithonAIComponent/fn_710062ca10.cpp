// CObsydomithonAIComponent's unit: the component's start. Runs the behaviour-tree base, hooks the
// callback fn_710062fe50 up to the owner's animation (+0x220) and sends it 2 under a fixed hash, keeps
// the collider registered under g_unk_71726f9438 and its bounding box's width and height, resets its
// state, and either raises a flag or zeroes the owner's angle. Names and layouts are placeholders.
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkTU55.h"
#include "game_s2/UnkWeakHandle.h"
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

using base::global::CStrId;

void fn_710062fe50();
struct UnkObsydomithonAI_l62ca10;
void fn_710062cc94(UnkObsydomithonAI_l62ca10* self);
void fn_710062cd74(UnkObsydomithonAI_l62ca10* self, f32 value);

extern CStrId g_unk_71726f9438 __attribute__((visibility("hidden")));

// A collider's bounding box.
struct UnkBounds_l62ca10 {
    f32 min[3];
    f32 max[3];
};

// A collider's shape: its vf_0x28 gives the bounding box of the shape data at its +0x8.
struct UnkShape_l62ca10 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual UnkBounds_l62ca10 vf_0x28(const void* shape);

    u8 field_0x8[1];

    UnkBounds_l62ca10 GetBounds() { return vf_0x28(field_0x8); }
};

struct UnkCollider_l62ca10 {
    u8 pad_0x0[0x158];
    UnkShape_l62ca10* shape;  // 0x158
};

struct UnkOwner_l62ca10 {
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
    virtual void vf_0xd0(const f32* pos, bool flag);
    virtual void vf_0xd8(const f32* angle, bool flag);

    u8 pad_0x8[0x218];
    UnkWeakTarget* animation;  // 0x220
    u8 pad_0x228[0x78];
    UnkCollisionContainer* collision;  // 0x2a0
};

// The callback a link calls, with the object it calls it for.
struct UnkCallback_l62ca10 {
    void* context;
    void (*function)();
};

struct UnkLink_l62ca10 {
    u8 pad_0x0[0x8];
    UnkCallback_l62ca10 callback;
};

struct UnkObsydomithonAI_l62ca10 {
    u8 pad_0x0[0x30];
    UnkOwner_l62ca10* owner;  // 0x30
    u8 pad_0x38[0x121f - 0x38];
    u8 field_0x121f;
    u8 pad_0x1220[0x15d0 - 0x1220];
    u8 field_0x15d0;
    u8 pad_0x15d1[0x14738 - 0x15d1];
    UnkLink_l62ca10 link;    // 0x14738
    UnkWeakHandle* handle;   // 0x14750
    u8 field_0x14758;
    u8 pad_0x14759[3];
    f32 field_0x1475c;
    f32 field_0x14760;
    u8 field_0x14764;
    u8 pad_0x14765[3];
    f32 width;   // 0x14768
    f32 height;  // 0x1476c
    u32 field_0x14770;
    u8 pad_0x14774[0x1479c - 0x14774];
    u8 field_0x1479c[0xc];
    UnkCollider_l62ca10* collider;  // 0x147a8
};
static_assert(__builtin_offsetof(UnkObsydomithonAI_l62ca10, link) == 0x14738, "placeholder layout");
static_assert(__builtin_offsetof(UnkObsydomithonAI_l62ca10, collider) == 0x147a8, "placeholder layout");

void fn_710062ca10(UnkObsydomithonAI_l62ca10* self) {
    fn_7100369650(reinterpret_cast<char*>(self));
    self->field_0x1475c = 0.0f;
    self->field_0x14760 = 0.0f;
    self->field_0x14764 = false;

    if (self->owner->animation) {
        UnkLink_l62ca10* link = &self->link;
        link->callback = UnkCallback_l62ca10{self, fn_710062fe50};
        UnkWeakTarget* track = self->owner->animation;
        if (UnkWeakHandle* handle = self->handle) {
            if (UnkWeakTarget* target = handle->target)
                fn_710117b3f0(target, link);
            ResetRef(&self->handle);
        }
        if (track) {
            fn_7100316018(track, &self->handle);
            fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
        }
        base::reflection::refl_EmitU32(self->owner->animation, 0x1ba5f261458e5017, 0xfc45ff84f571ba40, 2,
                                       &g_unk_710164b958, &g_unk_7172769740);
    }

    if (UnkCollisionContainer* collision = self->owner->collision) {
        auto* collider = reinterpret_cast<UnkCollider_l62ca10*>(fn_7100864194(collision, &g_unk_71726f9438));
        self->collider = collider;
        if (collider) {
            UnkBounds_l62ca10 bounds = collider->shape->GetBounds();
            self->width = bounds.max[0] - bounds.min[0];
            self->height = bounds.max[1] - bounds.min[1];
        }
    }

    fn_710062cc94(self);
    fn_710062cd74(self, 0.0f);

    if (self->field_0x15d0) {
        self->field_0x121f = 1;
    } else {
        f32 angle[3] = {0.0f, 0.0f, 0.0f};
        self->owner->vf_0xd8(angle, true);
    }
    self->field_0x14758 = 0;
    self->field_0x14770 = 0;
    fn_710007b444(self->field_0x1479c, reinterpret_cast<const UnkVec3*>(&g_unk_710164779c));
}
