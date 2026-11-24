// CCoreXAIComponent unit (tools/tu_map.py unit 759): follows the linked actor (at a model node of its
// own when it names one), then turns the aim direction towards the target by at most a speed that
// grows with the angle, and rebuilds the frame from the body's axis. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "game/CComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkKraidCreatureVec {
    f32 x;
    f32 y;
    f32 z;
    UnkKraidCreatureVec(const UnkKraidCreatureVec& other) : x(other.x), y(other.y), z(other.z) {}
};

struct UnkMatrix44Words {
    u32 m[16];

    UnkMatrix44Words() {}
    UnkMatrix44Words(const UnkMatrix44Words& other);
};

UnkMatrix44Words CopyMatrixWithoutTranslation(const UnkMatrix44Words* src);
UnkVec3f RotateVec3AboutZ(const UnkVec3f& v, f32 angle);
void fn_71000769f0(UnkMatrix44* out, const void* a, const void* b, const void* c, const void* d);

// The name of the model node the AI component follows, and that node's offset.
struct UnkCoreXNodeName_l4d9b88 {
    base::global::UnkStringData* data;
};
const UnkCoreXNodeName_l4d9b88* fn_710110f224(void* ai);
const UnkVec3f* fn_710110f230(void* ai);

__attribute__((visibility("hidden"))) extern const f32 g_unk_7101649bc0[3];
__attribute__((visibility("hidden"))) extern const f32 g_unk_7101649bcc[3];

struct UnkCoreXBody_l4d9b88 {
    u8 pad_0x0[0x1d8];
    UnkVec3f axis;  // 0x1d8
};

struct UnkCoreXOwner_l4d9b88 {
    u8 pad_0x0[0x38];
    UnkXform transform;  // 0x38
    u8 pad_0x94[0x15c];
    UnkCoreXBody_l4d9b88* body;  // 0x1f0
    u8 pad_0x1f8[0x1b8];
    bool moved;  // 0x3b0
};
static_assert(__builtin_offsetof(UnkCoreXOwner_l4d9b88, moved) == 0x3b0, "layout");

struct UnkCoreXLink_l4d9b88 {
    u8 pad_0x0[0x10];
    UnkActor* actor;  // 0x10
};

struct UnkCoreXAI_l4d9b88 {
    u8 pad_0x0[0x30];
    UnkCoreXOwner_l4d9b88* owner;  // 0x30
    u8 pad_0x38[0x158c];
    UnkVec3f followed;  // 0x15c4
    u8 pad_0x15d0[0x133e8];
    UnkVec3f aim;  // 0x149b8
    u8 pad_0x149c4[0xc];
    UnkCoreXLink_l4d9b88* link;  // 0x149d0
};
static_assert(__builtin_offsetof(UnkCoreXAI_l4d9b88, aim) == 0x149b8, "layout");
static_assert(__builtin_offsetof(UnkCoreXAI_l4d9b88, link) == 0x149d0, "layout");

// The address of a temporary, for the callees that take their matrix by pointer.
template <class T>
static inline const T* AddressOf(const T& value) {
    return &value;
}

template <class V>
static inline void Normalize(V& v) {
    f32 inv = 1.0f / (sqrtf(v.x * v.x + v.y * v.y + v.z * v.z) + 0.0001f);
    v.x = v.x * inv;
    v.y = inv * v.y;
    v.z = inv * v.z;
}

