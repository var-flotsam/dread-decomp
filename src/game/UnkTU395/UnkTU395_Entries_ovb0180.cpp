// Unit 395 (tools/tu_map.py): growable arrays and iterator values of the reflected collections:
// 16-byte entries (copy construction and assignment of the array) and 0x48-byte entries (an array,
// a string id and three words: clear, free, move and copy of the array, insert, erase, and the
// iterator value's copy and assignment). Names and layouts are placeholders.
#include <new>

#include "base/UnkCollectionIter.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// ---- 16-byte entries ----

struct UnkEntry16_ovb0180 {
    f32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
    f32 field_0xc;

    UnkEntry16_ovb0180(const UnkEntry16_ovb0180& other) { __builtin_memcpy(this, &other, sizeof(*this)); }
};
using UnkEntries16_ovb0180 = base::UnkVector<UnkEntry16_ovb0180>;

void fn_71002cbc20(UnkEntries16_ovb0180* self, const UnkEntries16_ovb0180* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    self->Reserve(other->capacity);
    for (const UnkEntry16_ovb0180 *p = other->data, *e = p + other->count; p != e; ++p)
        self->PushBackCopy(*p);
}

struct UnkPart;
UnkPart* fn_71002cbddc(UnkPart* dst, UnkPart* src) {
    UnkEntries16_ovb0180* self = reinterpret_cast<UnkEntries16_ovb0180*>(dst);
    const UnkEntries16_ovb0180* other = reinterpret_cast<const UnkEntries16_ovb0180*>(src);
    if (self != other) {
        self->count = 0;
        self->Reserve(other->capacity);
        for (const UnkEntry16_ovb0180 *p = other->data, *e = p + other->count; p != e; ++p)
            self->PushBackCopy(*p);
    }
    return dst;
}


// ---- 0x48-byte entries ----

struct UnkList {
    base::UnkVector<void*> list;
    UnkList() : list(0xf) {}
    UnkList(const UnkList& other) asm("fn_71002cbc20");
    UnkList& operator=(const UnkList& other) asm("fn_71002cbddc");
    ~UnkList() { list.Free(); }
};

struct UnkBlock24 {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
};

struct UnkElement {
    UnkList field_0x0;
    // Released by hand: the destructor decrements and tests in two steps, like CStrId's move.
    union {
        CStrId field_0x28;
        base::global::UnkStringData* id;
    };
    UnkBlock24 field_0x30;

    // Takes a reference to the id with an int-typed increment: the original keeps the pool pointer
    // in a register across it.
    void RefId() {
        if (&g_unk_7101d47108->empty != id) {
            s32* count = &id->ref_count;
            ++*count;
        }
    }

    UnkElement();
    UnkElement& operator=(const UnkElement& other) {
        field_0x0 = other.field_0x0;
        field_0x28 = other.field_0x28;
        field_0x30 = other.field_0x30;
        return *this;
    }
    UnkElement(const UnkElement& other) : field_0x0(other.field_0x0), id(other.id) {
        RefId();
        __builtin_memcpy(&field_0x30, &other.field_0x30, sizeof(field_0x30));
    }
    UnkElement(UnkElement&& other) : field_0x0(other.field_0x0), id(other.id) {
        RefId();
        field_0x30.field_0x0 = other.field_0x30.field_0x0;
        field_0x30.field_0x8 = other.field_0x30.field_0x8;
        field_0x30.field_0x10 = other.field_0x30.field_0x10;
    }
    ~UnkElement() {
        base::global::UnkStringPool* pool = g_unk_7101d47108;
        if (pool) {
            base::global::UnkStringData* data = field_0x28.data;
            if (data && data != &pool->empty) {
                --field_0x28.data->ref_count;
                if (field_0x28.data->ref_count == 0) {
                    fn_71000803a4(pool, field_0x28.data);
                    pool = g_unk_7101d47108;
                }
            }
            field_0x28.data = &pool->empty;
        }
    }
};

inline UnkElement::UnkElement() : id(g_unk_7101d470e0.data) {
    RefId();
    field_0x30.field_0x10 = 0;
    field_0x30.field_0x8 = 0;
    field_0x30.field_0x0 = 0xbf800000bf800000ull;
}

typedef base::UnkVector<UnkElement> UnkElements;
void fn_71002cd854(base::UnkVector<UnkElement>* self, u32 capacity);

void fn_71002cd178(UnkElements* v) { v->Clear(); }

void fn_71002cd4c0(UnkElements* v) { v->Free(); }

UnkElements* fn_71002cd5ec(UnkElements* v, UnkElements* other) { return &(*v = static_cast<UnkElements&&>(*other)); }

