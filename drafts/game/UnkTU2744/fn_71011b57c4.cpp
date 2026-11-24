// Unit 2744 (gui/scripts/guimanager.lua): tells the GUI root about an object. While the root defers (+0x70)
// and the object is listed, its weak handle is queued (+0xc8); otherwise the object leaves its parent and
// the root's lists, loses the focus property and is released. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiRoot.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntVector;

void fn_71011bb844(void* self, u32 capacity);  // reserve
void fn_71011cd340(UnkGuiDisplayObject* self);

// The root as this function reads it: the deferral flag and three lists.
struct UnkGuiRootLists_b57c4 {
    u8 pad_0x0[0x70];
    bool field_0x70;
    u8 pad_0x71[0x7];
    CRntVector<UnkGuiDisplayObject*> field_0x78;
    CRntVector<UnkGuiDisplayObject*> field_0xa0;
    CRntVector<UnkWeakHandle*> field_0xc8;  // the queued handles
};
static_assert(__builtin_offsetof(UnkGuiRootLists_b57c4, field_0xc8) == 0xc8, "UnkGuiRootLists_b57c4 layout");

// The parent container as this function reads it: its child count.
struct UnkGuiParent_b57c4 {
    u8 pad_0x0[0x1c8];
    u32 count;  // 0x1c8
};

// The object as this function reads it: its parent and whether it is in one of the root's lists.
struct UnkGuiObjectLinks_b57c4 {
    u8 pad_0x0[0x120];
    UnkGuiParent_b57c4* parent;  // 0x120
    u8 pad_0x128[0x5f];
    bool field_0x187;  // listed
};
static_assert(__builtin_offsetof(UnkGuiObjectLinks_b57c4, field_0x187) == 0x187, "UnkGuiObjectLinks_b57c4 layout");

// A counted reference to a weak handle, held while it is appended.
struct UnkHandleRef_b57c4 {
    UnkWeakHandle* handle;

    explicit UnkHandleRef_b57c4(UnkWeakHandle* p) : handle(p) {
        if (handle)
            ++handle->ref_count;
    }
    ~UnkHandleRef_b57c4() {
        if (handle && --handle->ref_count == 0)
            handle->vf_0x10();
    }
};

// A slot of the queue: a counted reference to a weak handle.
struct UnkHandleSlot_b57c4 {
    UnkWeakHandle* handle;

    void Assign(UnkWeakHandle* p) {
        if (p) {
            ++p->ref_count;
            if (UnkWeakHandle* old = handle) {
                if (--old->ref_count == 0)
                    old->vf_0x10();
            }
            handle = p;
        }
    }
};

static inline void AppendHandle(CRntVector<UnkWeakHandle*>& list, const UnkHandleRef_b57c4& value) {
    u32 n = list.count;
    u32 new_capacity = list.capacity;
    if (new_capacity <= n && list.step != 0)
        new_capacity = (n / list.step + 1) * list.step;
    // A capacity that doesn't grow past the count (no step) hangs, as in the original.
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    fn_71011bb844(&list, new_capacity);
    UnkHandleSlot_b57c4* slot = new (&list.data[list.count]) UnkHandleSlot_b57c4();
    slot->Assign(value.handle);
    ++list.count;
}

// The index of `object` in `list`, or the count when it is not there.
static inline u32 IndexOf(const CRntVector<UnkGuiDisplayObject*>& list, UnkGuiDisplayObject* object) {
    for (u32 i = 0; i < list.count; ++i)
        if (list.data[i] == object)
            return i;
    return list.count;
}

void fn_71011b57c4(UnkGuiRoot* root, void* object) {
    UnkGuiRootLists_b57c4* self = reinterpret_cast<UnkGuiRootLists_b57c4*>(root);
    UnkGuiDisplayObject* display = static_cast<UnkGuiDisplayObject*>(object);
    UnkGuiObjectLinks_b57c4* links = static_cast<UnkGuiObjectLinks_b57c4*>(object);
    if (self->field_0x70 && links->field_0x187) {
        UnkHandleRef_b57c4 handle(GetWeakHandle(static_cast<UnkWeakTarget*>(object)));
        AppendHandle(self->field_0xc8, handle);
        return;
    }

    if (links->parent && links->parent->count)
        CDisplayObjectContainer_RemoveChild(reinterpret_cast<UnkDisplayObjectContainer_ovb0032*>(links->parent),
                      static_cast<UnkDisplayObject_ovb0032*>(object));
    fn_71011cd340(display);
    if (links->field_0x187) {
        u32 i = IndexOf(self->field_0xa0, display);
        if (i != self->field_0xa0.count)
            self->field_0xa0.EraseAt(i);
        else
            self->field_0x78.EraseAt(IndexOf(self->field_0x78, display));
        links->field_0x187 = false;
    }
    if (fn_71011cde14(display)->field_0x3e)
        fn_71011b5d94(root, display);
    display->vf_0x10();
}
