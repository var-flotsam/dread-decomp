// Unit 281 (tools/tu_map.py, "base::reflection"): an owner of rectangle entries (0x28 bytes each, in a
// growable array at +8; src/base/reflection/UnkUnit281_RectSize_A1_b76.cpp reads them), with a fallback
// rectangle at +0x30, a stack of up to eight parents at +0x48 and a view at +0xa0. Here: the owner's
// constructor (fn_71001ffb38), an entry's rectangle (fn_71002028c4), dropping the view (fn_710020002c),
// and the entry array's resize (fn_71002056f0), growing access (fn_71002002c0) and copy constructor
// (fn_7100204338). The arrays have base/UnkVector.h's layout. Names and layouts are placeholders.
#include <new>

#include "base/UnkNamedRegistry.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/reflection.h"
#include "base/tunable/CTunable.h"
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

// Resizes to n entries, default-constructing the new ones.
void fn_71002056f0(UnkRectEntries_ovb0061* self, u32 n) {
    if (self->capacity < n)
        self->Reallocate(n);
    for (u32 i = self->count; i < n; ++i)
        new (&self->data[i]) UnkRectEntry_ovb0061();
    self->count = n;
}

// The entry at `index`; when the array may grow, an index past the end first grows it to the next
// multiple of the step and then to exactly index + 1 entries.
UnkRectEntry_ovb0061* fn_71002002c0(UnkRectEntries_ovb0061* self, u32 index) {
    if (self->growable && self->count <= index) {
        u32 cap = self->capacity;
        if (cap <= index && self->step != 0) {
            u32 grown = (index / self->step + 1) * self->step;
            if (cap < grown)
                self->Reallocate(grown);
        }
        fn_71002056f0(self, index + 1);
    }
    return &self->data[index];
}

// The copy constructor: the same settings, the other's capacity, and each entry appended
// (fn_7100204460).
void fn_7100204338(UnkRectEntries_ovb0061* self, const UnkRectEntries_ovb0061* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->growable = other->growable;
    self->flags = other->flags;
    self->allocator = other->allocator;
    if (u32 n = other->capacity)
        self->Reallocate(n);
    const UnkRectEntry_ovb0061* end = other->data + other->count;
    for (const UnkRectEntry_ovb0061* e = other->data; e != end; ++e)
        fn_7100204460(self, e);
}

// The view the owner draws into.
struct UnkRectViewVisitor_ovb0061 {
    void** vtable;
};

struct UnkRectViewRoot_ovb0061 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78(UnkRectViewVisitor_ovb0061* visitor);
};

struct UnkRectView_ovb0061 {
    virtual void vf_0x0();
    virtual void Delete();  // the deleting destructor
    UnkRectViewRoot_ovb0061* root;  // +0x8
    u8 pad_0x10[0x18];
    void* field_0x28;
};

// The parent the owner is created under.
struct UnkRectParent_ovb0061 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual u32 vf_0x40();
    virtual s32 vf_0x48();
};

// A pointer array with base/UnkVector.h's layout, made with its growth step and flags.
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

// The array at +0xa8, made with room for 0x800 pointers.
struct UnkRectReservedArray_ovb0061 : UnkRectPtrArray_ovb0061 {
    UnkRectReservedArray_ovb0061() : UnkRectPtrArray_ovb0061(0x100, 0x39) { Reserve(0x800); }
};

struct UnkRectColor_ovb0061 {
    f32 r, g, b, a;
    UnkRectColor_ovb0061() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}
    UnkRectColor_ovb0061(const UnkRectColor_ovb0061& o) : r(o.r), g(o.g), b(o.b), a(o.a) {}
};

// The name, a copy of g_unk_7101d58d10 (fn_710007ea60).
struct UnkRectName_ovb0061 {
    union {
        base::global::CRntString str;
    };
    UnkRectName_ovb0061() {
        fn_710007ea60(reinterpret_cast<base::UnkNamedEntry*>(&str),
                      reinterpret_cast<const base::UnkNamedEntry*>(&g_unk_7101d58d10));
    }
    UnkRectName_ovb0061(const UnkRectName_ovb0061& o) {
        fn_710007ea60(reinterpret_cast<base::UnkNamedEntry*>(&str), reinterpret_cast<const base::UnkNamedEntry*>(&o.str));
    }
    ~UnkRectName_ovb0061() { fn_710007ecd4(&str); }
};

void fn_7100204338(UnkRectEntries_ovb0061* self, const UnkRectEntries_ovb0061* other);

struct UnkRectEntriesInit_ovb0061 : UnkRectEntries_ovb0061 {
    UnkRectEntriesInit_ovb0061(const UnkRectEntriesInit_ovb0061& o) { fn_7100204338(this, &o); }
    UnkRectEntriesInit_ovb0061() {
        data = nullptr;
        count = 0;
        capacity = 0;
        step = 10;
        growable = true;
        flags = 0xf;
        allocator = fn_7100083990();
    }
};

