// CAttackComponent unit (tools/tu_map.py): picks the damage source of a hit. A frozen owner gives none;
// otherwise a playing modify-damage track whose sources include the hit's collider sets it, and the
// owner's source lookup (virtual, at +0x240) may override it. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CCharClassAttackComponent.h"
#include "game/CCollisionComponent.h"
#include "game/CFrozenComponent.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::global::CStrId;

// What the modify-damage track resolves: the colliders it applies to and the source it sets.
struct UnkModifyDamageRecord {
    u8 pad_0x0[0x20];
    CStrId* colliders;
    u32 collider_count;
    u8 pad_0x2c[0x1c];
    CStrId source;
    u32 field_0x50;
};
static_assert(__builtin_offsetof(UnkModifyDamageRecord, source) == 0x48, "layout");

// The owner's lookup of a source by name (vtable slot 0x648).
struct UnkSourceLookup_l1368a0 {
    void** vtable;

    CStrId Lookup(const CStrId& name) {
        return reinterpret_cast<CStrId (*)(UnkSourceLookup_l1368a0*, CStrId)>(vtable[0x648 / 8])(this, name);
    }
};

struct UnkAttackOwner_l1368a0 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
    u8 pad_0x228[0x18];
    UnkSourceLookup_l1368a0* sources;
    u8 pad_0x248[0x58];
    UnkCollisionContainer* collision;
    u8 pad_0x2a8[0x20];
    UnkFrostbiteComponent* frozen;
};
static_assert(__builtin_offsetof(UnkAttackOwner_l1368a0, sources) == 0x240, "layout");
static_assert(__builtin_offsetof(UnkAttackOwner_l1368a0, collision) == 0x2a0, "layout");
static_assert(__builtin_offsetof(UnkAttackOwner_l1368a0, frozen) == 0x2c8, "layout");

struct UnkAttackDef_l1368a0 {
    u8 pad_0x0[0x58];
    CStrId default_source;
};

struct UnkAttackComponent_l1368a0 {
    u8 pad_0x0[0x30];
    UnkAttackOwner_l1368a0* owner;
    u8 pad_0x38[0xf8];
    UnkAttackDef_l1368a0* def;
};
static_assert(__builtin_offsetof(UnkAttackComponent_l1368a0, def) == 0x130, "layout");

struct UnkHitPart_l1368a0 {
    u8 pad_0x0[0x10];
    void* object;
};

// The hit: what was hit and what hit it.
struct UnkHit_l1368a0 {
    UnkHitPart_l1368a0* target;
    UnkHitPart_l1368a0* collider;
};

struct UnkDamageSource_l1368a0 {
    CStrId source;
    u32 field_0x8;
    f32 field_0xc;
};

static inline bool IsEmpty(const CStrId& id) {
    return id.data == &g_unk_7101d47108->empty;
}

// The record the query resolved, as this track's.
inline UnkModifyDamageRecord* Record(const UnkTrackQuery& query) {
    return reinterpret_cast<UnkModifyDamageRecord*>(query.result);
}

inline bool HitMatches(UnkAttackComponent_l1368a0* self, UnkHit_l1368a0* hit, const UnkTrackQuery& query) {
    if (!hit->target || !hit->target->object)
        return false;
    void* object = hit->collider ? hit->collider->object : nullptr;
    UnkCollisionContainer* collision = self->owner->collision;
    if (!collision)
        return false;
    CStrId* name = fn_7100864018(collision, object);
    UnkModifyDamageRecord* record = Record(query);
    bool found = false;
    for (const CStrId* it = record->colliders, *end = it + record->collider_count; it != end; ++it) {
        if (*it == *name) {
            found = true;
            break;
        }
    }
    return found;
}

bool fn_71011368a0(UnkAttackComponent_l1368a0* self, UnkHit_l1368a0* hit, UnkDamageSource_l1368a0* out) {
    if (UnkFrostbiteComponent* frozen = self->owner->frozen) {
        if (fn_71007f9f20(frozen) && !fn_71007f9fd0(frozen)) {
            out->field_0x8 = 0;
            return true;
        }
    }
    bool applied = false;
    UnkTrackQuery query = {fn_7100d217d8(), nullptr, -1.0f};
    if (UnkAnimationComponent* animation = self->owner->animation) {
        fn_710117b08c(animation, &query);
        if (Record(query)) {
            applied = !hit || Record(query)->collider_count == 0 || HitMatches(self, hit, query);
            if (applied) {
                if (!IsEmpty(Record(query)->source)) {
                    out->source = Record(query)->source;
                    out->field_0xc = fn_7100789f5c(&Record(query)->source);
                }
                out->field_0x8 = Record(query)->field_0x50;
            }
        }
    }
    if (UnkSourceLookup_l1368a0* sources = self->owner->sources) {
        CStrId found = sources->Lookup(IsEmpty(out->source) ? self->def->default_source : out->source);
        if (!IsEmpty(found)) {
            out->source = found;
            out->field_0xc = fn_7100789f5c(&found);
            applied = true;
        }
    }
    return applied;
}
