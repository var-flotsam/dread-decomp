// CCharClassSoundProofTriggerComponent unit (tools/tu_map.py unit 1544): the trigger's update. After the
// base trigger's update, while nothing is inside (+0x9b8), each entity other than the player that is (or,
// by the flag at +0xaf5, is not) among the trigger's links and has an audio component (+0x2b8) gets its
// sound groups muffled or restored. Names and layouts are placeholders.
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAudioComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::reflection::UnkReflObject;

// The link list's entries and the link constructor (CCharClassSoundProofTriggerComponent_Links_ovb0130.cpp).
struct UnkLink_ovb0130;
struct UnkTargetLink_ovb0130;
struct UnkWeakTarget;
bool CSoundProofTriggerComponent_ActorsToIgnoreContains(base::global::CRntVector<UnkLink_ovb0130>* self, UnkLink_ovb0130* link);
void CCharClassSoundProofTriggerComponent_ConstructResolvedLinkToTarget(UnkTargetLink_ovb0130* self, UnkWeakTarget* target);

struct UnkAudioComponent_ovb0015;
void fn_710071f634(UnkAudioComponent_ovb0015* self, const u32* groups, s32 level, f32 time);

// The base of a link: its callback array and its state.
struct UnkSoundProofLinkBase_la3037c {
    void** vtable;
    UnkErasedVector callbacks;  // 0x8
    u32 state;                  // 0x30: 1 until resolved, then 2

    ~UnkSoundProofLinkBase_la3037c() {
        vtable = &g_unk_7101a20998[2];
        if (callbacks.data) {
            for (u64 i = 0; i < callbacks.count; ++i) {
                UnkErasedValue_0x10& v = callbacks.data[i];
                if (v.ops) {
                    v.ops->destroy(&v);
                    v.ops = nullptr;
                }
            }
            callbacks.count = 0;
            callbacks.allocator->vf_0x48(callbacks.data);
            callbacks.capacity = 0;
        }
    }
};

// A link to an object of the scene (game/CGameLink.h).
struct UnkSoundProofLink_la3037c : UnkSoundProofLinkBase_la3037c {
    u32 pad_0x34;
    base::UnkRefHandle handle;  // 0x38

    ~UnkSoundProofLink_la3037c() {
        vtable = &g_unk_7101a20950[2];
        if (base::UnkRefCounted* p = handle.ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
};
static_assert(sizeof(UnkSoundProofLink_la3037c) == 0x40, "UnkSoundProofLink_la3037c layout");

struct UnkSoundProofEntity_la3037c : UnkReflObject {
    u8 pad_0x8[0x1b4 - 0x8];
    bool field_0x1b4;
    u8 pad_0x1b5[0x2b8 - 0x1b5];
    UnkAudioComponent_ovb0015* audio;  // 0x2b8
};

// The game manager's list of entities (its +0x2638).
struct UnkSoundProofEntityNode_la3037c {
    void* field_0x0;
    UnkSoundProofEntity_la3037c* entity;
    UnkSoundProofEntityNode_la3037c* next;
};

struct UnkSoundProofManager_la3037c {
    u8 pad_0x0[0x2638];
    UnkSoundProofEntityNode_la3037c* entities;  // 0x2638
};

struct UnkSoundProofTrigger_la3037c {
    void** vtable;
    u8 pad_0x8[0x9b8 - 0x8];
    s32 inside_count;  // 0x9b8
    u8 pad_0x9bc[0xac0 - 0x9bc];
    base::global::CRntVector<UnkLink_ovb0130> links;  // 0xac0
    u8 pad_after_links[0xaec - 0xac0 - sizeof(base::global::CRntVector<UnkLink_ovb0130>)];
    f32 field_0xaec;
    f32 field_0xaf0;
    bool field_0xaf4;
    bool linked_only;  // 0xaf5
    bool muffle;       // 0xaf6
};
static_assert(__builtin_offsetof(UnkSoundProofTrigger_la3037c, links) == 0xac0, "layout");
static_assert(__builtin_offsetof(UnkSoundProofTrigger_la3037c, muffle) == 0xaf6, "layout");

void fn_7100a47128(UnkSoundProofTrigger_la3037c* self);

extern const u32 g_unk_717274723c __attribute__((visibility("hidden")));  // the sound groups restored

// A sound-group mask whose statics are guarded (their constructors are not constexpr).
struct UnkSoundGroups_la3037c {
    u32 value;
    UnkSoundGroups_la3037c() { value = 0; }
};

struct UnkSoundGroupMask_la3037c {
    u32 value;
    explicit UnkSoundGroupMask_la3037c(const UnkSoundGroups_la3037c& groups) { value = ~groups.value & 0x7ff; }
};

inline const UnkSoundGroups_la3037c& UnkNoSoundGroups_la3037c() {
    static UnkSoundGroups_la3037c s_value;
    return s_value;
}

inline const UnkSoundGroupMask_la3037c& UnkAllSoundGroups_la3037c() {
    static UnkSoundGroupMask_la3037c s_value(UnkNoSoundGroups_la3037c());
    return s_value;
}

// A link already resolved to `entity` (fn_7100a30a9c constructs it in place).
static inline UnkSoundProofLink_la3037c LinkTo(UnkSoundProofEntity_la3037c* entity) {
    UnkSoundProofLink_la3037c link;
    CCharClassSoundProofTriggerComponent_ConstructResolvedLinkToTarget(reinterpret_cast<UnkTargetLink_ovb0130*>(&link), reinterpret_cast<UnkWeakTarget*>(entity));
    return link;
}

// Whether the trigger's links include one to the target of `link`.
static inline bool IsLinked(UnkSoundProofTrigger_la3037c* self, const UnkSoundProofLink_la3037c& link) {
    return CSoundProofTriggerComponent_ActorsToIgnoreContains(&self->links, reinterpret_cast<UnkLink_ovb0130*>(const_cast<UnkSoundProofLink_la3037c*>(&link)));
}

void fn_7100a3037c(UnkSoundProofTrigger_la3037c* self) {
    fn_7100a47128(self);
    self->field_0xaf4 = false;
    if (self->inside_count > 0)
        return;

    UnkPlayer* player = fn_7100dec788(g_unk_7101d47310);
    UnkSoundProofManager_la3037c* manager = reinterpret_cast<UnkSoundProofManager_la3037c*>(g_unk_7101d47310);
    bool muffle = self->muffle;
    const u32 restored = g_unk_717274723c;
    bool linked_only = self->linked_only;
    for (UnkSoundProofEntityNode_la3037c* node = manager->entities; node; node = node->next) {
        UnkSoundProofEntity_la3037c* entity = node->entity;
        if (!entity || entity == reinterpret_cast<UnkSoundProofEntity_la3037c*>(player))
            continue;
        if (linked_only ? !IsLinked(self, LinkTo(entity)) : IsLinked(self, LinkTo(entity)))
            continue;
        if (!fn_710009d50c(entity->vf_0x0(), fn_7100d70c20()))
            continue;
        if (!entity->field_0x1b4)
            continue;
        UnkAudioComponent_ovb0015* audio = entity->audio;
        if (!audio)
            continue;
        if (muffle) {
            u32 mask = UnkAllSoundGroups_la3037c().value;
            fn_710071a398(audio, &mask, 1.0f, self->field_0xaf0);
        } else {
            u32 groups = restored;
            fn_710071f634(audio, &groups, 0, self->field_0xaec);
        }
    }
}
