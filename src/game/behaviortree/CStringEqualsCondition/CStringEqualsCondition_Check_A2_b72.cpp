// behaviortree::CStringEqualsCondition's unit (tools/tu_map.py unit 536): unless the id at +0x68 is the
// empty string, resolves it (fn_7100395044) into a string with inline storage and compares that with the
// string at +0x38 (fn_710007f120); false for an empty id. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkTU57.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkCompared_0x30;

// A string with 0x100 bytes of inline storage (0x148 bytes), as in src/base/UnkLuaDoString_b50.cpp.
struct UnkInlineString_A2_b72 {
    u8 str[0x30];
    char* storage_vtable;  // +0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    UnkInlineString_A2_b72() {
        fn_710007e6e4(Str(), reinterpret_cast<base::UnkAllocator*>(&storage_vtable));
        storage_vtable = reinterpret_cast<char*>(g_unk_7101a194b8 + 2);
        field_0x138 = 0;
        field_0x140 = 0;
    }
    ~UnkInlineString_A2_b72() {
        fn_710007ed40(Str());
        fn_710007ecd4(Str());
    }
};

struct UnkStringEqualsCondition_A2_b72 {
    u8 pad_0x0[0x38];
    u8 field_0x38[0x30];
    CStrId field_0x68;
};

bool fn_71003a3c88(UnkStringEqualsCondition_A2_b72* self) {
    if (self->field_0x68.data == &g_unk_7101d47108->empty)
        return false;
    UnkInlineString_A2_b72 str;
    fn_7100395044(self, &self->field_0x68, &str);
    return fn_710007f120(reinterpret_cast<const UnkCompared_0x30*>(str.str),
                         reinterpret_cast<const UnkCompared_0x30*>(self->field_0x38));
}
