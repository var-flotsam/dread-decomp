// Unit 112 (the tunables' registry): tells a group's listeners, last to first, that a tunable changed;
// a listener that answers true is detached. Then the listeners removed and added meanwhile are
// applied. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU112.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

struct UnkTunableGroup_l13ffbc;

// A listener of a group: the group it is attached to and its callback with the callback's context.
struct UnkTunableListener_l13ffbc {
    UnkTunableGroup_l13ffbc* owner;
    void* context;
    s32 (*callback)(void* context, CStrId id);  // nonzero: detach the listener

    s32 Notify(CStrId id) { return callback(context, id); }
};

// The group (0x80 bytes; UnkTunableGroup in src/base/UnkTunableRegistry.cpp): its listeners, those
// added and those removed while it notified them, and whether it is notifying.
struct UnkTunableGroup_l13ffbc {
    CRntVector<UnkTunableListener_l13ffbc*> listeners;
    CRntVector<UnkTunableListener_l13ffbc*> added;
    CRntVector<UnkTunableListener_l13ffbc*> removed;
    bool notifying;
};
static_assert(__builtin_offsetof(UnkTunableGroup_l13ffbc, notifying) == 0x78, "");

// CRntVector::GrowAt, out of line for this element type.
UnkTunableListener_l13ffbc** fn_71001402a4(CRntVector<UnkTunableListener_l13ffbc*>* v, u32 i);

// Removes the element at `i`, moving the last one into its place.
static inline void SwapRemove(CRntVector<UnkTunableListener_l13ffbc*>& v, u32 i) {
    if (i < v.count) {
        u32 last = v.count - 1;
        if (i < last)
            v.data[i] = v.data[last];
        v.count = last;
    }
}

void fn_710013ffbc(UnkEntry_710013c3c8* entry, void* arg) {
    UnkTunableGroup_l13ffbc* self = reinterpret_cast<UnkTunableGroup_l13ffbc*>(entry);
    const CStrId* id = static_cast<const CStrId*>(arg);
    self->notifying = true;
    for (s32 i = self->listeners.count - 1; i >= 0; --i) {
        UnkTunableListener_l13ffbc* listener = *fn_71001402a4(&self->listeners, i);
        if (listener->Notify(*id)) {
            listener->owner = nullptr;
            SwapRemove(self->listeners, i);
        }
    }
    self->notifying = false;
    for (UnkTunableListener_l13ffbc **it = self->removed.data, **end = it + self->removed.count; it != end; ++it) {
        for (u64 j = 0; j < self->listeners.count; ++j) {
            if (self->listeners.data[j] == *it) {
                SwapRemove(self->listeners, j);
                break;
            }
        }
    }
    self->removed.count = 0;
    for (UnkTunableListener_l13ffbc **it = self->added.data, **end = it + self->added.count; it != end; ++it) {
        UnkTunableListener_l13ffbc* listener = *it;
        self->listeners.PushBack(listener);
    }
    self->added.count = 0;
}
