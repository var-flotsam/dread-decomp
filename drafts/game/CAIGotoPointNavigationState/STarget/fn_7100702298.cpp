// CAIGotoPointNavigationState::STarget's unit (tools/tu_map.py unit 1121): the go-to-point state's update.
// After the base update (fn_71007038c0), when the reflected target at +0x120 holds an STarget: picks the
// facing (toward the AI's first point, unless kept), points the path (+0x140) at the target or the entity,
// steps it with the state's speeds and flags, copies its position back, and when it arrives calls the
// state's callback or sends the actor a kind-5 block from its position. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU2702.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::CType;
using base::reflection::UnkReflValue;

extern __attribute__((visibility("hidden"))) base::reflection::CClass g_unk_717270cde8;  // STarget's class

// A position returned in memory (not a plain vector: the original builds it through x8).
struct UnkSluggerPos {
    f32 x;
    f32 y;
    f32 z;
    UnkSluggerPos(const UnkSluggerPos& other);
};

struct UnkTU1123_b257c;
void fn_71007038c0(UnkTU1123_b257c* self);

struct UnkSelf_np41;
void fn_710070699c(UnkSelf_np41* self, const CVector3D* target);
void fn_710070710c(UnkSelf_np41* self, void* entity);
void fn_71007077c8(UnkSelf_np41* self, f32 dt, f32 accel, f32 speed, bool keep_facing, bool flag);

// The target: its slot 0x18 gives the point to go to.
struct UnkGotoTarget_l702298 {
    void** vtable;

    UnkSluggerPos Position() {
        return reinterpret_cast<UnkSluggerPos (*)(UnkGotoTarget_l702298*)>(vtable[0x18 / 8])(this);
    }
};

struct UnkGotoCallback_l702298 {
    void* context;
    void (*fn)(void* context, void* owner);
};

// The path the state steps (+0x140).
struct UnkGotoPath_l702298 {
    u8 pad_0x0[0x24];
    CVector3D facing;  // 0x24
    u8 pad_0x30[0x37fcc - 0x30];
    CVector3D position;  // 0x37fcc
    f32 field_0x37fd8;
    bool arrived;  // 0x37fdc
    u8 pad_0x37fdd[3];
    UnkGotoCallback_l702298 callback;  // 0x37fe0
    u8 pad_0x37ff0[0x38040 - 0x37ff0];
    bool entity_changed;  // 0x38040
};
static_assert(__builtin_offsetof(UnkGotoPath_l702298, entity_changed) == 0x38040, "layout");

struct UnkGotoXform_l702298 {
    u8 pad_0x0[0x70];
    CVector3D position;  // 0x70
};

struct UnkGotoOwner_l702298 {
    u8 pad_0x0[0x218];
    UnkGotoXform_l702298* xform;  // 0x218
    u8 pad_0x220[0x20];
    void* ai;  // 0x240
    u8 pad_0x248[0x50];
    void* sink;  // 0x298
};

struct UnkGotoParams_l702298 {
    u8 pad_0x0[0x78];
    f32 speed;  // 0x78
};

struct UnkGotoState_l702298 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkGotoOwner_l702298* owner;  // 0x30
    CVector3D position;           // 0x38
    CVector3D facing;             // 0x44
    f32 field_0x50;
    u8 pad_0x54[4];
    UnkGotoCallback_l702298 callback;  // 0x58
    UnkGotoParams_l702298* params;     // 0x68
    u8 pad_0x70[0x48];
    CVector3D facing_value;  // 0xb8
    bool has_facing;         // 0xc4
    u8 pad_0xc5[3];
    f32 accel_value;  // 0xc8
    bool has_accel;   // 0xcc
    u8 pad_0xcd[0x1b];
    f32 speed_value;        // 0xe8
    bool has_speed;         // 0xec
    bool flag_value;        // 0xed
    bool has_flag;          // 0xee
    bool keep_facing_value; // 0xef
    bool has_keep_facing;   // 0xf0
    u8 pad_0xf1[0x1f];
    void* entity;     // 0x110
    bool has_entity;  // 0x118
    u8 pad_0x119[7];
    UnkReflValue target;  // 0x120
    u8 pad_0x138[8];
    UnkGotoPath_l702298 path;  // 0x140
};
static_assert(__builtin_offsetof(UnkGotoState_l702298, target) == 0x120, "layout");
static_assert(__builtin_offsetof(UnkGotoState_l702298, path) == 0x140, "layout");

