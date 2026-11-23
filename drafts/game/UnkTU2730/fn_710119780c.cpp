// UnkTU2730 (tools/tu_map.py unit 2730): marks `name` as seen: stores 1 under its sub-id of the cutscene
// definition in the section the id at +0xb18 names, in the game manager's blackboards at +0xab0 and
// +0x968, then resets that id and appends `name` to the list at +0xae8. Names and layouts are placeholders.
#include "base/UnkCloneRecords.h"
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkSubId_7100fc4b34.h"
#include "game/UnkTU2730.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CStrId;

struct UnkGameManagerNamedDef : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedName name;
};

static inline UnkSubId_7100fc4b34 SubId(UnkGameManagerNamedDef& def, const CStrId& name) {
    if (!def.name.str || !def.name.str[0])
        fn_7100ae9d7c(&def.name, &def, g_unk_7101d470e0);
    return fn_71003327a4(&def.name, &name);
}

// The game manager's two blackboards this function writes.
struct UnkManagerBlackboards_l19780c {
    u8 pad_0x0[0x968];
    CBlackboard field_0x968;
    u8 pad_0xa58[0xab0 - 0xa58];
    CBlackboard field_0xab0;
};
static_assert(__builtin_offsetof(UnkManagerBlackboards_l19780c, field_0xab0) == 0xab0, "layout");

struct UnkCutsceneLog_l19780c {
    u8 pad_0x0[0xae8];
    base::UnkStrIdVec seen;  // 0xae8
    u8 pad_0xb10[8];
    CStrId section;          // 0xb18
};
static_assert(__builtin_offsetof(UnkCutsceneLog_l19780c, section) == 0xb18, "layout");

struct UnkObj_710119770c;

// Stores 1 under `name`'s sub-id in the section `section_id` of `blackboard`, when there is one.
static inline void MarkSeen(CBlackboard* blackboard, const CStrId& section_id, const CStrId& name) {
    if (CBlackboard::CSection* section = fn_71010472e0(blackboard, section_id)) {
        u32 value;
        UnkTypedValue v;
        UnkSubId_7100fc4b34 key = SubId(g_unk_7172a0bb10, name);
        value = 1;
        v.type = fn_7100005178();
        v.value = &value;
        fn_7100332c80(section, reinterpret_cast<UnkPropertyName*>(&key.id), &v);
    }
}

void fn_710119780c(UnkObj_710119770c* object, const CStrId& name) {
    auto* self = reinterpret_cast<UnkCutsceneLog_l19780c*>(object);
    MarkSeen(&reinterpret_cast<UnkManagerBlackboards_l19780c*>(g_unk_7101d47310)->field_0xab0, self->section, name);
    MarkSeen(&reinterpret_cast<UnkManagerBlackboards_l19780c*>(g_unk_7101d47310)->field_0x968, self->section, name);
    self->section = g_unk_7172a0bc98;
    self->seen.PushBack(name);
}
