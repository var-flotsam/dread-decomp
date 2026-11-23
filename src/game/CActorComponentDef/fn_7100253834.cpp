// CActorComponentDef's unit (tools/tu_map.py unit 343): the definition class registered for a type id.
// For a component type it looks up "CCharClass" followed by the type's name without its leading "C",
// otherwise the type's name followed by "Def", walking up the base types until one is registered.
// Names and layouts are placeholders.
#include "base/animation/CBlendSpaceResource.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkHashData_b0129.h"
#include "game/UnkKeyId_b0129.h"
#include "game/UnkTU57.h"
#include "game/UnkTU83.h"
#include "types.h"

using base::global::CRntString;
using base::reflection::CType;

struct UnkRegClass_b0129;

static inline const CRntString* Addr(const CRntString& tmp) { return &tmp; }

// A string built from a string id out of line (fn_710007eb20) and destroyed as one.
struct UnkIdText_l253834 {
    union {
        CRntString str;
    };

    explicit UnkIdText_l253834(const base::global::CStrId& id) {
        CRntString_ConstructFromStrId(&str, reinterpret_cast<const UnkPooledStrId*>(&id));
    }
    ~UnkIdText_l253834() { CRntString_Destroy(&str); }
};

// The registered entry for the key (CRC-64) of `name`.
static inline UnkVersionEntry* FindByKey(const u64* key) {
    return fn_71000a5118(g_unk_7101d4c8d0, key);
}

// The type `registry` holds under `hash`.
static inline const CType* FindType(UnkVersionRegistry* registry, u64 hash) {
    return reinterpret_cast<const CType*>(fn_71000a5118(registry, &hash));
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
UnkRegClass_b0129* fn_7100253834(const UnkKeyId_b0129* id) {
    const CType* type = FindType(g_unk_7101d4c8d0, id->data->hash);
    UnkVersionEntry* found = nullptr;
    if (fn_710009d50c(type, fn_71010c23d0())) {
        for (; type; type = type->m_parent) {
            u64 key;
            fn_7100066630(&key, Addr(CRntString_ConcatCString(Addr(CRntString("CCharClass")), type->m_name.Text() + 1)));
            found = FindByKey(&key);
            if (found)
                break;
        }
    } else {
        while (type) {
            u64 key;
            fn_7100066630(&key, Addr(CRntString_ConcatCString(Addr(UnkIdText_l253834(type->m_name).str), "Def")));
            found = FindByKey(&key);
            type = type->m_parent;
            if (found)
                break;
        }
    }
    return reinterpret_cast<UnkRegClass_b0129*>(found);
}
