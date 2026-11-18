// CSamusMovement's unit (unit 1501): clears the movement's air state when it lands (jump bookkeeping, the
// speed floor from the run info), stops the gun, and lets the screw attack's spin jump track reset the
// animation. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/CInventoryComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGroundMovement_ovb0138.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU1501.h"
#include "types.h"

void fn_710083ec2c(void* gun);
void fn_710034eac8(void* object);

static inline const UnkQualifiedId& GetQualifiedId_l9ea070(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), base::global::CStrId::Empty());
    return qid;
}

void fn_71009ea070(UnkGroundMovement_ovb0138* self) {
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
