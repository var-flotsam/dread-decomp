// Unit 2571: stores three counters under their names in the game manager's blackboard (section 0x229
// of the reflection id table), unless the blackboard is locked. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2571.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkStrIdHandle;
void fn_7100ae986c(UnkStrIdHandle* self, const CStrId& id);

// The counters' names in the blackboard.

// A name's handle (an id and the object a registry has for it) as fn_7100ae986c builds it in place;
// only the id is released.
struct UnkNameHandle_ec6714 {
    UnkStringData* id;
    void* object;

    explicit UnkNameHandle_ec6714(const CStrId& name) {
        fn_7100ae986c(reinterpret_cast<UnkStrIdHandle*>(this), name);
    }
    ~UnkNameHandle_ec6714() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (id && id != &pool->empty) {
                --id->ref_count;
                if (id->ref_count == 0) {
                    StringPool_Release(pool, id);
                    pool = g_unk_7101d47108;
                }
            }
            id = &pool->empty;
        }
    }
};

struct UnkCountsOwner_ec6714 {
    s32* counts;  // three
};

// Stores counter `index` under `name`.
static inline void StoreCount(UnkGameManager* manager, CBlackboard* blackboard, const CStrId& name,
                              const UnkCountsOwner_ec6714* self, u32 index) {
    s32 value;
    UnkTypedValue v;
    UnkNameHandle_ec6714 handle(name);
    value = self->counts[index];
    const CStrId& section = base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x229];
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!manager->field_0x968.field_0xe8) {
            v.type = fn_7100014768();
            v.value = &value;
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(&handle), &v);
        }
    }
}

void fn_7100ec6714(UnkCountsOwner_ec6714* self) {
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    for (u32 i = 0; i < 3; ++i)
        StoreCount(manager, blackboard, g_unk_71729ce1a0[i], self, i);
}
