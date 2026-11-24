// UnkTU2523 (the CEntity unit): whether an entity is in view. Asks the render manager (fn_7100219588)
// about the entity's position (mode 0) or the points of its box (other modes: how many must pass).
// Names and layouts are placeholders.
#include "game/CModelUpdaterComponent.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU2689.h"
#include "game/UnkTU315.h"
#include "types.h"

struct UnkActor;

void* fn_7100d7c214(char* self);
bool fn_7100729978(void* self);

// The rectangle fn_71010d7aac gives (min x, min y, max x, max y), brought up to date by vf_0x98
// when it is out of date.
struct UnkMovementRect_d7bea0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    u8 unk_0x8[0x38];
    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;
    u8 unk_0x50[0x10];
    bool dirty;
};
static_assert(__builtin_offsetof(UnkMovementRect_d7bea0, dirty) == 0x60, "UnkMovementRect_d7bea0 layout");

struct UnkSize_d7bea0 {
    f32 width, height;
};

struct UnkEntity_d7bea0 {
    u8 unk_0x0[0x44];
    f32 x;
    u8 unk_0x48[0xc];
    f32 y;
    u8 unk_0x58[0xc];
    f32 z;
    u8 unk_0x68[0x1a0];
    UnkPlayerMovementCache* movement;
    u8 unk_0x210[0x18];
    void* model_updater;
    u8 unk_0x230[0x158];
    UnkSize_d7bea0 size;
    u8 unk_0x390[0x4];
    f32 center_y;  // added to y
};
static_assert(__builtin_offsetof(UnkEntity_d7bea0, center_y) == 0x394, "UnkEntity_d7bea0 layout");

struct alignas(16) UnkPoint_d7bea0 {
    f32 x, y, z;
};

static inline bool InView(const UnkPoint_d7bea0& p) {
    return IsPositionOnScreen(g_unk_7101d47300, &p, 0, 0);
}

bool fn_7100d7bea0(UnkActor* owner, u32 mode) {
    UnkEntity_d7bea0* self = reinterpret_cast<UnkEntity_d7bea0*>(owner);
    if (self->model_updater && fn_7101173964(self->model_updater))
        return false;
    if (void* found = fn_7100d7c214(reinterpret_cast<char*>(self)))
        return fn_7100729978(found);
    if (mode == 0) {
        UnkPoint_d7bea0 pos;
        pos.x = self->x;
        pos.y = self->y;
        pos.z = self->z;
        return InView(pos);
    }

    UnkPoint_d7bea0 center = {self->x, self->y + self->center_y, self->z};
    UnkSize_d7bea0 size = self->size;
    if (self->movement && fn_71010d7aac(self->movement)) {
        UnkMovementRect_d7bea0* rect = static_cast<UnkMovementRect_d7bea0*>(fn_71010d7aac(self->movement));
        if (rect->dirty)
            rect->vf_0x98();
        center.y = (rect->min_y + rect->max_y) * 0.5f;
        center.x = (rect->min_x + rect->max_x) * 0.5f;
        size.width = rect->max_x - rect->min_x;
        size.height = rect->max_y - rect->min_y;
        center.z = 0.0f;
    }

    if (mode == 3) {
        // The middles of the four sides: any three must be in view.
        f32 half_h = size.height * 0.5f;
        f32 half_w = size.width * 0.5f;
        UnkPoint_d7bea0 top = center;
        top.y += half_h;
        UnkPoint_d7bea0 bottom = center;
        bottom.y -= half_h;
        UnkPoint_d7bea0 left = center;
        left.x -= half_w;
        UnkPoint_d7bea0 right = center;
        right.x += half_w;
        bool t = InView(top);
        bool b = InView(bottom);
        bool l = InView(left);
        bool r = InView(right);
        return (t && b && l) || (t && b && r) || (t && l && r) || (b && l && r);
    }

    // The four corners: 1, any; 2, all; 4, one at the bottom and one at the top; 5, both at the
    // bottom or both at the top.
    f32 half_w = size.width * 0.5f;
    f32 half_h = size.height * 0.5f;
    UnkPoint_d7bea0 top_right = center;
    top_right.x += half_w;
    top_right.y += half_h;
    UnkPoint_d7bea0 top_left = center;
    top_left.x -= half_w;
    top_left.y += half_h;
    UnkPoint_d7bea0 bottom_right = center;
    bottom_right.x += half_w;
    bottom_right.y -= half_h;
    UnkPoint_d7bea0 bottom_left = center;
    bottom_left.x -= half_w;
    bottom_left.y -= half_h;
    // Mode 1 stops at the first corner in view; an unknown mode is never in view.
    bool known = true;
    bool result;  // unset in the original for an unknown mode (not read then)
    do {
        bool bl = InView(bottom_left);
        if (mode == 1 && bl) {
            result = true;
            break;
        }
        bool br = InView(bottom_right);
        if (mode == 1 && br) {
            result = true;
            break;
        }
        bool tr = InView(top_right);
        if (mode == 1 && tr) {
            result = true;
            break;
        }
        bool tl = InView(top_left);
        if (mode == 1 && tl) {
            result = true;
            break;
        }
        switch (mode) {
        case 4:
            result = (bl || br) && (tr || tl);
            break;
        case 2:
            result = bl && br && tr && tl;
            break;
        case 5:
            result = (bl && br) || (tr && tl);
            break;
        default:
            known = false;
            break;
        }
    } while (false);
    return known && result;
}
