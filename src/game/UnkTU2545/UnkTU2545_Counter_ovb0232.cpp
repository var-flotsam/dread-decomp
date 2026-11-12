// Unit 2545 of tools/tu_map.py: refreshes a counter panel: its count text with a sound when non-zero, then
// the panel children's alpha. Names and layouts are placeholders.
#include <stdio.h>

#include "base/global/timeline/CEvent.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiLabel.h"
#include "game/UnkTU0.h"
#include "game/UnkTU32.h"
#include "game/sound/CSoundManager.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

struct UnkStrIdPathRecord_b48 {
    u8 data[0x18];
};
struct UnkIdPair;
struct UnkStrIdPathPair;
struct UnkDropTarget;
struct UnkSoundRef;

extern UnkStrIdPathRecord_b48 g_unk_71727bdfb0 __attribute__((visibility("hidden")));
struct UnkWideStr_0x20;
using UnkGuiObj = UnkGuiDisplayObject;
void fn_71011cfba8(UnkGuiObj* object, f32 alpha);

struct UnkCountText_b0232 {
    UnkGuiText text;
    explicit UnkCountText_b0232(const char* str) { fn_7100066b38(reinterpret_cast<UnkWideStr_0x20*>(this), str); }
    ~UnkCountText_b0232() { fn_7100066e44(this); }
};

struct UnkCopiedIds_b0232 {
    UnkStrIdPathRecord_b48 record;
    UnkCopiedIds_b0232(const UnkStrIdPathRecord_b48* src) { fn_7100027464(&record, src); }
    ~UnkCopiedIds_b0232() { fn_7100026f70(reinterpret_cast<UnkIdPair*>(this)); }
};

struct UnkCounterPanel_b0232 {
    u8 pad_0x0[0xbc];
    bool field_0xbc;
    u8 pad_0xbd[0xf];
    u32 count;  // 0xcc
    u8 pad_0xd0[0x10];
    bool field_0xe0;
    u8 pad_0xe1[0x57];
    UnkGuiDisplayObject* field_0x138;
    UnkGuiDisplayObject* field_0x140;
    UnkGuiDisplayObject* field_0x148;
    UnkGuiDisplayObject* field_0x150;
};
static_assert(__builtin_offsetof(UnkCounterPanel_b0232, field_0x150) == 0x150, "layout");

void fn_7100dd686c(UnkCounterPanel_b0232* self) {
    if (self->field_0xe0) {
        char buf[0x80];
        sprintf(buf, "%u", self->count);
        {
            UnkCountText_b0232 text(buf);
            fn_71011cdce8(self->field_0x140, false);
            fn_71011cdc30(self->field_0x140, false);
            fn_71011ec468(self->field_0x140, text.text, false);
            fn_71011cdce8(self->field_0x140, true);
            fn_71011cdc30(self->field_0x140, true);
            if (self->count) {
                UnkDropTarget* target = g_unk_7101d47318;
                UnkCopiedIds_b0232 ids(&g_unk_71727bdfb0);
                fn_7101024e28(target, reinterpret_cast<const UnkStrIdPathPair*>(&ids), nullptr, false);
            }
        }
    }
    if (!self->field_0xbc) {
        fn_71011cfba8(self->field_0x148, 1.0f);
        if (self->field_0x150)
            fn_71011cfba8(self->field_0x150, 1.0f);
        if (self->field_0xe0) {
            fn_71011cfba8(self->field_0x140, 1.0f);
            fn_71011cfba8(self->field_0x138, 1.0f);
        }
    }
}
