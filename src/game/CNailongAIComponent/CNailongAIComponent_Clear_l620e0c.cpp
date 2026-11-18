// CNailongAIComponent's unit (tools/tu_map.py 937): after fn_7100621020, empties the string-id lists
// at +0x14848 and +0x14898 and the id records at +0x14870, sends ids 4 and 8 to the owner's +0x220
// controller (fn_71011785a8) and runs the base step fn_71010ff2b8. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkPooledId.h"
#include "game/CAnimationComponent.h"
#include "game/UnkShakernautId.h"
#include "game/UnkTU2702.h"
#include "types.h"

using UnkId_l620e0c = UnkPooledId<&g_unk_7101d47108>;

struct UnkNailongAI_b59a4;
void fn_7100621020(UnkNailongAI_b59a4* self);

// A string id and 0x10 bytes of plain data.
struct UnkNailongIdRecord_l620e0c {
    UnkId_l620e0c id;
    u8 field_0x8[0x10];
};

struct UnkNailongOwner_l620e0c {
    u8 pad_0x0[0x220];
    UnkShakernautCtrl_b53* ctrl;  // 0x220
};

struct UnkNailongAI_l620e0c {
    u8 pad_0x0[0x30];
    UnkNailongOwner_l620e0c* owner;  // 0x30
    u8 pad_0x38[0x14848 - 0x38];
    base::global::CRntVector<UnkId_l620e0c> ids;                // 0x14848
    base::global::CRntVector<UnkNailongIdRecord_l620e0c> records;  // 0x14870
    base::global::CRntVector<UnkId_l620e0c> flag_ids;           // 0x14898
};
static_assert(__builtin_offsetof(UnkNailongAI_l620e0c, flag_ids) == 0x14898, "layout");

void fn_7100620e0c(UnkNailongAI_l620e0c* self) {
    UnkShakernautCtrl_b53* ctrl = self->owner->ctrl;
    fn_7100621020(reinterpret_cast<UnkNailongAI_b59a4*>(self));
    self->ids.Clear();
    self->records.Clear();
    self->flag_ids.Clear();
    UnkShakernautId_b53 first;
    first.value = 4;
    fn_71011785a8(ctrl, &first, true, false);
    UnkShakernautId_b53 second;
    second.value = 8;
    fn_71011785a8(ctrl, &second, true, false);
    fn_71010ff2b8(reinterpret_cast<char*>(self));
}
