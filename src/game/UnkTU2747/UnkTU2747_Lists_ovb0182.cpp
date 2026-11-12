// Unit 2747 (hint GUI::CDisplayObject): copies of reflected collection slots seen in other units: singly linked
// list set-at, assign, insert-at and erase-at (src/game/CEntity/CEntity_SListSetAt_A6_b76.cpp,
// src/game/CActor/CActor_SListAssign_b60.cpp, src/game/CActor/CActor_SListInsertAt_A1_b74.cpp,
// src/game/CEntity/CEntity_SListErase_A6_b63.cpp), a list begin iterator (CEntity_IterBeginEnd_b54.cpp) and
// the 8-byte array erase (src/base/reflection/UnkArrayEraseAt_A1_b72.cpp), then the display object's parameter
// blocks. Names are placeholders.
#include <new>

#include "base/UnkDestructors.h"
#include "base/UnkSList.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CGameObject.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiEvent.h"
#include "../../game_s7/UnkGuiS7.h"
#include "game/UnkTU2747.h"
#include "game/UnkTU57.h"
#include "types.h"

struct UnkClone_fn_71011d3180;
struct UnkClone_fn_71011d3298;
using UnkSListA_ovb0182 = base::UnkSList<UnkClone_fn_71011d3180>;
using UnkSListB_ovb0182 = base::UnkSList<UnkClone_fn_71011d3298>;

extern template void UnkSListA_ovb0182::InsertBefore(const u64&, UnkSListA_ovb0182::Node*);
extern template void UnkSListB_ovb0182::InsertBefore(const u64&, UnkSListB_ovb0182::Node*);

// --- set at ---

template <class List>
static inline __attribute__((always_inline)) void SetAt_ovb0182(List* self, const u32* it, const u64* v) {
    u32 i = *it;
    if (i >= self->count) {
    push:
        self->PushBack(*v);
        return;
    }
    {
        typename List::Node* cur = self->head;
        if (!cur)
            goto push;
        typename List::Node* next = cur->next;
        typename List::Node* prev = nullptr;
        for (u32 k = 0; k != i; ++k) {
            if (!next)
                goto push;
            prev = cur;
            cur = next;
            next = next->next;
        }
        if (prev) {
            prev->next = next;
            if (cur == self->tail)
                self->tail = prev;
        } else {
            self->head = next;
        }
        if (--self->count == 0) {
            self->head = nullptr;
            self->tail = nullptr;
        }
        self->allocator->vf_0x48(cur);
        if (next) {
            self->InsertBefore(*v, next);
            return;
        }
    }
    goto push;
}

void fn_71011d2d54(UnkSListA_ovb0182* self, const u32* it, const u64* v) {
    SetAt_ovb0182(self, it, v);
}

void fn_71011d2e80(UnkSListB_ovb0182* self, const u32* it, const u64* v) {
    SetAt_ovb0182(self, it, v);
}

// --- assign ---

struct UnkSListTag_71011d2fe0;
using UnkValueSList_ovb0182 = base::UnkSList<UnkSListTag_71011d2fe0>;

static inline __attribute__((always_inline)) void AssignSList_ovb0182(UnkValueSList_ovb0182* self,
                                                                     const UnkValueSList_ovb0182* other) {
    if (self == other)
        return;
    UnkValueSList_ovb0182::Node* n = self->head;
    while (n) {
        UnkValueSList_ovb0182::Node* next = n->next;
        self->allocator->vf_0x48(n);
        n = next;
    }
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
    for (UnkValueSList_ovb0182::Node* it = other->head; it; it = it->next)
        self->PushBack(it->value);
}

UnkValueSList_ovb0182* fn_71011d2fe0(UnkValueSList_ovb0182* self, const UnkValueSList_ovb0182* other) {
    AssignSList_ovb0182(self, other);
    return self;
}

UnkValueSList_ovb0182* fn_71011d30b0(UnkValueSList_ovb0182* self, const UnkValueSList_ovb0182* other) {
    AssignSList_ovb0182(self, other);
    return self;
}

