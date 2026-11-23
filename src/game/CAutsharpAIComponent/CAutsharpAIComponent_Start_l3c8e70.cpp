// CAutsharpAIComponent unit (tools/tu_map.py unit 572): the start (fn_71003c8e70): the base start, the facing
// taken from the owner's matrix, the walk tuning copied in, the movement and the animation told the facing,
// and the patrol state cleared. Names and layouts are placeholders.
#include "game/CEnemyMovement.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkEffectParams_b0117.h"
#include "game/UnkTU55.h"
#include "game/UnkXformNode.h"
#include "game/behaviortree/CSetAnimDependsOnDirAction.h"
#include "game_s2/UnkParams.h"
#include "types.h"

char* fn_71003cd298(char* p0);
char* fn_71003cd2a4(char* p0, char* p1, char* p2);

struct UnkAutsharpOwner_l3c8e70 {
    void** vtable;
    u8 pad_0x8[0x38 - 0x8];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x218 - 0x94];
    void* field_0x218;
    u8 pad_0x220[0x298 - 0x220];
    void* field_0x298;
};
static_assert(__builtin_offsetof(UnkAutsharpOwner_l3c8e70, field_0x298) == 0x298, "owner layout");

// The tuning object at +0x146d0: its vf_0xb0 gives a speed for the owner.
struct UnkAutsharpTuning_l3c8e70 {
    void** vtable;
    u8 pad_0x8[0x534 - 0x8];
    f32 field_0x534;
    f32 field_0x538;
    f32 field_0x53c;
    f32 field_0x540;

    f32 GetSpeed(UnkAutsharpOwner_l3c8e70* owner) {
        return reinterpret_cast<f32 (*)(UnkAutsharpTuning_l3c8e70*, UnkAutsharpOwner_l3c8e70*)>(vtable[0xb0 / 8])(
            this, owner);
    }
};

// An object and the function to call on it.
struct UnkCallback_l3c8e70 {
    void* object;
    void* function;
};

struct UnkAutsharpAI_l3c8e70 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkAutsharpOwner_l3c8e70* owner;  // 0x30
    u8 pad_0x38[0x1590 - 0x38];
    UnkCallback_l3c8e70 field_0x1590;
    u8 pad_0x15a0[0x146d0 - 0x15a0];
    UnkAutsharpTuning_l3c8e70* tuning;  // 0x146d0
    s32 state;                          // 0x146d8
    u8 pad_0x146dc[4];
    UnkCallback_l3c8e70 field_0x146e0;
    u8 link[0x18];  // 0x146f0
    bool facing_right;  // 0x14708
    bool field_0x14709;
    bool field_0x1470a;
    bool field_0x1470b;
    bool field_0x1470c;
    u8 pad_0x1470d[3];
    f32 field_0x14710;
    f32 timer;  // 0x14714
    f32 field_0x14718;
    f32 field_0x1471c;
    f32 field_0x14720;
    f32 field_0x14724;
    f32 field_0x14728;
    f32 field_0x1472c;
    f32 field_0x14730;
    f32 field_0x14734;
    f32 field_0x14738;
    bool field_0x1473c;
    u8 pad_0x1473d[3];
    f32 field_0x14740;
    f32 field_0x14744;
    bool field_0x14748;
    u8 pad_0x14749[3];
    f32 field_0x1474c[2];
    UnkVec3f field_0x14754;
    UnkVec3f facing;  // 0x14760
    u8 pad_0x1476c[0x10];
    u32 field_0x1477c;
    bool field_0x14780;
    u8 pad_0x14781[3];
    bool field_0x14784;
    bool field_0x14785;
    u8 pad_0x14786[2];
    f32 field_0x14788;
    UnkVec3f field_0x1478c;
    f32 field_0x14798;
    UnkVec3f field_0x1479c;
    bool field_0x147a8;
    bool field_0x147a9;
    bool field_0x147aa;
    bool field_0x147ab;
};
static_assert(__builtin_offsetof(UnkAutsharpAI_l3c8e70, field_0x147ab) == 0x147ab, "layout");
static_assert(__builtin_offsetof(UnkAutsharpAI_l3c8e70, field_0x14784) == 0x14784, "layout");

struct UnkEffectSink;

// A direction that may be unset: three floats and a flag, cleared as 13 bytes.
struct UnkOptDir_l3c8e70 {
    UnkVec3f value;
    bool set;

