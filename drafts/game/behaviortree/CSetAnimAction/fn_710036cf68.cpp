// behaviortree::CSetAnimAction's unit (tools/tu_map.py): the start. Builds the animation's name from the id
// at +0x30 (or the one +0x38 resolves to), with a random number from [+0x44, +0x48] appended when both are
// set, plays it on the actor's (+0x18) character class, then sets and clears the timeline ids at +0x50 and
// +0x58. Names and layouts are placeholders.
#include "../../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkActor.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2772.h"
#include "game/UnkTU57.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStrId;

// A string with 0x100 bytes of inline storage (0x148 bytes).
struct UnkInlineString_l36cf68 {
    u8 str[0x30];
    char* storage_vtable;  // +0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    UnkInlineString_l36cf68() {
        CRntString_InitWithAllocator(Str(), reinterpret_cast<base::UnkAllocator*>(&storage_vtable));
        field_0x138 = 0;
        storage_vtable = reinterpret_cast<char*>(g_unk_7101a194b8 + 2);
        field_0x140 = 0;
    }
    // The text of a string id.
    explicit UnkInlineString_l36cf68(const UnkStrId* id) : UnkInlineString_l36cf68() { CRntString_AssignStrId(Str(), id); }
    ~UnkInlineString_l36cf68() {
        CRntString_Clear(Str());
        CRntString_Destroy(Str());
    }
};

struct UnkSetAnimAction_l36cf68 {
    u8 pad_0x0[0x18];
    UnkActor* actor;  // 0x18
    u8 pad_0x20[0x30 - 0x20];
    UnkStrId name;        // 0x30
    UnkStrId name_ref;    // 0x38: resolved through the blackboard when set
    u8 mode;              // 0x40
    bool mode_flag;       // 0x41
    u8 pad_0x42[2];
    s32 min_index;        // 0x44
    s32 max_index;        // 0x48
    s32 index;            // 0x4c
    UnkStrId timeline_set;    // 0x50
    UnkStrId timeline_clear;  // 0x58
    bool started;         // 0x60
    bool second_desc;     // 0x61
};

// A random number in [min, max].
inline s32 RandomIn_l36cf68(s32 min, s32 max) {
    s32 count = max + 1 - min;
    if (count != 0)
        min += (fn_71012395a0() & 0x7fff) % count;
    return min;
}

// Sets the timeline's value for an id to the all-flags mask, now.
inline void SetAll_l36cf68(UnkTimelineComponent* timeline, const u64* id) {
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = base::reflection::UnkReflTypeId<base::reflection::UnkReflTypeIdTag>::id();
    fn_7100a2cb7c(timeline, id, &mask.id, false, -1.0f);
}

void fn_710036cf68(UnkSetAnimAction_l36cf68* self) {
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    self->started = false;
    UnkInlineString_l36cf68 name(&self->name);
    if (self->name_ref.data != &g_unk_7101d47108->empty)
        CBehavior_ReadStringProperty(reinterpret_cast<UnkStringEqualsCondition_A2_b72*>(self),
                      static_cast<CStrId*>(static_cast<base::global::UnkStrIdBase*>(&self->name_ref)),
                      reinterpret_cast<UnkInlineString_A2_b72*>(&name));

    if (name.Str()->data && name.Str()->data[0] && self->actor) {
        if (void* animation = self->actor->field_0x220) {
            s32 index = self->min_index;
            if (index >= 0 && self->max_index >= 0) {
                index = RandomIn_l36cf68(index, self->max_index);
                self->index = index;
                fn_710007f634(name.Str(), index);
            }
            u32 mode = self->mode_flag ? self->mode | 0x80 : self->mode;
            const void* desc = self->second_desc ? static_cast<const void*>(&g_unk_7172769770)
                                                 : static_cast<const void*>(&g_unk_7172769740);
            if (base::reflection::refl_EmitU32(animation, name.Str()->key, 0xf4061ff25a2163d6, mode, &g_unk_710164b958,
                                               desc))
                self->started = true;
        }
    }

    UnkTimelineComponent* timeline =
        self->actor ? static_cast<UnkTimelineComponent*>(self->actor->field_0x2e8) : nullptr;
    if (timeline && self->timeline_set.data != &g_unk_7101d47108->empty) {
        u64 key = self->timeline_set.data->Key();
        SetAll_l36cf68(timeline, &key);
    }
    if (timeline && self->timeline_clear.data != &g_unk_7101d47108->empty) {
        u64 key = self->timeline_clear.data->Key();
        fn_7100a2b7b8(timeline, &key);
    }
}
