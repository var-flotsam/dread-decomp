// CSamusMovement's unit (unit 1501): clears the movement's air state when it lands (jump bookkeeping, the
// speed floor from the run info), stops the gun, and lets the screw attack's spin jump track reset the
// animation. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/CInventoryComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkItemProperties.h"
#include "types.h"

struct UnkInventory;

void fn_710083ec2c(void* gun);
void fn_710034eac8(void* object);

static inline const UnkQualifiedId& GetQualifiedId_l9ea070(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

struct UnkRunInfo_l9ea070 {
    u8 pad_0x0[0xc0];
    f32 field_0xc0;  // 0xc0
    u32 field_0xc4;  // 0xc4
};

struct UnkActor_l9ea070 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;  // 0x220
    u8 pad_0x228[0x48];
    UnkInventory* inventory;  // 0x270
    u8 pad_0x278[0x30];
    void* gun;  // 0x2a8
    void* field_0x2b0;  // 0x2b0
};

struct UnkMovement_l9ea070 {
    u8 pad_0x0[0x30];
    UnkActor_l9ea070* actor;  // 0x30
    u8 pad_0x38[0x1c8];
    f32 field_0x200;  // 0x200
    u8 pad_0x204[0x5c4];
    u32 field_0x7c8;  // 0x7c8
    u8 pad_0x7cc[0x1ac4];
    u32 field_0x2290;  // 0x2290
    u8 pad_0x2294[0x4];
    f32 field_0x2298;  // 0x2298
    u8 pad_0x229c[0x6c];
    u8 field_0x2308;  // 0x2308
    u8 pad_0x2309[0x1b];
    u8 field_0x2324;  // 0x2324
    u8 field_0x2325;  // 0x2325
    u8 pad_0x2326[0x2];
    u64 field_0x2328;  // 0x2328
    u8 pad_0x2330[0xc];
    u32 field_0x233c;  // 0x233c
    u8 pad_0x2340[0x3118];
    UnkRunInfo_l9ea070* info;  // 0x5458
};
static_assert(__builtin_offsetof(UnkMovement_l9ea070, field_0x200) == 0x200, "layout");
static_assert(__builtin_offsetof(UnkMovement_l9ea070, field_0x7c8) == 0x7c8, "layout");
static_assert(__builtin_offsetof(UnkMovement_l9ea070, info) == 0x5458, "layout");

void fn_71009ea070(UnkMovement_l9ea070* self) {
    UnkRunInfo_l9ea070* info = self->info;
    self->field_0x233c = 0;
    self->field_0x2328 = 0;
    self->field_0x7c8 = 0;
    self->field_0x2290 = info->field_0xc4;
    self->field_0x2298 = -1.0f;
    self->field_0x2308 = 0;
    self->field_0x2324 = 0;
    self->field_0x2325 = 0;
    f32 current = self->field_0x200;
    f32 floor = info->field_0xc0 + 1.1920929e-7f;
    self->field_0x200 = current > floor ? current : floor;
    UnkActor_l9ea070* actor = self->actor;
    if (actor->gun)
        fn_710083ec2c(actor->gun);
    if (fn_71011428bc(self->actor->inventory, GetQualifiedId_l9ea070(g_unk_717275a918))) {
        if (UnkAnimationComponent* anim = self->actor->field_0x220)
            fn_710117b348(anim, fn_7100d272e8());
    }
    if (self->actor->field_0x2b0)
        fn_710034eac8(self->actor->field_0x2b0);
}
