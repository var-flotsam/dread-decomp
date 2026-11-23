// CSwifterAIComponent's unit (tools/tu_map.py): a start. After the base update it sends the owner's character
// class the track named g_unk_7172709018 followed by a random digit (0 or 1), with a random delay of up to 6
// seconds, then points the owner's +0x298 (a kind-5 block) and movement +0x218 (a kind-3 block) along its
// forward axis. Names and layouts are placeholders.
#include <cstdlib>
#include <string>

#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/global/CRntString.h"
#include "base/global/timeline/CEvent.h"
#include "game/CAutsharpAIComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::global::CRntString;

extern const CRntString g_unk_7172709018 __attribute__((visibility("hidden")));

// A value that may be unset.
struct UnkOptF32_l6cd5c8 {
    f32 value;
    bool set;
};

// A direction that may be unset: three floats and a flag, cleared as 13 bytes.
struct UnkOptDir_l6cd5c8 {
    UnkVec3f value;
    bool set;

    void Clear() { __builtin_memset(this, 0, 13); }
};

// The blend a track send asks for.
struct UnkSendBlend_l6cd5c8 {
    s64 id;
    f32 weight;
    u32 flags;
};

// The delay a track send asks for.
struct UnkSendDelay_l6cd5c8 {
    u64 field_0x0;
    f32 delay;
};

// The kind-5 block (game_s2/UnkParams.h) with its direction and scale written out.
struct UnkDirParams_l6cd5c8 {
    void** vtable;
    UnkOptDir_l6cd5c8 dir;    // 0x8
    UnkOptF32_l6cd5c8 scale;  // 0x18
    u8 field_0x20[16];
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 kind;  // 0x34
    u8 pad_0x38;
    u8 field_0x39;
    u8 pad_0x3a;
    u8 field_0x3b;
    u8 pad_0x3c;
    u8 field_0x3d;
};
static_assert(__builtin_offsetof(UnkDirParams_l6cd5c8, field_0x3d) == 0x3d, "layout");

// The kind-3 block the movement takes.
struct UnkMoveParams_l6cd5c8 {
    void** vtable;
    UnkOptF32_l6cd5c8 scale;  // 0x8
    u8 field_0x10[4];
    u8 field_0x14;
    u8 pad_0x15[3];
    UnkOptDir_l6cd5c8 field_0x18;
    UnkOptDir_l6cd5c8 dir;  // 0x28
    u32 kind;  // 0x38
    u8 field_0x3c;
    u8 field_0x3d;
};
static_assert(__builtin_offsetof(UnkMoveParams_l6cd5c8, kind) == 0x38, "layout");

struct UnkSwifterMovement_l6cd5c8 {
    void** vtable;
    u8 pad_0x8[0xf1];
    bool field_0xf9;

    void vf_0x260() { reinterpret_cast<void (*)(UnkSwifterMovement_l6cd5c8*)>(vtable[0x260 / 8])(this); }
};

struct UnkSwifterAI_l6cd5c8 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
};

// A random value in [lo, hi].
static inline f32 RandomRange(f32 lo, f32 hi) {
    f32 r = (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
    return r * (hi - lo) + lo;
}

static inline const UnkMatrix44& Matrix(UnkActor* actor) {
    return fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38));
}

void fn_71006cd5c8(UnkSwifterAI_l6cd5c8* self) {
    fn_7100369650(reinterpret_cast<char*>(self));
    int variant = rand() & 1;
    f32 delay = RandomRange(0.0f, 6.0f);
    CRntString name(g_unk_7172709018);
    u64 hash;
    base::reflection::UnkReflTypeIdArg mode;
    base::reflection::UnkReflTypeIdArg mask;
    {
        std::string digit = std::to_string(variant);
        CRntString suffix(digit.c_str());
        fn_710007f3b4(&name, &suffix);
    }
    void* cls = self->owner->field_0x220;
    fn_7100066630(&hash, &name);
    {
        mode.id = 0;
        UnkSendBlend_l6cd5c8 blend;
        blend.id = -1;
        blend.weight = 0.6f;
        blend.flags = 1;
        UnkSendDelay_l6cd5c8 wait;
        wait.field_0x0 = 0;
        wait.delay = delay;
        mask.id = UnkAll_SpitSend().value;
        fn_7101176690(cls, hash, 0xf4061ff25a2163d6, &mode, &blend, &wait, &mask);
    }
    UnkDirParams_l6cd5c8 params;
    params.scale.set = false;
    __builtin_memset(params.field_0x20, 0, 16);
    params.vtable = g_unk_7101a25b10 + 2;
    params.dir.Clear();
    params.field_0x30 = 0;
    params.field_0x39 = 0;
    params.field_0x3b = 0;
    params.field_0x3d = 0;
    params.kind = 5;
    const UnkMatrix44& m = Matrix(self->owner);
    params.dir.value = UnkVec3f(m.m[2], m.m[6], m.m[10]);
    params.dir.set = true;
    params.scale.value = 1.0f;
    params.scale.set = true;
    fn_71006f4b0c(self->owner->field_0x298, &params, 1);
    UnkMoveParams_l6cd5c8 move;
    move.field_0x14 = 0;
    move.dir.Clear();
    move.field_0x18.Clear();
    move.vtable = g_unk_7101a2e208 + 2;
    move.kind = 3;
    move.field_0x3c = 0;
    move.field_0x3d = 1;
    move.scale.value = 1.0f;
    move.scale.set = true;
    const UnkMatrix44& n = Matrix(self->owner);
    move.dir.value = UnkVec3f(n.m[2], n.m[6], n.m[10]);
    move.dir.set = true;
    auto* movement = static_cast<UnkSwifterMovement_l6cd5c8*>(self->owner->field_0x218);
    fn_71010c7e6c(reinterpret_cast<UnkEffectSink*>(movement), &move, 1);
    if (movement->field_0xf9 != true) {
        movement->field_0xf9 = true;
        movement->vf_0x260();
    }
}
