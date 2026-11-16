// CSamusModelUpdaterComponent unit (1427): tints a part of the actor's model (+0x30's +0x280, the part named
// by hash 0x8d58474f6280e6bc) with a colour brightened by half and opaque unless it is black; for a positive
// time the tint fades in over it, otherwise it is shown at once. Names and layouts are placeholders.
#include "base/color/CColor4F.h"
#include "game/CFXComponent.h"
#include "game/CSamusModelUpdaterComponent.h"
#include "game/UnkTU247.h"
#include "game/UnkTU2712.h"
#include "game/UnkTU39.h"
#include "types.h"

using base::color::CColor4F;

struct UnkV4_b47 {
    f32 x, y, z, w;
    UnkV4_b47() {}
    UnkV4_b47(f32 x_, f32 y_, f32 z_, f32 w_) : x(x_), y(y_), z(z_), w(w_) {}
    UnkV4_b47(const UnkV4_b47& o) : x(o.x), y(o.y), z(o.z), w(o.w) {}
};

void fn_7101156bd0(UnkObject* model, UnkPart* part, f32 time, f32 from, f32 to, bool on, bool black);

// What a model part's colour goes to (+0x150): its current value at +0xc4.
struct UnkTintTarget_l93adc0 {
    u8 pad_0x0[0xc4];
    f32 value;  // 0xc4
};

// A part of the model: its index (+0xb) and its colour target (+0x150).
struct UnkTintPart_l93adc0 {
    u8 pad_0x0[0xb];
    u8 index;
    u8 pad_0xc[0x144];
    UnkTintTarget_l93adc0* target;  // 0x150
};

struct UnkTintActor_l93adc0 {
    u8 pad_0x0[0x280];
    UnkObject* model;  // 0x280
};

struct UnkTintComponent_l93adc0 {
    u8 pad_0x0[0x30];
    UnkTintActor_l93adc0* actor;  // 0x30
};

static inline bool IsNearZero(f32 v) {
    return v - 0.001f <= 0.0f && v + 0.001f >= 0.0f;
}

static inline UnkV4_b47 operator*(const CColor4F& c, f32 s) {
    return fn_7100030830(reinterpret_cast<const UnkV4_b47*>(&c), s);
}

// The colour's assignment (each component clamped to be non-negative).
static inline void Assign(CColor4F* c, const UnkV4_b47& v) {
    fn_7100030728(c, reinterpret_cast<const CColor4F*>(&v));
}

// Makes the colour opaque unless it is black; returns the sum of its components.
static inline f32 SetOpacity(CColor4F* c) {
    f32 sum = c->r + c->g + c->b;
    c->a = sum > 0.01f ? 1.0f : 0.0f;
    return sum;
}

void fn_710093adc0(base::reflection::UnkReflObject* comp, const void* a, const void*, f32 time) {
    UnkTintComponent_l93adc0* self = reinterpret_cast<UnkTintComponent_l93adc0*>(comp);
    const CColor4F* colour = static_cast<const CColor4F*>(a);
    UnkObject* model = self->actor->model;
    UnkTintPart_l93adc0* part =
        reinterpret_cast<UnkTintPart_l93adc0*>(fn_7101152b90(reinterpret_cast<UnkWorld*>(model), 0x8d58474f6280e6bcull));
    if (part && !fn_710114e674(model, part->index))
        part = nullptr;

    if (time > 0.0f) {
        CColor4F c(*colour);
        Assign(&c, c * 1.5f);
        f32 sum = SetOpacity(&c);
        f32 from = part->target->value;
        fn_71001bc10c(part->target, reinterpret_cast<const UnkV4_b47*>(&c), true);
        fn_7101156bd0(model, reinterpret_cast<UnkPart*>(part), time, from, c.a, true, IsNearZero(sum));
    } else {
        CColor4F c(*colour);
        Assign(&c, c * 1.5f);
        f32 sum = SetOpacity(&c);
        fn_71001bc10c(part->target, reinterpret_cast<const UnkV4_b47*>(&c), true);
        fn_7101152c0c(model, part, !IsNearZero(sum), 0);
    }
}
