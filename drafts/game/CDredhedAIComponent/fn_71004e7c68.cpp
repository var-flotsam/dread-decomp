// CDredhedAIComponent's unit (tools/tu_map.py 775): after the base speeds, with a target that is not on a
// magnet-spider pose track and is detected, casts a segment 800 units along the target's up axis; when it hits
// close enough below the target inside the owner's AI area, the second speed becomes 210. Without a target the
// two speeds become 260 and 300 when the owner's attack object says so. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAttackComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkTree;
struct UnkBox;
struct UnkUnit108List_A1_b73;
struct UnkUnit108Point_A1_b73;
bool fn_7100118b1c(const UnkUnit108List_A1_b73* self, const UnkUnit108Point_A1_b73* p, f32 r);

// A counted reference, released when the count drops to zero.
struct UnkDredhedHitRef_l4e7c68 {
    base::UnkRefCounted* ptr;

    UnkDredhedHitRef_l4e7c68() : ptr(nullptr) {}
    ~UnkDredhedHitRef_l4e7c68() {
        if (base::UnkRefCounted* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
};

struct UnkDredhedHitVec_l4e7c68 {
    f32 x, y, z;

    UnkDredhedHitVec_l4e7c68(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

// A collision hit: the object hit and a second reference, an id, the point, two vectors and flags.
struct UnkDredhedHit_l4e7c68 {
    UnkDredhedHitRef_l4e7c68 object;
    UnkDredhedHitRef_l4e7c68 field_0x8;
    s32 id;
    UnkDredhedHitVec_l4e7c68 point;
    UnkDredhedHitVec_l4e7c68 field_0x20;
    UnkDredhedHitVec_l4e7c68 normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkDredhedHit_l4e7c68()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
};

// The segment query's callback object: the closest hit so far and the segment's origin.
struct UnkDredhedQuery_l4e7c68 {
    void** vtable;
    UnkDredhedHit_l4e7c68 hit;
    bool field_0x50;
    u32 field_0x54;
    CVector3D origin;
    u64 field_0x68;

    explicit UnkDredhedQuery_l4e7c68(const CVector3D& o) {
        vtable = g_unk_7101a20fa8 + 2;
        field_0x50 = false;
        origin.x = o.x;
        origin.y = o.y;
        origin.z = o.z;
        field_0x68 = 0;
    }
    ~UnkDredhedQuery_l4e7c68() { vtable = g_unk_7101a20ed8 + 2; }
};

// The segment fn_7100d5c518 tests: its two ends and the vector between them.
struct UnkDredhedSegment_l4e7c68 {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkDredhedSegment_l4e7c68(const CVector2D& s, const CVector2D& e) {
        start.x = s.x;
        start.y = s.y;
        end.x = e.x;
        end.y = e.y;
        delta.x = e.x - s.x;
        delta.y = e.y - s.y;
    }
};

// The owner's AI component: the area it keeps (+0x68) and its state.
struct UnkDredhedOwnerAI_l4e7c68 {
    u8 pad_0x0[0x68];
    u8 area[8];   // 0x68
    u32 state;    // 0x70
    u8 pad_0x74[0x20];
    u8 engaged;   // 0x94

    // Whether the state is where it rests: 0, or 1 while not engaged.
    bool AtRest() const { return state == static_cast<u32>((state != 0) & !engaged); }
};

struct UnkDredhedOwner_l4e7c68 {
    u8 pad_0x0[0x240];
    UnkDredhedOwnerAI_l4e7c68* ai;  // 0x240
    u8 pad_0x248[0x30];
    void* attack;  // 0x278
    u8 pad_0x280[0xfc];
    f32 radius;    // 0x37c
};

struct UnkDredhedSpeeds_l4e7c68 {
    f32 field_0x0;
    f32 field_0x4;
};

struct UnkDredhedAI_l4e7c68 {
    u8 pad_0x0[0x30];
    UnkDredhedOwner_l4e7c68* owner;  // 0x30
};

void fn_7101106ae8(UnkDredhedAI_l4e7c68* self, UnkDredhedSpeeds_l4e7c68* speeds);

static inline UnkActor* Target(UnkDredhedAI_l4e7c68* self) {
    return fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner));
}

void fn_71004e7c68(UnkDredhedAI_l4e7c68* self, UnkDredhedSpeeds_l4e7c68* speeds) {
    fn_7101106ae8(self, speeds);
    if (UnkActor* target = Target(self)) {
        if (void* anim = target->field_0x220) {
            if (fn_710117b348(static_cast<UnkAnimationComponent*>(anim), fn_7100c3bf3c()))
                return;
        }
        if (target->field_0x208 && fn_71009e12d0(target->field_0x208)) {
            CVector2D start = {target->field_0x44, target->field_0x54};
            const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(Target(self)->field_0x38));
            CVector2D end = {start.x - m.m[1] * 800.0f, start.y - m.m[5] * 800.0f};
            UnkDredhedSegment_l4e7c68 segment(start, end);
            CVector3D origin = {target->field_0x44, target->field_0x54, target->field_0x64};
            UnkDredhedQuery_l4e7c68 query(origin);
            UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
            if (fn_7100d5c518(tree, 0x80409, reinterpret_cast<const UnkBox*>(&segment), &query, 2)) {
                CVector3D point;
                point.x = query.hit.point.x;
                point.y = query.hit.point.y;
                point.z = query.hit.point.z;
                UnkDredhedOwnerAI_l4e7c68* ai = self->owner->ai;
                if (!ai->AtRest()) {
                    f32 depth = start.y - point.y;
                    CVector2D flat;
                    fn_710007b430(&flat, &point);
                    bool inside = fn_7100118b1c(reinterpret_cast<const UnkUnit108List_A1_b73*>(ai->area),
                                                reinterpret_cast<const UnkUnit108Point_A1_b73*>(&flat),
                                                self->owner->radius);
                    if (depth <= 300.0f && inside)
                        speeds->field_0x4 = 210.0f;
                }
            }
            return;
        }
    }
    if (self->owner->attack && fn_7101136308(self->owner->attack)) {
        speeds->field_0x0 = 260.0f;
        speeds->field_0x4 = 300.0f;
    }
}
