// Unit 426 of tools/tu_map.py: the getter of the unit's singleton (the object src/game/UnkUnit426/UnkUnit426_Dtor.cpp
// destroys; the global is the resource manager fn_7100b2de48 loads through). Creates it on first use (0x98 bytes:
// the first base's vtable, the second base at +0x8 with its member at +0x10 (fn_71002f75ac) and a cleared
// word, then the final vtables, the cleared buffer pair at +0x68, +0x78, growth 10 / flags 14, the default
// allocator and an empty resource), then hands it to fn_71002f66c8. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkTU426.h"
#include "types.h"

struct UnkResourceManager;
extern UnkResourceManager* g_unk_71726ae8a8;
extern char g_unk_7101a222e8[];
extern void* g_unk_7101a22398[];
extern void* g_unk_7101a22378[];
void fn_71002f66c8(UnkResourceManager* self);

struct UnkUnit426Instance_A2_b74 {
    void* vtable;
    void* vtable2;  // 0x8, the second base
    u8 member[0x50];
    u32 field_0x60;
    u64 field_0x68;
    u64 field_0x70;
    u64 field_0x78;
    u32 growth;  // 0x80
    u32 flags;   // 0x84
    base::UnkAllocator* allocator;
    void* resource;
};
static_assert(sizeof(UnkUnit426Instance_A2_b74) == 0x98, "size");

void fn_71002f6620() {
    UnkResourceManager* instance = g_unk_71726ae8a8;
    if (!instance) {
        auto* p = static_cast<UnkUnit426Instance_A2_b74*>(fn_71000839a0(0x98, 0x3f));
        p->vtable = g_unk_7101a22378 + 2;
        p->vtable2 = g_unk_7101a22398 + 2;
        fn_71002f75ac(p->member, nullptr, 0xe);
        p->field_0x60 = 0;
        p->vtable = g_unk_7101a222e8 + 0x10;
        p->vtable2 = g_unk_7101a222e8 + 0x40;
        p->field_0x68 = 0;
        p->field_0x70 = 0;
        p->field_0x78 = 0;
        p->growth = 10;
        p->flags = 14;
        p->allocator = fn_7100083990();
        p->resource = nullptr;
        instance = reinterpret_cast<UnkResourceManager*>(p);
        g_unk_71726ae8a8 = instance;
    }
    fn_71002f66c8(instance);
}
