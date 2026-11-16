// CAutsniperShootAttack's unit (tools/tu_map.py unit 576): sends the owner towards a point. With the
// flag set, the point becomes one of the movement limits (the far one when the near one is within
// reach); unless the owner already stands at the point, its navigation is handed a go-to-point
// target (fn_71006f4b0c). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CAutsniperShootAttack.h"
#include "game/CEnemyMovement.h"
#include "game/UnkTU2702.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflValue;

extern void* g_unk_7101a25b38[];
extern void* g_unk_7101a25b60[];

// The attack's callback for the navigation (placeholder signature).
void fn_71003d886c(void* self);

// As src/game/CBaseBigFistAIComponent/CBaseBigFistAIComponent_Limits_ovb0050.cpp defines it.
struct UnkBigFistKind_ovb0050 {
    u32 value;
    u32 unk_0x4;
};

// A point (zero unless set).
struct UnkPoint_3d5450 {
    f32 x, y, z;
    UnkPoint_3d5450() : x(0.0f), y(0.0f), z(0.0f) {}
};

// The go-to-point target (CAIGotoPointNavigationState::SPointTarget).
struct UnkPointTarget_3d5450 {
    void** vtable;
    UnkPoint_3d5450 point;

    explicit UnkPointTarget_3d5450(const UnkPoint_3d5450& p) : vtable(g_unk_7101a25b60 + 2) {
        point.x = p.x;
        point.y = p.y;
        point.z = p.z;
    }
};

// A callback: an object and the function to call on it.
struct UnkCallback_3d5450 {
    void* object;
    void (*function)(void*);
};

// The parameter block handed to the navigation (vtable 0x7101a25b38).
struct alignas(16) UnkGotoParams_3d5450 {
    void** vtable;
    u8 field_0x8[13];
    u8 pad_0x15[0x7];
    bool field_0x1c;
    u8 pad_0x1d[0x3];
    UnkCallback_3d5450 callback;  // 0x20
    bool field_0x30;
    u8 pad_0x31[0x3];
    u32 field_0x34;
    f32 field_0x38;
    bool field_0x3c;
    u8 pad_0x3d;
    bool field_0x3e;
    bool field_0x3f;
    bool field_0x40;
    u8 pad_0x41[0x7];
    u64 field_0x48[2];
    bool field_0x58;
    u8 pad_0x59[0xf];
    bool field_0x68;
    u8 pad_0x69;
    bool field_0x6a;
    u8 pad_0x6b[0x5];
    UnkReflValue value;  // 0x70
    bool field_0x88;

    UnkGotoParams_3d5450() {
        field_0x1c = false;
        vtable = g_unk_7101a25b38 + 2;
        __builtin_memset(field_0x8, 0, 13);
        field_0x3c = false;
        value = base::reflection::refl_EmptyValue();
        field_0x3e = false;
        field_0x40 = false;
        field_0x68 = false;
        field_0x6a = false;
        field_0x58 = false;
        field_0x88 = false;
        field_0x34 = 1;
        field_0x30 = false;
        __builtin_memset(field_0x48, 0, 16);
        callback.object = nullptr;
        callback.function = nullptr;
    }
    ~UnkGotoParams_3d5450() {
        vtable = g_unk_7101a25b38 + 2;
        fn_71000a1c60(&value);
    }

    // Stores the target in the reflected value: in place when it already holds a compatible type.
    void SetTarget(const UnkPointTarget_3d5450& target) {
        const CType* type = fn_7100701c54();
        const CType* current = reinterpret_cast<const CType*>(value.type);
        if (current && value.field_0x14 == 0 &&
            (current == type ? *reinterpret_cast<void* const*>(reinterpret_cast<const char*>(type) + 0x48) != nullptr
                             : fn_710009d52c(type, current))) {
            fn_71000a1cc8(&value, type, &target);
        } else {
            fn_71000a1c60(&value);
            fn_71000a18d4(&value, type, &target, false);
        }
    }
};
static_assert(__builtin_offsetof(UnkGotoParams_3d5450, field_0x88) == 0x88, "UnkGotoParams_3d5450 layout");