// --- insert at ---

template <class List>
static inline __attribute__((always_inline)) typename List::Node* At_ovb0182(List* self, u32 i) {
    typename List::Node* n = self->head;
    for (u32 k = 0; k < self->count; ++k, n = n->next)
        if (k == i)
            return n;
    return nullptr;
}

void fn_71011d2ac8(UnkSListA_ovb0182* self, const u32* it, const u64* v) {
    u32 i = *it;
    if (i >= self->count) {
        self->PushBack(*v);
        return;
    }
    self->InsertBefore(*v, At_ovb0182(self, i));
}

void fn_71011d2b84(UnkSListB_ovb0182* self, const u32* it, const u64* v) {
    u32 i = *it;
    if (i >= self->count) {
        self->PushBack(*v);
        return;
    }
    self->InsertBefore(*v, At_ovb0182(self, i));
}

// --- erase at ---

void fn_71011d2c40(UnkValueSList_ovb0182* self, const u32* it) {
    u32 i = *it;
    if (self->count <= i)
        return;
    UnkValueSList_ovb0182::Node* cur = self->head;
    if (!cur)
        return;
    UnkValueSList_ovb0182::Node* next = cur->next;
    UnkValueSList_ovb0182::Node* prev = nullptr;
    for (u32 k = 0; k != i; ++k) {
        if (!next)
            return;
        prev = cur;
        cur = next;
        next = next->next;
    }
    if (prev) {
        prev->next = next;
        if (cur == self->tail)
            self->tail = prev;
    } else {
        self->head = next;
    }
    if (--self->count == 0) {
        self->head = nullptr;
        self->tail = nullptr;
    }
    self->allocator->vf_0x48(cur);
}

// --- 8-byte array erase ---

struct UnkPodArray_ovb0182 {
    u64* data;
    u32 count;
};

void fn_71011d1cf0(UnkPodArray_ovb0182* self, const u32* it) {
    u64* base = reinterpret_cast<u64*>(self);
    u64* p = reinterpret_cast<u64*>(reinterpret_cast<char*>(base) + (u64)*it * sizeof(u64));
    u32 index = (u32)(p - base);
    if (index < self->count) {
        u64* pos = self->data + index;
        __builtin_memcpy(pos, pos + 1, (self->count - index - 1) * sizeof(u64));
        --self->count;
    }
}

// --- list begin iterator (first node at +0x10, iterated with a count) ---

struct UnkListNode_ovb0182;

struct UnkList_ovb0182 {
    u8 unk_0x0[0x10];
    UnkListNode_ovb0182* first;
};

struct UnkListIter_ovb0182 {
    UnkListNode_ovb0182* node;
    u32 index;
    union {
        u8 value[0x10];
        struct {
            const u32* index;
            UnkListNode_ovb0182* node;
        } ref;
    };
    s32 state;

    __attribute__((always_inline)) void Reset() {
        switch (state) {
        case 1:
            state = 0;
            break;
        case 2:
            state = 0;
            break;
        }
    }
    __attribute__((always_inline)) void SetRef(const u32* i, UnkListNode_ovb0182* n) {
        s32* p = &state;
        switch (*p) {
        case 1:
            state = 0;
            break;
        case 2:
            state = 0;
            break;
        }
        state = 2;
        ref.index = i;
        ref.node = n;
    }

    UnkListIter_ovb0182(UnkListNode_ovb0182* node, u32 index) : node(node), index(index), state(0) {
        if (node)
            SetRef(&this->index, node);
    }
    ~UnkListIter_ovb0182() { Reset(); }
    UnkListIter_ovb0182& operator=(const UnkListIter_ovb0182& other) {
        if (this != &other) {
            index = other.index;
            node = other.node;
            if (node)
                SetRef(&index, node);
            else
                Reset();
        }
        return *this;
    }
};

