// CAbilitySpeedBooster's unit (tools/tu_map.py unit 435): checks the contacts the owner has this frame
// against the boost. While the boost state is 1, a contact facing mostly along the boost direction and
// below the ground line that is not a plain collision ends the boost (fn_710033c350). Otherwise each
// contact is tested by the boost's mode (2 or 3, the direction flag at +0x3a0): tile groups and frozen
// collisions are skipped, a facing contact may stop the scan, the others ask the owner's movement for a
// bump or a turn animation, and in the end the boost is ended once the actor has lost enough height
// while a hold object is missing. Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilitySpeedBooster.h"
#include "game/CAnimationComponent.h"
#include "game/CBreakableTileGroup.h"
#include "game/CFrozenComponent.h"
#include "game/CMovementComponent.h"
#include "types.h"

using base::reflection::CClass;
using base::reflection::UnkReflObject;

struct UnkLinked_l0dcab0;
UnkFrostbiteComponent* fn_710032bc90(UnkLinked_l0dcab0* linked);
void fn_710033c350(struct UnkSpeedBoosterAbility_ovb0088* self);
bool fn_71009ecd58(struct UnkMoverGround_l33d6e8* mover);
struct UnkTrackMovement_ovb0021;
struct UnkTrackParams_l33d6e8 {
    u64 raw[6];
};
UnkTrackParams_l33d6e8 fn_71009767f4(UnkTrackMovement_ovb0021* self, bool forced);
u32 fn_710033dc00(void* component);
extern const UnkMovementVec_71010d08d4 g_unk_7101649038 __attribute__((visibility("hidden")));

// A collision object a contact refers to: its link at +0x18 and component at +0x30.
struct UnkContactObject_l33d6e8 : UnkReflObject {
    u8 pad_0x8[0x10];
    UnkLinked_l0dcab0* link;  // 0x18
    u8 pad_0x20[0x10];
    void* component;  // 0x30
};

struct UnkContactInner_l33d6e8 {
    u8 pad_0x0[0x10];
    UnkContactObject_l33d6e8* object;  // 0x10
};

struct UnkContactHolder_l33d6e8 {
    u8 pad_0x0[0x10];
    UnkContactInner_l33d6e8* inner;  // 0x10
};

#pragma pack(push, 4)
struct UnkContact_l33d6e8 {
    UnkContactHolder_l33d6e8* holder;  // 0x0
    u8 pad_0x8[0xc];
    u8 query[0xc];  // 0x14
    f32 px, py, pz;  // 0x20
    f32 nx, ny, nz;  // 0x2c
    u8 pad_0x38[0x10];
};
#pragma pack(pop)
static_assert(sizeof(UnkContact_l33d6e8) == 0x48, "contact layout");

struct UnkContactList_l33d6e8 {
    u8 pad_0x0[0x58];
    u32 count;  // 0x58
    UnkContact_l33d6e8 contacts[1];  // 0x5c
};

struct UnkBoostState_l33d6e8 {
    u8 pad_0x0[0x10];
    s32 mode;  // 0x10
};

struct UnkMoverGround_l33d6e8 {
    u8 pad_0x0[0xf9];
    bool field_0xf9;
    u8 pad_0xfa[0x1e];
    f32 field_0x118;
    u8 pad_0x11c[0x4094];
    bool blocked;  // 0x51b0
};

struct UnkHoldObject_l33d6e8 {
    u8 pad_0x0[0xf9];
    bool field_0xf9;
};

struct UnkBoostActor_l33d6e8 {
    u8 pad_0x0[0x54];
    f32 field_0x54;
    u8 pad_0x58[0x198];
    UnkHoldObject_l33d6e8* hold;  // 0x1f0
    u8 pad_0x1f8[0x10];
    UnkMoverGround_l33d6e8* mover;  // 0x208
    u8 pad_0x210[0x10];
    void* animation;  // 0x220
};

struct UnkBoostOwner_l33d6e8 {
    u8 pad_0x0[0x30];
    UnkBoostActor_l33d6e8* actor;  // 0x30
};

struct UnkSpeedBoosterContacts_l33d6e8 {
    u8 pad_0x0[0xc0];
    UnkBoostOwner_l33d6e8* owner;  // 0xc0
    u8 pad_0xc8[0x2d8];
    bool facing_back;  // 0x3a0
    u8 pad_0x3a1[2];
    bool tracking;  // 0x3a3
    f32 start_y;  // 0x3a4
    u8 pad_0x3a8[0x10];
    UnkBoostState_l33d6e8* boost;  // 0x3b8
};
static_assert(__builtin_offsetof(UnkSpeedBoosterContacts_l33d6e8, boost) == 0x3b8, "layout");

struct UnkSpeedBoosterTunable_l33d6e8 {
    u8 pad_0x0[0x3c];
    f32 field_0x3c;
};

static inline bool IsA(UnkReflObject* object, const CClass* cls) {
    return fn_710009d50c(object->vf_0x0(), cls);
}

// How much the contact's normal points along the boost direction.
static inline f32 Along(const UnkContact_l33d6e8& c) {
    return c.nx * 1.0f + c.ny * 0.0f + c.nz * 0.0f;
}

// How far the contact's point is below the actor's line.
static inline f32 Below(const UnkContact_l33d6e8& c) {
    return c.px * 0.0f - c.py + c.pz * 0.0f;
}

static inline UnkContactObject_l33d6e8* ObjectOf(const UnkContact_l33d6e8& c) {
    return c.holder->inner->object;
}

