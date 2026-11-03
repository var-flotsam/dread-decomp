// CTakumakuAIComponent unit (tools/tu_map.py unit 1079): fn_71006d4304 fills a table (fn_71006d4fdc, one
// key and one value per call): eight keys taken from +0x30 of entries of this unit's static table
// (g_unk_7172709b78; entries 4, 5, 15, 16, 18, 19, 0 and 1), then two keys from .rodata, each with its
// value from .data. Names and layouts are placeholders.
#include "types.h"

struct UnkTableEntry_tk {
    u8 pad_0x0[0x30];
    u64 key;
};

void fn_71006d4fdc(void* self, const u64* key __attribute__((noescape)), const u32* value);
extern UnkTableEntry_tk* g_unk_7172709b78[] __attribute__((visibility("hidden")));
extern const u64 g_unk_710164a7d8 __attribute__((visibility("hidden")));
extern const u64 g_unk_710164a7e8 __attribute__((visibility("hidden")));
extern const u32 g_unk_7101a67dd0;
extern const u32 g_unk_7101a67dd4;
extern const u32 g_unk_7101a67dd8;
extern const u32 g_unk_7101a67ddc;
extern const u32 g_unk_7101a67de0;
extern const u32 g_unk_7101a67de4;
extern const u32 g_unk_7101a67dec;
extern const u32 g_unk_7101a67df0;
extern const u32 g_unk_7101a67df4;
extern const u32 g_unk_7101a67df8;

static inline __attribute__((always_inline)) void Add_tk(void* self, u32 index, const u32* value) {
    u64 key = g_unk_7172709b78[index]->key;
    fn_71006d4fdc(self, &key, value);
}

void fn_71006d4304(void* self) {
    Add_tk(self, 4, &g_unk_7101a67dd8);
    Add_tk(self, 5, &g_unk_7101a67ddc);
    Add_tk(self, 15, &g_unk_7101a67dec);
    Add_tk(self, 16, &g_unk_7101a67df0);
    Add_tk(self, 18, &g_unk_7101a67df4);
    Add_tk(self, 19, &g_unk_7101a67df8);
    Add_tk(self, 0, &g_unk_7101a67dd0);
    Add_tk(self, 1, &g_unk_7101a67dd4);
    fn_71006d4fdc(self, &g_unk_710164a7e8, &g_unk_7101a67de4);
    fn_71006d4fdc(self, &g_unk_710164a7d8, &g_unk_7101a67de0);
}
