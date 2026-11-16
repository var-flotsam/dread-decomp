// Unit 2571: reads three counters back from the game manager's blackboard (section 0x229 of the
// reflection id table), the names fn_7100ec6714 stores them under; the first two can be kept as they
// are. Marks the counters loaded when all three were there. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkStrIdHandle;
void fn_7100ae986c(UnkStrIdHandle* self, const CStrId& id);

// The counters' names in the blackboard.
extern CStrId g_unk_71729ce1a0[3] __attribute__((visibility("hidden")));

// A name's handle (an id and the object a registry has for it) as fn_7100ae986c builds it in place;
// only the id is released.
struct UnkNameHandle_ec6c58 {
    UnkStringData* id;
    void* object;

    explicit UnkNameHandle_ec6c58(const CStrId& name) {
        fn_7100ae986c(reinterpret_cast<UnkStrIdHandle*>(this), name);
    }
    ~UnkNameHandle_ec6c58() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (id && id != &pool->empty) {
                --id->ref_count;
                if (id->ref_count == 0) {
                    fn_71000803a4(pool, id);
                    pool = g_unk_7101d47108;
                }
            }
            id = &pool->empty;
        }
    }
};

struct UnkCounters_ec6c58 {
    s32* counts;  // three
    bool loaded;  // 0x8
};

static inline const CStrId& Section() {
    return base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x229];
}

// Whether the blackboard holds a value under `name`.
static inline bool Has(CBlackboard* blackboard, const UnkNameHandle_ec6c58& name) {
    return fn_71010474f4(blackboard, Section(), *reinterpret_cast<const CStrId*>(&name));
}

// The int under `name` (zero when the section is missing).
static inline s32 ReadInt(CBlackboard* blackboard, const UnkNameHandle_ec6c58& name) {
    s32 value = 0;
    if (CBlackboard::CSection* s = fn_7101047270(blackboard, Section()))
        fn_7100395a30(s, &name, &value);
    return value;
}

// With `keep`, the first two counters are left as they are and count as read.
void fn_7100ec6c58(UnkCounters_ec6c58* self, bool keep) {
    self->loaded = false;
    CBlackboard* blackboard = &g_unk_7101d47310->field_0x968;
    u32 found = 0;
    for (u32 i = 0; i < 3; ++i) {
        UnkNameHandle_ec6c58 name(g_unk_71729ce1a0[i]);
        bool present = Has(blackboard, name);
        if (keep && i < 2) {
            ++found;
            continue;
        }
        self->counts[i] = 0;
        if (present) {
            self->counts[i] = ReadInt(blackboard, name);
            ++found;
        }
    }
    if (found == 3)
        self->loaded = true;
}
