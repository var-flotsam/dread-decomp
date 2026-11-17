// CShelmitAIComponent's unit (tools/tu_map.py unit 1054): after the base request handler, a request of
// one kind turns the owner to face along x (by its own speed, or towards the other actor when it is
// standing still); a playing animation cancels the request's flag; in mode 1 the request's animation is
// swapped for its mirrored counterpart. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CCharClassAttackComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU55.h"
#include "types.h"

using base::global::CStrId;

f32 fn_7100d7be34(void* actor);

__attribute__((visibility("hidden"))) extern CStrId g_unk_71727071b8;
__attribute__((visibility("hidden"))) extern CStrId g_unk_71727071c0;
__attribute__((visibility("hidden"))) extern CStrId g_unk_71727071c8;
__attribute__((visibility("hidden"))) extern CStrId g_unk_71727071d0;

struct UnkKraidCreatureVec {
    f32 x;
    f32 y;
    f32 z;
    UnkKraidCreatureVec(const UnkKraidCreatureVec& other) : x(other.x), y(other.y), z(other.z) {}
};

struct UnkTurnVec_l6bb2c4 {
    f32 x;
    f32 y;
    f32 z;
    UnkTurnVec_l6bb2c4() : x(0.0f), y(0.0f), z(0.0f) {}
    UnkTurnVec_l6bb2c4(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkTurnVec_l6bb2c4 operator*(f32 s) const { return UnkTurnVec_l6bb2c4(x * s, y * s, z * s); }
};

struct UnkShelmitActor_l6bb2c4 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44, the position's x
    u8 pad_0x48[0x220 - 0x48];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkRequestKind_l6bb2c4 {
    u8 pad_0x0[0x30];
    u64 id;  // 0x30
};

struct UnkRequestDesc_l6bb2c4 {
    u8 pad_0x0[0x20];
    UnkRequestKind_l6bb2c4* kind;  // 0x20
};

struct UnkShelmitRequest_l6bb2c4 {
    u8 pad_0x0[0x18];
    bool field_0x18;
    u8 pad_0x19[0x11];
    bool field_0x2a;
    u8 pad_0x2b[0x1e];
    bool field_0x49;
    u8 pad_0x4a[2];
    UnkTurnVec_l6bb2c4 direction;  // 0x4c
    u8 pad_0x58[0x40];
    UnkRequestDesc_l6bb2c4* desc;  // 0x98
    u8 pad_0xa0[0x18];
    CStrId animation;  // 0xb8
};
static_assert(__builtin_offsetof(UnkShelmitRequest_l6bb2c4, direction) == 0x4c, "layout");
static_assert(__builtin_offsetof(UnkShelmitRequest_l6bb2c4, animation) == 0xb8, "layout");

struct UnkShelmitAI_l6bb2c4 {
    u8 pad_0x0[0x30];
    UnkShelmitActor_l6bb2c4* owner;  // 0x30
    u8 pad_0x38[0x14784 - 0x38];
    u32 mode;  // 0x14784
};

static inline bool IsZero(f32 v) { return v - 0.001f <= 0.0f && v + 0.001f >= 0.0f; }

static inline bool IsZero(const UnkTurnVec_l6bb2c4& v) { return IsZero(v.x) && IsZero(v.y) && IsZero(v.z); }

static inline f32 Sign(f32 v) { return (f32)((v > 0.0f) - (v < 0.0f)); }

static inline bool IsEmpty(const CStrId& id) { return &g_unk_7101d47108->empty == id.data; }

void fn_71006bb2c4(UnkShelmitAI_l6bb2c4* self, UnkShelmitActor_l6bb2c4* other, UnkShelmitRequest_l6bb2c4* request) {
    fn_7101105c90(reinterpret_cast<UnkKraidAI2*>(self), other, reinterpret_cast<UnkKraidRequest*>(request));
    if (request->desc && request->desc->kind->id == 0xae488c85e788b697ull) {
        UnkTurnVec_l6bb2c4 facing;
        f32 speed = fn_7100d7be34(self->owner);
        const UnkTurnVec_l6bb2c4 right(1.0f, 0.0f, 0.0f);
        if (!IsZero(speed))
            facing = right * Sign(fn_7100d7be08(self->owner).x);
        else if (other)
            facing = right * Sign(other->x - self->owner->x);
        if (!IsZero(facing)) {
            request->field_0x18 = true;
            request->field_0x49 = true;
            fn_710007b444(&request->direction, reinterpret_cast<const UnkVec3*>(&facing));
        }
        request->field_0x2a = false;
    }
    UnkShelmitActor_l6bb2c4* owner = self->owner;
    if (owner && owner->animation && fn_710117ae38(owner->animation, 0x84b4b8ccfe2bc01dull, false))
        request->field_0x2a = false;
    if (self->mode == 1) {
        CStrId mirrored = g_unk_7101d470e0;
        if (request->animation == g_unk_71727071c0)
            mirrored = g_unk_71727071d0;
        else if (request->animation == g_unk_71727071b8)
            mirrored = g_unk_71727071c8;
        if (!IsEmpty(mirrored)) {
            request->animation = mirrored;
            fn_71008c1538(reinterpret_cast<UnkRequest*>(request), fn_7100789f5c(&mirrored));
        }
    }
}
