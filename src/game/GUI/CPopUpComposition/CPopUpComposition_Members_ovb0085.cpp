// GUI::CPopUpComposition's unit (tools/tu_map.py): the composition's constructor (fn_7100b5e5c0) and
// destructor (fn_7100b5e7c8), its input handler (fn_7100b5ed84), the alpha of the panel's sprites and
// labels (fn_7100b5effc) and a flag of the label named by the string id g_unk_71727653a8 (fn_7100b6026c).
// The members: two registrations in deferred sets (+0x230, +0x248), three entry lists (+0x260; the entries
// of the first point back at their owner), a closing flag (+0x2e1), the mode (+0x2e8), a record array
// (+0x2f0, emptied by fn_7100b607f0), the panel (+0x318, refcounted), string and path ids, and the actions
// for accept, select and back (+0x330, +0x340, +0x350) of which one becomes pending (+0x360). Names and
// layouts are placeholders.
#include <new>

#include "base/UnkDeferredSet.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

// As src/game_s7/UnkGuiS7.h declares them.
using UnkGuiObj = UnkGuiDisplayObject;
UnkGuiObj* fn_71011d9508(UnkGuiObj* self, const char* const* path);
void fn_71011cfba8(UnkGuiObj* object, f32 alpha);

// As src/game/GUI/CPopUpGeneric/GUI_CPopUpGeneric_Dtor.cpp declares it: an id pair's destructor.
struct UnkIdPair;
void fn_7100026f70(UnkIdPair*);

extern char g_unk_7101a9d758[];  // the composition's vtable
void fn_7100b607b4();
void fn_7100b607d0();

// The child's name: a string id whose text block keeps a use count at +0x44 and the text at +0x10.
extern u8* g_unk_71727653a8 __attribute__((visibility("hidden")));

static inline const char* ChildName() {
    u8* data = g_unk_71727653a8;
    ++*reinterpret_cast<u32*>(data + 0x44);
    const char* text = *reinterpret_cast<const char* const*>(data + 0x10);
    return text ? text : g_unk_710162e2ed;
}

static inline UnkGuiObj* FindByPath(void* self, const CRntString& path) {
    return fn_71011d9508(static_cast<UnkGuiObj*>(self), reinterpret_cast<const char* const*>(&path));
}

static inline UnkGuiDisplayObject* AsLabel(UnkGuiDisplayObject* obj) {
    return obj && fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(obj->GetClass()),
                                reinterpret_cast<const base::reflection::CType*>(fn_71011e8dfc()))
               ? obj
               : nullptr;
}

// The object's class is read before the getter runs.
template <const base::reflection::CClass* (*Getter)()>
static inline bool IsA(UnkGuiDisplayObject* obj) {
    return fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(obj->GetClass()),
                         reinterpret_cast<const base::reflection::CType*>(Getter()));
}

// A string id whose destructor decrements and tests in two steps (as in fn_7100b607f0.cpp).
struct UnkReleasedStrId_ovb0085 {
    UnkStringData* data;

    __attribute__((always_inline)) ~UnkReleasedStrId_ovb0085() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

// A registration in a deferred set: the set, this composition and the callback.
struct UnkPopUpListener_ovb0085 {
    base::UnkDeferredSet<void>* set;
    void* target;
    void (*callback)();

    __attribute__((always_inline)) void Init(void* owner, void (*f)()) {
        set = nullptr;
        target = owner;
        callback = f;
    }
    __attribute__((always_inline)) void Destroy() {
        if (set) {
            fn_71004f3ea4(set, this);
            set = nullptr;
        }
        callback = nullptr;
        target = nullptr;
    }
};

struct UnkPopUpEntry_ovb0085 {
    void* owner;
};

// Value-constructed as a whole: three arrays with allocator flags 0xf.
struct UnkPopUpLists_ovb0085 {
    base::UnkVector<UnkPopUpEntry_ovb0085*> list_0x0 = base::UnkVector<UnkPopUpEntry_ovb0085*>(0xf);
    base::UnkVector<UnkPopUpEntry_ovb0085*> list_0x28 = base::UnkVector<UnkPopUpEntry_ovb0085*>(0xf);
    base::UnkVector<UnkPopUpEntry_ovb0085*> list_0x50 = base::UnkVector<UnkPopUpEntry_ovb0085*>(0xf);

