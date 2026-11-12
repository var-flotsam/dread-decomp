// Near gui/scripts/msemenu_ingame.lua's strings: takes the base name of the object's path (+0x58,
// the text between the last '/' and the last '.'), calls the script function
// "<name>.OnLuaLibrariesReloaded", then fn_71011b6b5c on the GUI root. Names and layouts are
// placeholders.
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2744.h"
#include "game/UnkTU57.h"

using base::global::CRntString;

// A string with 0x100 bytes of inline storage (the string part is 0x30 bytes, followed by its
// inline allocator).
struct UnkInlineString_710118b384 {
    u8 str[0x30];
    void** vtable;
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;
};

struct UnkObj_710118b384 {
    u8 unk_0x0[0x58];
    CRntString path;
};

void fn_710118b384(UnkObj_710118b384* self) {
    CRntString* path = &self->path;
    if (!path->data || !path->data[0])
        return;
    u32 start = fn_710007fb10(path, '/', -1) + 1;
    s32 end = fn_710007fb10(path, '.', -1);
    if (end == -1)
        end = self->path.length;
    const char* text = path->data + start;
    u32 length = end - start;
    UnkInlineString_710118b384 s;
    auto* str = reinterpret_cast<CRntString*>(&s);
    fn_710007e6e4(str, reinterpret_cast<base::UnkAllocator*>(&s.vtable));
    s.vtable = &g_unk_7101a194b8[2];
    s.field_0x138 = 0;
    s.field_0x140 = 0;
    fn_710007e7f8(str, text, length);
    fn_710007f564(str, ".OnLuaLibrariesReloaded");
    fn_7100000790(str->data, "");
    fn_71011b6b5c(g_unk_717271e8f8, 0);
    fn_710007ed40(str);
    fn_710007ecd4(str);
}
