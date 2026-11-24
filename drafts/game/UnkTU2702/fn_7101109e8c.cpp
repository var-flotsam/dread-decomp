// Unit 2702 (CAIComponent?): the navigation-mesh side of the owner's target, by the target's kind
// (a path segment, a remembered hit, or the owner's own position). False when there is none.
// Names and layouts are placeholders.
#include "base/UnkEmptyDtorElem.h"
#include "base/math/CVector.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/UnkNavMeshHit.h"
#include "game/UnkObject_0x100.h"
#include "game/UnkPath442.h"
#include "game/UnkTU362.h"
#include "game/UnkTU376.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

void fn_71002874bc(void* self);
// Constructs a hit from a cell, a side and a point.
void fn_71002875d8(UnkNavMeshHit* self, u16 cell, u8 side, const CVector2D* at);

// A hit's point, copied component by component.
struct UnkAITargetPoint {
    f32 x;
    f32 y;

    UnkAITargetPoint& operator=(const UnkAITargetPoint& other) {
        x = other.x;
        y = other.y;
        return *this;
    }
};

// A navigation-mesh hit as this unit copies it: the cell and side together, then the point.
struct UnkAITargetHit {
    u16 cell;
    u8 side;
    UnkAITargetPoint point;

    UnkAITargetHit(u16 cell_, u8 side_, const CVector2D* at) {
        fn_71002875d8(reinterpret_cast<UnkNavMeshHit*>(this), cell_, side_, at);
    }
    UnkAITargetHit(const u32* cell_side, const CVector2D* at) {
        fn_71002875bc(reinterpret_cast<UnkNavMeshHit*>(this), cell_side, at);
    }
    ~UnkAITargetHit() { fn_710028760c(reinterpret_cast<UnkElem_7100286624*>(this)); }
};

// A packed cell and side.
struct UnkAITargetCell {
    u16 cell;
    u8 side;
};

// A 4-byte packed cell and side, built and destroyed out of line.
struct UnkAITargetKey {
    u32 value;
    UnkAITargetKey() { fn_71002874bc(this); }
    ~UnkAITargetKey() { fn_71002874dc(this); }
};

// The remembered target record (0x100 bytes).
struct UnkAITargetRecord {
    u8 pad_0x0[0x2];
    s16 field_0x2;
    u8 pad_0x4[0x18];
    u8 side;  // 0x1c
    u8 pad_0x1d;
    u16 cell;  // 0x1e
    CVector2D point;  // 0x20
    u8 pad_0x28[0xd8];

    UnkAITargetRecord() { fn_71002b57bc(reinterpret_cast<UnkObject_0x100*>(this)); }
    ~UnkAITargetRecord() { fn_71002b6094(reinterpret_cast<UnkObject_0x100*>(this)); }
    UnkAITargetRecord& operator=(const UnkAITargetRecord& other) {
        fn_71002b5c90(reinterpret_cast<UnkObject_0x100*>(this), reinterpret_cast<const UnkObject_0x100*>(&other));
        return *this;
    }
};
static_assert(sizeof(UnkAITargetRecord) == 0x100, "UnkAITargetRecord layout");

struct UnkAITarget {
    u8 pad_0x0[0x974];
    CVector2D point;  // 0x974
    u8 pad_0x97c[0x8];
    u32 segment;  // 0x984
    u8 pad_0x988[0xee8];
    UnkAITargetRecord record;  // 0x1870
    bool has_record;  // 0x1970
};
static_assert(__builtin_offsetof(UnkAITarget, record) == 0x1870, "UnkAITarget layout");
static_assert(__builtin_offsetof(UnkAITarget, has_record) == 0x1970, "UnkAITarget layout");

struct UnkAITargetComponent {
    u8 pad_0x0[0x68];
    UnkPath442 path;  // 0x68
};

struct UnkAITargetSelf {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
};

static inline UnkAITarget* Target(UnkAITargetSelf* self) {
    return static_cast<UnkAITarget*>(self->owner->field_0x218);
}

static inline UnkAITargetHit& AsHit(UnkNavMeshHit* hit) {
    return *reinterpret_cast<UnkAITargetHit*>(hit);
}

struct UnkAIComponent;

bool fn_7101109e8c(UnkAIComponent* component, UnkNavMeshHit* side) {
    UnkAITargetSelf* self = reinterpret_cast<UnkAITargetSelf*>(component);
    side->cell = 0xffff;
    side->side = 0xff;
    side->point.x = 0.0f;
    side->point.y = 0.0f;

    UnkAITarget* target = Target(self);
    if (target && fn_71010cc3a4(target) == 1) {
        UnkAITargetComponent* ai = static_cast<UnkAITargetComponent*>(self->owner->field_0x240);
        const UnkAITargetCell* cell =
            reinterpret_cast<const UnkAITargetCell*>(GetPathSegmentEntryOrFallback(&ai->path, target->segment));
        CVector2D at;
        at.x = target->point.x;
        at.y = target->point.y;
        UnkAITargetHit hit(cell->cell, cell->side, &at);
        AsHit(side) = hit;
        return true;
    }

    target = Target(self);
    if (target && fn_71010cc3a4(target) == 4) {
        UnkAITargetRecord record;
        if (target->has_record) {
            record = target->record;
            if (target->has_record && record.field_0x2 != -1 && record.cell != 0xffff) {
                CVector2D at;
                at.x = record.point.x;
                at.y = record.point.y;
                UnkAITargetHit hit(record.cell, record.side, &at);
                AsHit(side) = hit;
                return true;
            }
        }
    }

    target = Target(self);
    if (!target || fn_71010cc3a4(target) != 3)
        return false;
    UnkAITargetKey key;
    CVector3D pos;
    pos.x = self->owner->field_0x44;
    pos.y = self->owner->field_0x54;
    pos.z = self->owner->field_0x64;
    CVector2D at;
    fn_710007b430(&at, &pos);
    UnkAITargetHit hit(&key.value, &at);
    AsHit(side) = hit;
    return true;
}