void fn_71011d33c4(const UnkList_ovb0182* list, UnkListIter_ovb0182* out) {
    *out = UnkListIter_ovb0182(list->first, 0);
}

// --- parameter blocks of a display object ---

struct UnkCompared_0x30;
void fn_710120280c(void* param, void* owner);
void fn_7101202d1c(void* param, void* owner);

struct UnkIdEntry_ovb0182 {
    u8 pad_0x0[0x10];
    u8 key[0x34];  // 0x10
    union {
        s32 refs;  // 0x44
    };
};

struct UnkNamedChild_ovb0182 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual UnkCompared_0x30* vf_0x10();
};

struct UnkNamedChildren_ovb0182 {
    u8 pad_0x0[0x100];
    u32 count;  // 0x100
};

template <u64 Off>
static inline __attribute__((always_inline)) UnkNamedChild_ovb0182* FindChild(UnkNamedChildren_ovb0182* self) {
    UnkIdEntry_ovb0182*& slot = *reinterpret_cast<UnkIdEntry_ovb0182**>(reinterpret_cast<char*>(&GameManagerIdTable()) + Off);
    ++slot->refs;
    const UnkCompared_0x30* key = reinterpret_cast<const UnkCompared_0x30*>(slot->key);
    UnkNamedChild_ovb0182** first = reinterpret_cast<UnkNamedChild_ovb0182**>(reinterpret_cast<char*>(self) + 0x104);
    for (UnkNamedChild_ovb0182** it = first; it != first + self->count;) {
        UnkNamedChild_ovb0182* r = fn_710007f120((*it)->vf_0x10(), key) ? *it : nullptr;
        ++it;
        if (r)
            return r;
    }
    return nullptr;
}

template <u64 Off, void (*Ctor)(void*, void*)>
static inline __attribute__((always_inline)) void* GetOrCreate(UnkNamedChildren_ovb0182* self) {
    UnkNamedChild_ovb0182* child = FindChild<Off>(self);
    if (!child) {
        child = static_cast<UnkNamedChild_ovb0182*>(fn_71000839a0(0x40, 0xc));
        Ctor(child, self);
        reinterpret_cast<UnkNamedChild_ovb0182**>(reinterpret_cast<char*>(self) + 0x104)[self->count] = child;
        self->count = self->count + 1;
    }
    return child;
}

// The parameter blocks the display object hands out: the one with the given id is looked up in the array at
// +0x104 (count at +0x100) or created (0x40 bytes) and appended.
void* fn_71011ce0d8(UnkGuiDisplayObject* self) {
    return GetOrCreate<0x940, fn_710120280c>(reinterpret_cast<UnkNamedChildren_ovb0182*>(self));
}
void* fn_71011ce24c(UnkGuiDisplayObject* self) {
    return GetOrCreate<0xfc0, fn_7101202d1c>(reinterpret_cast<UnkNamedChildren_ovb0182*>(self));
}

// The same lookup alone, for the two ids.
char* fn_71011ce37c(char* self) {
    return reinterpret_cast<char*>(FindChild<0xfc0>(reinterpret_cast<UnkNamedChildren_ovb0182*>(self)));
}
char* fn_71011ce9b4(char* self) {
    return reinterpret_cast<char*>(FindChild<0x940>(reinterpret_cast<UnkNamedChildren_ovb0182*>(self)));
}

// --- the colour block ---

