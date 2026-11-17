// CChozoWarriorAIComponent unit (tools/tu_map.py): the component's reset: after the base reset, clears the
// fight state, switches off the owner's collision for the warrior's kind, creates a new trail effect, hooks
// the callback fn_710049d620 up to the object the owner tracks and faces the owner. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"

#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/CChozoWarriorAIComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkRenderManager.h"
#include "game/UnkSceneObject.h"
#include "game/UnkTU140.h"
#include "game/UnkTU252.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkObject_71003ad278;
struct UnkShieldAttack_ovb0130;
void fn_71003ad278(UnkObject_71003ad278* self);
void fn_7100494578(UnkShieldAttack_ovb0130* self);
void fn_710049d620();

// A colour (its copy constructor makes it passed through memory).
struct UnkTrailColor_ovb0137 {
    f32 r, g, b, a;
    UnkTrailColor_ovb0137(f32 r_, f32 g_, f32 b_, f32 a_) : r(r_), g(g_), b(b_), a(a_) {}
    UnkTrailColor_ovb0137(const UnkTrailColor_ovb0137& o) : r(o.r), g(o.g), b(o.b), a(o.a) {}
};
void fn_7100148190(UnkSceneObject* self, u32 kind, UnkTrailColor_ovb0137 color, bool flag, f32 a, f32 b, f32 c);

struct UnkTunable;
extern UnkTunable g_unk_71726d0e00;
extern char g_unk_71016499d8[];
extern char g_unk_71016499e0[];

// The warrior's tunables (g_unk_71726d0e00).
struct UnkChozoWarriorTunable_l498b30 {
    u8 pad_0x0[0x28];
    u32 field_0x28;
};

static inline const UnkChozoWarriorTunable_l498b30& Tunable() {
    return reinterpret_cast<const UnkChozoWarriorTunable_l498b30&>(g_unk_71726d0e00);
}

struct UnkChozoWarriorOwner_l498b30 {
    u8 pad_0x0[0x220];
    UnkWeakTarget* track;               // 0x220
    UnkFlagTarget* model_updater;       // 0x228
    u8 pad_0x230[0x70];
    UnkCollisionContainer* collision;  // 0x2a0
};

struct UnkChozoWarriorDef_l498b30 {
    u8 pad_0x0[0x7f8];
    u8 field_0x7f8;
};

struct UnkChozoWarriorTrackLink_l498b30 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

// What the warrior follows: its state and the link hooked up to the object the owner tracks.
struct UnkChozoWarriorTrack_l498b30 {
    u64 field_0x0;
    u32 field_0x8;
    u8 field_0xc;
    u8 pad_0xd[0xb];
    u32 field_0x18;
    u8 pad_0x1c[4];
    UnkChozoWarriorTrackLink_l498b30 link;  // 0x20
    UnkWeakHandle* handle;                  // 0x38
};

