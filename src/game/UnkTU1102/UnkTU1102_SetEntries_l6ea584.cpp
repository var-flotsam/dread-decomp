// Unit 1102 (CXParasiteAIComponent hint): replaces the component's entries (+0x14740) with the weak
// handles of the spawn points registered for the targets of a list of breakable parts.
// Names and layouts are placeholders.
#include <new>

#include "game/CAISmartObjectComponent.h"
#include "game/UnkTU1102.h"
#include "game/UnkTU365.h"
#include "game/UnkXParasiteAI.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// Signature as src/game/UnkRefPtrList_b50.cpp declares it.
struct UnkRefPtrList_b50;

// One 0x40-byte part of the list handed in.
struct UnkPart_ea584 {
    u8 pad_0x0[0x40];
};

struct UnkPartList_ea584 {
    UnkPart_ea584* data;
    u32 count;
};

// A counted reference to a weak handle, held while it is appended.
struct UnkHandleRef_ea584 {
    UnkWeakHandle* handle;

    explicit UnkHandleRef_ea584(UnkWeakHandle* p) : handle(p) {
        if (handle)
            ++handle->ref_count;
    }
    ~UnkHandleRef_ea584() {
        if (handle && --handle->ref_count == 0)
            handle->vf_0x10();
    }
};

// The weak handle of `target`, created on first use (GetWeakHandle with the count set before the
// vtable, the order this unit stores them in).
static inline UnkWeakHandle* WeakHandleOf(UnkWeakTarget* target) {
    UnkWeakHandle** slot = target->vf_0x28();
    if (!slot)
        return nullptr;
    UnkWeakHandle* handle = *slot;
    if (!handle) {
        handle = static_cast<UnkWeakHandle*>(fn_71000839a0(0x18, 0x3e));
        handle->ref_count = 0;
        *reinterpret_cast<void***>(handle) = &g_unk_7101a209e0[2];
        handle->target = target;
        AssignRef(slot, handle);
    }
    return handle;
}

// A slot of the entries: a counted reference to a weak handle.
struct UnkHandleSlot_ea584 {
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

static inline void AppendHandle(base::global::CRntVector<UnkWeakHandle*>& list, const UnkHandleRef_ea584& value) {
    u32 n = list.count;
    u32 new_capacity = list.capacity;
    if (new_capacity <= n && list.step != 0)
        new_capacity = (n / list.step + 1) * list.step;
    // A capacity that doesn't grow past the count (no step) hangs, as in the original.
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    fn_71006ebe54(reinterpret_cast<UnkRefPtrList_b50*>(&list), new_capacity);
    UnkHandleSlot_ea584* slot = new (&list.data[list.count]) UnkHandleSlot_ea584();
    slot->Assign(value.handle);
    ++list.count;
}

void fn_71006ea584(UnkXParasiteAI* self, void* inner) {
    base::global::CRntVector<UnkWeakHandle*>& entries = self->entries;
    for (u32 i = 0; i < entries.count; ++i) {
        if (UnkWeakHandle* entry = entries.data[i]) {
            if (--entry->ref_count == 0)
                entry->vf_0x10();
        }
    }
    entries.count = 0;

    UnkPartList_ea584* parts = static_cast<UnkPartList_ea584*>(inner);
    for (UnkPart_ea584 *part = parts->data, *end = parts->data + parts->count; part != end; ++part) {
        UnkWeakHandle* target = *reinterpret_cast<UnkWeakHandle**>(
            fn_71002a76a4(reinterpret_cast<UnkBreakablePart*>(part)));
        void* object = target ? target->target : nullptr;
        if (UnkSpawnRegistered_ovb0065* spawn = fn_7100360044(object)) {
            UnkHandleRef_ea584 handle(WeakHandleOf(reinterpret_cast<UnkWeakTarget*>(spawn)));
            AppendHandle(entries, handle);
        }
    }
}
