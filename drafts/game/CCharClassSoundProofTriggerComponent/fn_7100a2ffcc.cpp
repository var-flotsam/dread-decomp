// CCharClassSoundProofTriggerComponent's unit (tools/tu_map.py, unit 1544): after the base trigger's
// update, when the trigger changed, mutes or restores the sound of every entity inside (or outside) it.
// Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAudioComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU341.h"
#include "types.h"

struct UnkAudioComponent_ovb0015;
struct UnkLink_ovb0130;
struct UnkTargetLink_ovb0130;
struct UnkWeakTarget;

void fn_7100a47124(char*);
bool CSoundProofTriggerComponent_ActorsToIgnoreContains(base::global::CRntVector<UnkLink_ovb0130>* self, UnkLink_ovb0130* link);
void CCharClassSoundProofTriggerComponent_ConstructResolvedLinkToTarget(UnkTargetLink_ovb0130* self, UnkWeakTarget* target);
void fn_710071f634(UnkAudioComponent_ovb0015* self, const u32* groups, s32 level, f32 time);

// The sound groups the trigger restores.
__attribute__((visibility("hidden"))) extern const u32 g_unk_717274723c;

// A link to one entity, built (fn_7100a30a9c) to look it up in the trigger's list: a callback set
// (base/UnkCloneHelpers.h's UnkCallbackListShort) whose constructor is out of line.
struct UnkSoundProofLinkBase_l2ffcc {
    void** vtable;
    base::UnkCallable* data;  // 0x8
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x1c;
    u32 flags;
    base::UnkAllocator* allocator;  // 0x28

    ~UnkSoundProofLinkBase_l2ffcc() {
        vtable = g_unk_7101a20998 + 2;
        if (data) {
            for (u64 i = 0; i < count; ++i)
                data[i].~UnkCallable();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkSoundProofEntity_l2ffcc;

struct UnkSoundProofLink_l2ffcc : UnkSoundProofLinkBase_l2ffcc {
    u64 field_0x30;
    base::UnkRefPtrMember owner;  // 0x38

    explicit UnkSoundProofLink_l2ffcc(UnkSoundProofEntity_l2ffcc* entity) {
        CCharClassSoundProofTriggerComponent_ConstructResolvedLinkToTarget(reinterpret_cast<UnkTargetLink_ovb0130*>(this), reinterpret_cast<UnkWeakTarget*>(entity));
    }
    ~UnkSoundProofLink_l2ffcc() { vtable = g_unk_7101a20950 + 2; }
};
static_assert(sizeof(UnkSoundProofLink_l2ffcc) == 0x40, "UnkSoundProofLink_l2ffcc layout");

struct UnkSoundProofEntity_l2ffcc : base::reflection::UnkReflObject {
    u8 pad_0x8[0x1ac];
    bool field_0x1b4;
    u8 pad_0x1b5[0x103];
    UnkAudioComponent_ovb0015* audio;  // 0x2b8
};
static_assert(__builtin_offsetof(UnkSoundProofEntity_l2ffcc, audio) == 0x2b8, "UnkSoundProofEntity_l2ffcc layout");

// The game manager's list of entities (its +0x2638).
struct UnkSoundProofEntityNode_l2ffcc {
    void* field_0x0;
    UnkSoundProofEntity_l2ffcc* entity;
    UnkSoundProofEntityNode_l2ffcc* next;
};

static inline UnkSoundProofEntityNode_l2ffcc* Entities(UnkGameManager* manager) {
    return *reinterpret_cast<UnkSoundProofEntityNode_l2ffcc**>(reinterpret_cast<char*>(manager) + 0x2638);
}

struct UnkSoundProofTrigger_l2ffcc {
    u8 pad_0x0[0x9b8];
    s32 field_0x9b8;
    u8 pad_0x9bc[0x104];
    base::global::CRntVector<UnkLink_ovb0130> links;  // 0xac0
    u8 pad_0xad8[0xae8 - 0xac0 - sizeof(base::global::CRntVector<UnkLink_ovb0130>)];
    s32 level;     // 0xae8
    f32 time;      // 0xaec
    u8 pad_0xaf0[0x4];
    bool changed;  // 0xaf4
    bool inside;   // 0xaf5: whether the entities in the list are the ones affected
    bool mute;     // 0xaf6
};
static_assert(__builtin_offsetof(UnkSoundProofTrigger_l2ffcc, mute) == 0xaf6, "UnkSoundProofTrigger_l2ffcc layout");

// A channel mask with no channels set (its constructor is not a constant expression).
struct UnkChannelMask_l2ffcc {
    u32 bits;
    UnkChannelMask_l2ffcc() { bits = 0; }
};

inline const UnkChannelMask_l2ffcc& NoChannels_l2ffcc() {
    static UnkChannelMask_l2ffcc s_none;
    return s_none;
}

// Every channel (the low 11 bits).
inline u32 AllChannels_l2ffcc() {
    static const u32 s_all = ~NoChannels_l2ffcc().bits & 0x7ff;
    return s_all;
}

void fn_7100a2ffcc(UnkSoundProofTrigger_l2ffcc* self) {
    fn_7100a47124(reinterpret_cast<char*>(self));
    if (!self->changed)
        return;
    self->changed = false;
    if (self->field_0x9b8 <= 0)
        return;
    UnkPlayer* player = fn_7100dec788(g_unk_7101d47310);
    s32 level = self->level;
    bool mute = self->mute;
    bool inside = self->inside;
    for (UnkSoundProofEntityNode_l2ffcc* n = Entities(g_unk_7101d47310); n; n = n->next) {
        UnkSoundProofEntity_l2ffcc* entity = n->entity;
        if (!entity || static_cast<void*>(entity) == player)
            continue;
        // Inside: only the listed entities; outside: only the others.
        if (inside) {
            UnkSoundProofLink_l2ffcc link(entity);
            if (!CSoundProofTriggerComponent_ActorsToIgnoreContains(&self->links, reinterpret_cast<UnkLink_ovb0130*>(&link)))
                continue;
        } else {
            UnkSoundProofLink_l2ffcc link(entity);
            if (CSoundProofTriggerComponent_ActorsToIgnoreContains(&self->links, reinterpret_cast<UnkLink_ovb0130*>(&link)))
                continue;
        }
        if (!fn_710009d50c(entity->vf_0x0(), fn_7100d70c20()))
            continue;
        if (!entity->field_0x1b4)
            continue;
        UnkAudioComponent_ovb0015* audio = entity->audio;
        if (!audio)
            continue;
        if (mute) {
            u32 mask = AllChannels_l2ffcc();
            fn_710071a398(audio, &mask, 0.001f, self->time);
        } else {
            u32 groups = g_unk_717274723c;
            fn_710071f634(audio, &groups, level, self->time);
        }
    }
}
