// CTimelineComponent unit (tools/tu_map.py): the unit's static initialiser (game/UnkTUInit.h): the
// header data, the CTimelineComponent::ENextPolicy enum type with its converters, the class list
// entry, and a pool of 500 blocks of 0x178 bytes (fn_7100a2ddec) destroyed at exit. Names and
// layouts are placeholders.
#include "base/UnkBlockArrayPool.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUInit.h"

// The unit's data: the header, two floats and a flag.
struct UnkTimelineTUData_ovb0089 {
    UnkTUHeader header;
    u8 unk_0x18[0x8];
    f32 field_0x20;
    f32 field_0x24;
    bool field_0x28;
};

extern UnkTimelineTUData_ovb0089 g_unk_7172746f68 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_7172746d98;  // CTimelineComponent::ENextPolicy
void fn_7100a29e60();
void fn_7100a2a00c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_7172746eb0;
void fn_7100a2d660() __attribute__((visibility("hidden")));
void fn_7100a2d66c() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_7172746eb8;
extern UnkConverter g_unk_7172746ed8;
extern char g_unk_7172746ef8;
extern char g_unk_7172746f98 __attribute__((visibility("hidden")));
extern UnkBlockArrayPool g_unk_7172746f00;
void fn_7100a2ab14(void* pool);
void fn_7100a2ddec(UnkBlockArrayPool* self, u64 n, u32 flags, base::UnkAllocator* allocator);

void fn_7100a2ea20() {
    UnkTimelineTUData_ovb0089& data = g_unk_7172746f68;
    InitHeader(data.header);
    data.field_0x20 = -1.0f;
    data.field_0x24 = -1.0f;
    data.field_0x28 = true;
    InitEnum(&g_unk_7172746d98, "CTimelineComponent::ENextPolicy", fn_7100a2a00c, fn_7100a29e60);
    fn_710009ec00(&g_unk_7172746eb0, &g_unk_7172746d98);
    InitEnumConverters(&g_unk_7172746d98, g_unk_7172746eb8, fn_7100a2d660, g_unk_7172746ed8, fn_7100a2d66c);
    fn_7100a2a190();
    fn_710009ec00(&g_unk_7172746ef8, &g_unk_7172746f98);
    UnkBlockArrayPool* pool = &g_unk_7172746f00;
    pool->allocator = nullptr;
    fn_710008377c(&pool->list);
    new (&pool->entries) base::UnkVector<UnkBlockArrayEntry>(0xf);
    fn_7100a2ddec(pool, 500, 0x1c, nullptr);
    fn_7100000250(fn_7100a2ab14, pool, &g_unk_7101a19000);
}
