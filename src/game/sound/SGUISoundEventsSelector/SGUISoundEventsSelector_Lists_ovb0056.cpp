// sound::SGUISoundEventsSelector's unit (tools/tu_map.py): a sound list's destructors, the selector's
// constructor and move-assignment, a selector's registration (fn_7100fc9084) and playing the sound a
// selector picks (fn_7100fc9d78), and the selector's events as a string value (fn_7100fcbb3c). Names
// and layouts are placeholders.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/UnkDestructors.h"
#include "base/UnkStrIdPathPair.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU0.h"
#include "game/UnkTU32.h"
#include "game/sound/CSoundManager.h"
#include "game/sound/SGUISoundEventsSelector.h"
#include "types.h"

using base::UnkAllocator;

extern void* g_unk_7101caba70[];  // the sound list's vtable group (two address points)

// A list of playing sounds: entries of two handles (0x20 bytes) with a parallel key array, from an
// allocator, plus a counted resource. fn_7100fcb0f8 is its destructor, fn_7100fcb190 the deleting one.
struct UnkSoundList_ovb56 {
    void** vtable;
    void** vtable_8;
    void* entries;  // 0x10
    void* keys;     // 0x18
    u32 count;      // 0x20
    u8 unk_0x24[0xc];
    UnkAllocator* allocator;        // 0x30
    UnkCountedResource* resource;  // 0x38
};

static inline void Destroy_ovb56(UnkSoundList_ovb56* self) {
    self->vtable = &g_unk_7101caba70[2];
    self->vtable_8 = &g_unk_7101caba70[9];
    if (UnkCountedResource* resource = self->resource) {
        if (--resource->ref_count == 0)
            fn_710001737c(resource);
    }
    self->count = 0;
    if (self->keys)
        self->allocator->vf_0x48(self->keys);
    if (self->entries)
        self->allocator->vf_0x48(self->entries);
}

void fn_7100fcb0f8(UnkSoundList_ovb56* self) {
    Destroy_ovb56(self);
}

void fn_7100fcb190(UnkSoundList_ovb56* self) {
    Destroy_ovb56(self);
    fn_7100083c00(self);
}

extern void* g_unk_7101a1b750[];

// The selector: a name with 0x80 bytes of inline storage (the string, then its inline allocator)
// and an array of 0x10-byte entries.
struct UnkSoundSelector_ovb56 {
    void** vtable;
    u8 name[0x30];  // 0x8, a CRntString
    void** name_allocator_vtable;  // 0x38
    u8 name_buffer[0x80];
    u64 field_0xc0;
    u32 field_0xc8;
    void* entries;  // 0xd0
    u32 count;      // 0xd8
    u32 capacity;   // 0xdc
    u32 step;       // 0xe0
    bool field_0xe4;
    u32 flags;  // 0xe8
    UnkAllocator* allocator;  // 0xf0
};
static_assert(__builtin_offsetof(UnkSoundSelector_ovb56, allocator) == 0xf0, "layout");

void fn_7100fcb2ec(UnkSoundSelector_ovb56* self) {
    self->vtable = &g_unk_7101cabad0[2];
    fn_710007e6e4(reinterpret_cast<base::global::CRntString*>(self->name),
                  reinterpret_cast<UnkAllocator*>(&self->name_allocator_vtable));
    self->name_allocator_vtable = &g_unk_7101a1b750[2];
    self->field_0xc0 = 0;
    self->field_0xc8 = 0;
    self->vtable = &g_unk_7101caba40[2];
    self->entries = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = 10;
    self->field_0xe4 = true;
    self->flags = 0xf;
    self->allocator = fn_7100083990();
}

// An entry of the selector (0x10 bytes): a kind and an id.
struct UnkSelectorEntry_ovb56 {
    u32 kind;
    base::UnkStrIdMember id;  // 0x8
};

// The selector's entry array, moved by taking the other's storage (its own is not freed).
struct UnkSelectorEntries_ovb56 {
    UnkSelectorEntry_ovb56* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    UnkAllocator* allocator;

