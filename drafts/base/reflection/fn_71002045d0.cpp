// Unit 281 ("base::reflection"): the rectangle-entry owner's copy assignment (the owner
// src/base/reflection/reflection_rects_ovb0061.cpp constructs), member by member. Names and
// layouts are placeholders.
#include <new>

#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "types.h"

using base::UnkAllocator;

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

struct UnkRect_ovb0061 {
    u32 x;
    u32 y;
    u32 w;
    u32 h;

    bool IsSet() const { return w || h || x || y; }
};

// A rectangle with two factors (0 and 1 by default); copied member by member.
struct UnkRectValue_ovb0061 {
    UnkRect_ovb0061 rect;
    f32 field_0x10;
    f32 field_0x14;

    UnkRectValue_ovb0061() : rect{0, 0, 0, 0}, field_0x10(0.0f), field_0x14(1.0f) {}
    UnkRectValue_ovb0061(const UnkRectValue_ovb0061& o)
        : rect{o.rect.x, o.rect.y, o.rect.w, o.rect.h}, field_0x10(o.field_0x10), field_0x14(o.field_0x14) {}
    UnkRectValue_ovb0061& operator=(const UnkRectValue_ovb0061&) = default;
};

struct UnkRectEntry_ovb0061 {
    void* owner;
    UnkRectValue_ovb0061 value;  // +0x8
    u8 selected;                 // +0x20

    UnkRectEntry_ovb0061() : owner(nullptr), selected(0) {}
    UnkRectEntry_ovb0061(const UnkRectEntry_ovb0061& o) : owner(o.owner), value(o.value) {
        // Copied as a plain byte (an untyped store: the array's count is read again after it).
        __builtin_memcpy(&selected, &o.selected, sizeof(selected));
    }
};
static_assert(sizeof(UnkRectEntry_ovb0061) == 0x28, "UnkRectEntry_ovb0061 layout");

struct UnkRectEntries_ovb0061 {
    UnkRectEntry_ovb0061* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    UnkAllocator* allocator;

    UnkRectEntry_ovb0061& operator[](u32 i) {
        if (!(i < count))
            __assert_fail("i < m_iSize", "..\\Base/global/RntVector.h", 0x15b, "operator[]");
        return data[i];
    }

    void Reallocate(u32 new_capacity) {
        UnkRectEntry_ovb0061* p = static_cast<UnkRectEntry_ovb0061*>(
            allocator->vf_0x40((u64)new_capacity * sizeof(UnkRectEntry_ovb0061), 8, flags));
        if (data) {
            UnkRectEntry_ovb0061* src = data;
            UnkRectEntry_ovb0061* dst = p;
            for (u32 i = 0; i < count; ++i, ++dst, ++src)
                new (dst) UnkRectEntry_ovb0061(*src);
            allocator->vf_0x48(data);
        }
        data = p;
        capacity = new_capacity;
    }
};

void fn_7100204460(UnkRectEntries_ovb0061* self, const UnkRectEntry_ovb0061* value);

// A pointer array with base/global/CRntVector.h's layout, made with its growth step and flags.
struct UnkRectPtrArray_ovb0061 {
    void** data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    UnkAllocator* allocator;

    UnkRectPtrArray_ovb0061(u32 step_, u32 flags_)
        : data(nullptr), count(0), capacity(0), step(step_), growable(true), flags(flags_),
          allocator(fn_7100083990()) {}
    UnkRectPtrArray_ovb0061(const UnkRectPtrArray_ovb0061& o)
        : data(nullptr), count(0), capacity(0), step(o.step), growable(o.growable), flags(o.flags),
          allocator(o.allocator) {
        if (u32 n = o.capacity)
            Reallocate(n);
        __builtin_memcpy(data, o.data, (u64)o.count * sizeof(void*));
        count = o.count;
    }

    void Reallocate(u32 n) {
        void** p = static_cast<void**>(allocator->vf_0x40((u64)n * sizeof(void*), 8, flags));
        if (data) {
            __builtin_memcpy(p, data, (u64)count * sizeof(void*));
            allocator->vf_0x48(data);
        }
        data = p;
        capacity = n;
    }

