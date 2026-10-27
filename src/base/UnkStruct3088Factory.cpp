// A small unit (0x7100150b50-0x710015142c) whose factory creates one of two objects: the
// 0x3088-byte one (four 0xc10-byte tables at +0x10, +0xc20, +0x1830, +0x2440, constructor
// fn_71001d81f8) or the 0x178-byte one (constructor fn_71001e83cc).
#include "types.h"
#include "base/UnkVector.h"

struct UnkStruct_0x3088;
struct UnkStruct_0x178;

// Constructs the 0x3088-byte object in place; `flag` is kept at +0x3080.
void fn_71001d81f8(UnkStruct_0x3088* self, u32 id, bool flag);
// Constructs the 0x178-byte object in place.
void fn_71001e83cc(UnkStruct_0x178* self, u32 id, bool flag);

// Creates the object for `id`: the large one when `large` is set (keeping `small`'s flag), none
// when `none` is set, else the small one when `small` is set and the large one otherwise.
void* fn_7100150ff0(u32 id, bool small, bool large, bool none) {
    if (large) {
        auto* obj = static_cast<UnkStruct_0x3088*>(fn_71000839a0(0x3088, 8));
        fn_71001d81f8(obj, id, small);
        return obj;
    }
    if (none)
        return nullptr;
    if (small) {
        auto* obj = static_cast<UnkStruct_0x178*>(fn_71000839a0(0x178, 8));
        fn_71001e83cc(obj, id, true);
        return obj;
    }
    auto* obj = static_cast<UnkStruct_0x3088*>(fn_71000839a0(0x3088, 8));
    fn_71001d81f8(obj, id, false);
    return obj;
}
