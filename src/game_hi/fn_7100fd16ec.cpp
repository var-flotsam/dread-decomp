// Flushes a pending request: if the dirty flag is set, clears it, submits the two stored
// requests to the manager singleton with the full mask, empties both path lists and resets
// the two string ids to the global default and the priority to the maximum.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkPooledId.h"
#include "game/CBossAIComponent.h"
#include "game/UnkMask.h"
#include "game/UnkPathEntry_0x18.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/UnkMgr_0xf18.h"

struct UnkRequest_7100fd16ec {
    u8 data[0xc];
};

static inline UnkMgr_0xf18* GetMgr() {
    if (!g_unk_71726bc560) {
        UnkMgr_0xf18* m = static_cast<UnkMgr_0xf18*>(fn_71000839a0(0xf18, 0x3f));
        fn_7100fd3804(m);
        g_unk_71726bc560 = m;
    }
    return g_unk_71726bc560;
}

struct UnkStruct_0x150 {
    u8 pad_0x0[0x30];
    bool field_0x30;
    u8 pad_0x31[0x57];
    UnkRequest_7100fd16ec field_0x88;
    f32 field_0x94;
    u8 pad_0x98[0x18];
    base::global::CRntVector<UnkPathEntry_0x18> field_0xb0;
    u8 pad_0xd8[0x8];
    base::global::CStrId field_0xe0;
    UnkRequest_7100fd16ec field_0xe8;
    f32 field_0xf4;
    u8 pad_0xf8[0x18];
    base::global::CRntVector<UnkPathEntry_0x18> field_0x110;
    u8 pad_0x138[0x8];
    base::global::CStrId field_0x140;
    u32 field_0x148;
};

void fn_7100fd16ec(UnkStruct_0x150* self, u32, u32) {
    if (!self->field_0x30)
        return;
    self->field_0x30 = false;
    fn_7100fd7c3c(GetMgr(), 0, &self->field_0x88, self->field_0x94, UnkMask(2), UnkMaskOf<UnkMusicMaskTag>::All());
    fn_7100fd7c3c(GetMgr(), 1, &self->field_0xe8, self->field_0xf4, UnkMask(2), UnkMaskOf<UnkMusicMaskTag>::All());
    self->field_0xb0.Clear();
    self->field_0x110.Clear();
    self->field_0xe0 = base::global::CStrId::Empty();
    self->field_0x140 = base::global::CStrId::Empty();
    self->field_0x148 = 0x7fffffff;
}
