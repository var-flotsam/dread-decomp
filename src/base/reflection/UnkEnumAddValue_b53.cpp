// Unit 22 (tools/tu_map.py): adds a (name, value) pair to an enum type's value list at +0xf0 (the
// enum registrations in src/clones/ call it once per value). The list grows like
// base::UnkVector::PushBack, through its own reserve (fn_7100014a48). Names and layouts are
// placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkEnumValue_b53 {
    CStrId name;
    u32 value;
};

struct UnkEnumValueList_b53 {
    UnkEnumValue_b53* data;  // 0xf0
    u32 count;               // 0xf8
    u32 capacity;            // 0xfc
    u32 step;                // 0x100
};

struct UnkEnumType_b53 {
    u8 pad_0x0[0xf0];
    UnkEnumValueList_b53 values;
};

void fn_7100014a48(UnkEnumValueList_b53* list, u32 capacity);

void fn_7100014948(void* e, const CStrId& name, u32 value) {
    UnkEnumValueList_b53& list = static_cast<UnkEnumType_b53*>(e)->values;
    UnkEnumValue_b53 entry = {name, value};
    u32 n = list.count;
    u32 capacity = list.capacity;
    if (capacity <= n && list.step != 0)
        capacity = (n / list.step + 1) * list.step;
    if ((s32)n >= (s32)capacity)
        for (;;) {
        }
    fn_7100014a48(&list, capacity);
    UnkEnumValue_b53* slot = &list.data[list.count];
    new (&slot->name) CStrId(entry.name);
    __builtin_memcpy(&slot->value, &entry.value, sizeof(u32));
    ++list.count;
}
