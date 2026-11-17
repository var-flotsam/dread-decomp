// build-flags: no-strict-aliasing
// CCharClass's unit (tools/tu_map.py unit 2740): reads the class's action sets from a file: a count, then
// one path per set. The set named "timeline" is kept apart; the others go in the table at +0xd0 under
// their names. Names and layouts are placeholders.
#include <new>

#include "base/UnkAssetResRef.h"
#include "base/UnkCountedResource.h"
#include "base/UnkStrIdPathPair.h"
#include "base/animation/CBlendSpace1D.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU2527.h"
#include "game/UnkTU27.h"
#include "game/UnkTU56.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

// As src/game/UnkTU365/UnkTU365_Reserve_l2a6d38.cpp defines them.
struct UnkCountedRef_l2a6d38 {
    UnkCountedResource* ptr;

    UnkCountedRef_l2a6d38(UnkCountedRef_l2a6d38&& other) {
        ptr = nullptr;
        if (ptr != other.ptr) {
            UnkCountedResource* p = other.ptr;
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
    }
    ~UnkCountedRef_l2a6d38() {
        UnkCountedResource* p = ptr;
        if (p && --p->ref_count == 0)
            fn_710001737c(p);
    }
};

using UnkCountedRefArrays_l2a6d38 = base::global::CRntSmallDictionary<CStrId, UnkCountedRef_l2a6d38>;
void fn_71002a6d38(UnkCountedRefArrays_l2a6d38* self, u32 new_capacity);

// An action set: a counted resource with its name at +0x58.
struct UnkActionSet_l1ac144 : UnkCountedResource {
    CStrId name;  // 0x58
};

// Points `slot` at `p`, taking a reference to it and dropping the old one.
static inline void Assign(UnkCountedResource*& slot, UnkCountedResource* p) {
    if (slot != p) {
        if (p)
            ++p->ref_count;
        UnkCountedResource* old = slot;
        if (old && --old->ref_count == 0)
            fn_710001737c(old);
        slot = p;
    }
}

// The ids an action set is looked up by: its category and its path (counted copies).
struct UnkActionSetIds_l1ac144 {
    CStrId category;
    CFilePathStrId path;

    UnkActionSetIds_l1ac144(CStrId c, CFilePathStrId p) : category(c), path(p) {}
};

static inline UnkAssetResRef Resolve(UnkRegistry_7100d92158* registry, const UnkActionSetIds_l1ac144& ids) {
    return fn_710002038c(registry, reinterpret_cast<const UnkStrIdPathPair*>(&ids), true, nullptr);
}

// The resource as an action set (a new reference), or none when it is not one.
static inline UnkAssetResRef AsActionSet(const UnkAssetResRef& found) {
    UnkAssetResRef set;
    UnkCountedResource* res = found.ptr;
    if (res && fn_710009d50c(reinterpret_cast<base::reflection::UnkReflObject*>(res)->vf_0x0(), fn_7100b8bbf4()))
        Assign(set.ptr, res);
    return set;
}

struct UnkCharClass_l1ac144 {
    u8 pad_0x0[0xd0];
    UnkCountedRefArrays_l2a6d38 action_sets;  // 0xd0
    UnkCountedResource* timeline;             // 0xf8

    void AddActionSet(const CStrId& name, UnkCountedResource* set) {
        UnkCountedRefArrays_l2a6d38& d = action_sets;
        if (d.count == d.capacity)
            fn_71002a6d38(&d, d.count + d.step);
        new (&d.keys[d.count]) CStrId(name);
        UnkCountedResource*& slot = d.values[d.count].ptr;
        slot = nullptr;
        Assign(slot, set);
        ++d.count;
    }
};

bool fn_71011ac144(UnkCharClass_l1ac144* self, base::global::CRntFile* file) {
    static const CStrId s_timeline("timeline", true);
    s32 count = 0;
    fn_710007c714(file, &count, 4, 1);
    for (s32 i = 0; i < count; ++i) {
        const char* path;
        fn_710007cee4(file, &path);
        UnkAssetResRef set = AsActionSet(Resolve(
            g_unk_7101d4bbc0, UnkActionSetIds_l1ac144(reinterpret_cast<const CStrId&>(g_unk_71727b9630),
                                                      CFilePathStrId(path, true))));
        UnkActionSet_l1ac144* actions = static_cast<UnkActionSet_l1ac144*>(set.ptr);
        if (actions->name == s_timeline)
            Assign(self->timeline, actions);
        else
            self->AddActionSet(actions->name, actions);
    }
    return true;
}
