// CSamusHangMovementState's unit (tools/tu_map.py unit 1463). fn_71009a1f40 (called by the destructor,
// CSamusHangMovementState_Dtor_b49a4.cpp) unregisters the state's listener part (+0x188) from the ledge it holds
// (+0x1b8, a counted reference whose object at +0x10 keeps a singly linked list at +0x128, base/UnkSList.h) and
// releases the reference. fn_71009a0104 tracks the owner's (+0x30) stick input (+0x1d8, +0x9ec/+0x9f0) against the
// values at the start of the hang (+0x1c8) and at the last grab (+0x1d4, taken when the grab at +0x2b0 ends),
// latching a flag per axis once the input leaves its start value. Names and layouts are placeholders.
#include "base/UnkSList.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkHangListenersTag_ovb0041;
using UnkHangListeners_ovb0041 = base::UnkSList<UnkHangListenersTag_ovb0041>;
using UnkHangListenerNode_ovb0041 = UnkHangListeners_ovb0041::Node;

static inline s32 IndexOf(UnkHangListeners_ovb0041* list, u64 value) {
    u32 i = 0;
    for (UnkHangListenerNode_ovb0041* n = list->head; n; n = n->next, ++i) {
        if (n->value == value)
            return static_cast<s32>(i);
    }
    return -1;
}

static inline UnkHangListenerNode_ovb0041* Find(UnkHangListeners_ovb0041* list, u64 value) {
    for (UnkHangListenerNode_ovb0041* n = list->head; n; n = n->next) {
        if (n->value == value)
            return n;
    }
    return nullptr;
}

static inline void Erase(UnkHangListeners_ovb0041* list, UnkHangListenerNode_ovb0041* node) {
    UnkHangListenerNode_ovb0041* prev = nullptr;
    UnkHangListenerNode_ovb0041* it = list->head;
    while (it) {
        UnkHangListenerNode_ovb0041* cur = it;
        UnkHangListenerNode_ovb0041* next = it->next;
        it = next;
        if (cur != node) {
            prev = cur;
            continue;
        }
        if (prev) {
            prev->next = next;
            if (list->tail == node)
                list->tail = prev;
        } else {
            list->head = next;
        }
        if (--list->count == 0) {
            list->head = nullptr;
            list->tail = nullptr;
        }
        list->allocator->vf_0x48(node);
        return;
    }
}

struct UnkHangLedgeOwner_ovb0041 {
    u8 pad_0x0[0x128];
    UnkHangListeners_ovb0041 listeners;  // 0x128
};

struct UnkHangLedgeRef_ovb0041 : UnkRefObject {
    UnkHangLedgeOwner_ovb0041* owner;  // 0x10
};

struct UnkSamusHangState2_ovb0041 {
    u8 pad_0x0[0x188];
    u8 listener[0x30];  // 0x188, the part registered with the ledge
    UnkHangLedgeRef_ovb0041* ledge;  // 0x1b8
};

void fn_71009a1f40(void* p) {
    UnkSamusHangState2_ovb0041* self = static_cast<UnkSamusHangState2_ovb0041*>(p);
    UnkHangLedgeRef_ovb0041* ledge = self->ledge;
    if (!ledge || !ledge->owner)
        return;
    UnkHangListeners_ovb0041* list = &ledge->owner->listeners;
    u64 key = reinterpret_cast<u64>(self->listener);
    if (IndexOf(list, key) >= 0) {
        if (UnkHangListenerNode_ovb0041* node = Find(list, key))
            Erase(list, node);
    }
    if (UnkHangLedgeRef_ovb0041* r = self->ledge) {
        if (--r->ref_count == 0)
            r->vf_0x10();
        self->ledge = nullptr;
    }
}

struct UnkHangGrabState_ovb0041 {
    u8 pad_0x0[0x360];
    u8 grabbing;  // 0x360
};

struct UnkHangInput_ovb0041 {
    u8 pad_0x0[0x9ec];
    f32 x;  // 0x9ec
    f32 y;  // 0x9f0
};

struct UnkHangOwner_ovb0041 {
    u8 pad_0x0[0x1d8];
    UnkHangInput_ovb0041* input;  // 0x1d8
    u8 pad_0x1e0[0x40];
    void* anim;  // 0x220
    u8 pad_0x228[0x88];
    UnkHangGrabState_ovb0041* grab;  // 0x2b0
};

struct UnkSamusHangState3_ovb0041 {
    u8 pad_0x0[0x30];
    UnkHangOwner_ovb0041* owner;  // 0x30
    u8 pad_0x38[0x190];
    f32 start_x;  // 0x1c8
    f32 start_y;  // 0x1cc
    bool moved_x;  // 0x1d0
    bool moved_y;  // 0x1d1
    u8 pad_0x1d2[2];
    f32 grab_x;  // 0x1d4
    f32 grab_y;  // 0x1d8
    bool grab_moved_x;  // 0x1dc
    bool grab_moved_y;  // 0x1dd
    u8 was_grabbing;  // 0x1de
};

// Whether the input left its starting value: the start was (nearly) zero, or the sign flipped.
static inline bool Changed(f32 start, f32 now) {
    if (start + 0.001f >= 0.0f && start - 0.001f <= 0.0f)
        return true;
    if (now >= 0.0f && start < 0.0f)
        return true;
    if (now <= 0.0f && start > 0.0f)
        return true;
    return false;
}

void fn_71009a0104(UnkSamusHangState3_ovb0041* self) {
    UnkHangOwner_ovb0041* owner = self->owner;
    UnkHangGrabState_ovb0041* grab = owner->grab;
    void* anim = owner->anim;
    if (grab && anim) {
        f32 y = 0.0f;
        f32 x = 0.0f;
        if (UnkHangInput_ovb0041* input = owner->input) {
            x = input->x;
            y = input->y;
        }
        u8 grabbing = grab->grabbing;
        if ((grabbing == 0) & (self->was_grabbing != 0)) {
            self->grab_moved_x = false;
            self->grab_moved_y = false;
            self->grab_x = x;
            self->grab_y = y;
        }
        self->was_grabbing = grabbing;
        if (!self->moved_x && Changed(self->start_x, x))
            self->moved_x = true;
        if (!self->moved_y && Changed(self->start_y, y))
            self->moved_y = true;
        if (!grabbing) {
            if (!self->grab_moved_x && Changed(self->grab_x, x))
                self->grab_moved_x = true;
            if (!self->grab_moved_y && Changed(self->grab_y, y))
                self->grab_moved_y = true;
        }
    }
}
