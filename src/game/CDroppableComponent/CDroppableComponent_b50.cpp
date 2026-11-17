// CDroppableComponent unit (tools/tu_map.py unit 1216). fn_71007ba484 resets the component: the
// base reset (fn_71010c4228), cleared state, the empty sound handle at +0x78, and the id and path
// pair at +0x88 assigned from the unit's path text (g_unk_717271ab80, built by fn_710001f074).
// Names and layouts are placeholders.
#include "base/UnkPathIdsFromText.h"
#include "base/UnkStrIdPathPair.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CComponent.h"
#include "game/UnkTU101.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"


struct UnkParent;

struct UnkSoundRef;

extern base::global::CRntString g_unk_717271ab80 __attribute__((visibility("hidden")));

struct UnkDroppable_b50 {
    u8 pad_0x0[0x44];
    u32 field_0x44;
    u32 field_0x48;
    bool field_0x4c;
    u8 pad_0x4d[0x3];
    u64 field_0x50;
    u64 field_0x58;
    bool field_0x60;
    u8 pad_0x61[0xf];
    u32 field_0x70;
    u8 pad_0x74[0x4];
    UnkSoundHandle sound;       // 0x78
    UnkStrIdPathResource path;  // 0x88
};

void fn_71007ba484(UnkDroppable_b50* self) {
    fn_71010c4228(reinterpret_cast<UnkParent*>(self));
    self->field_0x4c = false;
    self->field_0x44 = 0;
    self->field_0x48 = 0;
    self->field_0x50 = 0;
    self->field_0x58 = 0;
    self->sound = g_unk_7108b6c460;
    fn_71000267f8(&self->path, UnkPathIdsFromText(g_unk_717271ab80).ids);
    self->field_0x60 = false;
    self->field_0x70 = 0;
}
