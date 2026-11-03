// Unit 2669 (tools/tu_map.py): builds a string with 0x100 bytes of inline storage, fills it from
// `source` (fn_710109e0a0, whose first argument is an empty tag) and, when the result is not empty,
// runs fn_710109dd6c with it and the two flags. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "types.h"

using base::global::CRntString;

// A string (header of 0x30 bytes) with 0x100 bytes of inline storage (as in src/game/CGameManager.cpp).
struct UnkUnit2669StrStorage {
    void* vtable;
    u8 pad_0x8[0x100];
    void* field_0x108;
    u32 field_0x110;
};
struct UnkUnit2669Str {
    const char* data;
    u32 length;
    u8 pad_0xc[0x24];
    UnkUnit2669StrStorage storage;

    CRntString* Str() { return reinterpret_cast<CRntString*>(this); }

    UnkUnit2669Str() {
        fn_710007e6e4(Str(), reinterpret_cast<base::UnkAllocator*>(&storage));
        storage.vtable = g_unk_7101a194b8 + 2;
        storage.field_0x108 = nullptr;
        storage.field_0x110 = 0;
    }
    ~UnkUnit2669Str() {
        fn_710007ed40(Str());
        fn_710007ecd4(Str());
    }
};

struct UnkUnit2669Tag {};
void fn_710109e0a0(UnkUnit2669Tag tag, void* source, UnkUnit2669Str* out);
u32 fn_710109dd6c(void* self, void* source, UnkUnit2669Str* name, bool a, bool b);

u32 fn_710109e1ac(void* self, void* source, bool a, bool b) {
    UnkUnit2669Str name;
    fn_710109e0a0(UnkUnit2669Tag(), source, &name);
    if (!name.data || !name.data[0])
        return 0;
    return fn_710109dd6c(self, source, &name, a, b);
}
