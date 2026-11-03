// Unit 36 of tools/tu_map.py: builds the unit's static object g_unk_7101d4fc30 in place (base part
// by fn_710002cff8, then its vtable), records it as the current instance in g_unk_7101d50288 and
// returns it. Names and layouts are placeholders.
#include "types.h"

struct UnkUnit36Static {
    char* vtable;
};

void fn_710002cff8(UnkUnit36Static* self);

extern UnkUnit36Static g_unk_7101d4fc30;
extern UnkUnit36Static* g_unk_7101d50288;
extern char g_unk_7101a19f60[];

UnkUnit36Static* fn_710002cbec() {
    fn_710002cff8(&g_unk_7101d4fc30);
    g_unk_7101d4fc30.vtable = g_unk_7101a19f60 + 0x10;
    g_unk_7101d50288 = &g_unk_7101d4fc30;
    return &g_unk_7101d4fc30;
}
