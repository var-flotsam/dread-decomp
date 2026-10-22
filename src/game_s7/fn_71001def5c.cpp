// Sorts a list of render entries into five pointer lists by entry type (the lists are emptied
// first; null entries are skipped).
#include "base/UnkVector.h"
#include "types.h"

using base::UnkVector;

struct UnkRenderParams {
    u8 pad_0x0[0xa9];
    bool field_0xa9;
    u8 pad_0xaa[0x6];
    bool field_0xb0;
};

struct UnkRenderShape {
    u8 pad_0x0[0xec];
    f32 field_0xec;
};

struct UnkRenderObject {
    u8 pad_0x0[0x78];
    UnkRenderShape* field_0x78;
    u8 pad_0x80[0x8];
    UnkRenderParams* field_0x88;
    u8 pad_0x90[0x2cb];
    bool field_0x35b;
    u8 pad_0x35c[0x5];
    bool field_0x361;
};
static_assert(__builtin_offsetof(UnkRenderObject, field_0x361) == 0x361, "");

struct UnkRenderEntry {
    s32 type;
    s32 field_0x4;
    u8 pad_0x8[0x8];
    UnkRenderObject* field_0x10;
};

struct UnkRenderLists {
    u8 pad_0x0[0x8];
    UnkVector<UnkRenderObject*> field_0x8;
    UnkVector<UnkRenderEntry*> field_0x30;
    UnkVector<UnkRenderEntry*> field_0x58;
    UnkVector<UnkRenderEntry*> field_0x80;
    UnkVector<UnkRenderEntry*> field_0xa8;
};
static_assert(__builtin_offsetof(UnkRenderLists, field_0xa8) == 0xa8, "");


// UnkVector::PushBack, with growth expected to be rare.
template <class T>
__attribute__((always_inline)) inline void Push(UnkVector<T>& v, T value) {
    u32 n = v.count;
    u32 old_capacity = v.capacity;
    u32 new_capacity = old_capacity;
    if (__builtin_expect(old_capacity <= n, 0) && v.step != 0)
        new_capacity = (n / v.step + 1) * v.step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    T* p;
    if (old_capacity < new_capacity) {
        p = static_cast<T*>(v.allocator->vf_0x40((u64)new_capacity * sizeof(T), 8, v.flags));
        if (v.data) {
            __builtin_memcpy(p, v.data, (u64)v.count * sizeof(T));
            v.allocator->vf_0x48(v.data);
        }
        n = v.count;
        v.data = p;
        v.capacity = new_capacity;
    } else {
        p = v.data;
    }
    __builtin_memcpy(&p[n], &value, sizeof(T));
    ++v.count;
}

// NON_MATCHING(minor): same code; the blocks that compute a grown capacity are laid out after all
// the others instead of between them (the original's branch weights are unknown).
void fn_71001def5c(UnkRenderLists* self, UnkRenderEntry** entries, u32 count) {
    self->field_0x8.Clear();
    self->field_0x30.Clear();
    self->field_0x58.Clear();
    self->field_0x80.Clear();
    self->field_0xa8.Clear();
    for (u32 i = 0; i < count; i++) {
        UnkRenderEntry* entry = entries[i];
        if (!entry)
            continue;
        switch (entry->type) {
        case 0x10: {
            UnkRenderObject* object = entry->field_0x10;
            if (object->field_0x78->field_0xec > 0.0001f)
                Push(self->field_0x8, object);
            break;
        }
        case 0x4000:
            Push(self->field_0x30, entry);
            Push(self->field_0x58, entry);
            Push(self->field_0x80, entry);
            break;
        case 0x800:
            Push(self->field_0x30, entry);
            Push(self->field_0x58, entry);
            break;
        case 0x10000:
            Push(self->field_0x30, entry);
            Push(self->field_0x80, entry);
            break;
        case 0x2: {
            UnkRenderObject* object = entry->field_0x10;
            if (!object->field_0x361 && object->field_0x35b)
                Push(self->field_0x80, entry);
            else
                Push(self->field_0x58, entry);
            Push(self->field_0x30, entry);
            break;
        }
        case 0x2000:
            Push(self->field_0x30, entry);
            Push(self->field_0x80, entry);
            break;
        case 0x20:
            if (entry->field_0x4 == 0x4000 && entry->field_0x10 && entry->field_0x10->field_0x88->field_0xa9 &&
                entry->field_0x10->field_0x88->field_0xb0) {
                Push(self->field_0xa8, entry);
            } else {
                Push(self->field_0x30, entry);
                Push(self->field_0x80, entry);
            }
            break;
        case 0x8000:
            Push(self->field_0x30, entry);
            Push(self->field_0x80, entry);
            break;
        }
    }
}
