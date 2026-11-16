// UnkTU811 (CEmmyAIComponent's unit): switches the EMMI to the action `data` names (patrol, the action
// at g_unk_71726df1a0, or chase). The action is kept in the blackboard (+0x1260); when it changes, the
// animation's layers 6 to 9 are set for the new one, and entering a chase from the other two may start a
// timeline entry. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU811.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "game/UnkTypedValue.h"
#include "game/behaviortree/CEmmyChaseAction.h"
#include "game/behaviortree/CEmmyPatrolAction.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStrIdInline;

struct UnkAnimComponent_b52;
struct UnkActorTarget;
void fn_7101178e08(UnkAnimComponent_b52* self, u32 value, bool apply, bool flag, f32 time);
void fn_710117662c(UnkActorTarget* self, u32 value, u32 extra, bool apply, bool flag, f32 time);
void fn_710051d464(CBlackboard::CSection* section, UnkPropertyName* name, CStrId* out);
struct UnkEnumType;
UnkEnumType* fn_710009ffd4();  // base::global::TRntString256's type

extern char g_unk_71726df1a0[];
extern const u32 g_unk_7101649db8;

inline const UnkGameManagerIdTable& IdTable_l544a34() {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    return s_table;
}

// A slot of the id table its header has no member for yet.
inline const CStrId& TableId_l544a34(u32 offset) {
    return reinterpret_cast<const CStrId*>(&IdTable_l544a34())[offset / sizeof(CStrId)];
}

// A string with 0x100 bytes of inline storage (0x148 bytes), as in src/base/UnkTunableRegistry.cpp.
struct UnkInlineString_l544a34 {
    u8 str[0x30];
    char* storage_vtable;  // +0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    UnkInlineString_l544a34() {
        fn_710007e6e4(Str(), reinterpret_cast<base::UnkAllocator*>(&storage_vtable));
        storage_vtable = reinterpret_cast<char*>(g_unk_7101a194b8 + 2);
        field_0x138 = 0;
        field_0x140 = 0;
    }
    ~UnkInlineString_l544a34() {
        fn_710007ed40(Str());
        fn_710007ecd4(Str());
    }
};

// Stores `text` under `name` in the blackboard section `section` names (unless the blackboard is locked).
static inline void SetSectionString(CBlackboard* blackboard, const CStrId& section, UnkPropertyName* name,
                                    const char* text) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkInlineString_l544a34 value;
            fn_710007e77c(value.Str(), text);
            UnkTypedValue v;
            v.type = reinterpret_cast<const base::reflection::CType*>(fn_710009ffd4());
            v.value = &value;
            fn_7100332c80(s, name, &v);
        }
    }
}

static inline const CStrId& Id(const char* global) { return *reinterpret_cast<const CStrId*>(global); }

struct UnkEmmyEntity_l544a34 {
    u8 pad_0x0[0x220];
    void* animation;  // 0x220
    u8 pad_0x228[0x2e8 - 0x228];
    UnkTimelineComponent* timeline;  // 0x2e8
};

struct UnkEmmyAI_l544a34 {
    u8 pad_0x0[0x30];
    UnkEmmyEntity_l544a34* entity;  // 0x30
    u8 pad_0x38[0x1260 - 0x38];
    CBlackboard blackboard;  // 0x1260
    u8 pad_after_blackboard[0x27720 - 0x1260 - sizeof(CBlackboard)];
    bool field_0x27720;
};
static_assert(__builtin_offsetof(UnkEmmyAI_l544a34, field_0x27720) == 0x27720, "layout");

static inline UnkAnimComponent_b52* Layers(UnkEmmyAI_l544a34* self) {
    return static_cast<UnkAnimComponent_b52*>(self->entity->animation);
}
static inline UnkActorTarget* Target(UnkEmmyAI_l544a34* self) {
    return static_cast<UnkActorTarget*>(self->entity->animation);
}

// The chase layers; entering a chase from the other two actions may start a timeline entry.
static inline void SetChaseLayers(UnkEmmyAI_l544a34* self, const CStrId& current, bool on) {
    fn_7101178e08(Layers(self), 6, false, false, -1.0f);
    fn_7101178e08(Layers(self), 7, false, false, -1.0f);
    u32 layer = g_unk_7101649db8;
    fn_710117662c(Target(self), 8, layer, false, false, -1.0f);
    if (!self->field_0x27720)
        fn_710117662c(Target(self), 9, layer + 1, false, false, -1.0f);
    if (on && (current == Id(g_unk_71726ded60) || current == Id(g_unk_71726df1a0)) && self->field_0x27720) {
        if (UnkTimelineComponent* timeline = self->entity->timeline) {
            u64 id = 0xe2bddc93bcc7c84aull;
            if (fn_7100a2c13c(timeline, &id)) {
                u32 mask = UnkAll_SpitSend().value;
                fn_7100a2cb7c(self->entity->timeline, &id, &mask, false, -1.0f);
            }
        }
    }
}

// Sets the animation layers for the new action.
static inline void SetLayers(UnkEmmyAI_l544a34* self, const CStrId& action, const CStrId& current, bool on) {
    if (action == Id(g_unk_71726ded60)) {
        fn_7101178e08(Layers(self), 7, false, false, -1.0f);
        fn_7101178e08(Layers(self), 8, false, false, -1.0f);
        fn_7101178e08(Layers(self), 9, false, false, -1.0f);
        fn_710117662c(Target(self), 6, g_unk_7101649db8, false, false, -1.0f);
    } else if (action == Id(g_unk_71726df1a0)) {
        fn_7101178e08(Layers(self), 6, false, false, -1.0f);
        fn_7101178e08(Layers(self), 8, false, false, -1.0f);
        fn_7101178e08(Layers(self), 9, false, false, -1.0f);
        fn_710117662c(Target(self), 7, g_unk_7101649db8, false, false, -1.0f);
    } else if (action == Id(g_unk_71726debd0)) {
        SetChaseLayers(self, current, on);
    }
}

void fn_7100544a34(UnkEmmyActor_b55* actor, char* data, bool on) {
    UnkEmmyAI_l544a34* self = reinterpret_cast<UnkEmmyAI_l544a34*>(actor);
    const CStrId& action = Id(data);
    CStrId current(UnkStrIdInline(), nullptr, false);
    if (CBlackboard::CSection* section = fn_7101047270(&self->blackboard, TableId_l544a34(0x1b58)))
        fn_710051d464(section, reinterpret_cast<UnkPropertyName*>(g_unk_71726e20a8), &current);
    if (!(action == current)) {
        SetSectionString(&self->blackboard, TableId_l544a34(0x1b58), reinterpret_cast<UnkPropertyName*>(g_unk_71726e20a8),
                         action.Text());
        if (self->entity->animation)
            SetLayers(self, action, current, on);
    }
}
