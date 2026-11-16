// CGunComponent unit (tools/tu_map.py unit 1304): sends an event to every listener, last first,
// dropping each listener whose callback returns nonzero; then applies the removals and additions
// queued while the event was being sent. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

struct UnkGunEvent_l841f60 {
    CStrId id;
    u32 field_0x8;
    u8 field_0xc;
};

struct UnkGunListener_l841f60 {
    void* owner;
    void* context;                                       // 0x8
    s32 (*callback)(void* context, UnkGunEvent_l841f60 event);  // 0x10

    s32 Invoke(UnkGunEvent_l841f60 event) { return callback(context, event); }
};

using UnkGunListeners_l841f60 = CRntVector<UnkGunListener_l841f60*>;

struct UnkGunDispatcher_l841f60 {
    UnkGunListeners_l841f60 listeners;       // 0x0
    UnkGunListeners_l841f60 pendingAdds;     // 0x28
    UnkGunListeners_l841f60 pendingRemoves;  // 0x50
    bool dispatching;                        // 0x78
};

// The listener at `index`, growing the array to cover it first (CRntVector::GrowAt, out of line).
UnkGunListener_l841f60*& fn_710084227c(UnkGunDispatcher_l841f60* self, u32 index);

// Removes the listener at `index`; the last one takes its place.
static inline void RemoveAt(UnkGunListeners_l841f60* v, u32 index) {
    if (v->count > index) {
        u32 last = v->count - 1;
        if (last > index)
            v->data[index] = v->data[last];
        v->count = last;
    }
}

void fn_7100841f60(UnkGunDispatcher_l841f60* self, UnkGunEvent_l841f60 event) {
    self->dispatching = true;
    for (s32 i = self->listeners.count - 1; i >= 0; --i) {
        UnkGunListener_l841f60* listener = fn_710084227c(self, i);
        if (listener->Invoke(event)) {
            listener->owner = nullptr;
            RemoveAt(&self->listeners, i);
        }
    }
    self->dispatching = false;

    UnkGunListener_l841f60** end = self->pendingRemoves.data + self->pendingRemoves.count;
    for (UnkGunListener_l841f60** it = self->pendingRemoves.data; it != end; ++it) {
        for (u32 j = 0; j < self->listeners.count; ++j) {
            if (self->listeners.data[j] == *it) {
                RemoveAt(&self->listeners, j);
                break;
            }
        }
    }
    self->pendingRemoves.count = 0;

    for (u32 i = 0; i < self->pendingAdds.count; ++i)
        self->listeners.PushBack(self->pendingAdds.data[i]);
    self->pendingAdds.count = 0;
}