// PushBack's growth with the array grown out of line (fn_71002cd854), copying the element in.
static inline __attribute__((always_inline)) void PushBackCopyOol(UnkElements* v, const UnkElement& value) {
    u32 n = v->count;
    u32 old_capacity = v->capacity;
    u32 new_capacity = old_capacity;
    if (old_capacity <= n && v->step != 0)
        new_capacity = (n / v->step + 1) * v->step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    fn_71002cd854(v, new_capacity);
    new (&v->data[v->count]) UnkElement(value);
    ++v->count;
}

void fn_71002cd720(UnkElements* self, const UnkElements* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    fn_71002cd854(self, other->capacity);
    for (const UnkElement *p = other->data, *e = p + other->count; p != e; ++p)
        PushBackCopyOol(self, *p);
}

UnkElements* fn_71002cd9b8(UnkElements* self, const UnkElements* other) {
    if (self != other) {
        self->Clear();
        fn_71002cd854(self, other->capacity);
        for (const UnkElement *p = other->data, *e = p + other->count; p != e; ++p)
            PushBackCopyOol(self, *p);
    }
    return self;
}

// Insert with the array grown out of line.
static inline __attribute__((always_inline)) void InsertOol(UnkElements* v, UnkElement&& value, u32 index) {
    u32 n = v->count;
    u32 cap = v->capacity;
    if (n == index) {
        u32 new_capacity = cap;
        if (cap <= n && v->step != 0)
            new_capacity = (index / v->step + 1) * v->step;
        if ((s32)new_capacity <= (s32)index)
            for (;;) {
            }
        fn_71002cd854(v, new_capacity);
        new (&v->data[v->count]) UnkElement(value);
    } else {
        u32 new_capacity = cap;
        if (cap <= n && v->step != 0)
            new_capacity = (n / v->step + 1) * v->step;
        fn_71002cd854(v, new_capacity);
        for (u32 i = v->count - 1; i >= index; --i) {
            UnkElement* q = &v->data[i];
            new (q + 1) UnkElement(static_cast<UnkElement&&>(*q));
            v->data[i].~UnkElement();
            if (i == 0)
                break;
        }
        new (&v->data[index]) UnkElement(value);
    }
    ++v->count;
}

void fn_71002cdb84(UnkElements* v, UnkElement* value, u32 index) {
    InsertOol(v, static_cast<UnkElement&&>(*value), index);
}

void fn_71002cddd0(UnkElements* v, UnkElement* value, u32 index) {
    InsertOol(v, static_cast<UnkElement&&>(*value), index);
}

struct UnkCollection48_A2_b68;
void fn_71002ce01c(UnkCollection48_A2_b68* collection, u32 index) {
    UnkElements* self = reinterpret_cast<UnkElements*>(collection);
    if (index < self->count) {
        u64 i = index;
        self->data[i].~UnkElement();
        for (; i < (u32)(self->count - 1); ++i) {
            UnkElement* cur = &self->data[i];
            new (cur) UnkElement(static_cast<UnkElement&&>(cur[1]));
            (&self->data[i])[1].~UnkElement();
        }
        self->count = self->count - 1;
    }
}

__attribute__((flatten)) void fn_71002cb454(base::UnkCollectionIter<UnkElement>* it) { base::refl_IterSync(it); }

typedef base::UnkIterValue<UnkElement, u32> UnkElementValue;

void fn_71002cc9fc(void* dst, const void* src) {
    UnkElementValue* d = static_cast<UnkElementValue*>(dst);
    const UnkElementValue* s = static_cast<const UnkElementValue*>(src);
    d->state = 0;
    if (s->state == 1) {
        d->state = 1;
        new (&d->value) UnkElementValue::Value{s->value.index, s->value.element};
    } else if (s->state == 2) {
        d->state = 1;
        UnkElementValue::Ref r = s->ref;
        new (&d->value) UnkElementValue::Value{*r.index, *r.element};
    }
}

void fn_71002ccf54(void* dst, const void* src) {
    UnkElementValue* d = static_cast<UnkElementValue*>(dst);
    const UnkElementValue* s = static_cast<const UnkElementValue*>(src);
    d->state = 0;
    if (s->state == 1) {
        d->state = 1;
        new (&d->value) UnkElementValue::Value{s->value.index, s->value.element};
    } else if (s->state == 2) {
        d->state = 1;
        UnkElementValue::Ref r = s->ref;
        new (&d->value) UnkElementValue::Value{*r.index, *r.element};
    }
}

// The copy assignment of the pair an iterator value refers to: nothing for an empty one.
UnkElementValue* fn_71002cccf8(UnkElementValue* self, const UnkElementValue* other) {
    if (other->state == 1) {
        self->Index() = other->value.index;
        self->Element() = other->value.element;
    } else if (other->state == 2) {
        self->Index() = *other->ref.index;
        self->Element() = *other->ref.element;
    }
    return self;
}
