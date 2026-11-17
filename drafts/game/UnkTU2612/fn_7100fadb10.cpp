// Unit 2612 of tools/tu_map.py (no name hint): a fixed array of nine string ids built from a list of ids:
// every id starts empty, then, when the list fits, each of its ids is copied in order. Names and layouts
// are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStrIdBase;

// An empty id, creating the pool first when there is none.
static inline void MakeEmpty(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

// The nine ids.
struct UnkStrIds_lfadb10 {
    static constexpr u64 kCount = 9;

    CStrId field_0x0;
    CStrId field_0x8;
    CStrId field_0x10;
    CStrId field_0x18;
    CStrId field_0x20;
    CStrId field_0x28;
    CStrId field_0x30;
    CStrId field_0x38;
    CStrId field_0x40;

    CStrId& operator[](u32 index) { return (&field_0x0)[index]; }
};
static_assert(sizeof(UnkStrIds_lfadb10) == 0x48, "nine ids");

void fn_7100fadb10(void* self, const UnkStrIdBase* ids, u32 count) {
    auto* array = static_cast<UnkStrIds_lfadb10*>(self);
    MakeEmpty(&array->field_0x0);
    MakeEmpty(&array->field_0x8);
    MakeEmpty(&array->field_0x10);
    MakeEmpty(&array->field_0x18);
    MakeEmpty(&array->field_0x20);
    MakeEmpty(&array->field_0x28);
    MakeEmpty(&array->field_0x30);
    MakeEmpty(&array->field_0x38);
    MakeEmpty(&array->field_0x40);
    u32 n = count;
    if (n <= UnkStrIds_lfadb10::kCount) {
        u32 i = 0;
        for (const UnkStrIdBase* it = ids; it != ids + count; ++it) {
            CStrId id = static_cast<const CStrId&>(*it);
            (*array)[i++] = id;
        }
    }
}
