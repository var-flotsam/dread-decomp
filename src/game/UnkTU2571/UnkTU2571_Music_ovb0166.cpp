// Unit 2571 of tools/tu_map.py (no class hint): two small widgets. A list of music entries (count at +0x8)
// that can be stopped (fn_7100ec9a24) and started (fn_7100ec9640: its toggle, labels and music mode, via the
// music manager created on first use as src/game/GUI/CPowerDescriptionComposition/
// CPowerDescriptionComposition_Show_ovb0087.cpp), and a store of counters (enabled flag at +0x8) kept in the game manager's
// blackboard (fn_7100ec78e8 writes true under a name). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "engine/scene/UnkToggle.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2571.h"
#include "game/UnkTU2578.h"
#include "game/UnkTUProperty.h"
#include "types.h"
#include "game/UnkTypedValue.h"

using base::global::CRntString;
using base::global::CStrId;

// ---- the music manager (as the composition sources declare it) ----

struct UnkStruct_0x128;

static inline UnkStruct_0x128* Music() {
    UnkStruct_0x128* m = g_unk_7101d47320;
    if (!m) {
        m = static_cast<UnkStruct_0x128*>(fn_71000839a0(0x150, 0x3f));
        fn_7100ef3494(m);
        g_unk_7101d47320 = m;
    }
    return m;
}

// ---- the music list ----

struct UnkMusicEntry_b0166 {
    u32 mode;
    u32 field_0x4;
};

struct UnkGuiLabel_b0166 {
    u8 pad_0x0[0x13c];
    u32 field_0x13c;
    u8 pad_0x140[0x46];
    u8 field_0x186;
};

struct UnkToggleHolder_b0166 {
    u8 pad_0x0[0x18];
    u8 toggle[0xf9];  // 0x18
};

struct UnkMusicList_b0166 {
    u8 pad_0x0[8];
    u32 count;  // 0x8
    u8 pad_0xc[0x1c];
    u64 field_0x28;
    UnkToggleHolder_b0166* holder;  // 0x30
    UnkGuiDisplayObject* field_0x38;
    UnkGuiDisplayObject* field_0x40;
    u32 field_0x48;
    u8 pad_0x4c[0xc];
    bool field_0x58;
    bool field_0x59;
};

UnkMusicEntry_b0166* fn_7100ec9780(UnkMusicList_b0166* self, u32 index);
using UnkGuiObj = UnkGuiDisplayObject;
void fn_71011ec424(UnkGuiObj* label, const CRntString& text);

void fn_7100ec9a24(UnkMusicList_b0166* self) {
    for (u32 i = 0; i < self->count; ++i) {
        UnkMusicEntry_b0166* entry = fn_7100ec9780(self, i);
        fn_7100f011c4(Music(), entry->mode);
        fn_7100f00f4c(Music(), entry->mode, true);
    }
    fn_71011cdce8(self->field_0x38, false);
    fn_71011cdc30(self->field_0x38, false);
    fn_71011ec424(self->field_0x38, CRntString(g_unk_710162e2ed));
    fn_71011cdce8(self->field_0x40, false);
    fn_71011cdc30(self->field_0x40, false);
    fn_710020ac34(reinterpret_cast<UnkToggle*>(self->holder->toggle), false);
    self->field_0x58 = false;
}

void fn_7100ec9640(UnkMusicList_b0166* self) {
    if (!self->field_0x59 || self->field_0x58)
        return;
    self->field_0x58 = true;
    self->field_0x28 = 0;
    fn_710020ac34(reinterpret_cast<UnkToggle*>(self->holder->toggle), true);
    fn_71011cdce8(self->field_0x38, false);
    fn_71011cdc30(self->field_0x38, false);
    fn_71011cdce8(self->field_0x40, false);
    fn_71011cdc30(self->field_0x40, false);
    auto* a = reinterpret_cast<UnkGuiLabel_b0166*>(self->field_0x38);
    a->field_0x13c = 0xfff85ee0;
    a->field_0x186 = true;
    auto* b = reinterpret_cast<UnkGuiLabel_b0166*>(self->field_0x40);
    b->field_0x13c = 0xfff85ee0;
    b->field_0x186 = true;
    UnkMusicEntry_b0166* entry = fn_7100ec9780(self, self->field_0x28);
    if (!fn_7100f00a24(Music(), entry->mode))
        fn_7100f01838(Music(), entry->mode);
    self->field_0x48 = entry->field_0x4;
}

// ---- the counters ----

void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

struct UnkSubId_7100fc4b34 {
    CStrId id;
    u64 field_0x8;
};
using UnkQualifiedName_7100fc4b34 = UnkQualifiedName;
const UnkSubId_7100fc4b34& fn_71003327a4(UnkQualifiedName_7100fc4b34* self, const CStrId* name);

extern UnkPropertyEx<0x160> g_unk_7172759680;

struct UnkCounters_b0166 {
    u32* counters;  // 0x0
    bool enabled;   // 0x8
};

static inline UnkSubId_7100fc4b34 SubId(UnkPropertyEx<0x160>& def, const CStrId& name) {
    UnkQualifiedName* qualified = reinterpret_cast<UnkQualifiedName*>(def.field_0x10);
    if (!qualified->str || !qualified->str[0])
        fn_7100ae9d7c(qualified, reinterpret_cast<UnkIdSource*>(&def), g_unk_7101d470e0);
    return fn_71003327a4(qualified, &name);
}

void fn_7100ec78e8(UnkCounters_b0166* self, const CStrId* name) {
    if (!self->enabled)
        return;
    UnkGameManager* manager = g_unk_7101d47310;
    UnkTypedValue v;
    CBlackboard* blackboard = &manager->field_0x968;
    UnkSubId_7100fc4b34 key = SubId(g_unk_7172759680, *name);
    bool value = true;
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, g_unk_71729ce198)) {
        if (!manager->field_0x968.field_0xe8) {
            v.type = fn_710000d228();
            v.value = &value;
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(&key.id), &v);
        }
    }
}
