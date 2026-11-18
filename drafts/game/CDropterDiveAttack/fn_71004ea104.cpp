// CDropterDiveAttack's unit (tools/tu_map.py unit 778): the dive's update. After the base update, state 0
// waits for a dive direction and point (fn_71004ea344), then sends the owner's navigation the direction
// with its vertical part held to 0.7, normalised (a kind-5 parameter block); state 1 flies to the point and,
// once there, or when a check hits, moves on to state 2. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::math::CVector2D;

// The kind-5 parameter block with its direction set (vtable g_unk_7101a25b10).
struct UnkDiveParams_l4ea104 {
    void** vtable;
    f32 dir_x, dir_y, dir_z;  // 0x8
    bool has_dir;             // 0x14
    u8 pad_0x15[3];
    u32 field_0x18;
    bool field_0x1c;
    u8 pad_0x1d[3];
    u8 field_0x20[16];
    bool field_0x30;
    u8 pad_0x31[3];
    u32 kind;  // 0x34
    u8 pad_0x38;
    bool field_0x39;
    u8 pad_0x3a;
    bool field_0x3b;
    u8 pad_0x3c;
    bool field_0x3d;
};

// The segment fn_7100d5c518 tests: its two ends and the vector between them.
struct UnkDiveSegment_l4ea104 {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkDiveSegment_l4ea104(const CVector2D& s, const CVector2D& e) {
        start.x = s.x;
        start.y = s.y;
        end.x = e.x;
        end.y = e.y;
        delta.x = e.x - s.x;
        delta.y = e.y - s.y;
    }
};

struct UnkDropterDive_l4ea104 {
    void** vtable;
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x20];
    f32 reach;     // 0x30: the squared distance that counts as there
    f32 point[3];  // 0x34
    u8 pad_0x40[0xc];
    f32 dir[3];  // 0x4c
    u8 pad_0x58[0xc];
    s32 state;  // 0x64
    bool blocked;  // 0x68
};
static_assert(__builtin_offsetof(UnkDropterDive_l4ea104, blocked) == 0x68, "layout");

struct UnkSpitAttack;
void fn_71003609a8(UnkSpitAttack* self, f32 dt);
bool fn_71004ea344(UnkDropterDive_l4ea104* self, u32 kind, f32* dir, f32* reach);
bool fn_71004ea588(UnkDropterDive_l4ea104* self);
bool fn_71004ea7b0(UnkDropterDive_l4ea104* self);
bool fn_71004ea914(UnkDropterDive_l4ea104* self);
void fn_71004e9b20(UnkDropterDive_l4ea104* self, s32 state, bool flag);

static inline f32 Sign(f32 v) { return (s32)((v > 0.0f) - (v < 0.0f)); }

void fn_71004ea104(UnkDropterDive_l4ea104* self, f32 dt) {
    fn_71003609a8(reinterpret_cast<UnkSpitAttack*>(self), dt);
    switch (self->state) {
    case 0:
        if (fn_71004ea344(self, 1, self->dir, &self->reach)) {
            f32 x = self->dir[0];
            f32 y = self->dir[1];
            f32 a = fabsf(y);
            y = (a < 0.7f ? a : 0.7f) * Sign(y);
            f32 z = self->dir[2];
            f32 inv = 1.0f / (sqrtf(x * x + y * y + z * z) + 0.0001f);
            void* navigation = self->owner->field_0x298;
            UnkDiveParams_l4ea104 params;
            params.field_0x30 = false;
            __builtin_memset(params.field_0x20, 0, 16);
            params.vtable = g_unk_7101a25b10 + 2;
            params.field_0x39 = false;
            params.field_0x3b = false;
            params.field_0x3d = false;
            params.kind = 5;
            params.field_0x18 = 0;
            params.field_0x1c = true;
            params.has_dir = true;
            params.dir_x = x * inv;
            params.dir_y = y * inv;
            params.dir_z = z * inv;
            fn_71006f4b0c(navigation, &params, 1);
        }
        break;
    case 1: {
        UnkActor* owner = self->owner;
        f32 ox = owner->field_0x44;
        f32 oy = owner->field_0x54;
        f32 dx = self->point[0] - ox;
        f32 dy = self->point[1] - oy;
        f32 distance = dx * dx + dy * dy;
        f32 dz = self->point[2] - owner->field_0x64;
        distance += dz * dz;
        if (distance > self->reach) {
            UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
            CVector2D start = {ox, oy};
            CVector2D end = {ox + self->dir[0] * 140.0f, oy + self->dir[1] * 140.0f};
            UnkDiveSegment_l4ea104 segment(start, end);
            self->blocked = fn_7100d5c518(tree, 0x80409, reinterpret_cast<const UnkBox*>(&segment), nullptr, 2);
            fn_71004e9b20(self, 2, false);
        } else if (fn_71004ea588(self) || fn_71004ea7b0(self) || fn_71004ea914(self)) {
            self->blocked = true;
            fn_71004e9b20(self, 2, false);
        }
        break;
    }
    }
}
