// CCharClassShootActivatorComponent unit (and the CBeamBoxComponent unit's identical copies): the reflected
// begin (+0x160) and end (+0x168) slots of the array of 0x48-byte entries (a callback set and a string id) whose
// clear is in UnkEntryVector_Clear_A4_b73.cpp. The iterator caches its (index, entry) pair in a holder that is
// empty, owns a copy, or refers to the iterator's own index and entry (base/UnkCollectionIter.h's shape); the
// assignment resets an owned copy through the out-of-line pair destructor (fn_71008933d4 / fn_71008a939c), while
// the temporary's destructor has it inlined. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "types.h"

extern void* g_unk_7101a20998[];  // the callback sets' base vtable
extern void* g_unk_7101a33fa0[];  // their vtable

void fn_71008933d4(char*);
void fn_71008a939c(char*);

struct UnkIterEntry_A4_b73 {
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
static_assert(sizeof(UnkIterEntry_A4_b73) == 0x48, "layout");

struct UnkIterVector_A4_b73 {
    UnkIterEntry_A4_b73* data;
    u32 count;
};

template <void (*DestroyPair)(char*)>
struct UnkEntryIterValue_A4_b73 {
    union {
        struct {
            u32 index;
            UnkIterEntry_A4_b73 element;
        } value;
        struct {
            const u32* index;
            UnkIterEntry_A4_b73* element;
        } ref;
    };
    s32 state;

    UnkEntryIterValue_A4_b73() : state(0) {}
    __attribute__((always_inline)) ~UnkEntryIterValue_A4_b73() {
        if (state == 2) {
        } else if (state == 1) {
            value.element.Destroy();
        } else {
            return;
        }
        state = 0;
    }
    __attribute__((always_inline)) void Reset() {
        if (state == 2) {
        } else if (state == 1) {
            DestroyPair(reinterpret_cast<char*>(this));
        } else {
            return;
        }
        state = 0;
    }
    __attribute__((always_inline)) void SetRef(const u32* i, UnkIterEntry_A4_b73* e) {
        Reset();
        state = 2;
        ref.index = i;
        ref.element = e;
    }
};

template <void (*DestroyPair)(char*)>
struct UnkEntryIter_A4_b73 {
    u32 index;
    UnkIterEntry_A4_b73* element;
    UnkEntryIterValue_A4_b73<DestroyPair> current;

    __attribute__((always_inline)) UnkEntryIter_A4_b73(u32 i, UnkIterEntry_A4_b73* e) : index(i), element(e) {
        if (e)
            current.SetRef(&index, e);
    }
    __attribute__((always_inline)) UnkEntryIter_A4_b73& operator=(const UnkEntryIter_A4_b73& other) {
        if (this != &other) {
            index = other.index;
            element = other.element;
            if (element)
                current.SetRef(&index, element);
            else
                current.Reset();
        }
        return *this;
    }
};

void fn_7100895fe4(UnkIterVector_A4_b73* self, UnkEntryIter_A4_b73<fn_71008933d4>* out) {
    *out = UnkEntryIter_A4_b73<fn_71008933d4>(0, self->data);
}

void fn_71008961bc(UnkIterVector_A4_b73* self, UnkEntryIter_A4_b73<fn_71008933d4>* out) {
    *out = UnkEntryIter_A4_b73<fn_71008933d4>(self->count, self->data + self->count);
}

void fn_71008ac02c(UnkIterVector_A4_b73* self, UnkEntryIter_A4_b73<fn_71008a939c>* out) {
    *out = UnkEntryIter_A4_b73<fn_71008a939c>(0, self->data);
}

void fn_71008ac204(UnkIterVector_A4_b73* self, UnkEntryIter_A4_b73<fn_71008a939c>* out) {
    *out = UnkEntryIter_A4_b73<fn_71008a939c>(self->count, self->data + self->count);
}

