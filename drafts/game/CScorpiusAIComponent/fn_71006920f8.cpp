// CScorpiusAIComponent unit (tools/tu_map.py, unit 1027): fn_71006920f8 moves the phase at `current` to
// `phase`. With `notify`, it sets the owner's timeline id for the transition and writes a tunable float
// through reflection; it then records the phase at the front of a bounded history list, dropping the oldest
// entry when the list is full. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CRntVector.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "game/CCharClassTweakPlayRateTrack.h"
#include "game/UnkActor.h"
#include "game/UnkTU85.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflTypedPtr;

extern const u64 g_unk_710164a5e0;  // reached through the GOT
extern const u64 g_unk_710164a5d8;  // reached through the GOT
extern const u64 g_unk_710164a5d0;  // reached through the GOT
extern const u64 g_unk_710164a5c8;  // reached through the GOT
extern const u64 g_unk_710164a5c0;  // reached through the GOT
extern const u64 g_unk_710164a5b8;  // reached through the GOT

f32 fn_7100690754(void* object);

// A string's data: its key (CRC-64) at +0x30.
struct UnkScorpiusStrData_l6920f8 {
    u8 pad_0x0[0x30];
    u64 key;
};

struct UnkScorpiusNamed_l6920f8 {
    UnkScorpiusStrData_l6920f8* name;
};

extern UnkScorpiusNamed_l6920f8 g_unk_7172704238 __attribute__((visibility("hidden")));

// A reflected field (0xb0 bytes).
struct UnkScorpiusField_l6920f8 {
    u8 pad_0x0[0xb0];
};

// A class as the lookup sees it: its fields and their name hashes.
struct UnkScorpiusClass_l6920f8 {
    u8 pad_0x0[0xe8];
    UnkScorpiusField_l6920f8* fields;  // 0xe8
    u64* keys;                         // 0xf0
    u32 count;                         // 0xf8

    UnkScorpiusField_l6920f8* End() { return fields + count; }
    UnkScorpiusField_l6920f8* Find(u64 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &fields[i];
        return End();
    }
    // The field with that name hash, null when there is none.
    UnkScorpiusField_l6920f8* FindField(u64 key) {
        UnkScorpiusField_l6920f8* it = Find(key);
        return it != End() ? it : nullptr;
    }
};

// A reflected object: its class is vtable slot 0.
struct UnkScorpiusObject_l6920f8 {
    virtual UnkScorpiusClass_l6920f8* GetClass();
};

struct UnkScorpiusValue_l6920f8 {
    const CType* type;
    void* value;
};

// Writes the float at `in` to the object's field named by `key`, if it has one.
static inline void SetTunable(UnkScorpiusObject_l6920f8* obj, u64 key, f32* in) {
    UnkScorpiusClass_l6920f8* cls = obj->GetClass();
    UnkReflTypedPtr typed;
    typed.type = reinterpret_cast<char*>(obj->GetClass());
    typed.ptr = obj;
    UnkScorpiusValue_l6920f8 value = {fn_710000abbc(), in};
    if (UnkScorpiusField_l6920f8* field = cls->FindField(key))
        fn_71000e7a80(reinterpret_cast<CVariable*>(field), reinterpret_cast<const UnkReflFieldObject*>(&typed),
                      reinterpret_cast<UnkReflValueArg*>(&value));
}

// Sets the timeline id to the all-flags mask.
static inline void SetTimelineMask(UnkTimelineComponent* timeline, const u64* id) {
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, id, &mask.id, false, -1.0f);
}

struct UnkScorpiusPhaseNode_l6920f8 {
    u32 phase;
    UnkScorpiusPhaseNode_l6920f8* next;
};

// A singly linked list of phases, newest first, with nodes from an allocator.
struct UnkScorpiusPhaseList_l6920f8 {
    u8 pad_0x0[8];
    u32 count;                                // 0x8
    UnkScorpiusPhaseNode_l6920f8* head;       // 0x10
    UnkScorpiusPhaseNode_l6920f8* tail;       // 0x18
    base::UnkAllocator* allocator;            // 0x20
    u32 alloc_flags;                          // 0x28
    u8 pad_0x2c[0x40 - 0x2c];
    u64 capacity_bytes;                       // 0x40

    u64 Capacity() const { return capacity_bytes / sizeof(UnkScorpiusPhaseNode_l6920f8); }

    // Unlinks and frees the node at `index`, if there is one.
    void RemoveAt(u32 index) {
        if (index >= count)
            return;
        UnkScorpiusPhaseNode_l6920f8* node = head;
        if (!node)
            return;
        UnkScorpiusPhaseNode_l6920f8* prev = nullptr;
        for (; index != 0; --index) {
            if (!node->next)
                return;
            prev = node;
            node = node->next;
        }
        if (!prev) {
            head = node->next;
        } else {
            prev->next = node->next;
            if (node == tail)
                tail = prev;
        }
        if (--count == 0) {
            head = nullptr;
            tail = nullptr;
        }
        allocator->vf_0x48(node);
    }

    void PushFront(u32 phase) {
        UnkScorpiusPhaseNode_l6920f8* node = static_cast<UnkScorpiusPhaseNode_l6920f8*>(
            allocator->vf_0x40(sizeof(UnkScorpiusPhaseNode_l6920f8), 8, alloc_flags));
        if (!node)
            return;
        node->phase = phase;
        node->next = nullptr;
        if (count) {
            node->next = head;
            head = node;
        } else {
            head = node;
            tail = node;
        }
        ++count;
    }
};

struct UnkScorpiusAI_l6920f8 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x149a0 - 0x38];
    void* field_0x149a0;  // 0x149a0
};
static_assert(__builtin_offsetof(UnkScorpiusAI_l6920f8, field_0x149a0) == 0x149a0, "layout");

void fn_71006920f8(UnkScorpiusAI_l6920f8* self, u32 phase, u32* current, UnkScorpiusPhaseList_l6920f8* history,
                   bool notify) {
    u32 previous = *current;
    if (previous == phase)
        return;
    *current = phase;
    if (notify) {
        if (UnkTimelineComponent* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
            switch (previous) {
            case 1:
                if (phase == 2)
                    SetTimelineMask(timeline, &g_unk_710164a5b8);
                else if (phase == 3)
                    SetTimelineMask(timeline, &g_unk_710164a5c0);
                break;
            case 2:
                if (phase == 1)
                    SetTimelineMask(timeline, &g_unk_710164a5c8);
                else if (phase == 3)
                    SetTimelineMask(timeline, &g_unk_710164a5d0);
                break;
            case 3:
                if (phase == 1)
                    SetTimelineMask(timeline, &g_unk_710164a5d8);
                else if (phase == 2)
                    SetTimelineMask(timeline, &g_unk_710164a5e0);
                break;
            }
        }
        u64 key = g_unk_7172704238.name->key;
        f32 value = fn_7100690754(self->field_0x149a0);
        SetTunable(reinterpret_cast<UnkScorpiusObject_l6920f8*>(&g_unk_7172790a08), key, &value);
    }
    if (history->count == history->Capacity())
        history->RemoveAt(history->count - 1);
    history->PushFront(phase);
}
