// Unit 1689 (map menu): clamps the player's map position to the area's bounds widened by 550, shows whether the
// stick is scrolling the map, and plays on each of the four edge arrows "OnUsing", "OnLimitReached" or "OnRest".
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkGameManager.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;

extern CStrId g_unk_7172763da0[] __attribute__((visibility("hidden")));

struct UnkMapWidget_lb3dbe0 {
    void** vtable;

    void vf_0x1f0(const CStrId* anim) {
        reinterpret_cast<void (*)(UnkMapWidget_lb3dbe0*, const CStrId*)>(vtable[0x1f0 / 8])(this, anim);
    }
};

struct UnkMapWidgetRef_lb3dbe0 {
    u8 pad_0x0[0x10];
    UnkMapWidget_lb3dbe0* widget;  // 0x10
};

struct UnkMapBoundsOwner_lb3dbe0 {
    u8 pad_0x0[0x20];
    UnkXformNode node;
};

struct UnkMapBounds_lb3dbe0 {
    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;
    u8 pad_0x10[0x1f8];
    UnkMapBoundsOwner_lb3dbe0* owner;  // 0x208
};

struct UnkMapWorld_lb3dbe0 {
    u8 pad_0x0[0xd63d40];
    UnkMapBounds_lb3dbe0 bounds;  // 0xd63d40
};

struct UnkMapScroll_lb3dbe0 {
    u8 pad_0x0[0x260];
    bool scrolling;  // 0x260
    bool field_0x261;
    u8 pad_0x262[6];
    UnkMapWidgetRef_lb3dbe0* indicator;  // 0x268
    u8 pad_0x270[0x2d8 - 0x270];
    UnkMapWidgetRef_lb3dbe0* arrow_up;     // 0x2d8
    UnkMapWidgetRef_lb3dbe0* arrow_down;   // 0x2e0
    UnkMapWidgetRef_lb3dbe0* arrow_left;   // 0x2e8
    UnkMapWidgetRef_lb3dbe0* arrow_right;  // 0x2f0
    u8 pad_0x2f8[0x8b4 - 0x2f8];
    bool input_enabled;  // 0x8b4
};
static_assert(__builtin_offsetof(UnkMapScroll_lb3dbe0, input_enabled) == 0x8b4, "UnkMapScroll layout");

static inline void Play(UnkMapWidgetRef_lb3dbe0* ref, const CStrId& anim) {
    if (ref && ref->widget)
        ref->widget->vf_0x1f0(&anim);
}

void fn_7100b3dbe0(UnkMapScroll_lb3dbe0* self, const base::math::CVector2D* stick) {
    UnkMapBounds_lb3dbe0* bounds =
        &static_cast<UnkMapWorld_lb3dbe0*>(g_unk_7101d47310->field_0x2228)->bounds;
    UnkVec3f world = fn_7100206154(&bounds->owner->node);
    base::math::CVector2D projected;
    fn_710007b430(&projected, reinterpret_cast<const base::math::CVector3D*>(&world));
    base::math::CVector2D pos = projected;

    f32 min_x = bounds->min_x - 550.0f;
    f32 min_y = bounds->min_y - 550.0f;
    f32 max_x = bounds->max_x + 550.0f;
    f32 max_y = bounds->max_y + 550.0f;
    f32 x = pos.x > min_x ? (pos.x >= max_x ? max_x : pos.x) : min_x;
    f32 y = pos.y > min_y ? (pos.y >= max_y ? max_y : pos.y) : min_y;

    bool scrolling = false;
    if (self->input_enabled)
        scrolling = stick->x * stick->x + stick->y * stick->y > 0.0f;
    if (self->scrolling != scrolling && !self->field_0x261) {
        self->indicator->widget->vf_0x1f0(&g_unk_7172763da0[scrolling ? 1 : 0]);
        self->scrolling = scrolling;
    }

    static const CStrId s_rest("OnRest", true);
    static const CStrId s_using("OnUsing", true);
    static const CStrId s_limit("OnLimitReached", true);
    // Unused in this build.
    CStrId anim = scrolling ? s_using : s_rest;

    if (scrolling) {
        f32 dy = pos.y - y;
        Play(self->arrow_up, dy <= 0.0f ? s_using : s_limit);
        Play(self->arrow_down, dy >= 0.0f ? s_using : s_limit);
        f32 dx = pos.x - x;
        Play(self->arrow_left, dx >= 0.0f ? s_using : s_limit);
        Play(self->arrow_right, dx <= 0.0f ? s_using : s_limit);
    } else {
        Play(self->arrow_up, s_rest);
        Play(self->arrow_down, s_rest);
        Play(self->arrow_left, s_rest);
        Play(self->arrow_right, s_rest);
    }
}
