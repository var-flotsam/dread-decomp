// Unit 2510: rebuilds an owner's working lists when it is marked dirty. Dead weak handles are
// erased; each live item is given its index, added to the active list and its flags are merged.
// Names and layouts are placeholders.

#include "base/UnkVector.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkTU2510Owner;

struct UnkTU2510Item : UnkWeakTarget {
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual u32 vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();

    u8 pad_0x8[0x20];
    u8 field_0x28;
    UnkTU2510Owner* field_0x30;
    u32 field_0x38;
    u64 field_0x40[4];
    u8 field_0x60;
};
static_assert(__builtin_offsetof(UnkTU2510Item, field_0x60) == 0x60, "UnkTU2510Item layout");

// A counted reference to a weak handle.
struct UnkTU2510Ref {
    UnkWeakHandle* handle;

    UnkTU2510Ref() : handle(nullptr) {}
    UnkTU2510Ref(UnkWeakHandle* h) : handle(nullptr) { AssignRef(&handle, h); }
    UnkTU2510Ref(const UnkTU2510Ref& other) : handle(nullptr) { AssignRef(&handle, other.handle); }
    ~UnkTU2510Ref() {
        if (handle) {
            if (--handle->ref_count == 0)
                handle->vf_0x10();
        }
    }
};

typedef base::UnkVector<UnkTU2510Ref> UnkTU2510RefVector;

// Grows the array to at least `capacity` elements.
void fn_7100864cec(UnkTU2510RefVector* self, u32 capacity);
// Takes over `other`'s array.
void fn_710010d6e8(base::UnkVector<void*>* self, base::UnkVector<void*>* other);

struct UnkTU2510Owner {
    void* vtable;
    UnkTU2510RefVector field_0x8;
    u8 pad_0x30[0x2c];
    u32 field_0x5c;
    base::UnkVector<void*> field_0x60;
    u8 field_0x88;
    UnkTU2510RefVector field_0x90;
};
static_assert(__builtin_offsetof(UnkTU2510Owner, field_0x5c) == 0x5c, "UnkTU2510Owner layout");
static_assert(__builtin_offsetof(UnkTU2510Owner, field_0x90) == 0x90, "UnkTU2510Owner layout");

static inline void PushRef(UnkTU2510RefVector* v, const UnkTU2510Ref& value) {
    u32 n = v->count;
    u32 new_capacity = v->capacity;
    if (new_capacity <= n && v->step != 0)
        new_capacity = (n / v->step + 1) * v->step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    fn_7100864cec(v, new_capacity);
    new (&v->data[v->count]) UnkTU2510Ref(value);
    ++v->count;
}

static inline void EraseRef(UnkTU2510RefVector* v, u32 index) {
    if (index < v->count) {
        v->data[index].~UnkTU2510Ref();
        for (u64 i = index; i < v->count - 1; ++i) {
            new (&v->data[i]) UnkTU2510Ref(v->data[i + 1]);
            v->data[i + 1].~UnkTU2510Ref();
        }
        v->count = v->count - 1;
    }
}

void fn_7100d59dd8(UnkTU2510Owner* self) {
    if (!self->field_0x88)
        return;

    self->field_0x5c = 0;
    self->field_0x90.Clear();
    fn_7100864cec(&self->field_0x90, self->field_0x8.count);

    base::UnkVector<void*> active(5);
    active.Reserve(self->field_0x8.count);

    UnkTU2510Ref* it = self->field_0x8.data;
    while (it != &self->field_0x8.data[self->field_0x8.count]) {
        UnkWeakHandle* handle = it->handle;
        UnkTU2510Item* item = handle ? static_cast<UnkTU2510Item*>(handle->target) : nullptr;
        if (!item) {
            u32 index = (u32)(it - self->field_0x8.data);
            EraseRef(&self->field_0x8, index);
            it = &self->field_0x8.data[index];
            continue;
        }
        if (item->field_0x28) {
            item->field_0x30 = self;
            item->field_0x38 = self->field_0x90.count;
            {
                UnkTU2510Ref ref(GetWeakHandle(item));
                PushRef(&self->field_0x90, ref);
            }
            if (item->field_0x60)
                item->vf_0x98();
            active.PushBack(&item->field_0x40);
            self->field_0x5c |= item->vf_0x58();
        }
        ++it;
    }

    fn_710010d6e8(&self->field_0x60, &active);
    self->field_0x88 = 0;
    active.Free();
}