struct UnkAutsniperAI_3d5450 : UnkReflObject {
    u8 pad_0x8[0x27240];
    bool field_0x27248;
};
static_assert(__builtin_offsetof(UnkAutsniperAI_3d5450, field_0x27248) == 0x27248, "UnkAutsniperAI_3d5450 layout");

struct UnkOwner_3d5450 {
    u8 pad_0x0[0x44];
    f32 x;  // the transform's translation
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;
    u8 pad_0x68[0x188];
    UnkReflObject* movement;  // 0x1f0, a CEnemyMovement
    u8 pad_0x1f8[0x48];
    UnkReflObject* ai;  // 0x240
    u8 pad_0x248[0x50];
    void* navigation;  // 0x298
};
static_assert(__builtin_offsetof(UnkOwner_3d5450, navigation) == 0x298, "UnkOwner_3d5450 layout");

struct UnkAttack_3d5450 {
    u8 pad_0x0[0x8];
    UnkOwner_3d5450* owner;
    u8 pad_0x10[0xa8];
    UnkCallback_3d5450 callback;  // 0xb8
    u8 pad_0xc8[0x12a90];
    bool use_max;  // 0x12b58
};
static_assert(__builtin_offsetof(UnkAttack_3d5450, use_max) == 0x12b58, "UnkAttack_3d5450 layout");

// Whether `p` lies in the box of half-size `r` around the owner.
static inline bool IsNear(const UnkOwner_3d5450* owner, UnkPoint_3d5450 p, f32 r) {
    return owner->x + r >= p.x && owner->x - r <= p.x && owner->y + r >= p.y && owner->y - r <= p.y &&
           owner->z + r >= p.z && owner->z - r <= p.z;
}

void fn_71003d5450(UnkSelf* self_, const UnkVec3* v, u32 a2) {
    UnkAttack_3d5450* self = reinterpret_cast<UnkAttack_3d5450*>(self_);
    // The point is written back (the declaration's const is not the original's).
    UnkPoint_3d5450* target = reinterpret_cast<UnkPoint_3d5450*>(const_cast<UnkVec3*>(v));
    bool to_limit = a2 & 1;  // a bool in the original

    UnkReflObject* movement = self->owner->movement;
    if (!movement || !fn_710009d50c(movement->vf_0x0(), fn_71010c74e0()) || fn_71010cc3a4(movement) != 1)
        return;

    if (to_limit) {
        UnkPoint_3d5450 min;
        UnkPoint_3d5450 max;
        UnkBigFistKind_ovb0050 kind;
        kind.value = 2;
        if (fn_71010fc864(reinterpret_cast<UnkBigFistAI3_ovb0050*>(self->owner->ai), &kind,
                          reinterpret_cast<UnkBigFistLimit_ovb0050*>(&min),
                          reinterpret_cast<UnkBigFistLimit_ovb0050*>(&max))) {
            // The chosen limit, or the other one when the owner is already near it.
            UnkPoint_3d5450 limit = self->use_max ? max : min;
            if (IsNear(self->owner, limit, 65.0f))
                limit = self->use_max ? min : max;
            *target = limit;
        }
        UnkReflObject* ai = self->owner->ai;
        if (ai && fn_710009d50c(ai->vf_0x0(), fn_71003da2e0()))
            static_cast<UnkAutsniperAI_3d5450*>(ai)->field_0x27248 = false;
    }

    if (IsNear(self->owner, *target, 10.0f))
        return;
    void* navigation = self->owner->navigation;
    if (!navigation)
        return;

    UnkGotoParams_3d5450 params;
    self->callback.object = self;
    self->callback.function = fn_71003d886c;
    UnkPointTarget_3d5450 point(*target);
    params.field_0x88 = true;
    params.SetTarget(point);
    params.callback = self->callback;
    params.field_0x30 = true;
    params.field_0x3c = true;
    params.field_0x38 = 10.0f;
    params.field_0x3f = true;
    params.field_0x40 = true;
    fn_71006f4b0c(navigation, &params, 1);
}
