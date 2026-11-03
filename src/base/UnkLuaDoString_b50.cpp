// From unit 2668 of tools/tu_map.py (0x710109c64c-0x710109d670), next to src/base/UnkLuaDoString.cpp:
// resolves a name into a string with inline storage (fn_710109c6d0) and, when it is not empty,
// runs fn_710109d0e0 on it. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "types.h"

using base::global::CRntString;

// A string with 0x100 bytes of inline storage (0x148 bytes), as in src/base/UnkTunableRegistry.cpp.
struct UnkInlineString_710109d390 {
    u8 str[0x30];
    char* storage_vtable;  // +0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    UnkInlineString_710109d390() {
        fn_710007e6e4(Str(), reinterpret_cast<base::UnkAllocator*>(&storage_vtable));
        storage_vtable = reinterpret_cast<char*>(g_unk_7101a194b8 + 2);
        field_0x138 = 0;
        field_0x140 = 0;
    }
    ~UnkInlineString_710109d390() {
        fn_710007ed40(Str());
        fn_710007ecd4(Str());
    }
};

void fn_710109c6d0(void* name, UnkInlineString_710109d390* out);
bool fn_710109d0e0(UnkInlineString_710109d390* str, void* arg, bool a, bool b);

bool fn_710109d390(void* name, void* arg, bool a, bool b) {
    UnkInlineString_710109d390 str;
    fn_710109c6d0(name, &str);
    const char* data = str.Str()->data;
    if (!data || !data[0])
        return false;
    return fn_710109d0e0(&str, arg, a, b);
}
