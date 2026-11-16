// CTunableAIManager's unit (tools/tu_map.py unit 2540): walks the manager's AI list and wakes each AI
// (its state's vf_0x180, then fn_7100232c68 on the actor), optionally skipping the Emmy and central
// unit AIs, those whose navigation group refuses, and those the game manager's frustum does not see.
// Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAIManager/CTunableAIManager.h"
#include "game/CAISmartObjectComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkChargeComponent.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkNavComponent;
struct UnkNavGroupInfo;
UnkNavGroupInfo* fn_710110c3d8(UnkNavComponent* component);
bool fn_710112baa0(void* actor);

struct UnkScaledRecord {
    void* vtable;
    f32 scale;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 vec[3];
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2c;
    f32 field_0x30;
    u32 pad_0x34;
    s64 scaled;
    s32 fixed[3];
    u8 isZero;
    u8 field_0x4d;
};
void fn_710021688c(f32 scale, UnkScaledRecord* self, const u32* vec);

struct UnkFrustumView;
struct UnkBoundSphere;
s32 fn_71001bab30(const UnkFrustumView* self, UnkBoundSphere* sphere);

struct UnkAIState_lda9990;
struct UnkAIStateVtbl_lda9990 {
    void* pad_0x0[0x30];
    void (*vf_0x180)(UnkAIState_lda9990* self, bool on, bool force);
};
struct UnkAIState_lda9990 {
    const UnkAIStateVtbl_lda9990* vtable;
};

struct UnkAIActor_lda9990 {
    u8 pad_0x0[0x44];
    u32 x;  // 0x44
    u8 pad_0x48[0xc];
    u32 y;  // 0x54
    u8 pad_0x58[0xc];
    u32 z;  // 0x64
    u8 pad_0x68[0x150];
    UnkAIState_lda9990* state;  // 0x1b8
    u8 pad_0x1c0[0x80];
    UnkReflObject* component;  // 0x240
};
static_assert(__builtin_offsetof(UnkAIActor_lda9990, state) == 0x1b8, "UnkAIActor_lda9990 layout");
static_assert(__builtin_offsetof(UnkAIActor_lda9990, component) == 0x240, "UnkAIActor_lda9990 layout");

struct UnkAILink_lda9990 {
    UnkAIActor_lda9990* actor;
    UnkAILink_lda9990* next;
};

struct UnkAIManagerView_lda9990 {
    u8 pad_0x0[0x20];
    UnkAILink_lda9990* ais;  // 0x20
};

struct UnkNavResult_lda9990 {
    u8 pad_0x0[0x350];
    bool field_0x350;
};

struct UnkCamera_lda9990 {
    u8 pad_0x0[0x130];
    UnkFrustumView* frustum;
};

struct UnkView_lda9990 {
    u8 pad_0x0[0x40];
    UnkCamera_lda9990* camera;
};

// The Emmy tunable's member read here.
struct UnkEmmyTunable_lda9990 {
    u8 pad_0x0[0x24];
    f32 view_radius;
};

template <const base::reflection::CClass* (*GetClass)()>
static inline bool IsKindOf(UnkReflObject* object) {
    return fn_710009d50c(object->vf_0x0(), GetClass());
}

void fn_7100da9990(UnkAIManager_da92b0* self_, bool visible_only, bool skip_special, bool check_nav) {
    auto* self = reinterpret_cast<UnkAIManagerView_lda9990*>(self_);
    for (UnkAILink_lda9990* link = self->ais; link; link = link->next) {
        UnkAIActor_lda9990* actor = link->actor;
        if (skip_special) {
            if (!actor->component)
                continue;
            if (IsKindOf<fn_710053b934>(actor->component))
                continue;
            if (!actor->component)
                continue;
            if (IsKindOf<fn_71004030f4>(actor->component))
                continue;
        }
        UnkReflObject* component = actor->component;
        UnkAIState_lda9990* state = actor->state;
        if (!component || !state)
            continue;
        if (check_nav) {
            if (UnkNavGroupInfo* info = fn_710110c3d8(reinterpret_cast<UnkNavComponent*>(component))) {
                auto* nav = reinterpret_cast<UnkNavResult_lda9990*>(fn_7100360044(info));
                if (nav && (!nav->field_0x350 || fn_710112baa0(nav)))
                    continue;
            }
        }
        if (visible_only) {
            if (auto* view = static_cast<UnkView_lda9990*>(fn_7100defcec(g_unk_7101d47310))) {
                f32 radius = reinterpret_cast<const UnkEmmyTunable_lda9990&>(g_unk_71726e0aa8).view_radius;
                UnkScaledRecord sphere;
                alignas(8) u32 pos[3] = {actor->x, actor->y, actor->z};
                fn_710021688c(radius, &sphere, pos);
                s32 result = fn_71001bab30(view->camera->frustum, reinterpret_cast<UnkBoundSphere*>(&sphere));
                fn_7100216934(reinterpret_cast<char*>(&sphere));
                if (result != 1)
                    continue;
            }
        }
        state->vtable->vf_0x180(state, true, false);
        fn_7100232c68(actor);
    }
}
