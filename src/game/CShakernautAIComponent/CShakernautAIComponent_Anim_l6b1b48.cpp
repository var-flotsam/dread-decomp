// CShakernautAIComponent unit (tools/tu_map.py unit 1047): picks the animation to play next. While the
// owner's attack (+0x278) is active, sets the two flags at 0x1488c from the AI component's facing and the
// current animation (kind 0xf4061ff25a2163d6), and returns one of four ids. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/CAttackComponent.h"
#include "game/CShakernautAIComponent.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::UnkReflObject;

extern const CStrId g_unk_7172706458 __attribute__((visibility("hidden")));
extern const CStrId g_unk_7172706460 __attribute__((visibility("hidden")));
extern const CStrId g_unk_7172706468 __attribute__((visibility("hidden")));
extern const CStrId g_unk_7172706470 __attribute__((visibility("hidden")));

struct UnkVec2_sh {
    f32 x;
    f32 y;
};

struct UnkAIComponent_sha {
    u8 pad_0x0[0x14810];
    UnkVec2_sh facing;  // 0x14810
};

struct UnkOwner_sha {
    u8 pad_0x0[0x220];
    void* anim;  // 0x220
    u8 pad_0x228[0x18];
    UnkReflObject* ai;  // 0x240
    u8 pad_0x248[0x30];
    void* attack;  // 0x278
};

struct UnkShakernautAIComponent_sha {
    u8 pad_0x0[0x30];
    UnkOwner_sha* owner;  // 0x30
    u8 pad_0x38[0x14854];
    bool facing_back;  // 0x1488c
    bool alt;  // 0x1488d
};

static inline f32 Dot(const UnkVec2_sh& a, f32 x, f32 y) { return a.x * x + a.y * y; }

CStrId CShakernautAIComponent_SelectIdByFacingAndRecordFlags(UnkShakernautAIComponent_sha* self) {
    if (!fn_7101136308(self->owner->attack))
        return base::global::CStrId::Empty();
    UnkReflObject* ai = self->owner->ai;
    const CClass* cls = ai->vf_0x0();
    fn_71006ad0a4();
    UnkAIComponent_sha* comp =
        fn_710009d50c(cls, &g_unk_7172706520) ? reinterpret_cast<UnkAIComponent_sha*>(ai) : nullptr;
    f32 facing = Dot(comp->facing, 1.0f, 0.0f);
    CStrId current(*fn_7101177ae8(static_cast<UnkAnimationComponent*>(self->owner->anim), 0xf4061ff25a2163d6));
    u64 key = current.data->Key();
    if (facing > 0.0f) {
        self->facing_back = false;
        if (key == 0x7eeb7d2f773a9a6a) {
            self->alt = true;
            return g_unk_7172706468;
        }
        self->alt = false;
        return g_unk_7172706470;
    }
    self->facing_back = true;
    if (key == 0x1f5052c3add9473b) {
        self->alt = true;
        return g_unk_7172706458;
    }
    self->alt = false;
    return g_unk_7172706460;
}
