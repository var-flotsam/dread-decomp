// Dropped-item manager (unit 2562, CLogicDroppedItemManager::CTunableDroppableBillboard): sets up the
// yellow X-Parasite billboard, one entry with its id, colours and counts, and points its three models at
// the tunable's X-Parasite name under each prefix. Names and layouts are placeholders.
#include "base/UnkNoArg.h"
#include "base/UnkPathIdsFromText.h"
#include "base/UnkStrIdPathPair.h"
#include "base/color/CColor4F.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CDropLayout.h"
#include "game/CLogicDroppedItemManager/CTunableDroppableBillboard.h"
#include "game/UnkDroppableTunable.h"
#include "game/UnkTU2561.h"
#include "game/UnkTU2562.h"
#include "game/UnkTU25.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::color::CColor4F;
using base::global::CRntString;
using base::global::CStrId;

struct UnkBillboardManager_e2a734 {
    u8 pad_0x0[0x780c];
    u32 field_0x780c;
    u8 pad_0x7810[0x7880 - 0x7810];
    CDrop* current;  // 0x7880
    u8 pad_0x7888[0x9b20 - 0x7888];
    CDrop billboard;  // 0x9b20
};

static inline CRntString operator+(const CRntString& a, const CRntString& b) { return fn_710007eee0(&a, &b); }

// Points a drop's model record at the ids parsed from a path (the path is built first). The record is the
// id/path/resource triple fn_71000267f8 is declared over (UnkStrIdPathResource, base/UnkStrIdPathPair.h),
// which CDrop's layout still types as UnkDropResource.
static inline void LoadPath(const CRntString& path, UnkDropResource* model) {
    fn_71000267f8(reinterpret_cast<UnkStrIdPathResource*>(model), UnkPathIdsFromText(path).ids);
}

static inline void Show(CDrop* billboard, const CStrId& id, void* owner, f32 count, const UnkDropTable& set) {
    billboard->item = id;
    billboard->owner = owner;
    billboard->callback = fn_7100e2fa00;
    billboard->amount = count;
    billboard->table = set;
    g_unk_717269c4d8 = true;
    billboard->Start();
    g_unk_717269c4d8 = false;
}

void fn_7100e2a734(UnkBillboardManager_e2a734* self) {
    UnkDropTable set;
    set.count = 1;

    UnkDropTableEntry& entry = set.entries[0];
    entry.field_0x30 = 0;
    entry.field_0x38 = self->field_0x780c;
    {
        CStrId id("X-Parasite Yellow", true);
        entry.id.ReleaseAndTake(id);
    }
    entry.field_0x8 = g_unk_71727c1088[6];
    entry.field_0x3c = -1;
    entry.field_0x34 = g_unk_71727c10c8.fMiniXYellowMotionScale;
    entry.color = CColor4F(1.0f, 1.0f, 0.0f, 1.0f);
    entry.field_0x20 = CColor4F(0.0f, 0.0f, 0.0f, 0.0f);
    fn_7100e2c124(0.5f, UnkNoArg(), reinterpret_cast<UnkDropTarget_ovb0208*>(&g_unk_71727c1078), &entry.id, 0x10, 0x10,
                  0x10, 0, 0, true);

    CDrop* billboard = &self->billboard;
    {
        UnkDropTable copy = set;
        Show(billboard, g_unk_71727c1078, self, g_unk_71727c10c8.fMiniXLifeAmount, copy);
    }
    billboard->mode = 2;
    const CRntString& name = g_unk_71727c10c8.sMiniXFlyPresetName;
    LoadPath(g_unk_71727c13c8[0] + name + g_unk_71727c13c8[2], &billboard->field_0x4a0);
    LoadPath(g_unk_71727c13c8[0] + name + g_unk_71727c13c8[3], &billboard->field_0x4b8);
    LoadPath(g_unk_71727c13c8[0] + name + g_unk_71727c13c8[4], &billboard->field_0x4d0);
    billboard->field_0x4e8 = true;
    self->current = billboard;
}