static inline bool IsTarget(UnkReflValue* v) {
    const CType* type = reinterpret_cast<const CType*>(v->type);
    if (!type)
        return false;
    fn_71007019f0();
    return fn_710009d50c(type, reinterpret_cast<const CType*>(&g_unk_717270cde8));
}

static inline UnkGotoTarget_l702298* GetTarget(UnkReflValue* v) {
    if (!v->type)
        return nullptr;
    return const_cast<UnkGotoTarget_l702298*>(base::reflection::refl_ArgValue<UnkGotoTarget_l702298>(v));
}

// An optional setting's value, or `fallback` when it is unset.
static inline const f32& OrDefault(const f32& fallback, bool has, const f32& value) {
    return has ? value : fallback;
}

static inline void SetFacing(CVector3D& facing, f32 x) {
    facing.x = x;
    facing.y = 0.0f;
    facing.z = 0.0f;
}

void fn_7100702298(UnkGotoState_l702298* self, f32 dt) {
    fn_71007038c0(reinterpret_cast<UnkTU1123_b257c*>(self));
    UnkGotoTarget_l702298* target = IsTarget(&self->target) ? GetTarget(&self->target) : nullptr;
    if (!target)
        return;
    f32 speed = OrDefault(self->params->speed, self->has_speed, self->speed_value);
    u8 keep_facing = self->has_keep_facing ? self->keep_facing_value : 0;
    f32 accel = self->has_accel ? self->accel_value : 1.0f;
    u8 flag = self->has_flag ? self->flag_value : 0;
    UnkGotoPath_l702298* path = &self->path;
    if (!keep_facing) {
        UnkSluggerPos point = fn_71011008a8(self->owner->ai, 0);
        CVector2D flat;
        fn_710007b430(&flat, reinterpret_cast<const CVector3D*>(&point));
        if (self->position.x > flat.x)
            SetFacing(self->facing, 1.0f);
        else if (self->position.x < flat.x)
            SetFacing(self->facing, -1.0f);
    }
    UnkSelf_np41* query = reinterpret_cast<UnkSelf_np41*>(path);
    if (self->has_entity && self->entity) {
        if (path->entity_changed) {
            fn_710070710c(query, self->entity);
            path->entity_changed = false;
        }
    } else {
        UnkSluggerPos point = target->Position();
        fn_710070699c(query, reinterpret_cast<const CVector3D*>(&point));
    }
    if (self->has_facing)
        self->facing = self->facing_value;
    path->facing = self->facing;
    path->callback = self->callback;
    fn_71007077c8(query, dt, accel, speed, keep_facing != 0, flag != 0);
    self->position = path->position;
    self->field_0x50 = path->field_0x37fd8;
    if (path->arrived) {
        if (self->callback.fn) {
            self->callback.fn(self->callback.context, self->owner);
        } else {
            UnkParams params;
            params.field_0x1c = 0;
            __builtin_memset(params.field_0x20, 0, 16);
            params.field_0x30 = 0;
            params.vtable = g_unk_7101a25b10 + 2;
            params.field_0x39 = 0;
            params.field_0x3b = 0;
            params.field_0x3d = 0;
            params.field_0x34 = 5;
            __builtin_memcpy(params.field_0x8, &self->owner->xform->position, sizeof(CVector3D));
            params.field_0x8[12] = 1;
            fn_71006f4b0c(self->owner->sink, &params, 0);
        }
    }
}
