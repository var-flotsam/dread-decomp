// CChozoCommanderBeamBurstAttack's unit (tools/tu_map.py unit 646): the burst's preparation. Picks the
// shooting time (long on a coin toss unless the previous burst asked for the short one), then sends the
// owner's navigation (+0x298) to a point between the owner and its target, above the AI component's arena
// and inside its edges, with the burst start (fn_710042a7dc) as the arrival callback, and plays the
// preparation animation. Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CAnimationComponent.h"
#include "game/CCharacterMovement.h"
#include "game/UnkActor.h"
#include "game_s2/UnkChozoCommanderAttack.h"
#include "game_s2/UnkParams.h"
#include "types.h"

extern "C" int rand();

using base::reflection::CType;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflValue;
using base::reflection::UnkTunable;

struct UnkTakumakuComponent;

struct UnkAIReach {
    f32 radius;
    f32 field_0x4;

    UnkAIReach() : radius(0.0f), field_0x4(0.0f) {}
    UnkAIReach(const UnkAIReach& other);
};

void fn_7100424b54(UnkChozoCommanderAttack* self);
void fn_710042a7dc();

extern UnkTunable g_unk_71726c56e8;
extern void* g_unk_7101a25b38[];
extern void* g_unk_7101a25b60[];

struct UnkBurstDir_l42880c {
    f32 x;
    f32 y;
    f32 z;
};

// The two facings of the burst, chosen by the animation's state 4.
__attribute__((visibility("hidden"))) extern const UnkBurstDir_l42880c g_unk_7101649418;
__attribute__((visibility("hidden"))) extern const UnkBurstDir_l42880c g_unk_7101649424;

// CTunableCharClassChozoCommanderBeamBurstAttack, as far as the burst reads it.
struct UnkBurstTunable_l42880c {
    u8 pad_0x0[0x24];
    f32 height_to_start_attack;  // 0x24 fHeihtToStartAttack
    u8 pad_0x28[0x18];
    f32 time_shooting;       // 0x40 fTimeShooting
    f32 time_shooting_long;  // 0x44 fTimeShootingLong
};

// CAIGotoPointNavigationState::SPointTarget.
struct UnkPointTarget_l42880c {
    void** vtable;
    f32 x;
    f32 y;
    f32 z;

    UnkPointTarget_l42880c(f32 px, f32 py, f32 pz) : vtable(g_unk_7101a25b60 + 2), x(px), y(py), z(pz) {}
};

// The navigation parameter block of the vtable-0x7101a25b38 kind.
struct UnkBurstParams_l42880c {
    void** vtable;
    UnkBurstDir_l42880c dir;  // 0x8
    bool has_dir;             // 0x14
    u8 pad_0x15[0x7];
    bool field_0x1c;
    u8 pad_0x1d[0x3];
    void* callback_context;  // 0x20
    void (*callback)();      // 0x28
    bool has_callback;       // 0x30
    u8 pad_0x31[0x3];
    u32 field_0x34;
    f32 speed;  // 0x38
    bool has_speed;
    bool field_0x3d;
    bool has_0x3d;
    bool field_0x3f;
    bool has_0x3f;
    u8 pad_0x41[0x7];
    u64 field_0x48[2];
    bool field_0x58;
    u8 pad_0x59[0xf];
    bool field_0x68;
    u8 pad_0x69;
    bool field_0x6a;
    u8 pad_0x6b[0x5];
    UnkReflValue target;  // 0x70
    bool has_target;      // 0x88

    // The direction is left unset until it is given (has_dir stays clear).
    UnkBurstParams_l42880c() {
        field_0x1c = false;
        has_speed = false;
        has_0x3d = false;
        has_0x3f = false;
        callback_context = nullptr;
        callback = nullptr;
        vtable = g_unk_7101a25b38 + 2;
        target = base::reflection::refl_EmptyValue();
        has_callback = false;
        field_0x68 = false;
        field_0x6a = false;
        field_0x48[0] = 0;
        field_0x48[1] = 0;
        field_0x58 = false;
        has_target = false;
        field_0x34 = 1;
    }
    ~UnkBurstParams_l42880c() {
        vtable = g_unk_7101a25b38 + 2;
        fn_71000a1c60(&target);
    }

