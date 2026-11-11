// CCharClassShootActivatorComponent unit (and the CBeamBoxComponent unit's identical copies): the reflected
// "clear" (+0x110), move-assignment (+0x50) and destructor (+0x40) slots of the array of 0x48-byte entries (a callback set and a
// string id) that src/game/CCharClassShootActivatorComponent/fn_71008947b8.cpp copies and assigns. Clearing
// destroys every entry and keeps the buffer; the move-assignment destroys the entries, then takes the other
// array's fields, leaving it empty (the old buffer is not freed). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "game/CBeamBoxComponent.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/UnkTU341.h"
#include "types.h"


struct UnkClearEntry_A4_b73 {
    void* vtable;
    UnkErasedVector list;  // 0x8
    u32 field_0x30;
    base::UnkRefHandle owner;  // 0x38
    base::global::CStrId id;   // 0x40

    __attribute__((always_inline)) void Destroy() {
        id.Reset();
        vtable = reinterpret_cast<char*>(g_unk_7101a33fa0) + 0x10;
        if (base::UnkRefCounted* p = owner.ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
        vtable = reinterpret_cast<char*>(g_unk_7101a20998) + 0x10;
        reinterpret_cast<UnkOwnedErasedVector*>(&list)->~UnkOwnedErasedVector();
    }
};
static_assert(sizeof(UnkClearEntry_A4_b73) == 0x48, "layout");

struct UnkClearVector_A4_b73 {
    UnkClearEntry_A4_b73* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) void Clear() {
        for (u64 i = 0; i < count; ++i)
            data[i].Destroy();
        count = 0;
    }

    __attribute__((always_inline)) void Destroy() {
        if (data) {
            Clear();
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }

    __attribute__((always_inline)) UnkClearVector_A4_b73* MoveAssign(UnkClearVector_A4_b73* other) {
        if (this != other) {
            Clear();
            allocator = other->allocator;
            flags = other->flags;
            step = other->step;
            growable = other->growable;
            count = other->count;
            capacity = other->capacity;
            data = other->data;
            other->data = nullptr;
            other->count = 0;
            other->capacity = 0;
        }
        return this;
    }
};

void fn_7100894a68(UnkClearVector_A4_b73* self) {
    self->Clear();
}

void fn_71008aa8cc(UnkClearVector_A4_b73* self) {
    self->Clear();
}

UnkClearVector_A4_b73* fn_7100895288(UnkClearVector_A4_b73* self, UnkClearVector_A4_b73* other) {
    return self->MoveAssign(other);
}

UnkClearVector_A4_b73* fn_71008ab2d0(UnkClearVector_A4_b73* self, UnkClearVector_A4_b73* other) {
    return self->MoveAssign(other);
}

// The CBeamBoxComponent unit's array destructor (+0x40); the CCharClassShootActivatorComponent unit has none.
void fn_71008ab0b8(UnkClearVector_A4_b73* self) {
    self->Destroy();
}

// PushBack of a copy (fn_710089566c, fn_71008ab6b4: one per array type, each calling its own Reserve): grows like
// base::UnkVector::PushBack (hang included), then copy-constructs the entry in place.
struct UnkPushEntry_A4_b73 {
    void* vtable;
    UnkErasedValue_0x10* list_data;  // 0x8, an UnkErasedVector written as flat members
    u32 list_count;
    u32 list_capacity;
    u32 list_step;
    bool list_grow;
    u32 list_flags;
    base::UnkAllocator* list_allocator;
    u32 field_0x30;
    base::UnkRefHandle owner;  // 0x38
    base::global::CStrId id;   // 0x40

    __attribute__((always_inline)) UnkPushEntry_A4_b73(const UnkPushEntry_A4_b73& o) : id(o.id) {}
};

struct UnkEntryVector;
void fn_7100895498(UnkEntryVector* v, u32 n);

struct UnkPushVector_A4_b73 {
    UnkPushEntry_A4_b73* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    base::UnkAllocator* allocator;

    template <void (*Reserve)(UnkEntryVector*, u32)>
    __attribute__((always_inline)) void PushBack(const UnkPushEntry_A4_b73* value) {
        u32 n = count;
        u32 cap = capacity;
        if (cap <= n && step != 0)
            cap = (n / step + 1) * step;
        if ((s32)n >= (s32)cap)
            for (;;) {
            }
        Reserve(reinterpret_cast<UnkEntryVector*>(this), cap);
        UnkPushEntry_A4_b73* d = data;
        u32 k = count;  // the list copy below recomputes the entry (the original keeps data and count, not it)
        UnkPushEntry_A4_b73* e = &d[k];
        e->vtable = reinterpret_cast<char*>(g_unk_7101a20998) + 0x10;
        e->list_data = nullptr;
        e->list_count = 0;
        e->list_capacity = 0;
        e->list_step = 10;
        e->list_grow = true;
        e->list_flags = 0xf;
        e->list_allocator = fn_7100083990();
        e->field_0x30 = 0;
        e->vtable = reinterpret_cast<char*>(g_unk_7101a33fa0) + 0x10;
        e->owner.ptr = nullptr;
        e->owner.Set(value->owner.ptr);
        e->field_0x30 = value->field_0x30;
        asm("" : "+r"(d));
        e = &d[k];
        fn_710023b29c(reinterpret_cast<UnkErasedVector*>(&e->list_data),
                      reinterpret_cast<const UnkErasedVector*>(&value->list_data));
        new (&e->id) base::global::CStrId(value->id);
        ++count;
    }
};

void fn_710089566c(UnkPushVector_A4_b73* v, const UnkPushEntry_A4_b73* value) {
    v->PushBack<fn_7100895498>(value);
}

void fn_71008ab6b4(UnkPushVector_A4_b73* v, const UnkPushEntry_A4_b73* value) {
    v->PushBack<fn_71008ab4e0>(value);
}
