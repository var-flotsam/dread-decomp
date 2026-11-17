// CQuetzoaXAIComponent unit: keeps six saved values (handing them out, or taking them while all are zero), then
// grows the area box around the boss, its target and a tracked actor and eases the stored box towards it
// at a speed scaled by the frame time. Names and layouts are placeholders.
#include "types.h"
#include "game/UnkActor.h"
#include "game/UnkTU0.h"

extern f32 g_unk_7101a58f70;  // how fast the area box follows, per second

struct UnkQuetzoaXVec {
    f32 x, y, z;
};

static inline UnkQuetzoaXVec operator+(const UnkQuetzoaXVec& a, const UnkQuetzoaXVec& b) {
    UnkQuetzoaXVec r = {a.x + b.x, a.y + b.y, a.z + b.z};
    return r;
}

static inline UnkQuetzoaXVec operator-(const UnkQuetzoaXVec& a, const UnkQuetzoaXVec& b) {
    UnkQuetzoaXVec r = {a.x - b.x, a.y - b.y, a.z - b.z};
    return r;
}

template <class T>
static inline UnkQuetzoaXVec PositionOf(const T* actor) {
    UnkQuetzoaXVec r = {actor->field_0x44, actor->field_0x54, actor->field_0x64};
    return r;
}

static inline bool IsNearZero(f32 v) {
    return v + 0.001f >= 0.0f && v - 0.001f <= 0.0f;
}

static inline f32 Approach(f32 value, f32 goal, f32 step) {
    if (value < goal) {
        value += step;
        return value > goal ? goal : value;
    }
    value -= step;
    return value < goal ? goal : value;
}

static inline f32 FrameTime() {
    return *reinterpret_cast<const f32*>(reinterpret_cast<const u8*>(g_unk_7101d47328) + 0x30);
}

// Moves a bound out to `v` when `v` lies beyond it.
static inline void Lower(f32& bound, f32 v) {
    bound = bound < v ? bound : v;
}

static inline void Raise(f32& bound, f32 v) {
    bound = bound > v ? bound : v;
}

static inline bool AllNearZero(const f32* values, s32 count) {
    for (s32 i = 0; i < count; i++) {
        if (!IsNearZero(values[i]))
            return false;
    }
    return true;
}

static inline void CopyValues(f32* to, const f32* from, s32 count) {
    for (s32 i = 0; i < count; i++)
        to[i] = from[i];
}

// An axis-aligned box: the lowest corner, then the highest.
struct UnkQuetzoaXBox {
    UnkQuetzoaXVec min;
    UnkQuetzoaXVec max;

    void Set(const UnkQuetzoaXVec& p) {
        min = p;
        max = p;
    }

    void Extend(const UnkQuetzoaXVec& p) {
        Lower(min.x, p.x);
        Lower(min.y, p.y);
        Lower(min.z, p.z);
        Raise(max.x, p.x);
        Raise(max.y, p.y);
        Raise(max.z, p.z);
    }
};

struct UnkQuetzoaXSlot {
    u8 pad_0x0[0x1a0];
    void* field_0x1a0;
};

struct UnkQuetzoaXOwner {
    u8 pad_0x0[0x44];
    f32 field_0x44;
    u8 pad_0x48[0xc];
    f32 field_0x54;
    u8 pad_0x58[0xc];
    f32 field_0x64;
    u8 pad_0x68[0x210];
    UnkQuetzoaXSlot* field_0x278;
};

struct UnkQuetzoaXTracked {
    u8 pad_0x0[0x10];
    UnkActor* field_0x10;
};

// The saved values, then the area box the camera keeps around the fight.
struct UnkQuetzoaXArea {
    f32 saved[6];
    UnkQuetzoaXBox box;
};

struct UnkQuetzoaXAI_l63fee0 {
    u8 pad_0x0[0x30];
    UnkQuetzoaXOwner* owner;
    u8 pad_0x38[0x149e8];
    UnkQuetzoaXTracked* field_0x14a20;
    u8 pad_0x14a28[0x100];
    u8 field_0x14b28[0xa8];
    UnkQuetzoaXArea field_0x14bd0;
};
static_assert(__builtin_offsetof(UnkQuetzoaXAI_l63fee0, field_0x14bd0) == 0x14bd0, "");

void fn_710063fee0(UnkQuetzoaXAI_l63fee0* self, f32* saved) {
    UnkQuetzoaXArea& view = self->field_0x14bd0;
    if (AllNearZero(view.saved, 6))
        CopyValues(view.saved, saved, 6);
    else
        CopyValues(saved, view.saved, 6);

    UnkQuetzoaXVec pos = PositionOf(self->owner);
    UnkActor* target = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner));
    UnkQuetzoaXBox& area = view.box;
    if (target != nullptr) {
        area.Set(pos);
        area.Extend(pos + UnkQuetzoaXVec{200.0f, 200.0f, 0.0f});
        area.Extend(pos + UnkQuetzoaXVec{-200.0f, -50.0f, 0.0f});
        if (target->field_0x208 != nullptr) {
            area.Extend(PositionOf(target) + UnkQuetzoaXVec{250.0f, 250.0f, 0.0f});
            area.Extend(PositionOf(target) - UnkQuetzoaXVec{250.0f, 250.0f, 0.0f});
        } else {
            area.Extend(PositionOf(target) + UnkQuetzoaXVec{250.0f, 150.0f, 0.0f});
            area.Extend(PositionOf(target) - UnkQuetzoaXVec{250.0f, 150.0f, 0.0f});
        }
    }

    UnkQuetzoaXBox goal = area;
    UnkQuetzoaXTracked* tracked = self->field_0x14a20;
    if (tracked != nullptr && tracked->field_0x10 != nullptr) {
        UnkQuetzoaXVec p = PositionOf(tracked->field_0x10);
        area.Extend(UnkQuetzoaXVec{p.x - 150.0f, p.y, p.z});
        area.Extend(UnkQuetzoaXVec{p.x + 150.0f, p.y + 150.0f, p.z});
    }

    UnkQuetzoaXOwner* owner = self->owner;
    UnkQuetzoaXSlot* slot = owner->field_0x278;
    if (slot != nullptr && slot->field_0x1a0 != nullptr && slot->field_0x1a0 == self->field_0x14b28) {
        goal.Extend(PositionOf(owner) + UnkQuetzoaXVec{500.0f, 400.0f, 0.0f});
        goal.Extend(PositionOf(owner) + UnkQuetzoaXVec{-500.0f, 0.0f, 0.0f});
    }

    const f32 speed = g_unk_7101a58f70;
    f32 step = speed * FrameTime();
    area.max.x = Approach(area.max.x, goal.max.x, step);
    area.max.y = Approach(area.max.y, goal.max.y, step);
    area.max.z = Approach(area.max.z, goal.max.z, step);
    step = speed * FrameTime();
    area.min.x = Approach(area.min.x, goal.min.x, step);
    area.min.y = Approach(area.min.y, goal.min.y, step);
    area.min.z = Approach(area.min.z, goal.min.z, step);
}
