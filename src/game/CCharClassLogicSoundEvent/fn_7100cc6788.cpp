// CCharClassLogicSoundEvent's unit (tools/tu_map.py): creates the event a name (a string id) stands for
// in a chained hash map (+0x68): a new 0x48-byte object built from the map's entry (fn_7100f0475c), kept
// in the list at +0x30 and returned as a counted reference (empty when the name is unknown).
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// Signature as src/game/CActor/UnkRefPtrSlotReserve_b43.cpp defines it.
template <class Tag>
struct UnkSlotArray;
struct UnkTag_710095025c;
void fn_710095025c(UnkSlotArray<UnkTag_710095025c>* self, u32 capacity);

// An event: virtual destructor first, reference count at +0x30.
struct UnkSoundEvent_lcc6788 {
    virtual ~UnkSoundEvent_lcc6788();
    u8 pad_0x8[0x28];
    s32 ref_count;  // 0x30
    u8 pad_0x34[0x14];
};
static_assert(sizeof(UnkSoundEvent_lcc6788) == 0x48, "layout");

// The definition an event is built from (a map value).
struct UnkSoundEventDef_lcc6788 {
    u8 pad_0x0[0x28];
};

// A pair of words passed by value.
struct UnkSoundEventArg_lcc6788 {
    u64 a;
    u64 b;
};

void fn_7100f0475c(UnkSoundEvent_lcc6788* self, const UnkSoundEventDef_lcc6788* def, void* owner, u64 arg,
                   UnkSoundEventArg_lcc6788 arg2);

// A counted reference to an event.
struct UnkSoundEventRef_lcc6788 {
    UnkSoundEvent_lcc6788* ptr;

    UnkSoundEventRef_lcc6788() : ptr(nullptr) {}
    explicit UnkSoundEventRef_lcc6788(UnkSoundEvent_lcc6788* p) : ptr(p) {
        if (ptr)
            ++ptr->ref_count;
    }
    UnkSoundEventRef_lcc6788& operator=(const UnkSoundEventRef_lcc6788& other) {
        if (UnkSoundEvent_lcc6788* p = other.ptr) {
            UnkSoundEvent_lcc6788* old = ptr;
            ptr = p;
            ++p->ref_count;
            if (old && --old->ref_count == 0)
                delete old;
        }
        return *this;
    }
    ~UnkSoundEventRef_lcc6788() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }
};

// A slot of the event list: a counted reference.
struct UnkSoundEventSlot_lcc6788 {
    UnkSoundEvent_lcc6788* ptr;

    void Assign(UnkSoundEvent_lcc6788* p) {
        if (p) {
            ++p->ref_count;
            if (UnkSoundEvent_lcc6788* old = ptr) {
                if (--old->ref_count == 0)
                    delete old;
            }
            ptr = p;
        }
    }
};

struct UnkSoundEventList_lcc6788 {
    UnkSoundEventSlot_lcc6788* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    void* allocator;

    void PushBack(const UnkSoundEventRef_lcc6788& value) {
        u32 n = count;
        u32 new_capacity = capacity;
        if (new_capacity <= n && step != 0)
            new_capacity = (n / step + 1) * step;
        // A capacity that doesn't grow past the count (no step) hangs, as in the original.
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        fn_710095025c(reinterpret_cast<UnkSlotArray<UnkTag_710095025c>*>(this), new_capacity);
        UnkSoundEventSlot_lcc6788* slot = new (&data[count]) UnkSoundEventSlot_lcc6788();
        slot->Assign(value.ptr);
        ++count;
    }
};

struct UnkSoundEventNode_lcc6788 {
    CStrId name;                    // 0x0
    UnkSoundEventDef_lcc6788 def;   // 0x8
    UnkSoundEventNode_lcc6788* next;  // 0x30
};

struct UnkSoundEventMap_lcc6788 {
    UnkSoundEventNode_lcc6788** buckets;  // 0x0
    u32 count;                            // 0x8
    u32 bucket_count;                     // 0xc

    const UnkSoundEventDef_lcc6788* Find(const CStrId& name) const {
        if (count == 0)
            return nullptr;
        UnkSoundEventNode_lcc6788** bucket = name.Hash() % bucket_count + buckets;
        if (!bucket)
            return nullptr;
        for (UnkSoundEventNode_lcc6788* node = *bucket; node; node = node->next) {
            if (node->name == name)
                return &node->def;
        }
        return nullptr;
    }
};

struct UnkSoundEventLogic_lcc6788 {
    u8 pad_0x0[0x30];
    UnkSoundEventList_lcc6788 events;  // 0x30
    u8 pad_0x58[0x10];
    UnkSoundEventMap_lcc6788 defs;     // 0x68
};
static_assert(__builtin_offsetof(UnkSoundEventLogic_lcc6788, defs) == 0x68, "layout");

UnkSoundEventRef_lcc6788 fn_7100cc6788(UnkSoundEventLogic_lcc6788* self, const CStrId* name, void* owner,
                                       const u64* arg, const UnkSoundEventArg_lcc6788* arg2) {
    UnkSoundEventRef_lcc6788 result;
    const UnkSoundEventDef_lcc6788* def = self->defs.Find(*name);
    if (!def)
        return result;
    UnkSoundEvent_lcc6788* event = static_cast<UnkSoundEvent_lcc6788*>(fn_71000839a0(0x48, 5));
    fn_7100f0475c(event, def, owner, *arg, *arg2);
    UnkSoundEventRef_lcc6788 ref(event);
    result = ref;
    self->events.PushBack(ref);
    return result;
}