    // The first list's entries forget their owner; then all three are emptied and freed.
    __attribute__((always_inline)) void Destroy() {
        UnkPopUpEntry_ovb0085** end = list_0x0.data + list_0x0.count;
        for (UnkPopUpEntry_ovb0085** p = list_0x0.data; p != end; ++p)
            (*p)->owner = nullptr;
        list_0x0.count = 0;
        list_0x28.count = 0;
        list_0x50.count = 0;
        list_0x50.Free();
        list_0x28.Free();
        list_0x0.Free();
    }
};

struct UnkIdPair20Array;
void fn_7100b607f0(UnkIdPair20Array* self, u32 n);
struct UnkPopUpIdRecord_ovb0085;

struct UnkPopUpAction_ovb0085 {
    void* field_0x0;
    void* field_0x8;
};

struct UnkPopUpIds_ovb0085 {
    CStrId id;
    CFilePathStrId path;
};

// A child of the panel container (+0x1c8 the count, +0x1d0 the list).
struct UnkPopUpNode_ovb0085 {
    UnkGuiDisplayObject* child;
    UnkPopUpNode_ovb0085* next;
};

struct UnkPopUpContainer_ovb0085 {
    u8 pad_0x0[0x1c8];
    s32 count;  // 0x1c8
    u8 pad_0x1cc[0x4];
    UnkPopUpNode_ovb0085* head;  // 0x1d0
};

// The panel's refcounted holder: the panel object at +0x10.
struct UnkPopUpPanelRef_ovb0085 {
    u8 pad_0x0[0x10];
    UnkGuiDisplayObject* object;
};

struct UnkPopUpLabel_ovb0085 {
    u8 pad_0x0[0xb4];
    bool ready;  // 0xb4
};

struct UnkPopUpEvent_ovb0085 {
    u8 pad_0x0[0x8];
    s32 kind;  // 0x8
    u8 pad_0xc[0xe];
    bool handled;  // 0x1a
};

struct UnkPopUpComposition_ovb0085 {
    void* vtable;
    u8 pad_0x8[0xac];
    bool ready;  // 0xb4
    u8 pad_0xb5[0x17b];
    UnkPopUpListener_ovb0085 on_show;  // 0x230
    UnkPopUpListener_ovb0085 on_hide;  // 0x248
    UnkPopUpLists_ovb0085 lists;       // 0x260
    bool field_0x2d8;
    u8 pad_0x2d9[0x7];
    bool field_0x2e0;
    bool closing;  // 0x2e1
    bool field_0x2e2;
    bool field_0x2e3;
    bool field_0x2e4;
    u8 pad_0x2e5[0x3];
    s32 mode;  // 0x2e8
    u8 pad_0x2ec[0x4];
    base::UnkVector<UnkPopUpIdRecord_ovb0085> records;  // 0x2f0
    base::UnkRefPtrMember panel;                         // 0x318
    base::UnkRefPtrMember field_0x320;
    UnkReleasedStrId_ovb0085 id_0x328;
    UnkPopUpAction_ovb0085 on_accept;  // 0x330
    UnkPopUpAction_ovb0085 on_select;  // 0x340
    UnkPopUpAction_ovb0085 on_back;    // 0x350
    UnkPopUpAction_ovb0085 pending;    // 0x360
    UnkPopUpIds_ovb0085 ids_0x370;
    u64 field_0x380;
    UnkPopUpIds_ovb0085 ids_0x388;
    u64 field_0x398;
    bool field_0x3a0;
    bool field_0x3a1;
    u8 pad_0x3a2[0x2];
    f32 field_0x3a4;
};
static_assert(__builtin_offsetof(UnkPopUpComposition_ovb0085, mode) == 0x2e8, "layout");
static_assert(__builtin_offsetof(UnkPopUpComposition_ovb0085, field_0x3a4) == 0x3a4, "layout");

void fn_7100b5e5c0(UnkPopUpComposition_ovb0085* self) {
    fn_71011d8d24(self);
    self->vtable = g_unk_7101a9d758 + 0x10;
    self->on_show.Init(self, fn_7100b607b4);
    self->on_hide.Init(self, fn_7100b607d0);
    new (&self->lists) UnkPopUpLists_ovb0085;
    self->field_0x2d8 = false;
    self->field_0x2e0 = false;
    self->closing = true;
    self->field_0x2e2 = false;
    self->field_0x2e3 = false;
    self->field_0x2e4 = false;
    new (&self->records) base::UnkVector<UnkPopUpIdRecord_ovb0085>(0xf);
    self->panel.ptr = nullptr;
    self->field_0x320.ptr = nullptr;
    new (reinterpret_cast<CStrId*>(&self->id_0x328)) CStrId(g_unk_7101d470e0);
    self->pending.field_0x8 = nullptr;
    self->pending.field_0x0 = nullptr;
    self->on_back.field_0x8 = nullptr;
    self->on_back.field_0x0 = nullptr;
    self->on_select.field_0x8 = nullptr;
    self->on_select.field_0x0 = nullptr;
    self->on_accept.field_0x8 = nullptr;
    self->on_accept.field_0x0 = nullptr;
    new (&self->ids_0x370.id) CStrId(g_unk_7101d470e0);
    new (&self->ids_0x370.path) CFilePathStrId(g_unk_7101d470e8);
    self->field_0x380 = 0;
    new (&self->ids_0x388.id) CStrId(g_unk_7101d470e0);
    new (&self->ids_0x388.path) CFilePathStrId(g_unk_7101d470e8);
    self->field_0x398 = 0;
    self->field_0x3a0 = false;
    self->field_0x3a1 = false;
    self->field_0x3a4 = -1.0f;
}

void fn_7100b5e7c8(UnkPopUpComposition_ovb0085* self) {
    self->vtable = g_unk_7101a9d758 + 0x10;
    fn_7100026f70(reinterpret_cast<UnkIdPair*>(&self->ids_0x388));
    fn_7100026f70(reinterpret_cast<UnkIdPair*>(&self->ids_0x370));
    self->id_0x328.~UnkReleasedStrId_ovb0085();
    self->field_0x320.~UnkRefPtrMember();
    self->panel.~UnkRefPtrMember();
    if (self->records.data) {
        fn_7100b607f0(reinterpret_cast<UnkIdPair20Array*>(&self->records), 0);
        self->records.allocator->vf_0x48(self->records.data);
        self->records.capacity = 0;
    }
    self->lists.Destroy();
    self->on_hide.Destroy();
    self->on_show.Destroy();
    fn_71011d8e90(reinterpret_cast<UnkGuiDisplayObject*>(self));
}

void fn_7100b60190(UnkPopUpComposition_ovb0085* self);

// Closes the pop-up (fn_7100b60190) with `action` pending, when it has one.
static inline void Close(UnkPopUpComposition_ovb0085* self, const UnkPopUpAction_ovb0085& action) {
    fn_7100b60190(self);
    self->pending.field_0x8 = nullptr;
    self->pending.field_0x0 = nullptr;
    if (action.field_0x8)
        self->pending = action;
}

// Mode 0: event 0x11 accepts unless the named label is not ready yet. Mode 1: event 0x11 selects and
// 0x12 goes back. A handled event (also every event while closing) is marked.
void fn_7100b5ed84(UnkPopUpComposition_ovb0085* self, UnkPopUpEvent_ovb0085* event) {
    if (event && event->handled)
        return;
    if (!self->closing) {
        switch (self->mode) {
        case 0: {
            if (event && event->kind != 0x11)
                return;
            auto* label =
                reinterpret_cast<UnkPopUpLabel_ovb0085*>(AsLabel(FindByPath(self, CRntString(ChildName()))));
            if (!label || label->ready)
                Close(self, self->on_accept);
            break;
        }
        case 1:
            if (!event)
                return;
            switch (event->kind) {
            case 0x11:
                Close(self, self->on_select);
                break;
            case 0x12:
                self->field_0x3a1 = false;
                Close(self, self->on_back);
                break;
            default:
                return;
            }
            break;
        default:
            return;
        }
    }
    if (event)
        event->handled = true;
}

static inline UnkPopUpNode_ovb0085* NodeAt(UnkPopUpContainer_ovb0085* container, s32 index) {
    UnkPopUpNode_ovb0085* node = container->head;
    if (index)
        for (s32 j = 0; j < index; ++j)
            node = node->next;
    return node;
}

static inline void SetAlpha(UnkGuiDisplayObject* child, f32 alpha) {
    bool visible = alpha > 0.0f;
    fn_71011cfba8(child, alpha);
    fn_71011cdc30(child, visible);
}

// Sets the alpha (and the visibility, while above zero) of the panel's sprites and labels.
void fn_7100b5effc(UnkPopUpComposition_ovb0085* self, f32 alpha) {
    auto* ref = reinterpret_cast<UnkPopUpPanelRef_ovb0085*>(self->panel.ptr);
    if (!ref)
        return;
    UnkGuiDisplayObject* panel = ref->object;
    if (!panel || !IsA<fn_71011d7b84>(panel))
        return;
    auto* container = reinterpret_cast<UnkPopUpContainer_ovb0085*>(panel);
    s32 n = container->count;
    for (s32 i = 0; i < n; ++i) {
        UnkGuiDisplayObject* child = NodeAt(container, i)->child;
        if (!child)
            continue;
        if (IsA<fn_71011f6890>(child))
            SetAlpha(child, alpha);
        if (IsA<fn_71011e8dfc>(child))
            SetAlpha(child, alpha);
    }
}

// While the composition is set up, sets the named label's flag (fn_71011cdce8) to `value`.
void fn_7100b6026c(UnkPopUpComposition_ovb0085* self, bool value) {
    if (!self->ready)
        return;
    if (UnkGuiDisplayObject* label = AsLabel(FindByPath(self, CRntString(ChildName()))))
        fn_71011cdce8(label, value);
}