    UnkSelectorEntries_ovb56& operator=(UnkSelectorEntries_ovb56&& other) {
        for (u64 i = 0; i < count; ++i)
            data[i].id.~UnkStrIdMemberOf();
        count = 0;
        allocator = other.allocator;
        flags = other.flags;
        step = other.step;
        field_0x14 = other.field_0x14;
        count = other.count;
        capacity = other.capacity;
        data = other.data;
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
        return *this;
    }
};

struct UnkSoundSelectorMove_ovb56 {
    void** vtable;
    u8 name[0x30];  // 0x8, a CRntString
    u8 unk_0x38[0x98];
    UnkSelectorEntries_ovb56 entries;  // 0xd0
};
static_assert(sizeof(UnkSoundSelectorMove_ovb56) == 0xf8, "layout");

// Move-assignment: the name is copied and the other's entries taken over.
UnkSoundSelectorMove_ovb56* fn_7100fcb500(UnkSoundSelectorMove_ovb56* self, UnkSoundSelectorMove_ovb56* other) {
    if (self == other)
        return self;
    fn_710007eaa0(reinterpret_cast<base::global::CRntString*>(self->name),
                  reinterpret_cast<const base::global::CRntString*>(other->name));
    self->entries = static_cast<UnkSelectorEntries_ovb56&&>(other->entries);
    return self;
}

using base::global::UnkStringData;

struct UnkDropTarget;
struct UnkIdPair;

// An object given with its type (the object's vf_0x0).
class UnkSelectorSource_ovb56 {
public:
    virtual const void* vf_0x0();
};
struct UnkTypedObject_ovb56 {
    const void* type;
    void* object;

    UnkTypedObject_ovb56(UnkSelectorSource_ovb56* source) : type(source ? source->vf_0x0() : nullptr), object(source) {}
    UnkTypedObject_ovb56(const UnkTypedObject_ovb56& other) : type(other.type), object(other.object) {}
};

// The sound ids a selector picks (a string id and a file-path id, and their resource), released out
// of line.
struct UnkSelectedIds_ovb56 {
    UnkStringData* id;
    UnkStringData* path;
    void* resource;
    ~UnkSelectedIds_ovb56() { fn_7100026f70(reinterpret_cast<UnkIdPair*>(this)); }
};

struct UnkSoundEventsSelector_ovb56;
UnkSelectedIds_ovb56 fn_7100fc9e58(UnkSoundEventsSelector_ovb56* selector, void* event,
                                   const UnkTypedObject_ovb56& source);

// The owner the callers pass (game/sound/SGUISoundEventsSelector.h declares it).
struct UnkStateSink {
    u8 unk_0x0[0x38];
    UnkSoundEventsSelector_ovb56* selector;  // 0x38
};

// Plays the sound the selector picks for `event` from `source` through the drop manager; the empty
// handle when the pick has no id or no path.
UnkSoundHandle fn_7100fc9d78(UnkStateSink* self, UnkStateKey* event, void* state) {
    UnkSelectorSource_ovb56* source = static_cast<UnkSelectorSource_ovb56*>(state);
    UnkSoundEventsSelector_ovb56* selector = self->selector;
    UnkSelectedIds_ovb56 ids = fn_7100fc9e58(selector, event, UnkTypedObject_ovb56(source));
    UnkSoundHandle handle;
    if (ids.id != &g_unk_7101d47108->empty && ids.path != &g_unk_7101d47200->empty)
        handle = fn_7101024e28(g_unk_7101d47318, reinterpret_cast<const UnkStrIdPathPair*>(&ids), nullptr, false);
    else
        handle = {nullptr, -1};
    return handle;
}

using base::global::CStrId;

struct UnkSoundEventsSelector2_ovb56;

// Every selector made, with its id (two parallel arrays), destroyed at exit (fn_7100fcbce8).
struct UnkSelectorRegistry_ovb56 {
    UnkSoundEventsSelector2_ovb56** selectors;
    CStrId* ids;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkSelectorRegistry_ovb56()
        : selectors(nullptr), ids(nullptr), count(0), capacity(0), step(10), flags(0xe),
          allocator(fn_7100083990()) {}
    ~UnkSelectorRegistry_ovb56() asm("fn_7100fcbce8");
};
void fn_7100fcbde4(UnkSelectorRegistry_ovb56* self, u32 capacity);

