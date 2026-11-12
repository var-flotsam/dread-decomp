// CSpawnGroupComponent unit: destroys the 0x40-byte polymorphic elements at +0x68 (vtable slot 0),
// unregisters the component from the game manager under one key (only while the flag at +0x140 is
// set, which it then clears) and another (fn_7100de9730), then runs fn_71010c4230. Names and
// layouts are placeholders.
#include "game/CComponent.h"
#include "game/CSpawnGroupComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkHandleOwner_71010c4230;

struct UnkSpawnGroupKey;
extern UnkSpawnGroupKey g_unk_7172a067b8 __attribute__((visibility("hidden")));

struct UnkSpawnGroupElem {
    void** vtable;
    u64 words[7];
};

struct UnkSpawnGroupRelease {
    u8 pad_0x0[0x68];
    UnkSpawnGroupElem* data;  // 0x68
    u32 count;                // 0x70
    u8 pad_0x74[0xcc];
    bool field_0x140;
};

void fn_7101119f4c(UnkSpawnGroupRelease* self) {
    for (u32 i = 0; i < self->count; ++i)
        reinterpret_cast<void (*)(UnkSpawnGroupElem*)>(self->data[i].vtable[0])(&self->data[i]);
    self->count = 0;
    if (self->field_0x140) {
        fn_7100de9730(g_unk_7101d47310, &g_unk_7172a067b8, self);
        self->field_0x140 = false;
    }
    fn_7100de9730(g_unk_7101d47310, &g_unk_7172a067c0, self);
    fn_71010c4230(reinterpret_cast<UnkHandleOwner_71010c4230*>(self));
}
