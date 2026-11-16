// CGrappleBeamComponent unit (1277): classifies what a grapple hit struck. The hit's object (+0x10 of +0x10 of
// its first word) must be a collision component whose owner (+0x30) is a swingable grapple point; the result is
// that point with a kind, 2 when the id the collision container maps the hit's ref to has the same name as the
// table's id at +0x2288, otherwise 1; both zero when it is no swingable point. Names and layouts are
// placeholders.
#include "base/UnkNamedRegistry.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCollisionComponent.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::reflection::UnkReflObject;

UnkReflObject* fn_710034dd54(void* object);

struct UnkTargetRefB_l81bb50 {
    u8 pad_0x0[0x10];
    UnkReflObject* object;  // 0x10
};
struct UnkTargetRefA_l81bb50 {
    u8 pad_0x0[0x10];
    UnkTargetRefB_l81bb50* ref;  // 0x10
};
struct UnkTargetHit_l81bb50 {
    UnkTargetRefA_l81bb50* ref;
};
struct UnkTargetCollider_l81bb50 : UnkReflObject {
    u8 pad_0x8[0x28];
    void* owner;  // 0x30
};

// The pooled string of an id (a CRntString at +0x10), counting the read as UnkStrIdBase::Text does.
static inline const base::UnkNamedEntry* Key_l81bb50(const base::global::CStrId& id) {
    ++id.data->reads;
    return reinterpret_cast<const base::UnkNamedEntry*>(&id.data->text);
}

struct UnkTargetKey_l81bb50 {
    base::UnkNamedEntry entry;
    explicit UnkTargetKey_l81bb50(const base::global::CStrId& id) { fn_710007ea60(&entry, Key_l81bb50(id)); }
    ~UnkTargetKey_l81bb50() { fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(&entry)); }
};

struct UnkGrappleTarget_l81bb50 {
    u64 kind;
    UnkReflObject* point;
};

UnkGrappleTarget_l81bb50 fn_710081bb50(UnkTargetHit_l81bb50* hit) {
    UnkTargetCollider_l81bb50* collider = static_cast<UnkTargetCollider_l81bb50*>(hit->ref->ref->object);
    if (collider && fn_710009d50c(collider->vf_0x0(), fn_710085fc44())) {
        UnkReflObject* point = fn_710034dd54(collider->owner);
        if (point && fn_710009d50c(point->vf_0x0(), fn_710082f310())) {
            UnkTargetRefB_l81bb50* ref = hit->ref ? hit->ref->ref : nullptr;
            bool same;
            {
                UnkTargetKey_l81bb50 key(*fn_7100864018(reinterpret_cast<UnkCollisionContainer*>(collider), ref));
                const base::global::CStrId& expected = *reinterpret_cast<const base::global::CStrId*>(
                    reinterpret_cast<u8*>(&GameManagerIdTable()) + 0x2288);
                same = fn_710007f120(reinterpret_cast<const UnkCompared_0x30*>(&key.entry),
                                     reinterpret_cast<const UnkCompared_0x30*>(Key_l81bb50(expected)));
            }
            return {same ? 2u : 1u, point};
        }
    }
    return {0, nullptr};
}