namespace unk_gui {

// Sets the display object's colour (+0xa0) unless it is already within 0.001 of it, and when asked also
// stores it in both colour slots of the block fn_71011ce37c finds (+0xc and +0x1c).
void fn_71011cfc24(UnkGuiObj* object, const UnkColor& color, u32 value) {
    char* block = fn_71011ce37c(reinterpret_cast<char*>(object));
    // Keeps the block's null test ahead of the flag test.
    asm("" : "+r"(value));
    if (block && (value & 1)) {
        __builtin_memcpy(block + 0x1c, &color, 16);
        __builtin_memcpy(block + 0xc, &color, 16);
    }
    f32* current = reinterpret_cast<f32*>(reinterpret_cast<char*>(object) + 0xa0);
    const f32* want = &color.r;
    if (__builtin_fabsf(current[0] - want[0]) < 0.001f && __builtin_fabsf(current[1] - want[1]) < 0.001f &&
        __builtin_fabsf(current[2] - want[2]) < 0.001f && __builtin_fabsf(current[3] - want[3]) < 0.001f)
        return;
    __builtin_memcpy(current, &color, 16);
    if (!object->field_0x184) {
        object->field_0x184 = true;
        object->vf_0xa0();
    }
    object->field_0x183 = true;
    object->field_0x144 |= 0x20;
}

}  // namespace unk_gui

// --- changing the parent ---

void fn_71011cee40(UnkGuiDisplayObject* object);

struct UnkDisplayObjectContainer_ovb0032;
struct UnkDisplayObject_ovb0032;

// The members the parent change touches.
struct UnkGuiChildLinks_ovb0182 {
    u8 pad_0x0[0x120];
    UnkDisplayObjectContainer_ovb0032* parent;  // 0x120
};

struct UnkGuiChangeBits_ovb0182 {
    u8 pad_0x0[0x144];
    u32 changed;  // 0x144
};

static inline void Notify_ovb0182(UnkGuiDisplayObject* self, s32 kind) {
    UnkGuiEvent ev;
    s32 k = kind;
    fn_710120235c(&ev, self, &k, nullptr);
    fn_71011b6e0c(g_unk_717271e8f8, &ev);
    fn_7101202488(&ev);
}

// Moves the object under another parent (fn_71011d932c takes it out of the old one).
void fn_71011cef74(void* object, void* new_parent) {
    UnkGuiDisplayObject* self = static_cast<UnkGuiDisplayObject*>(object);
    UnkGuiChildLinks_ovb0182* links = reinterpret_cast<UnkGuiChildLinks_ovb0182*>(self);
    auto* parent = static_cast<UnkDisplayObjectContainer_ovb0032*>(new_parent);
    if (links->parent == parent)
        return;
    if (links->parent) {
        if (self->field_0x183)
            fn_71011cee40(self);
        if (self->field_0x74)
            Notify_ovb0182(self, 1);
        fn_71011d932c(links->parent, reinterpret_cast<UnkDisplayObject_ovb0032*>(self));
    }
    bool first = !self->field_0x184;
    links->parent = parent;
    if (first) {
        self->field_0x184 = true;
        self->vf_0xa0();
    }
    self->field_0x183 = true;
    reinterpret_cast<UnkGuiChangeBits_ovb0182*>(self)->changed = ~0u;
    fn_71011cee40(self);
    if (self->field_0x74) {
        if (self->field_0x183)
            fn_71011cee40(self);
        if (self->field_0x76)
            Notify_ovb0182(self, 0);
    }
    self->vf_0x50();
}

// --- setting the object up ---

using base::global::CStrId;

// The display object with the virtual slots past vf_0xa0 that the set-up calls.
struct UnkGuiDisplayObjectSlots_ovb0182 : UnkGuiDisplayObject {
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170();
    virtual void vf_0x178();
    virtual void vf_0x180();
    virtual void vf_0x188();
    virtual void vf_0x190();
    virtual void vf_0x198();
    virtual void vf_0x1a0();
    virtual void vf_0x1a8();
    virtual void vf_0x1b0();
    virtual void vf_0x1b8();
    virtual void vf_0x1c0();
    virtual void vf_0x1c8();
    virtual void vf_0x1d0();
    virtual void vf_0x1d8();
    virtual void vf_0x1e0();
    virtual void vf_0x1e8();
    virtual void vf_0x1f0();
    virtual void vf_0x1f8();
    virtual void vf_0x200();
    virtual void vf_0x208();
    virtual void vf_0x210();
    virtual void vf_0x218();
    virtual void vf_0x220();
    virtual void vf_0x228();
};

