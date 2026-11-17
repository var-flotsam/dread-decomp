// Unit 2565 (tools/tu_map.py): a reflected getter that copies the u32 list of the object `obj` points to
// at +8 into a vector and returns it as a reflected value. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

extern base::reflection::CType g_unk_7101d5d978;

// An id of 32 bits; its type is not known.
enum UnkListId_le7c6fc : u32 {};

struct UnkU32List_le7c6fc {
    u8 pad_0x0[0x20];
    UnkListId_le7c6fc items[0x17cf];  // 0x20
    u32 count;          // 0x5f5c
};
static_assert(__builtin_offsetof(UnkU32List_le7c6fc, count) == 0x5f5c, "");

UnkReflValue fn_7100e7c6fc(void*, char* obj) {
    auto* list = *reinterpret_cast<UnkU32List_le7c6fc**>(obj + 8);
    u32 n = list->count;
    base::global::CRntVector<UnkListId_le7c6fc> values(0xf);
    values.Reserve(n);
    for (u64 i = 0; i < list->count; ++i) {
        UnkListId_le7c6fc v = list->items[i];
        values.PushBack(v);
    }
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    fn_71000a18d4(&r, &g_unk_7101d5d978, &values, false);
    values.Free();
    return r;
}
