// UnkTU372: the move constructor of a record that holds a string id, two moved arrays, a small
// fixed list of (id, shared object, value) entries and a callback re-bound to the new record.
// Names and layouts are placeholders.
#include <new>

#include "types.h"
#include "base/global/CStrId.h"
#include "game/UnkTU372.h"
#include "game/UnkTU39.h"

using base::global::CStrId;

void fn_71002b44fc();

struct UnkShared_l2b3114 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Destroy();
    s32 ref_count;
};

// An intrusive reference to a shared object.
struct UnkSharedRef_l2b3114 {
    UnkShared_l2b3114* ptr;

    UnkSharedRef_l2b3114() : ptr(nullptr) {}
    UnkSharedRef_l2b3114(const UnkSharedRef_l2b3114& other) : ptr(nullptr) { *this = other; }
    UnkSharedRef_l2b3114& operator=(const UnkSharedRef_l2b3114& other) {
        UnkShared_l2b3114* object = other.ptr;
        if (object)
            ++object->ref_count;
        if (ptr && --ptr->ref_count == 0)
            ptr->Destroy();
        ptr = object;
        return *this;
    }
};

struct UnkEntry_l2b3114 {
    CStrId id;
    UnkSharedRef_l2b3114 object;
    u32 value;
};

// An array that gives its storage away when moved.
struct UnkArray_l2b3114 {
    void* data;
    u32 size;
    u32 capacity;
    u32 field_0x10;
    u8 field_0x14;
    u32 field_0x18;
    u64 field_0x20;

    UnkArray_l2b3114(UnkArray_l2b3114&& other)
        : data(other.data), size(other.size), capacity(other.capacity), field_0x10(other.field_0x10),
          field_0x14(other.field_0x14), field_0x18(other.field_0x18), field_0x20(other.field_0x20) {
        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;
    }
};

struct UnkEntryList_l2b3114 {
    UnkArray_l2b3114 array;
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
    u32 count;
    // Storage for up to eight entries, constructed as they are added.
    alignas(UnkEntry_l2b3114) u8 storage[8 * sizeof(UnkEntry_l2b3114)];

    UnkEntry_l2b3114* Entries() { return reinterpret_cast<UnkEntry_l2b3114*>(storage); }

};

struct UnkFlags_l2b3114 {
    u8 a;
    u8 b;
    u8 c;
};

struct UnkCallback_l2b3114 {
    UnkLinkList* link;
    void* object;
    void (*function)();

    UnkCallback_l2b3114(UnkCallback_l2b3114&& other)
        : link(other.link), object(other.object), function(other.function) {
        if (other.link) {
            fn_71002b46ec(other.link, &other.link);
            other.link = nullptr;
        }
        other.object = nullptr;
        other.function = nullptr;
        if (link)
            fn_71002b4538(link, &link);
    }
};

struct UnkRecord_l2b3114 {
    u64 field_0x0;
    CStrId name;
    u32 field_0x10;
    alignas(8) u8 field_0x18[0xb0];
    UnkArray_l2b3114 array;          // 0xc8
    UnkEntryList_l2b3114 entries;    // 0xf0
    UnkFlags_l2b3114 state;          // 0x1e8
    UnkFlags_l2b3114 flags;          // 0x1ec
    UnkCallback_l2b3114 callback;    // 0x1f0
};
static_assert(__builtin_offsetof(UnkRecord_l2b3114, array) == 0xc8, "UnkRecord_l2b3114 layout");
static_assert(__builtin_offsetof(UnkRecord_l2b3114, entries) == 0xf0, "UnkRecord_l2b3114 layout");
static_assert(__builtin_offsetof(UnkRecord_l2b3114, state) == 0x1e8, "UnkRecord_l2b3114 layout");
static_assert(__builtin_offsetof(UnkRecord_l2b3114, callback) == 0x1f0, "UnkRecord_l2b3114 layout");

void fn_71002b3114(UnkRecord_l2b3114* self, UnkRecord_l2b3114* other) {
    self->field_0x0 = other->field_0x0;
    self->name.data = other->name.data;
    other->name.data = &g_unk_7101d47108->empty;
    self->field_0x10 = other->field_0x10;
    ConstructIdListCopyingNodesAndMovingCallbacks(self->field_0x18, other->field_0x18);
    new (&self->array) UnkArray_l2b3114(static_cast<UnkArray_l2b3114&&>(other->array));
    UnkEntryList_l2b3114& entries = self->entries;
    UnkEntryList_l2b3114& source = other->entries;
    new (&entries.array) UnkArray_l2b3114(static_cast<UnkArray_l2b3114&&>(source.array));
    entries.field_0x28 = source.field_0x28;
    entries.field_0x2c = source.field_0x2c;
    entries.field_0x30 = source.field_0x30;
    for (u64 i = 0; i < source.count; i++)
        new (&entries.Entries()[i]) UnkEntry_l2b3114(source.Entries()[i]);
    entries.count = source.count;
    self->state = UnkFlags_l2b3114();
    self->flags = other->flags;
    new (&self->callback) UnkCallback_l2b3114(static_cast<UnkCallback_l2b3114&&>(other->callback));
    self->callback.object = self;
    self->callback.function = fn_71002b44fc;
}
