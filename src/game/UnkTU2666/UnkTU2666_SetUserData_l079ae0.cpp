// Unit 2666 (Lua bindings): reads the two user-data flags from the table argument and stores them
// in the "USER_DATA" section of the user-data blackboard (unless it is locked). The inverse of
// fn_71010798e0. Names and layouts are placeholders.
#include <algorithm>

#include "../../game_s7/UnkIter.h"
#include "../../game_s7/UnkPropBagS7.h"
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2666.h"
#include "game/UnkTU2677.h"
#include "game/UnkTU2768.h"
#include "game/UnkTU56.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CStrId;
using unk_prop_bag::UnkPropBag;

struct UnkUserDataOwner;

// A value in the bag: where it is in the bag's stream, the hash of its name and its type.
struct UnkBagValue_l079ae0 {
    u64 offset;
    u64 key;
    s32 type;  // 'b' for a bool
};

static inline UnkBagValue_l079ae0* Values_l079ae0(UnkPropBag& bag) {
    return reinterpret_cast<UnkBagValue_l079ae0*>(bag.values.data);
}

// The value named `id`, or the end of the values when there is none.
static inline UnkBagValue_l079ae0* FindValue_l079ae0(UnkPropBag& bag, const CStrId& id) {
    UnkBagValue_l079ae0* values = Values_l079ae0(bag);
    u64 key = id.data->Key();
    UnkIter<UnkBagValue_l079ae0> first{values};
    UnkIter<UnkBagValue_l079ae0> last{&values[bag.values.count]};
    return std::find_if(first, last, [key](const UnkBagValue_l079ae0& v) { return v.key == key; }).p;
}

// Reads the bool named `id` from the bag into `out`, when the bag has one.
static inline void ReadBool_l079ae0(UnkPropBag& bag, const CStrId& id, bool* out) {
    UnkBagValue_l079ae0* it = FindValue_l079ae0(bag, id);
    if (it != &Values_l079ae0(bag)[bag.values.count] && it->type == 'b') {
        CRntFile_Seek(&bag.lock, it->offset, 0);
        fn_710007c70c(&bag.lock, out, 1);
    }
}

// Stores the flag under `name` in the section (unless the blackboard is locked).
static inline void SetSectionBool_l079ae0(CBlackboard* blackboard, CBlackboard::CSection* section, CStrId* name,
                                          bool* value) {
    if (section && !blackboard->field_0xe8) {
        UnkTypedValue v;
        v.type = fn_710000d228();
        v.value = value;
        fn_7100332c80(section, reinterpret_cast<UnkPropertyName*>(name), &v);
    }
}

s32 fn_7101079ae0(lua_State* L) {
    bool first;
    bool second;
    UnkPropBag bag;
    if (!fn_71010c0270(L, "t", &bag))
        return 0;
    first = false;
    second = false;
    ReadBool_l079ae0(bag, g_unk_7172a01968[0], &first);
    ReadBool_l079ae0(bag, g_unk_7172a01968[1], &second);
    UnkUserDataOwner* provider = g_unk_7101d47308;
    if (!provider) {
        provider = static_cast<UnkUserDataOwner*>(fn_71000839a0(0x2b8, 0x3f));
        fn_710120320c(provider);
        g_unk_7101d47308 = provider;
    }
    if (CBlackboard* blackboard = fn_710120384c(provider)) {
        {
            CStrId id("USER_DATA", true);
            SetSectionBool_l079ae0(blackboard, fn_71010472e0(blackboard, id), &g_unk_7172a01968[0], &first);
        }
        {
            CStrId id("USER_DATA", true);
            SetSectionBool_l079ae0(blackboard, fn_71010472e0(blackboard, id), &g_unk_7172a01968[1], &second);
        }
    }
    return 0;
}