    void Clear() { __builtin_memset(this, 0, 13); }
};

// The kind-5 block with its direction at +0x8.
struct UnkDirParams_l3c8e70 {
    void** vtable;
    UnkOptDir_l3c8e70 dir;
    u32 field_0x18;
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u8 field_0x20[16];
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;
    u8 pad_0x38;
    u8 field_0x39;
    u8 pad_0x3a;
    u8 field_0x3b;
    u8 pad_0x3c;
    u8 field_0x3d;
};

void fn_71003c8e70(UnkAutsharpAI_l3c8e70* self) {
    fn_7100369650(reinterpret_cast<char*>(self));
    self->state = 0;
    const UnkMatrix44& m = self->owner->xform.GetMatrix();
    self->facing = UnkVec3f(m.m[2], m.m[6], m.m[10]);
    self->facing_right = self->facing.x >= 0.0f;
    self->field_0x1472c = self->tuning->field_0x53c;
    self->field_0x14730 = 0.0f;
    self->field_0x14728 = self->tuning->GetSpeed(self->owner);

    UnkEffectSink* sink = static_cast<UnkEffectSink*>(self->owner->field_0x218);
    UnkEffectParams_b0117 effect;
    effect.field_0xc = 0;
    effect.field_0x14 = 0;
    __builtin_memset(effect.field_0x18, 0, 13);
    __builtin_memset(effect.field_0x28, 0, 13);
    effect.vtable = g_unk_7101a24db8 + 2;
    effect.field_0x3d = 0;
    effect.field_0x3f = 0;
    effect.field_0x41 = 0;
    effect.field_0x52 = 0;
    __builtin_memset(effect.field_0x44, 0, 13);
    effect.field_0x38 = 1;
    fn_71010c7e6c(sink, &effect, 1);

    if (self->owner->field_0x298) {
        UnkDirParams_l3c8e70 params;
        params.field_0x1c = 0;
        __builtin_memset(params.field_0x20, 0, 16);
        params.field_0x30 = 0;
        params.vtable = g_unk_7101a25b10 + 2;
        params.field_0x39 = 0;
        params.dir.Clear();
        params.field_0x3b = 0;
        params.field_0x3d = 0;
        params.field_0x34 = 5;
        const UnkMatrix44& n = self->owner->xform.GetMatrix();
        params.dir.value = UnkVec3f(n.m[2], n.m[6], n.m[10]);
        params.dir.set = true;
        fn_71006f4b0c(self->owner->field_0x298, &params, 1);
    }

    self->field_0x14780 = false;
    self->field_0x1477c = 0;
    self->field_0x14748 = false;
    self->field_0x1473c = false;
    self->field_0x14784 = false;
    self->field_0x14785 = false;
    self->field_0x14754 = UnkVec3f(0.0f, 0.0f, 0.0f);
    self->field_0x14710 = 0.0f;
    self->timer = 0.0f;
    self->field_0x14709 = false;
    self->field_0x1470a = false;
    self->field_0x1470b = false;
    self->field_0x1470c = false;
    self->field_0x147a8 = false;
    self->field_0x147a9 = false;
    self->field_0x147aa = false;
    self->field_0x147ab = false;
    self->field_0x146e0.object = self;
    self->field_0x146e0.function = reinterpret_cast<void*>(fn_71003cd298);
    self->field_0x1590.object = self;
    self->field_0x1590.function = reinterpret_cast<void*>(fn_71003cd2a4);
    self->field_0x14720 = self->tuning->field_0x534;
    self->field_0x14738 = self->tuning->field_0x540;
    self->field_0x14724 = self->tuning->field_0x538;
    self->field_0x14718 = 0.4f;
    self->field_0x1471c = 0.0f;
    self->field_0x14734 = 0.0f;
    self->field_0x14788 = 0.0f;
    self->field_0x1478c = self->owner->xform.GetTranslation();
    self->field_0x14798 = 0.0f;
    self->field_0x1479c = self->owner->xform.GetTranslation();
    self->field_0x14740 = 0.3f;
    self->field_0x14744 = 0.0f;
    UnkVec3f pos = self->owner->xform.GetTranslation();
    fn_710007b444(self->field_0x1474c, reinterpret_cast<const UnkVec3*>(&pos));
}
