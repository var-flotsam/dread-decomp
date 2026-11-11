// The CFXPool::CDefinition unit (tools/tu_map.py): its array of file-path entries (a CFilePathStrId
// and a word, 0x10 bytes) as the reflected collection uses it (move-assignment, iterator end and
// iterator-value move, growing access), and the release of a pooled object back to the pool of up
// to 32 (a bit per object in the mask at +0x38; see fn_710104dc00.cpp). Names and layouts are
// placeholders.
#include <new>

#include "UnkClone.h"
#include "base/UnkCloneCalls.h"
#include "base/UnkCollectionIter.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "game/CFXPool/CDefinition.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkBeginIdArrayOwner;
struct UnkFxPooledObject;

// A file-path id paired with a word.
struct UnkFXPathEntry_b0068 {
    CFilePathStrId path;
    u64 field_0x8;
};

// Move-assignment of an entry.
UnkFXPathEntry_b0068* fn_71010504d4(UnkFXPathEntry_b0068* self, UnkFXPathEntry_b0068* other) {
    if (self != other) {
        self->path = static_cast<CFilePathStrId&&>(other->path);
        self->field_0x8 = other->field_0x8;
    }
    return self;
}

// The entries as the reflected iterator sees them (the element type of the Begin in
// src/base/reflection/UnkIdArrayBegin.cpp).
using UnkFxPathIdElem_b0068 = UnkCloneStrIdElem<UnkBeginIdArrayOwner, 0, 0x10, UnkCloneStrIdB<UnkBeginIdArrayOwner>>;

// The iterator past the last entry, synced by the unit's out-of-line fn_710104e720.
void fn_71010501b0(const base::UnkVector<UnkFxPathIdElem_b0068>* v,
                   base::UnkCollectionIter<UnkFxPathIdElem_b0068>* out) {
    base::UnkCollectionIter<UnkFxPathIdElem_b0068> it(v->count, v->data + v->count);
    if (out != &it) {
        out->index = it.index;
        out->element = it.element;
        fn_710104e720(out);
    }
}

using UnkFXPathIterValue_b0068 = base::UnkIterValue<UnkFXPathEntry_b0068, u32>;

// Moves the (index, entry) pair `other` holds or refers to into the one `self` holds or refers to
// (no self-check, unlike UnkIterValue's operator=).
UnkFXPathIterValue_b0068* fn_710104f1c0(UnkFXPathIterValue_b0068* self, UnkFXPathIterValue_b0068* other) {
    if (other->state == 1) {
        self->Index() = other->value.index;
        UnkFXPathEntry_b0068& dst = self->Element();
        dst.path = static_cast<CFilePathStrId&&>(other->value.element.path);
        dst.field_0x8 = other->value.element.field_0x8;
    } else if (other->state == 2) {
        self->Index() = *other->ref.index;
        UnkFXPathEntry_b0068& src = *other->ref.element;
        UnkFXPathEntry_b0068& dst = self->Element();
        dst.path = static_cast<CFilePathStrId&&>(src.path);
        dst.field_0x8 = src.field_0x8;
    }
    return self;
}

// Grows the array to at least `capacity` entries (out of line).
void fn_7100c8c210(base::UnkVector<UnkFXPathEntry_b0068>* self, u32 capacity);

// The entry at `index`; when growing is allowed (field_0x14), an index past the end grows the array
// to the step's next multiple and then to index + 1 entries, value-initialising the new ones.
UnkFXPathEntry_b0068* fn_710104ff64(base::UnkVector<UnkFXPathEntry_b0068>* self, u32 index) {
    if (self->field_0x14 && self->count <= index) {
        u32 cap = self->capacity;
        if (cap <= index && self->step != 0)
            cap = (index / self->step + 1) * self->step;
        fn_7100c8c210(self, cap);
        u32 n = index + 1;
        fn_7100c8c210(self, n);
        for (u32 i = self->count; i < n; ++i)
            new (&self->data[i]) UnkFXPathEntry_b0068();
        self->count = n;
    }
    return &self->data[index];
}

struct UnkFxReleasable_b0068 {
    void** vtable;

    void vf_0x40(bool b) { reinterpret_cast<void (*)(UnkFxReleasable_b0068*, bool)>(vtable[0x40 / 8])(this, b); }
    void vf_0x110() { reinterpret_cast<void (*)(UnkFxReleasable_b0068*)>(vtable[0x110 / 8])(this); }
};

struct UnkFxObjectPool_b0068 {
    u8 pad_0x0[0x10];
    base::UnkVector<UnkFxPooledObject*> objects;  // 0x10
    u32 used;                                     // 0x38
};

UnkFxPooledObject** fn_7100852548(base::UnkVector<UnkFxPooledObject*>* array, u32 index);

// Gives `object` back to the pool: when it is one of the pool's objects, it is stopped (vf_0x40,
// vf_0x110) and its bit cleared.
bool fn_710104ddcc(UnkFxObjectPool_b0068* self, UnkFxReleasable_b0068* object) {
    if (self->objects.count == 0)
        return false;
    UnkFxPooledObject* target = reinterpret_cast<UnkFxPooledObject*>(object);
    u32 i = 0;
    UnkFxPooledObject* o;
    for (;;) {
        o = *fn_7100852548(&self->objects, i);
        u32 count = self->objects.count;
        if (o != target)
            ++i;
        if (o == target || i >= count)
            break;
    }
    if (o != target)
        return false;
    object->vf_0x40(false);
    object->vf_0x110();
    self->used &= ~(1u << i);
    return true;
}