struct UnkRectOwner_ovb0061 {
    u32 field_0x0;
    UnkRectEntriesInit_ovb0061 entries;  // +0x8
    UnkRectValue_ovb0061 fallback;       // +0x30
    UnkRectParent_ovb0061* stack[8];     // +0x48
    u32 depth = 0;                       // +0x88
    u64 field_0x90 = 0;
    u64 field_0x98 = 0;
    UnkRectView_ovb0061* context = nullptr;  // +0xa0
    UnkRectReservedArray_ovb0061 field_0xa8;
    u32 field_0xd0;
    u32 field_0xd4;
    UnkRectColor_ovb0061 color;              // +0xd8
    UnkRectPtrArray_ovb0061 items{10, 0xf};  // +0xe8
    UnkRectName_ovb0061 name;                // +0x110
    bool field_0x140 = true;
    bool enabled = true;  // +0x141
    bool field_0x142 = true;
    bool field_0x143 = false;
    bool field_0x144 = true;

    __attribute__((always_inline)) UnkRectOwner_ovb0061(u32 a, UnkRectParent_ovb0061* parent, u32 b, bool flag);
    __attribute__((always_inline)) UnkRectOwner_ovb0061(const UnkRectOwner_ovb0061&) = default;
};
static_assert(__builtin_offsetof(UnkRectOwner_ovb0061, name) == 0x110, "UnkRectOwner_ovb0061 layout");
static_assert(__builtin_offsetof(UnkRectOwner_ovb0061, field_0x144) == 0x144, "UnkRectOwner_ovb0061 layout");

u32 fn_7100132f80();  // the display width
u32 fn_7100132f90();  // the display height
void fn_71001ffd58(UnkRectOwner_ovb0061* self);

// The constructor: pushes the parent, takes its +0x40 value, and without `flag` sizes the fallback
// itself (fn_71001ffd58); with it, centres a 640x480 rectangle when the parent's mode is 1, else
// covers the display (centred horizontally unless +0xd0 is set).
inline UnkRectOwner_ovb0061::UnkRectOwner_ovb0061(u32 a, UnkRectParent_ovb0061* parent, u32 b, bool flag)
    : field_0x0(a), field_0xd4(b) {
    if (depth < 8)
        stack[depth++] = parent;
    field_0xd0 = parent->vf_0x40();
    parent->vf_0x48();
    if (!flag) {
        fn_71001ffd58(this);
        return;
    }
    if (parent->vf_0x48() == 1) {
        fallback.rect.x = (fn_7100132fc0() - 0x280) / 2;
        fallback.rect.y = fn_7100132f90();
        fallback.rect.w = 0x280;
        fallback.rect.h = 0x1e0;
    } else {
        fallback.rect.x = field_0xd0 ? 0 : (fn_7100132fc0() - fn_7100132f80()) / 2;
        fallback.rect.y = 0;
        fallback.rect.w = fn_7100132f80();
        fallback.rect.h = fn_7100132f90();
    }
}

void fn_71001ffb38(UnkRectOwner_ovb0061* self, u32 a, UnkRectParent_ovb0061* parent, u32 b, bool flag) {
    new (self) UnkRectOwner_ovb0061(a, parent, b, flag);
}

// The copy constructor.
void fn_7100204110(UnkRectOwner_ovb0061* self, const UnkRectOwner_ovb0061* other) {
    new (self) UnkRectOwner_ovb0061(*other);
}

// The entry's rectangle and factors, or the fallback when its rectangle is unset.
void fn_71002028c4(UnkRectOwner_ovb0061* self, UnkRectValue_ovb0061* out, u32 i) {
    if (self->entries[i].value.rect.IsSet())
        *out = self->entries.data[i].value;
    else
        *out = self->fallback;
}

void fn_71001fd700(void* item);
void fn_71001ee6f8(void* view);
void fn_71001eefb4(void* view);
void fn_71001ef9a4(void* view, u32 arg);
void fn_71001ee784(void* view);

// The visitor handed to the view's root (vtable 0x7101a1f500).
extern void* g_unk_7101a1f500[];

// Detaches the view: hands it each item (fn_71001eed18) and releases them, empties the list, clears the
// owner's link to it, visits its root, deletes it, and resets the name.
void fn_710020002c(UnkRectOwner_ovb0061* self, UnkRectView_ovb0061** view) {
    void** end = self->items.data + self->items.count;
    for (void** it = self->items.data; it != end; ++it) {
        fn_71001eed18(*view, *it);
        fn_71001fd700(*it);
    }
    self->items.count = 0;
    fn_71001ee6f8(*view);
    fn_71001eefb4(*view);
    if (self->context == *view)
        self->context = nullptr;
    (*view)->field_0x28 = nullptr;
    fn_71001ef9a4(*view, 0);
    UnkRectViewVisitor_ovb0061 visitor;
    visitor.vtable = g_unk_7101a1f500 + 2;
    (*view)->root->vf_0x78(&visitor);
    fn_71001ee784(*view);
    if (*view) {
        (*view)->Delete();
        *view = nullptr;
    }
    fn_710007eaa0(&self->name.str, &g_unk_7101d58d10);
}
