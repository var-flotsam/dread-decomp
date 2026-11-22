// CCharClassLavaPoolComponent's unit: an actor enters the lava pool. After the liquid pool's handler
// (fn_7100901dd4), when the game manager accepts the actor (fn_7100df5c38) its player info (+0x38) is told
// the id made of the pool owner's resource name and the actor's, then given the owner's char class (its
// +0x98) and both resources. Names and layouts are placeholders.
#include "base/UnkTunableLine.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CPlayerInfo.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;
using base::reflection::UnkReflObject;

struct UnkVec3f;

// As src/game/CCharClassLiquidPoolComponent/CCharClassLiquidPoolComponent_Pool_ovb0075.cpp defines it.
struct UnkLiquidPoolEnter_ovb0075;
struct UnkLiquidPoolActor_ovb0075;
void fn_7100901dd4(UnkLiquidPoolEnter_ovb0075* self, UnkLiquidPoolActor_ovb0075* actor, const UnkVec3f* pos);

void fn_7100f0ecf4(void* info, const CStrId* id);

// A named resource whose name is counted on each read.
struct UnkLavaPoolResource_l8fc90c {
    u8 pad_0x0[0x10];
    CRntString name;  // 0x10
    u8 pad_0x40[0x4];
    u32 use_count;  // 0x44
};
static_assert(__builtin_offsetof(UnkLavaPoolResource_l8fc90c, use_count) == 0x44, "layout");

struct UnkLavaPoolCharClass_l8fc90c {
    u8 pad_0x0[0x98];
    u8 field_0x98[1];
};

struct UnkLavaPoolActor_l8fc90c {
    u8 pad_0x0[0x28];
    UnkReflObject* char_class;               // 0x28
    UnkLavaPoolResource_l8fc90c* resource;  // 0x30

    const CRntString& ResourceName() {
        ++resource->use_count;
        return resource->name;
    }
};

struct UnkLavaPoolEnter_l8fc90c {
    u8 pad_0x0[0x30];
    UnkLavaPoolActor_l8fc90c* owner;  // 0x30
};

// A string id built from a string's text, out of line (fn_7100000850).
struct UnkTextStrId_l8fc90c {
    UnkStringData* data;

    explicit UnkTextStrId_l8fc90c(const CRntString& text) {
        CStrId_ConstructFromRntString(reinterpret_cast<base::global::UnkStrIdBase*>(this), text, true);
    }
    ~UnkTextStrId_l8fc90c() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

static inline UnkLavaPoolCharClass_l8fc90c* CharClass(UnkReflObject* obj) {
    if (!obj)
        return nullptr;
    return fn_710009d50c(obj->vf_0x0(), fn_71011abbd4()) ? reinterpret_cast<UnkLavaPoolCharClass_l8fc90c*>(obj)
                                                         : nullptr;
}

void fn_71008fc90c(UnkLavaPoolEnter_l8fc90c* self, UnkLavaPoolActor_l8fc90c* actor, const UnkVec3f* pos) {
    fn_7100901dd4(reinterpret_cast<UnkLiquidPoolEnter_ovb0075*>(self),
                  reinterpret_cast<UnkLiquidPoolActor_ovb0075*>(actor), pos);
    if (!fn_7100df5c38(g_unk_7101d47310, actor))
        return;
    void* info = g_unk_7101d47310->field_0x38;
    UnkTunableLine key(self->owner->ResourceName());
    fn_710007f3b4(key.Str(), &actor->ResourceName());
    {
        UnkTextStrId_l8fc90c id(*key.Str());
        fn_7100f0ecf4(info, reinterpret_cast<const CStrId*>(&id));
    }
    fn_7100f0e98c(info, CharClass(self->owner->char_class)->field_0x98, &self->owner->resource, &actor->resource);
}
