// CHangableGrappleSurfaceComponent unit (tools/tu_map.py, unit 1283): whether the actor may hang on the
// surface for a grapple message. When the actor's movement (+0x208) is in the state fn_71009e6434 tests, the
// actor must be at least 200 units from the message's point; otherwise the movement's state of the kind
// fn_71009a60fc names must accept the point (the one fn_7100825258 works out when the message comes from the
// definition's grapple point) with a fresh request (fn_71009a64cc). Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CStrId.h"
#include "game/CSamusMagnetGloveMovementState/CParams.h"
#include "game/UnkActor.h"
#include "game/UnkTU1501.h"
#include "types.h"

using base::global::UnkStringData;

// A point copied member by member (a user-written copy: fn_7100825258 returns it in memory).
struct UnkHangPoint_b50 {
    f32 x, y, z;

    UnkHangPoint_b50(const UnkHangPoint_b50& other) : x(other.x), y(other.y), z(other.z) {}
    UnkHangPoint_b50& operator=(const UnkHangPoint_b50& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
};

struct UnkHangable_b50;
struct UnkHangMessage_b50;
UnkHangPoint_b50 fn_7100825258(UnkHangable_b50* self, UnkHangMessage_b50* msg);

struct UnkAirRequest_B71a6 {
    void* vtable;
    u8 pad_0x8[0x80 - 0x8];
    base::UnkRefPtrMember ref_0x80;
    base::UnkRefPtrMember ref_0x88;
    u8 pad_0x90[0xa0 - 0x90];
    u64 value;

    UnkAirRequest_B71a6();
    ~UnkAirRequest_B71a6() {
        vtable = reinterpret_cast<char*>(g_unk_7101a874f0) + 0x10;
    }
};

struct UnkAirMoveState_B71a6;
struct UnkAirWords_B71a6;
void fn_71009a609c(UnkAirRequest_B71a6* self);
inline UnkAirRequest_B71a6::UnkAirRequest_B71a6() { fn_71009a609c(this); }
bool fn_71009a64cc(UnkAirMoveState_B71a6* state, const UnkAirWords_B71a6* words, s32 a, UnkAirRequest_B71a6* req,
                   s32 b);

struct UnkGrappleSource_l825098 {
    u8 pad_0x0[0x20];
    UnkStringData* field_0x20;
};

struct UnkGrappleSender_l825098 {
    u8 pad_0x0[0x10];
    UnkGrappleSource_l825098* field_0x10;
};

struct UnkGrappleMessage_l825098 {
    UnkGrappleSender_l825098* sender;
    u8 pad_0x8[0xc];
    UnkHangPoint_b50 point;  // 0x14
};

struct UnkGrappleDef_l825098 {
    u8 pad_0x0[0x70];
    UnkStringData* field_0x70;  // CStrId
};

struct UnkHangSurface_l825098 {
    u8 pad_0x0[0x94];
    bool enabled;  // 0x94
    UnkGrappleDef_l825098* def;  // 0x98
};

struct UnkMoveState_l825098 {
    void* vtable;
    u8 pad_0x8[0x8];
    s32 kind;  // 0x10
};

#pragma pack(push, 4)
struct UnkMovement_l825098 {
    u8 pad_0x0[0x2d18];
    u32 state_count;
    UnkMoveState_l825098* states[1];  // 0x2d1c, 4-byte aligned
};
#pragma pack(pop)

static inline UnkMoveState_l825098* FindState(UnkMovement_l825098* self, s32 kind) {
    u64 size = (u64)self->state_count * sizeof(UnkMoveState_l825098*);
    if (size == 0)
        return nullptr;
    UnkMoveState_l825098** it = self->states;
    for (; size != 0; ++it, size -= sizeof(*it)) {
        if ((*it)->kind == kind)
            return *it;
    }
    return nullptr;
}

static inline f32 Square(f32 v) { return v * v; }

bool fn_7100825098(UnkHangSurface_l825098* self, UnkActor* actor, UnkGrappleMessage_l825098* msg) {
    if (!self->enabled)
        return false;
    if (UnkMovement_l825098* movement = static_cast<UnkMovement_l825098*>(actor->field_0x208)) {
        if (fn_71009e6434(reinterpret_cast<UnkHelper*>(movement))) {
            f32 distance_sq = Square(actor->field_0x44 - msg->point.x) + Square(actor->field_0x54 - msg->point.y) +
                              Square(actor->field_0x64 - msg->point.z);
            return distance_sq >= 40000.0f;
        }
        u32 kind = fn_71009a60fc();
        UnkMoveState_l825098* state = kind <= 14 ? FindState(movement, kind) : nullptr;
        if (state) {
            UnkHangPoint_b50 point = msg->point;
            if (msg->sender->field_0x10->field_0x20 == self->def->field_0x70)
                point = fn_7100825258(reinterpret_cast<UnkHangable_b50*>(self),
                                      reinterpret_cast<UnkHangMessage_b50*>(msg));
            UnkAirRequest_B71a6 req;
            if (!fn_71009a64cc(reinterpret_cast<UnkAirMoveState_B71a6*>(state),
                               reinterpret_cast<const UnkAirWords_B71a6*>(&point), 0, &req, 1))
                return false;
        }
    }
    return true;
}
