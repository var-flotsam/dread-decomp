// Unit 2565 (tools/tu_map.py): a keyed array of 0x248-byte records (values at +0, their u32 keys at +8, count,
// capacity, step, flags and allocator after them; each record starts with a string): its destructor
// (fn_7100e7a338), move assignment (fn_7100e7c018) and equality (fn_7100e7b980); the minimap cell's
// neighbour flags (fn_7100e78060); and a run-length text of one row of the minimap grid, one symbol of
// the character table per cell kind (fn_7100e7a110). Names and layouts are placeholders.
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;

struct UnkCompared_0x30;

struct UnkRecord_b0238 {
    CRntString name;
    u8 pad_0x30[0x218];
};
static_assert(sizeof(UnkRecord_b0238) == 0x248, "layout");

struct UnkRecordMap_b0238 {
    UnkRecord_b0238* values;
    u32* keys;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 field_0x1c;
    base::UnkAllocator* allocator;

    void DestroyValues() {
        if (values && count) {
            for (u32 i = 0; i < count; ++i) {
                UnkRecord_b0238* e = &values[i];
                fn_710007ed40(&e->name);
                fn_710007ecd4(&e->name);
            }
        }
    }
    UnkRecord_b0238* Find(u32 key) const {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return &values[count];
    }
};

void fn_7100e7a338(UnkRecordMap_b0238* self) {
    self->DestroyValues();
    u32* keys = self->keys;
    self->count = 0;
    if (keys)
        self->allocator->vf_0x48(keys);
    if (self->values)
        self->allocator->vf_0x48(self->values);
}

UnkRecordMap_b0238* fn_7100e7c018(UnkRecordMap_b0238* self, UnkRecordMap_b0238* other) {
    self->DestroyValues();
    self->count = 0;
    self->allocator = other->allocator;
    self->keys = other->keys;
    self->values = other->values;
    self->capacity = other->capacity;
    self->count = other->count;
    self->field_0x1c = other->field_0x1c;
    self->field_0x18 = other->field_0x18;
    other->count = 0;
    other->values = nullptr;
    other->keys = nullptr;
    return self;
}

bool fn_7100e7b980(const UnkRecordMap_b0238* self, const UnkRecordMap_b0238* other) {
    if (self->count != other->count)
        return false;
    for (u32 i = 0; i < self->count; ++i) {
        UnkRecord_b0238* found = other->Find(self->keys[i]);
        if (found == &other->values[other->count])
            return false;
        if (!fn_710007f120(reinterpret_cast<const UnkCompared_0x30*>(found),
                           reinterpret_cast<const UnkCompared_0x30*>(&self->values[i])))
            return false;
    }
    return true;
}

struct UnkLinkedCell_e {
    u8 unk_0x0[8];
    u8 kind;  // 0x8
    u8 unk_0x9[0xf];
    UnkLinkedCell_e* left;    // 0x18
    UnkLinkedCell_e* right;   // 0x20
    UnkLinkedCell_e* up;      // 0x28
    UnkLinkedCell_e* down;    // 0x30
    u8 unk_0x38[8];
    u8 flags;                 // 0x40
    u8 unk_0x41;
    u8 field_0x42;
};

static inline bool Solid(const UnkLinkedCell_e* c) { return c && c->kind >= 2; }

void fn_7100e78060(UnkLinkedCell_e* self) {
    self->flags = 0;
    if (self->kind >= 2) {
        u8 f = 0;
        if (UnkLinkedCell_e* l = self->left) {
            if (l->kind >= 2) {
                f = 1;
                self->flags = f;
            }
            if (Solid(l->up)) {
                f |= 0x10;
                self->flags = f;
            }
            if (Solid(l->down)) {
                f |= 0x20;
                self->flags = f;
            }
        }
        if (UnkLinkedCell_e* r = self->right) {
            if (r->kind >= 2) {
                f |= 2;
                self->flags = f;
            }
            if (Solid(r->up)) {
                f |= 0x40;
                self->flags = f;
            }
            if (Solid(r->down)) {
                f |= 0x80;
                self->flags = f;
            }
        }
        if (Solid(self->up)) {
            f |= 4;
            self->flags = f;
        }
        if (Solid(self->down)) {
            f |= 8;
            self->flags = f;
        }
    }
    self->field_0x42 = 0;
}

struct UnkCharMap : base::UnkVector<u8> {
    UnkCharMap() = delete;
};
extern UnkCharMap g_unk_71727c8f58;

struct UnkRowCell_b0238 {
    u8 unk_0x0[8];
    u8 kind;  // 0x8
    u8 unk_0x9[0x3f];
};

void fn_7100e7a110(UnkRowCell_b0238* cells, s32 row, CRntString* out) {
    s32 first = (s32)((u32)row * 0x28a);
    fn_710007eaa0(out, &g_unk_7101d58d10);
    u8 previous = 0;
    u32 run = 0;
    if (first > 0x7ffffd75)
        return;
    s64 i = first;
    for (s64 end = first + 0x28a; i < end; ++i) {
        u8 symbol = g_unk_71727c8f58.GrowAt(cells[i].kind);
        if (symbol == previous) {
            ++run;
            continue;
        }
        if (previous) {
            fn_710007f634(out, run);
            fn_710007f5ac(out, previous);
        }
        run = 1;
        previous = symbol;
    }
    if (run) {
        fn_710007f634(out, run);
        fn_710007f5ac(out, previous);
        // keeps the last append an ordinary call: the original does not tail-call it
        asm volatile("" ::: "memory");
    }
}
