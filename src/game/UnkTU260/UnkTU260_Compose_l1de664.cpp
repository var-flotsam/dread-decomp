// Unit 260 (render passes): a pass that binds four targets and draws, then binds a fifth and draws again
// with five textures bound in slots 0 to 4. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkBindList_b0237.h"
#include "game/UnkTU177.h"
#include "game/UnkTU179.h"
#include "game/UnkTU182.h"
#include "game/UnkTU259.h"
#include "game/UnkU64P_b0237.h"
#include "types.h"

using base::global::CStrId;

extern CStrId g_unk_716e5961b0 __attribute__((visibility("hidden")));
extern CStrId g_unk_716e5961b8 __attribute__((visibility("hidden")));

void fn_710015c3f0(UnkBoundTarget* target);
void fn_71001d6a0c(UnkRenderTargetA1* target);
void fn_71001d6ad0(UnkRenderTargetA1* target);

static_assert(sizeof(UnkBindList_b0237) == 0x90, "UnkBindList_b0237 layout");

// The render target as this pass reads it: two counts of queued work.
struct UnkRenderTargetView_1de664 {
    u8 pad_0x0[0xe0148];
    u32 field_0xe0148;
    u8 pad_0xe014c[0xf0150 - 0xe014c];
    u32 field_0xf0150;
};

// The pass's owner as this function reads it.
struct UnkRenderPass_1de664 {
    u8 pad_0x0[0xf0];
    UnkRenderTargetA1* target;  // 0xf0
    u8 pad_0xf8[0x20];
    u8 block_0x118[0x148];  // 0x118
    UnkBoundTarget* slot_0x260;  // 0x260
    UnkBoundTarget* slot_0x268;  // 0x268
    UnkBoundTarget* slot_0x270;  // 0x270
    u8 pad_0x278[0x1e8];
    UnkBoundTarget* slot_0x460;  // 0x460
    UnkBoundTarget* slot_0x468;  // 0x468
    u8 pad_0x470[0x28];
    UnkBoundTarget* slot_0x498;  // 0x498
    u8 pad_0x4a0[0xae51 - 0x4a0];
    u8 flag_0xae51;  // 0xae51
};
static_assert(__builtin_offsetof(UnkRenderPass_1de664, slot_0x498) == 0x498, "UnkRenderPass_1de664 layout");
static_assert(__builtin_offsetof(UnkRenderPass_1de664, flag_0xae51) == 0xae51, "UnkRenderPass_1de664 layout");

void fn_71001de664(UnkRenderPass_1de664* self, bool first_extra, bool second_extra) {
    const auto* view = reinterpret_cast<const UnkRenderTargetView_1de664*>(self->target);
    // Nothing queued: the two counts cancel out.
    if (-view->field_0xe0148 == view->field_0xf0150)
        return;

    fn_71001cdd74(self->target, self->block_0x118);
    fn_71001cdd6c(self->target, g_unk_716e5961b0);
    UnkBindList_b0237 list;
    list.tail = fn_710015abd0();
    list.items[0].v = reinterpret_cast<u64>(self->slot_0x260);
    list.items[1].v = reinterpret_cast<u64>(self->slot_0x268);
    list.items[2].v = reinterpret_cast<u64>(self->slot_0x270);
    list.items[3].v = reinterpret_cast<u64>(self->slot_0x460);
    list.count = 4;
    fn_71001cdba4(self->target, &list);
    fn_71001cb604(self->target, 0, nullptr, nullptr, nullptr, nullptr);
    fn_710015a260(false);
    fn_71001ced08(self->target, self->flag_0xae51);
    fn_710015f770(9, fn_710015c450(self->slot_0x498), true);
    if (first_extra)
        fn_71001d6a0c(self->target);
    if (second_extra)
        fn_71001d6ad0(self->target);
    fn_71001cdd18(self->target);
    fn_71001cdd70(self->target);

    fn_710015c3f0(self->slot_0x460);
    fn_71001cdd6c(self->target, g_unk_716e5961b8);
    list.items[0].v = reinterpret_cast<u64>(self->slot_0x468);
    list.count = 1;
    list.tail = fn_710015abd0();
    fn_71001cdba4(self->target, &list);
    void* textures[5];
    textures[0] = fn_710015c450(self->slot_0x260);
    textures[1] = fn_710015c450(self->slot_0x268);
    textures[2] = fn_710015c450(self->slot_0x270);
    textures[3] = fn_710015c450(self->slot_0x498);
    textures[4] = fn_710015c450(self->slot_0x460);
    for (u32 i = 0; i < 5; ++i) {
        fn_710015f770(i, textures[i], true);
        fn_71001606f0(i, textures[i] != nullptr, true);
        fn_7100160ab0(i, 0, true);
        fn_7100160990(i, 0, true);
    }
    fn_71001cb604(self->target, 0xc, nullptr, nullptr, nullptr, nullptr);
    fn_71001ced08(self->target, self->flag_0xae51);
    if (first_extra)
        fn_71001d6a0c(self->target);
    if (second_extra)
        fn_71001d6ad0(self->target);
    fn_71001cdd18(self->target);
    fn_71001cdd70(self->target);
    fn_71001cde48(self->target);
}
