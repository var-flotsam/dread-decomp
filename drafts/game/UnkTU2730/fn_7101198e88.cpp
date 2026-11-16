// Unit 2730 (tools/tu_map.py): for every unsigned property of one blackboard's cutscene section (g_unk_7172a0bc98)
// that holds `match`, stores `match` under the same name in another blackboard's section and `replacement` in the
// first, then clears both sections' changed flags. Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/UnkDestructors.h"
#include "base/UnkNoArg.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2730.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;

// The game manager's blackboards, 0x148 bytes apart from +0x968.
struct UnkBlackboardSlot_l198e88 {
    CBlackboard blackboard;
    u8 pad_0xf0[0x58];
};
static_assert(sizeof(UnkBlackboardSlot_l198e88) == 0x148, "layout");

struct UnkGameBlackboards_l198e88 {
    u8 pad_0x0[0x968];
    UnkBlackboardSlot_l198e88 slots[1];  // 0x968
};

// A property value owned by an iterator (its destructor is fn_71000a1c60).
struct UnkOwnedPropertyValue_l198e88 : UnkReflValue {
    ~UnkOwnedPropertyValue_l198e88() { fn_71000a1c60(this); }
};

// An iterator over a section's properties: the current name and value.
struct UnkPropertyIter_l198e88 {
    UnkSectionData* data;
    base::UnkIterValue<UnkOwnedPropertyValue_l198e88, base::UnkStrIdMember> current;

    explicit UnkPropertyIter_l198e88(UnkSectionData* data) : data(data) {
        current.SetRef(reinterpret_cast<const base::UnkStrIdMember*>(data->properties.ids),
                       static_cast<UnkOwnedPropertyValue_l198e88*>(data->properties.values));
    }
    // The value stored under the current name (the one past the end when there is none).
    UnkReflValue& Value() {
        u64 index = 0;
        u32 count = data->properties.count;
        if (count) {
            const base::UnkStrIdMember& name = current.Index();
            base::global::CStrId* ids = data->properties.ids;
            for (; index != count; ++index) {
                if (ids[index].data == name.data)
                    break;
            }
        }
        return static_cast<UnkReflValue*>(data->properties.values)[index];
    }

    void operator++() {
        ++current.ref.index;
        ++current.ref.element;
    }
};

static inline CBlackboard::CSection* CutsceneSection(u32 index) {
    UnkGameBlackboards_l198e88* manager = reinterpret_cast<UnkGameBlackboards_l198e88*>(g_unk_7101d47310);
    return fn_71010472e0(&manager->slots[index].blackboard, g_unk_7172a0bc98);
}

// The value read as an unsigned.
static inline const u32* AsUnsigned(UnkReflValue& value) {
    fn_710009d50c(reinterpret_cast<const CType*>(value.type), fn_7100005178());
    if (value.field_0x14 == 1 || *reinterpret_cast<const u64*>(value.type + 0x18) > 8)
        return static_cast<const u32*>(value.field_0x0);
    return reinterpret_cast<const u32*>(&value);
}

static inline void SetUnsigned(CBlackboard::CSection* section, UnkPropertyName* name, u32* value) {
    UnkTypedValue v;
    v.type = fn_7100005178();
    v.value = value;
    fn_7100332c80(section, name, &v);
}

// The flags arrive as 32-bit values (the declaration in UnkTU2730_Lists_ovb0188.cpp says bool, which would mask them).
void fn_7101198e88(UnkNoArg, u32 from_index, u32 replacement, u32 to_index, u32 match) {
    u32 new_value = replacement;
    u32 old_value = match;
    CBlackboard::CSection* from = CutsceneSection(from_index);
    if (!from)
        return;
    CBlackboard::CSection* to = CutsceneSection(to_index);
    UnkPropertyIter_l198e88 it(&from->data);
    u32 count = from->data.properties.count;
    for (u32 i = 0; i < count; ++i, ++it) {
        if (*AsUnsigned(it.Value()) == old_value) {
            SetUnsigned(to, reinterpret_cast<UnkPropertyName*>(&it.current.Index()), &old_value);
            SetUnsigned(from, reinterpret_cast<UnkPropertyName*>(&it.current.Index()), &new_value);
        }
    }
    to->data.field_0x38 = false;
    from->data.field_0x38 = false;
}
