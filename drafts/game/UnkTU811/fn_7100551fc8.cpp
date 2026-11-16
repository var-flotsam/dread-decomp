// UnkTU811 (CEmmyAIComponent's unit): fn_7100551fc8 switches the state set to the database's entry named
// by an id: when the entry exists and is not the current one, frees the per-state lists, clears the
// pending list, makes the entry current and tells the listener. Names and layouts are placeholders.
#include "base/UnkLinkedList.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkEmmyStateEntry_l551fc8 {
    u8 pad_0x0[0xd0];
};

struct UnkEmmyStateDb_l551fc8 {
    u8 pad_0x0[0x16a8];
    base::global::CRntSmallDictionary<CStrId, UnkEmmyStateEntry_l551fc8> states;
};

struct UnkEmmyPendingNode_l551fc8 {
    u8 pad_0x0[0x20];
    UnkEmmyPendingNode_l551fc8* next;
};

struct UnkEmmyStateListener_l551fc8 {
    void* context;
    void (*callback)(void* context, UnkEmmyStateEntry_l551fc8* from, UnkEmmyStateEntry_l551fc8* to);
};

struct UnkEmmyAI_l551fc8 {
    u8 pad_0x0[0x146d0];
    base::global::CRntVector<base::global::CRntVector<void*>> stateLists;
    u8 pad_0x146f8[0x14708 - 0x146f8];
    UnkEmmyStateEntry_l551fc8* currentState;
    u32 field_0x14710;
    u8 pad_0x14714[0x14728 - 0x14714];
    base::UnkLinkedList<UnkEmmyPendingNode_l551fc8> pending;
    u8 pad_0x14750[0x14760 - 0x14750];
    UnkEmmyStateListener_l551fc8 stateListener;
    u8 pad_0x14770[0x296c0 - 0x14770];
    UnkEmmyStateDb_l551fc8* db;
};
static_assert(__builtin_offsetof(UnkEmmyAI_l551fc8, stateListener) == 0x14760, "layout");

void fn_7100551fc8(UnkEmmyAI_l551fc8* self, const CStrId& id) {
    UnkEmmyStateDb_l551fc8* db = self->db;
    if (!db)
        return;
    UnkEmmyStateEntry_l551fc8* from = self->currentState;
    if (db->states.Find(id) == db->states.End())
        return;
    UnkEmmyStateEntry_l551fc8* to = db->states.Find(id);
    if (to == from)
        return;
    self->currentState = nullptr;
    for (u32 i = 0; i < self->stateLists.count; ++i)
        self->stateLists.data[i].Free();
    self->stateLists.count = 0;
    self->field_0x14710 = 0;
    self->pending.Clear();
    self->currentState = to;
    if (self->stateListener.callback)
        self->stateListener.callback(self->stateListener.context, from, to);
}
