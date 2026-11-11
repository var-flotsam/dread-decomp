// CCutSceneDef's unit (tools/tu_map.py): the copy constructor and copy assignment of an array
// (base::UnkVector's layout) of 0x40-byte elements, through the unit's out-of-line Reserve
// (fn_7100c8b758) and push-back (fn_7100c8b894); the assignment destroys the old elements one by one
// (fn_7100c8a490) first. The shape of fn_71010012d4 (src/game/GUI/CMissionLog/fn_7100b5bfdc.cpp).
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkTU2200.h"
#include "types.h"

struct UnkElem40_b42a3 {
    u8 unk_0x0[0x40];
};

struct UnkElem40Array_b42a3 {
    UnkElem40_b42a3* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_7100c8b894(UnkElem40Array_b42a3* self, const UnkElem40_b42a3* value);
void fn_7100c9115c(UnkElem40_b42a3* self);  // the element's constructor, after it is cleared
void fn_7100c91450(void*, const void*);  // the element's copy assignment, as src/clones/clones_04.cpp declares it
void fn_7100c9165c(void*, const void*);  // the element's move assignment, as src/clones/clones_04.cpp declares it

static inline void AppendAll_b42a3(UnkElem40Array_b42a3* self, const UnkElem40Array_b42a3& other) {
    fn_7100c8b758(self, other.capacity);
    for (const UnkElem40_b42a3 *it = other.data, *end = it + other.count; it != end; ++it)
        fn_7100c8b894(self, it);
}

void fn_7100c92280(UnkElem40Array_b42a3* self, const UnkElem40Array_b42a3& other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other.step;
    self->field_0x14 = other.field_0x14;
    self->flags = other.flags;
    self->allocator = other.allocator;
    AppendAll_b42a3(self, other);
}

UnkElem40Array_b42a3* fn_7100c923c8(UnkElem40Array_b42a3* self, const UnkElem40Array_b42a3& other) {
    if (self != &other) {
        for (u64 i = 0; i < self->count; ++i)
            fn_7100c8a490(&self->data[i]);
        self->count = 0;
        AppendAll_b42a3(self, other);
    }
    return self;
}

// Sets the element at an index (slots 0x140 and 0x148 of the reflected collection type): an index
// past the end of a growable array first reserves its capacity (grown to the next multiple of the step
// when the index reaches it) and then index + 1 elements, value-initialises the new ones (cleared, then constructed) and extends the count; the
// element is then copy- or move-assigned.
static inline UnkElem40_b42a3* GrowTo_b42a3(UnkElem40Array_b42a3* self, u32 index) {
    if (self->field_0x14 && self->count <= index) {
        u32 capacity = self->capacity;
        if (capacity <= index && self->step != 0)
            capacity = (index / self->step + 1) * self->step;
        fn_7100c8b758(self, capacity);
        u32 n = index + 1;
        fn_7100c8b758(self, n);
        for (u32 i = self->count; i < n; ++i) {
            UnkElem40_b42a3* p = &self->data[i];
            __builtin_memset(p, 0, sizeof(*p));
            fn_7100c9115c(p);
        }
        self->count = n;
    }
    return &self->data[index];
}

void fn_7100c92080(UnkElem40Array_b42a3* self, const u32* index, const UnkElem40_b42a3* value) {
    fn_7100c91450(GrowTo_b42a3(self, *index), value);
}

void fn_7100c92150(UnkElem40Array_b42a3* self, const u32* index, UnkElem40_b42a3* value) {
    fn_7100c9165c(GrowTo_b42a3(self, *index), value);
}

// The same for the unit's array of 0xa0-byte elements (reserve fn_7100c8b9c8, constructor
// fn_7100c8d58c, copy and move assignment fn_7100c8d9c0 and fn_7100c8e034).
struct UnkElemA0_b42a3 {
    u8 unk_0x0[0xa0];
};

struct UnkElemA0Array_b42a3 {
    UnkElemA0_b42a3* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_7100c8d58c(UnkElemA0_b42a3* self);
void fn_7100c8d9c0(void*, const void*);  // as src/clones/ declares it
void fn_7100c8e034(void*, const void*);  // as src/clones/ declares it

static inline UnkElemA0_b42a3* GrowTo_b42a3(UnkElemA0Array_b42a3* self, u32 index) {
    if (self->field_0x14 && self->count <= index) {
        u32 capacity = self->capacity;
        if (capacity <= index && self->step != 0)
            capacity = (index / self->step + 1) * self->step;
        fn_7100c8b9c8(self, capacity);
        u32 n = index + 1;
        fn_7100c8b9c8(self, n);
        for (u32 i = self->count; i < n; ++i) {
            UnkElemA0_b42a3* p = &self->data[i];
            __builtin_memset(p, 0, sizeof(*p));
            fn_7100c8d58c(p);
        }
        self->count = n;
    }
    return &self->data[index];
}

void fn_7100c8ec48(UnkElemA0Array_b42a3* self, const u32* index, const UnkElemA0_b42a3* value) {
    fn_7100c8d9c0(GrowTo_b42a3(self, *index), value);
}

void fn_7100c8ed38(UnkElemA0Array_b42a3* self, const u32* index, UnkElemA0_b42a3* value) {
    fn_7100c8e034(GrowTo_b42a3(self, *index), value);
}
