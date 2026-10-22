// Three functions that move an object from one pointer list to another: its flag (at 0x20 or
// 0x28) is cleared, then every entry equal to it is erased from the second list and the object
// appended to the first (once per entry). The index moves on after an erase, as in the
// original. The two lists sit at `Offset` + 8 and `Offset` + 0x30.
#include "base/UnkVector.h"
#include "types.h"

struct UnkListedObj;

// Erasing moves the later entries down with memcpy.
struct UnkPtrList {
    UnkListedObj** data;
    u32 count;

    __attribute__((always_inline)) void EraseAt(u64 index) {
        if (index < count) {
            __builtin_memcpy(&data[index], &data[index + 1], (u64)(u32)(count - 1 - (u32)index) * 8);
            --count;
        }
    }
};

struct UnkListPair {
    u8 unk_0x0[0x8];
    base::UnkVector<UnkListedObj*> active;
    UnkPtrList pending;
};

void fn_710020c8ac(void* flag, bool value);

template <u32 Offset, u32 Flag, UnkListedObj** (*Get)(UnkPtrList*, u32)>
__attribute__((always_inline)) static inline void Activate(char* self, UnkListedObj* obj) {
    if (!obj)
        return;
    UnkListPair* lists = reinterpret_cast<UnkListPair*>(self + Offset);
    fn_710020c8ac(reinterpret_cast<char*>(obj) + Flag, false);
    for (u64 i = 0; i < lists->pending.count; ++i) {
        if (*Get(&lists->pending, i) == obj) {
            lists->pending.EraseAt(i);
            lists->active.PushBack(obj);
        }
    }
}

UnkListedObj** fn_7100e0a49c(UnkPtrList* list, u32 index);
UnkListedObj** fn_7100e0a5ec(UnkPtrList* list, u32 index);
UnkListedObj** fn_7100e0a73c(UnkPtrList* list, u32 index);

void fn_7100e0babc(char* self, UnkListedObj* obj) { Activate<0x0, 0x20, fn_7100e0a49c>(self, obj); }
void fn_7100e0bc40(char* self, UnkListedObj* obj) { Activate<0x50, 0x28, fn_7100e0a5ec>(self, obj); }
void fn_7100e0bdc4(char* self, UnkListedObj* obj) { Activate<0xa0, 0x20, fn_7100e0a73c>(self, obj); }