// The boost ends once the actor has dropped further than the tunable's allowance (or at once when nothing holds it).
static void EndOrTrack(UnkSpeedBoosterContacts_l33d6e8* self, UnkBoostActor_l33d6e8* actor, UnkMoverGround_l33d6e8* mover) {
    UnkHoldObject_l33d6e8* hold = actor->hold;
    if (!hold || hold->field_0xf9) {
        f32 allowance = reinterpret_cast<const UnkSpeedBoosterTunable_l33d6e8*>(&g_unk_71726b0fc0)->field_0x3c;
        if (allowance > 0.0f) {
            if (!self->tracking) {
                self->tracking = true;
                self->start_y = actor->field_0x54;
                return;
            }
            f32 y = actor->field_0x54;
            f32 start = self->start_y;
            if (y >= start)
                return;
            if (start - y > allowance)
                fn_710033cd50(reinterpret_cast<UnkUnit435Self*>(self), 1);
            return;
        }
    }
    if (mover && fn_71009ecd58(mover))
        return;
    fn_710033cd50(reinterpret_cast<UnkUnit435Self*>(self), 1);
}

void fn_710033d6e8(UnkSpeedBoosterContacts_l33d6e8* self, UnkContactList_l33d6e8* list) {
    s32 mode = 4;
    if (self->boost) {
        mode = self->boost->mode;
        if (mode == 1) {
            for (const UnkContact_l33d6e8 *it = list->contacts, *end = it + list->count; it != end; ++it) {
                const UnkContact_l33d6e8& c = *it;
                f32 along = Along(c);
                if (along < -0.9f || along > 0.9f) {
                    if (Below(c) < 0.99f && c.holder && c.holder->inner && ObjectOf(c)) {
                        UnkContactObject_l33d6e8* object = ObjectOf(c);
                        if (!IsA(object, fn_710085fc44()) || !object->component || !fn_710033dc00(object->component))
                            fn_710033c350(reinterpret_cast<UnkSpeedBoosterAbility_ovb0088*>(self));
                    }
                }
            }
            return;
        }
    }
    UnkBoostActor_l33d6e8* actor = self->owner->actor;
    UnkMoverGround_l33d6e8* mover = actor->mover;
    if (mover && mover->blocked)
        return;
    if (list->count == 0) {
        self->tracking = false;
        return;
    }
    bool no_mover = mover == nullptr;
    bool any = false;
    bool decided = false;  // the scan stopped with an answer: `end_boost`
    bool end_boost = false;
    for (const UnkContact_l33d6e8 *it = list->contacts, *end = it + list->count; it != end; ++it) {
        const UnkContact_l33d6e8& c = *it;
        UnkContactObject_l33d6e8* object = ObjectOf(c);
        if (object && IsA(object, fn_7100ca0484()) && !fn_7100ca2048(object, const_cast<u8*>(c.query), 1))
            continue;
        object = ObjectOf(c);
        if (object && IsA(object, fn_710085fc44())) {
            if (object->link) {
                if (UnkFrostbiteComponent* frostbite = fn_710032bc90(object->link)) {
                    if (fn_71007f9f20(frostbite) && !fn_71007f9fd0(frostbite))
                        continue;
                }
            }
        }
        f32 along = Along(c);
        if (mode == 2) {
            bool facing = self->facing_back ? along < -0.85f : along > 0.85f;
            if (facing) {
                if (Below(c) >= 0.99f)
                    break;
                decided = true;
                end_boost = true;
                if (c.holder && c.holder->inner && ObjectOf(c)) {
                    UnkContactObject_l33d6e8* hit = ObjectOf(c);
                    if (IsA(hit, fn_710085fc44()) && hit->component) {
                        bool loose = fn_710033dc00(hit->component) == 0;
                        end_boost = any || loose;
                    }
                }
                break;
            }
            if (!mover)
                continue;
            if (fn_71010d1598(reinterpret_cast<UnkGroundPlayer_b52*>(mover))) {
                f32 gauge = mover->field_0x118;
                if (!(gauge + 0.001f < 0.0f) && gauge - 0.001f <= 0.0f) {
                    UnkTrackParams_l33d6e8 request = fn_71009767f4(reinterpret_cast<UnkTrackMovement_ovb0021*>(mover), true);
                    fn_7101176ea8(reinterpret_cast<UnkStruct_7101176ea8*>(actor->animation),
                                  reinterpret_cast<const UnkRequest_7101176ea8*>(&request), 0xf4061ff25a2163d6ull);
                    decided = true;
                    end_boost = any;
                    break;
                }
            }
            if (!mover->field_0xf9)
                continue;
        } else if (mode != 3) {
            continue;
        }
        bool near = along - 0.001f <= 0.0f && along + 0.001f >= 0.0f;
        if (self->facing_back) {
            if (along >= 0.0f || near)
                continue;
            if (mover)
                fn_71010d08d4(reinterpret_cast<UnkMovement_71010d08d4*>(mover), &g_unk_7101649038, true);
            decided = true;
            end_boost = true;
            break;
        }
        bool settled = along <= 0.0f || near;
        if (!(settled || no_mover)) {
            fn_71010d08d4(reinterpret_cast<UnkMovement_71010d08d4*>(mover), &g_unk_7101649038, true);
            decided = true;
            end_boost = true;
            break;
        }
        any |= !settled;
        if (settled)
            continue;
        break;
    }
    if (!decided)
        end_boost = any;
    if (!end_boost) {
        self->tracking = false;
        return;
    }
    EndOrTrack(self, actor, mover);
}
