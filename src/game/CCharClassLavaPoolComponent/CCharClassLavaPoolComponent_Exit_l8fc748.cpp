// CCharClassLavaPoolComponent's unit: an actor leaves the lava pool. When the game manager accepts the actor
// (fn_7100df5c38) and the pool's slot 0x1e8 says so, its player info (+0x38) is told the id made of the pool
// owner's resource name and the actor's, then given the owner's char class (its +0x98) and both resources;
// then the liquid pool's handler (fn_71009016a0) runs. Names and layouts are placeholders.
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

// As src/game/CCharClassLiquidPoolComponent/CCharClassLiquidPoolComponent_Exit_l9016a0.cpp defines it.
struct UnkLiquidPoolExit_l9016a0;
struct UnkLiquidPoolActor_l9016a0;
void fn_71009016a0(UnkLiquidPoolExit_l9016a0* self, UnkLiquidPoolActor_l9016a0* actor, const UnkVec3f* pos);

void fn_7100f0ecec(void* info, const CStrId* id);

// A named resource whose name is counted on each read.
struct UnkLavaPoolResource_l8fc748 {
    u8 pad_0x0[0x10];
    CRntString name;  // 0x10
    u8 pad_0x40[0x4];
    u32 use_count;  // 0x44
};
static_assert(__builtin_offsetof(UnkLavaPoolResource_l8fc748, use_count) == 0x44, "layout");

struct UnkLavaPoolCharClass_l8fc748 {
    u8 pad_0x0[0x98];
    u8 field_0x98[1];
};

struct UnkLavaPoolActor_l8fc748 {
    u8 pad_0x0[0x28];
    UnkReflObject* char_class;               // 0x28
    UnkLavaPoolResource_l8fc748* resource;  // 0x30

    const CRntString& ResourceName() {
        ++resource->use_count;
        return resource->name;
    }
};

struct UnkLavaPoolExit_l8fc748 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkLavaPoolActor_l8fc748* owner;  // 0x30

    bool vf_0x1e8() { return reinterpret_cast<bool (*)(UnkLavaPoolExit_l8fc748*)>(vtable[0x1e8 / 8])(this); }
};

// A string id built from a string's text, out of line (fn_7100000850).
struct UnkTextStrId_l8fc748 {
    UnkStringData* data;

    explicit UnkTextStrId_l8fc748(const CRntString& text) {
        CStrId_ConstructFromRntString(reinterpret_cast<base::global::UnkStrIdBase*>(this), text, true);
    }
    ~UnkTextStrId_l8fc748() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

static inline UnkLavaPoolCharClass_l8fc748* CharClass(UnkReflObject* obj) {
    if (!obj)
        return nullptr;
    return fn_710009d50c(obj->vf_0x0(), fn_71011abbd4()) ? reinterpret_cast<UnkLavaPoolCharClass_l8fc748*>(obj)
                                                         : nullptr;
}

void fn_71008fc748(UnkLavaPoolExit_l8fc748* self, UnkLavaPoolActor_l8fc748* actor, const UnkVec3f* pos) {
    if (fn_7100df5c38(g_unk_7101d47310, actor) && self->vf_0x1e8()) {
        void* info = g_unk_7101d47310->field_0x38;
        UnkTunableLine key(self->owner->ResourceName());
        CRntString_Append(key.Str(), &actor->ResourceName());
        {
            UnkTextStrId_l8fc748 id(*key.Str());
            fn_7100f0ecec(info, reinterpret_cast<const CStrId*>(&id));
        }
        fn_7100f0e4e4(info, CharClass(self->owner->char_class)->field_0x98, &self->owner->resource, &actor->resource);
    }
    fn_71009016a0(reinterpret_cast<UnkLiquidPoolExit_l9016a0*>(self),
                  reinterpret_cast<UnkLiquidPoolActor_l9016a0*>(actor), pos);
}
