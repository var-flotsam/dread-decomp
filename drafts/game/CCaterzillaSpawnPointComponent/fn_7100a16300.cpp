// CCaterzillaSpawnPointComponent unit (tools/tu_map.py unit 1527). fn_7100a16300: after the base spawn
// point's step, resets the wave state and resolves the linked spawn point (wpSpawnPointLinked); by the
// spawn order, one of the pair leads and the other follows. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CGameLink.h"
#include "game/CSpawnPointComponent.h"
#include "game/UnkTU2704.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct CActor;

struct UnkRefl_fn_7100a16450;
extern template base::reflection::UnkReflObject*
base::reflection::refl_FindTypedHidden<UnkRefl_fn_7100a16450>(void*);

struct UnkCaterzillaSpawnPoint_la16300 : CSpawnPointComponent {
    s32 eSpawnDir;     // 0x3b8
    s32 eSpawnOrder;   // 0x3bc
    u32 field_0x3c0;
    u32 field_0x3c4;
    u32 NumCaterzillas;  // 0x3c8
    u32 field_0x3cc;
    bool field_0x3d0;
    bool bInOutSpawnPoint;  // 0x3d1
    u8 pad_0x3d2[0x2];
    f32 fTimeToGenerateNextWave;  // 0x3d4
    u32 field_0x3d8;
    bool field_0x3dc;
    bool field_0x3dd;
    bool field_0x3de;
    bool field_0x3df;
    CGameLink<CActor> wpSpawnPointLinked;  // 0x3e0
    UnkCaterzillaSpawnPoint_la16300* linked;  // 0x420
    f32 fTimeToRespawnAllCaterzillas;      // 0x428
    u32 field_0x42c;
    bool field_0x430;
    u8 pad_0x431[0x33];
    u32 field_0x464;
};
static_assert(__builtin_offsetof(UnkCaterzillaSpawnPoint_la16300, wpSpawnPointLinked) == 0x3e0, "layout");
static_assert(__builtin_offsetof(UnkCaterzillaSpawnPoint_la16300, field_0x464) == 0x464, "layout");

// Makes this spawn point lead the pair (or follow it), and the linked one the opposite.
static inline void SetLead(UnkCaterzillaSpawnPoint_la16300* self, bool lead) {
    self->field_0x3d0 = lead;
    self->field_0x3dc = lead;
    self->linked->field_0x3d0 = !lead;
    self->linked->field_0x3dc = !lead;
    self->linked->eSpawnOrder = lead;
    self->field_0x3de = lead;
    self->linked->field_0x3de = !lead;
}

static inline void MarkPaired(UnkCaterzillaSpawnPoint_la16300* self) {
    self->field_0x3df = true;
    self->linked->field_0x3df = true;
}

static inline void ResetWave(UnkCaterzillaSpawnPoint_la16300* self) {
    self->field_0x3dd = false;
    self->field_0x3d8 = 0;
    self->field_0x42c = 0;
    self->field_0x464 = 0;
    self->field_0x430 = false;
}

void fn_7100a16300(UnkCaterzillaSpawnPoint_la16300* self) {
    fn_710112b7c4(self);
    u32 count = self->NumCaterzillas;
    self->field_0x3c4 = count;
    self->field_0x3c0 = count;
    self->field_0x3cc = 0;
    CGameLink<CActor>& link = self->wpSpawnPointLinked;
    if (link.state == 1) {
        {
            UnkRefPtr ref = fn_710023b580(&link.callbacks);
            fn_7100234f78(&link.handle, reinterpret_cast<const base::UnkRefHandle*>(&ref));
        }
        link.state = 2;
    }
    if (link.Target())
        self->linked = reinterpret_cast<UnkCaterzillaSpawnPoint_la16300*>(
            base::reflection::refl_FindTypedHidden<UnkRefl_fn_7100a16450>(link.Target()));
    else
        self->linked = nullptr;

    if (!self->linked) {
        // Nothing linked: this spawn point is the whole chain.
        self->field_0x3de = true;
        self->field_0x3d0 = true;
        self->field_0x3dc = true;
        self->field_0x3df = false;
        ResetWave(self);
        return;
    }
    switch (self->eSpawnOrder) {
    case 1:
        SetLead(self, false);
        break;
    case 0:
        SetLead(self, true);
        break;
    case 2:
        self->field_0x3de = false;
        self->field_0x3d0 = false;
        self->field_0x3dc = false;
        self->field_0x3df = false;
        ResetWave(self);
        return;
    }
    MarkPaired(self);
    ResetWave(self);
}