template <class A, class B>
static inline f32 Dot(const A& a, const B& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline UnkVec3f Cross(const UnkVec3f& a, const UnkVec3f& b) {
    return UnkVec3f(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

static inline UnkVec3f Negated(const UnkVec3f& v) {
    return UnkVec3f(-v.x, -v.y, -v.z);
}

static inline f32 Clamp(f32 v, f32 lo, f32 hi) {
    return v <= lo ? lo : (!(v >= hi) ? v : hi);
}

static inline f32 Lerp(f32 a, f32 b, f32 t) {
    if (t >= 1.0f)
        return b;
    if (t < 0.0f)
        return a;
    return t * b + (1.0f - t) * a;
}

void fn_71004d9b88(UnkCoreXAI_l4d9b88* self, f32 dt) {
    const f32 kQuarterPi = 0.78539819f;
    const f32 kPi = 3.14159274f;
    UnkVec3f* aim = &self->aim;
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));

    if (self->link && self->link->actor) {
        UnkActor* actor = self->link->actor;
        UnkVec3f pos(actor->field_0x44, actor->field_0x54, actor->field_0x64);
        void* ai = actor->field_0x240;
        if (ai) {
            UnkEntity* shape = static_cast<UnkEntity*>(actor->field_0x228);
            bool named = fn_710110f224(ai)->data != &g_unk_7101d47108->empty;
            if ((shape != nullptr) & named) {
                UnkMatrix44 m = {};
                m.m[15] = 1.0f;
                m.m[10] = 1.0f;
                m.m[5] = 1.0f;
                m.m[0] = 1.0f;
                bool found;
                {
                    u64 key = fn_710110f224(ai)->data->Key();
                    found = fn_71011735e8(shape, &key, &m, 0);
                }
                if (found) {
                    f32 x = m.m[3];
                    f32 y = m.m[7];
                    f32 z = m.m[11];
                    // The node's offset turned by the node's rotation.
                    UnkVec3f offset = TransformPointByMatrixWithPerspectiveDivide(
                        reinterpret_cast<const UnkMat4f*>(AddressOf(CopyMatrixWithoutTranslation(reinterpret_cast<const UnkMatrix44Words*>(&m)))),
                        fn_710110f230(ai));
                    pos.x = x + offset.x;
                    pos.y = y + offset.y;
                    pos.z = z + offset.z;
                }
            }
        }
        UnkCoreXOwner_l4d9b88* owner = self->owner;
        self->followed = pos;
        owner->transform.SetTranslation(pos);
        owner->moved = true;
    }

    // The direction from the target, in the variable that first takes fn_7100d7be08's result.
    UnkKraidCreatureVec dir = fn_7100d7be08(self->owner);
    UnkCoreXOwner_l4d9b88* owner = self->owner;
    f32 x = owner->transform.m[0][3];
    f32 y = owner->transform.m[1][3];
    f32 z = owner->transform.m[2][3];
    UnkActor* target = fn_7100d7c990(reinterpret_cast<UnkActor*>(owner));
    dir.x = x - target->field_0x44;
    dir.y = y - target->field_0x54;
    dir.z = z - target->field_0x64;
    Normalize(dir);

    f32 cos_angle = Dot(dir, *aim);
    if (cos_angle < 0.999f) {
        // Turn by the angle, but by no more than the turn speed: a quarter turn per second when
        // almost facing the target, up to half a turn per second from a quarter turn off.
        f32 angle = acosf(Clamp(cos_angle, -1.0f, 1.0f));
        f32 step = Lerp(kQuarterPi, kPi, Clamp(angle / kQuarterPi, 0.0f, 1.0f)) * dt;
        f32 turn = angle > step ? step : angle;
        const UnkVec3f up(0.0f, 0.0f, 1.0f);
        if (Dot(Cross(*aim, Negated(up)), dir) < 0.0f)
            turn = -turn;
        *aim = RotateVec3AboutZ(*aim, turn);
        Normalize(*aim);
    } else {
        *aim = UnkVec3f(dir.x, dir.y, dir.z);
    }

    const UnkVec3f up(0.0f, 0.0f, 1.0f);
    UnkVec3f side = Cross(up, *aim);
    *aim = self->owner->body->axis;
    Normalize(*aim);
    UnkMatrix44 frame;
    fn_71000769f0(&frame, aim, &side, g_unk_7101649bc0, g_unk_7101649bcc);
}