    // Stores the point in the reflected target: in place when it already holds a compatible type.
    void SetTarget(const UnkPointTarget_l42880c& point) {
        has_target = true;
        const CType* t = fn_7100701c54();
        UnkReflValue* v = &target;
        const CType* cur = reinterpret_cast<const CType*>(v->type);
        if (cur && v->field_0x14 == 0 &&
            (cur == t ? *reinterpret_cast<void* const*>(reinterpret_cast<const char*>(t) + 0x48) != nullptr
                      : fn_710009d52c(t, cur))) {
            fn_71000a1cc8(v, t, &point);
        } else {
            fn_71000a1c60(v);
            fn_71000a18d4(v, t, &point, false);
        }
    }
};

// CChozoCommanderAIComponent, as far as the burst reads it: the arena's edges and floor.
struct UnkBurstAI_l42880c {
    u8 pad_0x0[0x15d1c];
    f32 left;    // 0x15d1c
    f32 floor;   // 0x15d20
    f32 right;   // 0x15d24
};

struct UnkBeamBurstPrepare_l42880c {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x4];
    bool field_0x14;
    u8 pad_0x15[0x43];
    u32 state;  // 0x58
    u8 pad_0x5c[0x4];
    void* component;  // 0x60
    bool short_next;  // 0x68
    u8 pad_0x69[0xf];
    f32 shooting_time;  // 0x78
};

void fn_710042880c(UnkBeamBurstPrepare_l42880c* self) {
    fn_7100424b54(reinterpret_cast<UnkChozoCommanderAttack*>(self));
    self->component = nullptr;
    const UnkBurstTunable_l42880c& tunable = reinterpret_cast<const UnkBurstTunable_l42880c&>(g_unk_71726c56e8);
    self->shooting_time = tunable.time_shooting;
    if (self->short_next) {
        self->short_next = false;
    } else {
        f32 draw = (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
        if (draw < 0.5f)
            self->shooting_time = tunable.time_shooting_long;
    }
    self->field_0x14 = true;

    UnkActor* owner = self->owner;
    f32 owner_x = owner->field_0x44;
    f32 owner_z = owner->field_0x64;
    UnkActor* other = fn_7100d7c990(owner);
    f32 mid_x = (owner_x + other->field_0x44) * 0.5f;
    f32 mid_z = (owner_z + other->field_0x64) * 0.5f;

    UnkBurstParams_l42880c params;
    params.field_0x3d = false;
    params.has_0x3d = true;
    bool turned = fn_7101178e6c(static_cast<UnkTakumakuComponent*>(self->owner->field_0x220), 4);
    params.dir = turned ? g_unk_7101649418 : g_unk_7101649424;
    params.has_dir = true;
    params.field_0x3f = false;
    params.has_0x3f = true;
    params.speed = 20.0f;
    params.has_speed = true;

    UnkReflObject* obj = static_cast<UnkReflObject*>(self->owner->field_0x240);
    UnkBurstAI_l42880c* ai = reinterpret_cast<UnkBurstAI_l42880c*>(
        obj && fn_710009d50c(obj->vf_0x0(), fn_710044e708()) ? obj : nullptr);
    f32 y = ai->floor + tunable.height_to_start_attack;
    f32 lo;
    f32 hi;
    {
        UnkAIReach reach = fn_7100977b90(self->owner->field_0x218);
        f32 half = reach.radius * 0.5f;
        hi = ai->right - half;
        lo = half + ai->left;
    }
    f32 x = mid_x <= lo ? lo : (mid_x >= hi ? hi : mid_x);
    params.SetTarget(UnkPointTarget_l42880c(x, y, mid_z));

    params.callback_context = self;
    params.callback = fn_710042a7dc;
    params.has_callback = true;
    fn_71006f4b0c(self->owner->field_0x298, &params, 1);
    self->state = 0;
    fn_7100426b54(reinterpret_cast<UnkChozoCommanderAttack*>(self), 0xdc61630bd3a4ca6f, false);
}