inline UnkSelectorRegistry_ovb56& SelectorRegistry_ovb56() {
    static UnkSelectorRegistry_ovb56 s_registry;
    return s_registry;
}

struct UnkSoundEventsSelector2_ovb56 {
    CStrId id;
};

// Takes the id and adds the selector to the registry.
void fn_7100fc9084(UnkSoundEventsSelector2_ovb56* self, const CStrId* id) {
    new (&self->id) CStrId(*id);
    UnkSelectorRegistry_ovb56& registry = SelectorRegistry_ovb56();
    if (registry.count == registry.capacity)
        fn_7100fcbde4(&registry, registry.count + registry.step);
    // The id is copied as raw bytes, then retained.
    CStrId* slot = &registry.ids[registry.count];
    __builtin_memcpy(slot, &self->id, sizeof(CStrId));
    if (&g_unk_7101d47108->empty != slot->data)
        ++slot->data->ref_count;
    __builtin_memcpy(&registry.selectors[registry.count], &self, sizeof(self));
    ++registry.count;
}

using base::global::CRntString;
using base::reflection::UnkReflValue;

struct UnkEnumType;
UnkEnumType* fn_710009ffd4();  // base::global::TRntString256's type

// A string with 0x100 bytes of inline storage (the string part is 0x30 bytes, followed by its
// inline allocator).
struct UnkInlineString256_ovb56 {
    u8 str[0x30];
    void** vtable;
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;
};

// The string data's use count, a union member like ref_count.
struct UnkStringUse_ovb56 {
    u8 unk_0x0[0x44];
    union {
        u32 use_count;
    };
};

// The id's string (a CRntString at +0x10 of the string data), with its use count raised.
static inline const CRntString* UseStr_ovb56(UnkStringData* const& data) {
    ++reinterpret_cast<UnkStringUse_ovb56*>(data)->use_count;
    return reinterpret_cast<const CRntString*>(reinterpret_cast<char*>(data) + 0x10);
}

struct UnkSelectorEvent_ovb56 {
    u32 kind;
    UnkStringData* id;
};

struct UnkSelectorEvents_ovb56 {
    u8 unk_0x0[0xd0];
    UnkSelectorEvent_ovb56* events;  // 0xd0
    u32 count;  // 0xd8
};

struct UnkSelectorHolder_ovb56 {
    u8 unk_0x0[8];
    UnkSelectorEvents_ovb56* selector;
};

// The selector's events as one string ("@" before the ids of kind 0, joined with ","), as a reflected
// TRntString256 value.
UnkReflValue fn_7100fcbb3c(UnkSelectorHolder_ovb56* self) {
    UnkSelectorEvents_ovb56* selector = self->selector;
    UnkInlineString256_ovb56 s;
    CRntString* str = reinterpret_cast<CRntString*>(s.str);
    fn_710007e6e4(str, reinterpret_cast<base::UnkAllocator*>(&s.vtable));
    s.vtable = g_unk_7101a194b8 + 2;
    s.field_0x138 = 0;
    s.field_0x140 = 0;
    if (u32 count = selector->count) {
        UnkSelectorEvent_ovb56* event = selector->events;
        if (event->kind == 0)
            fn_710007f5ac(str, '@');
        fn_710007f3b4(str, UseStr_ovb56(event->id));
        for (UnkSelectorEvent_ovb56* it = event + 1, *end = event + count; it != end; ++it) {
            fn_710007f5ac(str, ',');
            if (it->kind == 0)
                fn_710007f5ac(str, '@');
            fn_710007f3b4(str, UseStr_ovb56(it->id));
        }
    }
    const base::reflection::CType* type = reinterpret_cast<const base::reflection::CType*>(fn_710009ffd4());
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, str, false);
    fn_710007ed40(str);
    fn_710007ecd4(str);
    return r;
}