struct UnkChozoWarriorAI_l498b30 {
    u8 pad_0x0[0x30];
    UnkChozoWarriorOwner_l498b30* owner;
    u8 pad_0x38[0x14961];
    u8 field_0x14999;
    u8 field_0x1499a;
    u8 pad_0x1499b;
    u32 field_0x1499c;
    u8 field_0x149a0;
    u8 field_0x149a1;
    u8 pad_0x149a2[6];
    u64 field_0x149a8;
    u64 field_0x149b0;
    u64 field_0x149b8;
    u64 field_0x149c0;
    u8 field_0x149c8;
    u8 pad_0x149c9[7];
    UnkChozoWarriorDef_l498b30* def;  // 0x149d0
    u8 pad_0x149d8[0x40];
    u8 field_0x14a18;
    u8 field_0x14a19;
    u8 pad_0x14a1a[2];
    u32 field_0x14a1c;
    u8 pad_0x14a20[8];
    UnkSceneObject* trail;  // 0x14a28
    u8 pad_0x14a30[0xb0];
    u8 field_0x14ae0[0x50];
    u64 field_0x14b30;
    u32 field_0x14b38;
    u8 field_0x14b3c;
    u8 pad_0x14b3d[0xb];
    u32 field_0x14b48;
    u8 pad_0x14b4c[4];
    UnkChozoWarriorTrackLink_l498b30 link;  // 0x14b50
    UnkWeakHandle* handle;                  // 0x14b68
};
static_assert(__builtin_offsetof(UnkChozoWarriorAI_l498b30, field_0x14999) == 0x14999, "layout");
static_assert(__builtin_offsetof(UnkChozoWarriorAI_l498b30, def) == 0x149d0, "layout");
static_assert(__builtin_offsetof(UnkChozoWarriorAI_l498b30, field_0x14a18) == 0x14a18, "layout");
static_assert(__builtin_offsetof(UnkChozoWarriorAI_l498b30, trail) == 0x14a28, "layout");
static_assert(__builtin_offsetof(UnkChozoWarriorAI_l498b30, field_0x14b30) == 0x14b30, "layout");
static_assert(__builtin_offsetof(UnkChozoWarriorAI_l498b30, field_0x14b48) == 0x14b48, "layout");
static_assert(__builtin_offsetof(UnkChozoWarriorAI_l498b30, handle) == 0x14b68, "layout");

void fn_7100498b30(UnkChozoWarriorAI_l498b30* self) {
    fn_71003ad278(reinterpret_cast<UnkObject_71003ad278*>(self));
    self->field_0x14999 = self->def->field_0x7f8;
    self->field_0x1499a = 0;
    self->field_0x1499c = Tunable().field_0x28;
    self->field_0x149a0 = 1;
    self->field_0x149a1 = 1;
    self->field_0x14a18 = 0;
    self->field_0x14a19 = 0;
    self->field_0x14a1c = 0;
    self->field_0x14b30 = 0;
    self->field_0x14b38 = 0;
    self->field_0x14b3c = 0;
    self->field_0x14b48 = 0;
    self->field_0x149b8 = 0;
    self->field_0x149c0 = 0;
    self->field_0x149a8 = 0;
    self->field_0x149b0 = 0;

    if (UnkFlagTarget* model_updater = self->owner->model_updater) {
        fn_7101172780(model_updater, g_unk_71016499d8);
        fn_7101172780(model_updater, g_unk_71016499e0);
    }
    if (UnkCollisionContainer* collision = self->owner->collision) {
        UnkCollisionObject* object =
            fn_71008640a4(collision, reinterpret_cast<const CStrId*>(&g_unk_71726d1260));
        if (object)
            fn_7100d35a98(reinterpret_cast<UnkEntry*>(object), false);
    }

    UnkRenderManager* manager = g_unk_7101d47300;
    UnkSceneObject* trail = nullptr;
    fn_7100149930(manager->field_0x218, nullptr, &trail, &g_unk_7101d470e0);
    fn_7100219ae0(manager, trail ? reinterpret_cast<UnkSceneNode*>(reinterpret_cast<char*>(trail) + 8) : nullptr, 0);
    self->trail = trail;
    fn_7100148190(trail, 6, UnkTrailColor_ovb0137(1.0f, 0.0f, 0.0f, 1.0f), true, 425.0f, 50.0f, 1.0f);
    fn_71001beee4(self->trail->field_0x130, 1);
    fn_7100149490(self->trail, false);

    UnkChozoWarriorTrackLink_l498b30* link = &self->link;
    link->context = self;
    link->callback = fn_710049d620;
    UnkWeakTarget* track = self->owner->track;
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, link);
        ResetRef(&self->handle);
    }
    if (track) {
        fn_7100316018(track, &self->handle);
        fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
    }

    self->field_0x149c8 = 1;
    fn_7100498db8(reinterpret_cast<UnkSelf*>(self));
    fn_7100494578(reinterpret_cast<UnkShieldAttack_ovb0130*>(self->field_0x14ae0));
}