// Deleted through its deleting destructor (vtable slot 1).
struct UnkGuiOwned_ovb0182 {
    virtual ~UnkGuiOwned_ovb0182();
};

struct UnkGuiEntryIter;
bool fn_71011cd5dc(UnkGuiDisplayObject* self, const UnkGuiEntryIter* __attribute__((noescape)));

struct UnkGuiEntry_ovb0182 {
    u8 data[0x18];
};

struct UnkGuiEntryList_ovb0182 {
    UnkGuiEntry_ovb0182* data;
    u32 count;
};

struct UnkGuiEntryCursor_ovb0182 {
    const UnkGuiEntryList_ovb0182* list;
    UnkGuiEntry_ovb0182* entry;
};

// The members the set-up touches.
struct UnkGuiSetupView_ovb0182 {
    u8 pad_0x0[0x100];
    u32 condition_count;  // 0x100
    u8 conditions[1];     // 0x104: an unaligned array of owned pointers
    u8 pad_0x105[0x1b];
    void* parent;  // 0x120
    UnkGuiOwned_ovb0182* owned;  // 0x128
    CStrId name;  // 0x130
    u8 pad_0x138[0x48];
    bool initialized;  // 0x180

    UnkGuiOwned_ovb0182** begin() { return reinterpret_cast<UnkGuiOwned_ovb0182**>(conditions); }
    UnkGuiOwned_ovb0182** end() { return begin() + condition_count; }
};
static_assert(__builtin_offsetof(UnkGuiSetupView_ovb0182, name) == 0x130, "UnkGuiSetupView_ovb0182 layout");
static_assert(__builtin_offsetof(UnkGuiSetupView_ovb0182, initialized) == 0x180, "UnkGuiSetupView_ovb0182 layout");

// (Re)initialises the object: tears down what an earlier set-up left, takes the name and applies the
// list of entries.
void fn_71011cd158(UnkGuiDisplayObject* object, const CStrId* name, const UnkGuiEntryList_ovb0182* list) {
    auto* self = static_cast<UnkGuiDisplayObjectSlots_ovb0182*>(object);
    UnkGuiSetupView_ovb0182* view = reinterpret_cast<UnkGuiSetupView_ovb0182*>(object);
    if (view->initialized) {
        self->vf_0x40();
        if (self->field_0x181)
            fn_71011b5d94(g_unk_717271e8f8, self);
        if (view->owned) {
            delete view->owned;
            view->owned = nullptr;
        }
        for (UnkGuiOwned_ovb0182** it = view->begin(); it != view->end(); ++it) {
            if (*it)
                delete *it;
        }
        view->condition_count = 0;
        view->initialized = false;
        self->vf_0x48();
    }
    view->initialized = true;
    view->parent = nullptr;
    view->name = *name;
    self->vf_0x30();
    if (list) {
        UnkGuiEntryCursor_ovb0182 it{list, list->data};
        bool changed = false;
        for (UnkGuiEntry_ovb0182* entry = it.entry; entry != list->data + list->count;) {
            changed |= fn_71011cd5dc(self, reinterpret_cast<const UnkGuiEntryIter*>(&it));
            it.entry = ++entry;
        }
        if (changed)
            self->vf_0x80();
    }
    self->vf_0x228();
    self->vf_0x38();
}

// --- the destructor ---

struct UnkTag_71011ccfb4;
struct UnkCameraCtrl_ovb0049;

void fn_7100b03a74(void* state);
void fn_7100b1e048(void* slots);

template <>
struct base::UnkDtorTraits<UnkTag_71011ccfb4> {
    static char* vtable() { return g_unk_7101cb5b90; }
    static void base(char* p) { fn_7101043934(reinterpret_cast<UnkCameraCtrl_ovb0049*>(p)); }
};

// An object deleted through its deleting destructor (vtable slot 1).
struct UnkDeleted_ovb0182 {
    virtual ~UnkDeleted_ovb0182();
};

