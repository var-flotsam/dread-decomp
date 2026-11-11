// CAIComponent unit (tools/tu_map.py, the name from reflection): reflected functions that add a
// string id to the id lists of the object at 8. The lists are 0x44-byte records from +0x13e4 (a
// count, then up to eight ids, 4-byte aligned), their number at +0x13e0. fn_7101115758 takes a list
// index and an id: when the object at +0x1610 has more than that many entries (fn_7100413fe0) and
// fn_7100912674 accepts the id, the lists up to the index are created empty and the id is appended to
// that list. fn_71011158f8 appends its id to the fourth list. Both return an empty value. Names and
// layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CCharClassAIComponent.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflValue;

typedef CStrId __attribute__((aligned(4))) UnkStrIdA4_b41a3;

struct UnkIdList_b41a3 {
    u32 count;
    UnkStrIdA4_b41a3 ids[8];
};
static_assert(sizeof(UnkIdList_b41a3) == 0x44, "UnkIdList_b41a3 layout");

struct UnkAIIdLists_b41a3 {
    u8 pad_0x0[0x13e0];
    u32 list_count;
    UnkIdList_b41a3 lists[1];
};

void* fn_7100912674(const CStrId* id);

static inline __attribute__((always_inline)) void Append_b41a3(UnkIdList_b41a3* list, const CStrId* id) {
    new (&list->ids[list->count]) CStrId(*id);
    ++list->count;
}

UnkReflValue fn_7101115758(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    auto* target = *reinterpret_cast<UnkAIIdLists_b41a3**>(obj + 8);
    u32 index = *base::reflection::refl_ArgValue<u32>(&a[0]);
    const CStrId* id;
    if (!a[1].type)
        id = nullptr;
    else if (a[1].field_0x14 == 1 || *reinterpret_cast<const u64*>(a[1].type + 0x18) > 8)
        id = static_cast<const CStrId*>(a[1].field_0x0);
    else
        id = reinterpret_cast<const CStrId*>(&a[1]);
    void* entries = *reinterpret_cast<void**>(reinterpret_cast<char*>(target) + 0x1610);
    if (entries && fn_7100413fe0(entries) > static_cast<s32>(index) && fn_7100912674(id)) {
        u32 n = index + 1;
        if (n > target->list_count) {
            for (u64 i = target->list_count; i < n; ++i)
                target->lists[i].count = 0;
            target->list_count = n;
        }
        Append_b41a3(&target->lists[index], id);
    }
    return base::reflection::refl_EmptyValue();
}

UnkReflValue fn_71011158f8(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    auto* target = *reinterpret_cast<UnkAIIdLists_b41a3**>(obj + 8);
    const CStrId* id;
    if (!a->type)
        id = nullptr;
    else if (a->field_0x14 == 1 || *reinterpret_cast<const u64*>(a->type + 0x18) > 8)
        id = static_cast<const CStrId*>(a->field_0x0);
    else
        id = reinterpret_cast<const CStrId*>(a);
    if (fn_7100912674(id))
        Append_b41a3(&target->lists[3], id);
    return base::reflection::refl_EmptyValue();
}
