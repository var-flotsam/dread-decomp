// CHearingSensor unit (unit 2580 of tools/tu_map.py): refills the sensor's holders (+0x20) with the
// sound sources of a list that the sensor's position (+0x8) lies within the radius of.
// Names and layouts are placeholders.
#include <new>

#include "types.h"

extern char g_unk_7101caa330[];  // the holder's base vtable
extern char g_unk_7101caa308[];  // the holder's vtable

// A counted sound source: its position and radius at +0x10.
struct UnkHearingSource {
    virtual ~UnkHearingSource();
    u8 pad_0x8[0x8];
    f32 x;  // 0x10
    f32 y;  // 0x14
    f32 z;  // 0x18
    f32 radius;  // 0x1c
    u8 pad_0x20[0x10];
    s32 ref_count;  // 0x30
};
static_assert(__builtin_offsetof(UnkHearingSource, ref_count) == 0x30, "UnkHearingSource layout");

static inline void Release(UnkHearingSource* p) {
    if (p && --p->ref_count == 0)
        delete p;
}

// A counted reference held while a holder is built from it.
struct UnkHearingRef {
    UnkHearingSource* p;

    explicit UnkHearingRef(UnkHearingSource* source) {
        p = nullptr;
        if (source) {
            ++source->ref_count;
            p = source;
        }
    }
    ~UnkHearingRef() { Release(p); }
};

// A holder's counted reference.
struct UnkHearingSlot {
    UnkHearingSource* p;

    void Assign(UnkHearingSource* source) {
        if (source) {
            ++source->ref_count;
            if (UnkHearingSource* old = p) {
                if (--old->ref_count == 0)
                    delete old;
            }
            p = source;
        }
    }
};

struct UnkHearingEntry;
void fn_7100f04300(UnkHearingEntry* self, const UnkHearingRef* source);

// The holder (0x18 bytes): a word and a counted source.
struct UnkHearingEntry {
    char* vtable;
    u32 value;
    UnkHearingSlot shared;

    explicit UnkHearingEntry(const UnkHearingRef& source) { fn_7100f04300(this, &source); }
    UnkHearingEntry(const UnkHearingEntry& other) : shared() {
        vtable = g_unk_7101caa330 + 0x10;
        value = other.value;
        vtable = g_unk_7101caa308 + 0x10;
        shared.Assign(other.shared.p);
    }
    ~UnkHearingEntry() {
        vtable = g_unk_7101caa308 + 0x10;
        Release(shared.p);
    }
    // The virtual destructor (slot 0).
    void Destroy() { reinterpret_cast<void (*)(UnkHearingEntry*)>(*reinterpret_cast<void**>(vtable))(this); }
};
static_assert(sizeof(UnkHearingEntry) == 0x18, "UnkHearingEntry layout");

struct UnkHearingEntries {
    UnkHearingEntry* data;
    u32 count;
    u32 capacity;
    u32 step;
};

// Signature as src/game/CHearingSensor/CHearingSensor_Reserve_ovb0129.cpp defines it.
struct UnkHearingHolderArray_b0129;
void fn_7100f038f8(UnkHearingHolderArray_b0129* self, u32 n);

static inline void Append(UnkHearingEntries& list, const UnkHearingEntry& value) {
    u32 n = list.count;
    u32 new_capacity = list.capacity;
    if (new_capacity <= n && list.step != 0)
        new_capacity = (n / list.step + 1) * list.step;
    // A capacity that doesn't grow past the count (no step) hangs, as in the original.
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    fn_7100f038f8(reinterpret_cast<UnkHearingHolderArray_b0129*>(&list), new_capacity);
    new (&list.data[list.count]) UnkHearingEntry(value);
    ++list.count;
}

struct UnkHearingSources {
    UnkHearingSource** data;
    u64 count;
};

struct UnkHearingSensor {
    u8 pad_0x0[0x8];
    f32 x;  // 0x8
    f32 y;  // 0xc
    f32 z;  // 0x10
    u8 pad_0x14[0xc];
    UnkHearingEntries entries;  // 0x20
};
static_assert(__builtin_offsetof(UnkHearingSensor, entries) == 0x20, "UnkHearingSensor layout");

void fn_7100f035dc(UnkHearingSensor* self, const UnkHearingSources* sources) {
    for (u64 i = 0; i < self->entries.count; ++i)
        self->entries.data[i].Destroy();
    self->entries.count = 0;

    for (UnkHearingSource **it = sources->data, **end = sources->data + sources->count; it != end; ++it) {
        UnkHearingSource* source = *it;
        f32 dx = source->x - self->x;
        f32 dy = source->y - self->y;
        f32 dz = source->z - self->z;
        if (dx * dx + dy * dy + dz * dz < source->radius * source->radius)
            Append(self->entries, UnkHearingEntry(UnkHearingRef(*it)));
    }
}