namespace {

// The state object at +0x30: destroyed by fn_7100b03a74, freed, and cleared.
struct UnkStateAt0x30 {
    static void Release(char* self) {
        void* state = *reinterpret_cast<void**>(self + 0x30);
        if (state) {
            fn_7100b03a74(state);
            ::operator delete(state);
            *reinterpret_cast<void**>(self + 0x30) = nullptr;
        }
    }
};

// The owned object at +0x128.
struct UnkOwnedAt0x128 {
    static void Release(char* self) {
        UnkDeleted_ovb0182*& owned = *reinterpret_cast<UnkDeleted_ovb0182**>(self + 0x128);
        if (owned) {
            delete owned;
            owned = nullptr;
        }
    }
};

// The render slots at +0xb8, whose count is at +0x100.
struct UnkSlotsAt0xb8 {
    static void Release(char* self) {
        *reinterpret_cast<u32*>(self + 0x100) = 0;
        fn_7100b1e048(self + 0xb8);
    }
};

}  // namespace

using UnkDisplayObjectMembers_ovb0182 =
    base::UnkMembers<UnkStateAt0x30, UnkOwnedAt0x128, base::UnkMemberAt<base::global::CRntString, 0x150>,
                     base::UnkStrIdAt<0x130>, UnkSlotsAt0xb8, base::UnkStrIdAt<0x20>, base::UnkStrIdAt<0x18>>;

void fn_71011ccfb4(char* self) { base::refl_Dtor<UnkTag_71011ccfb4, UnkDisplayObjectMembers_ovb0182>(self); }

// --- event dispatch ---

struct UnkChangeMenuStateButton_b48a3;
struct UnkGuiEvent_b48a3;

// The event as the dispatcher reads it: its kind and whether a handler has taken it.
struct UnkGuiEventData_ovb0182 {
    u8 pad_0x0[8];
    s32 kind;  // 0x8
    u8 pad_0xc[0xe];
    bool handled;  // 0x1a
};
static_assert(__builtin_offsetof(UnkGuiEventData_ovb0182, handled) == 0x1a, "UnkGuiEventData_ovb0182 layout");

// The display object's per-kind event handlers (vtable slots 0xd0 to 0x1b0).
struct UnkGuiEventTarget_ovb0182 {
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
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0(UnkGuiEventData_ovb0182* event);
    virtual void vf_0xd8(UnkGuiEventData_ovb0182* event);
    virtual void vf_0xe0(UnkGuiEventData_ovb0182* event);
    virtual void vf_0xe8(UnkGuiEventData_ovb0182* event);
    virtual void vf_0xf0(UnkGuiEventData_ovb0182* event);
    virtual void vf_0xf8(UnkGuiEventData_ovb0182* event);
    virtual void vf_0x100(UnkGuiEventData_ovb0182* event);
    virtual void vf_0x108(UnkGuiEventData_ovb0182* event);
    virtual void vf_0x110(UnkGuiEventData_ovb0182* event);
    virtual void vf_0x118(UnkGuiEventData_ovb0182* event);
    virtual void vf_0x120(UnkGuiEventData_ovb0182* event);
    virtual void vf_0x128(UnkGuiEventData_ovb0182* event);
    virtual void vf_0x130(UnkGuiEventData_ovb0182* event);
    virtual void vf_0x138(UnkGuiEventData_ovb0182* event);
    virtual void vf_0x140(UnkGuiEventData_ovb0182* event);
    virtual void vf_0x148(UnkGuiEventData_ovb0182* event);
    virtual void vf_0x150(UnkGuiEventData_ovb0182* event);
    virtual void vf_0x158(UnkGuiEventData_ovb0182* event);
    virtual void vf_0x160(UnkGuiEventData_ovb0182* event);
    virtual void vf_0x168(UnkGuiEventData_ovb0182* event);
    virtual void vf_0x170(UnkGuiEventData_ovb0182* event);
    virtual void vf_0x178(UnkGuiEventData_ovb0182* event);
    virtual void vf_0x180(UnkGuiEventData_ovb0182* event);
    virtual void vf_0x188(UnkGuiEventData_ovb0182* event);
    virtual void vf_0x190(UnkGuiEventData_ovb0182* event);
    virtual void vf_0x198(UnkGuiEventData_ovb0182* event);
    virtual void vf_0x1a0(UnkGuiEventData_ovb0182* event);
    virtual void vf_0x1a8(UnkGuiEventData_ovb0182* event);
    virtual void vf_0x1b0(UnkGuiEventData_ovb0182* event);
};

