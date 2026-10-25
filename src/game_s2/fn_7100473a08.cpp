// Looks up the entry of the table at +0x14b28 that belongs to the given owner (the handle at +0x38
// of an entry points at an object holding its owner at +0x10) and returns the 3-word value stored
// for it in the table's second array (12 bytes per entry). Entries in state 1 are first reset: their
// handle is replaced by a fresh one and the state becomes 2. When no entry is found the result is
// the default value.
#include "base/UnkRefHandle.h"
#include "types.h"

// The handle an entry refers to (base::UnkRefCounted's layout); its target, the entry's owner, is at +0x10.
struct UnkCounted : base::UnkRefCounted {
    void* owner;
};

// A counted reference; the destructor decrements and tests in one step.
struct UnkRef {
    UnkCounted* ptr;
    ~UnkRef() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }
};

struct UnkEntry {
    u8 field_0x0[8];
    u8 field_0x8[0x28];
    u32 state;
    u32 pad_0x34;
    base::UnkRefHandle handle;
};
static_assert(sizeof(UnkEntry) == 0x40, "UnkEntry layout");

// The result has a constructor, so it is returned in memory.
struct UnkValue {
    u32 x;
    u32 y;
    u32 z;
    UnkValue();  // fn_7100287598
    UnkValue(const UnkValue& other) : x(other.x), y(other.y), z(other.z) {}
};

struct UnkTable {
    UnkEntry* entries;
    u32 count;
    u8 pad_0xc[0x1c];
    UnkValue* values;
};

struct UnkSelf {
    u8 pad_0x0[0x14b28];
    UnkTable table;
};

UnkRef fn_710023b580(void* from);

UnkValue fn_7100473a08(UnkSelf* self, void* owner) {
    for (u64 i = 0; i < self->table.count; ++i) {
        UnkEntry* entry = &self->table.entries[i];
        if (entry->state == 1) {
            {
                UnkRef fresh = fn_710023b580(entry->field_0x8);
                // The reference's pointer is the handle's.
                fn_7100234f78(&entry->handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
            }
            entry->state = 2;
        }
        UnkCounted* handle = static_cast<UnkCounted*>(entry->handle.ptr);
        void* entry_owner = handle ? handle->owner : nullptr;
        if (entry_owner == owner)
            return self->table.values[i];
    }
    return UnkValue();
}