    void Reserve(u32 n) {
        if (capacity < n)
            Reallocate(n);
    }
};

// The entry array, assigned by emptying it, reserving the other's capacity and appending each entry.
struct UnkRectEntryList_l2045d0 : UnkRectEntries_ovb0061 {
    UnkRectEntryList_l2045d0& operator=(const UnkRectEntryList_l2045d0& other) {
        if (this != &other) {
            count = 0;
            if (capacity < other.capacity)
                Reallocate(other.capacity);
            for (const UnkRectEntry_ovb0061 *e = other.data, *end = e + other.count; e != end; ++e)
                fn_7100204460(this, e);
        }
        return *this;
    }
};

// A pointer array, assigned by copying the other's pointers in.
struct UnkRectPtrList_l2045d0 : UnkRectPtrArray_ovb0061 {
    UnkRectPtrList_l2045d0& operator=(const UnkRectPtrList_l2045d0& other) {
        if (this != &other) {
            count = 0;
            Reserve(other.capacity);
            __builtin_memcpy(data, other.data, (u64)other.count * sizeof(void*));
            count = other.count;
        }
        return *this;
    }
};

struct UnkRectColor_l2045d0 {
    f32 r, g, b, a;

    UnkRectColor_l2045d0& operator=(const UnkRectColor_l2045d0& o) {
        r = o.r;
        g = o.g;
        b = o.b;
        a = o.a;
        return *this;
    }
};

struct UnkRectName_l2045d0 {
    base::global::CRntString str;

    UnkRectName_l2045d0& operator=(const UnkRectName_l2045d0& o) {
        fn_710007eaa0(&str, &o.str);
        return *this;
    }
};

struct UnkRectParent_ovb0061;
struct UnkRectView_ovb0061;

// The fallback rectangle, the parent stack and the view: copied as one block.
struct UnkRectLayout_l2045d0 {
    UnkRectValue_ovb0061 fallback;    // +0x0
    UnkRectParent_ovb0061* stack[8];  // +0x18
    u32 depth;                        // +0x58
    u64 field_0x60;
    u64 field_0x68;
    UnkRectView_ovb0061* context;     // +0x70
};
static_assert(sizeof(UnkRectLayout_l2045d0) == 0x78, "UnkRectLayout_l2045d0 layout");

struct UnkRectDisplay_l2045d0 {
    u32 field_0x0;
    u32 field_0x4;
};

struct UnkRectFlags_l2045d0 {
    bool field_0x0;
    bool enabled;
    bool field_0x2;
    bool field_0x3;
    bool field_0x4;
};

struct UnkRectOwner_l2045d0 {
    u32 field_0x0;
    UnkRectEntryList_l2045d0 entries;  // +0x8
    UnkRectLayout_l2045d0 layout;      // +0x30
    UnkRectPtrList_l2045d0 field_0xa8;
    UnkRectDisplay_l2045d0 display;    // +0xd0
    UnkRectColor_l2045d0 color;        // +0xd8
    UnkRectPtrList_l2045d0 items;      // +0xe8
    UnkRectName_l2045d0 name;          // +0x110
    UnkRectFlags_l2045d0 flags;        // +0x140
};
static_assert(__builtin_offsetof(UnkRectOwner_l2045d0, name) == 0x110, "UnkRectOwner_l2045d0 layout");
static_assert(__builtin_offsetof(UnkRectOwner_l2045d0, flags) == 0x140, "UnkRectOwner_l2045d0 layout");

UnkRectOwner_l2045d0* fn_71002045d0(UnkRectOwner_l2045d0* self, const UnkRectOwner_l2045d0* other) {
    self->field_0x0 = other->field_0x0;
    self->entries = other->entries;
    self->layout = other->layout;
    self->field_0xa8 = other->field_0xa8;
    self->display = other->display;
    self->color = other->color;
    self->items = other->items;
    self->name = other->name;
    self->flags = other->flags;
    return self;
}