void fn_7100b1e3fc(void* slots, UnkGuiEventData_ovb0182* event);

// Hands the event to the handler for its kind; unhandled ones go on to the render slots (+0xb8).
void fn_71011cf39c(UnkChangeMenuStateButton_b48a3* object, UnkGuiEvent_b48a3* message) {
    auto* self = reinterpret_cast<UnkGuiEventTarget_ovb0182*>(object);
    auto* event = reinterpret_cast<UnkGuiEventData_ovb0182*>(message);
    if (event->handled)
        return;
    switch (event->kind) {
    case 0:
        self->vf_0x1a8(event);
        if (event->handled)
            return;
        break;
    case 1:
        self->vf_0x1b0(event);
        if (event->handled)
            return;
        break;
    case 2:
        self->vf_0xd0(event);
        if (event->handled)
            return;
        break;
    case 3:
        self->vf_0xd8(event);
        if (event->handled)
            return;
        break;
    case 5:
        self->vf_0x140(event);
        if (event->handled)
            return;
        break;
    case 6:
        self->vf_0x148(event);
        if (event->handled)
            return;
        break;
    case 7:
        self->vf_0x150(event);
        if (event->handled)
            return;
        break;
    case 8:
        self->vf_0x158(event);
        if (event->handled)
            return;
        break;
    case 13:
        self->vf_0x160(event);
        if (event->handled)
            return;
        break;
    case 14:
        self->vf_0x168(event);
        if (event->handled)
            return;
        break;
    case 15:
        self->vf_0x170(event);
        if (event->handled)
            return;
        break;
    case 16:
        self->vf_0x178(event);
        if (event->handled)
            return;
        break;
    case 17:
        self->vf_0xf0(event);
        if (event->handled)
            return;
        break;
    case 18:
        self->vf_0x108(event);
        if (event->handled)
            return;
        break;
    case 19:
        self->vf_0x130(event);
        if (event->handled)
            return;
        break;
    case 20:
        self->vf_0x138(event);
        if (event->handled)
            return;
        break;
    case 21:
        self->vf_0x120(event);
        if (event->handled)
            return;
        break;
    case 22:
        self->vf_0x128(event);
        if (event->handled)
            return;
        break;
    case 23:
        self->vf_0x180(event);
        if (event->handled)
            return;
        break;
    case 24:
        self->vf_0x188(event);
        if (event->handled)
            return;
        break;
    case 25:
        self->vf_0xe0(event);
        if (event->handled)
            return;
        break;
    case 26:
        self->vf_0xe8(event);
        if (event->handled)
            return;
        break;
    case 28:
        self->vf_0x100(event);
        if (event->handled)
            return;
        break;
    case 30:
        self->vf_0x190(event);
        if (event->handled)
            return;
        break;
    case 31:
        self->vf_0x198(event);
        if (event->handled)
            return;
        break;
    case 32:
        self->vf_0x1a0(event);
        if (event->handled)
            return;
        break;
    case 33:
        self->vf_0x110(event);
        if (event->handled)
            return;
        break;
    case 34:
        self->vf_0x118(event);
        if (event->handled)
            return;
        break;
    case 42:
        self->vf_0xf8(event);
        if (event->handled)
            return;
        break;
    default:
        break;
    }
    fn_7100b1e3fc(reinterpret_cast<char*>(self) + 0xb8, event);
}
