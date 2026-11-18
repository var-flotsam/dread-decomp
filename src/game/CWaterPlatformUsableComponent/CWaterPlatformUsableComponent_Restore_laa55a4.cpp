// CWaterPlatformUsableComponent's unit (tools/tu_map.py): after the base step, when the navigation manager
// knows "SP_Emmy", hands every item of every source of the owner's navmesh item to the block at +0x5c8
// (fn_71002b2bac); then the component reads the game blackboard (vtable slot 0x140). Names and layouts are
// placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/CUsableComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU372.h"
#include "types.h"

using base::global::CStrId;

struct UnkTeleporterSelf_b0233;
struct UnkWeakTarget;

static inline UnkNavMeshManager* NavMeshManager() {
    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    return manager;
}

// A source of the navmesh item: the items it lists.
struct UnkWaterPlatformSource_laa55a4 {
    u8 pad_0x0[0x10];
    void** items;  // 0x10
    u32 count;     // 0x18

    void** begin() { return items; }
    void** end() { return items + count; }
};

struct UnkWaterPlatformNavItem_laa55a4 {
    u8 pad_0x0[0x150];
    u32 source_count;  // 0x150
};

UnkWaterPlatformSource_laa55a4* fn_71009c0bbc(UnkWaterPlatformNavItem_laa55a4* self, u32 index);
void fn_71002b2bac(void* block, void* item);

struct UnkWaterPlatformUsable_laa55a4 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkWeakTarget* owner;  // 0x30
    u8 pad_0x38[0x590];
    u8 block[8];  // 0x5c8

    void vf_0x140(CBlackboard* blackboard) {
        reinterpret_cast<void (*)(UnkWaterPlatformUsable_laa55a4*, CBlackboard*)>(vtable[0x140 / 8])(this,
                                                                                                    blackboard);
    }
};
static_assert(__builtin_offsetof(UnkWaterPlatformUsable_laa55a4, block) == 0x5c8, "layout");

void fn_7100aa55a4(UnkWaterPlatformUsable_laa55a4* self) {
    fn_7100a9f300(reinterpret_cast<UnkTeleporterSelf_b0233*>(self));
    if (auto* nav = static_cast<UnkWaterPlatformNavItem_laa55a4*>(fn_71002b3b84(self->owner))) {
        static CStrId s_emmy("SP_Emmy", true);
        if (fn_7100ec1344(NavMeshManager(), &s_emmy)) {
            for (u32 i = 0; i < nav->source_count; ++i) {
                UnkWaterPlatformSource_laa55a4* source = fn_71009c0bbc(nav, i);
                if (!source)
                    continue;
                for (void* item : *source)
                    fn_71002b2bac(self->block, item);
            }
        }
    }
    self->vf_0x140(&g_unk_7101d47310->field_0x968);
}
