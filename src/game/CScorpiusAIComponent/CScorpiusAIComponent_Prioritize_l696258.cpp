// CScorpiusAIComponent unit: runs the base handler (fn_710110bc0c) and then, in phase 2, moves one of two
// attacks to the front of the other object's attack list (+0x28) when the phase state allows it and the
// attack is listed. Names and layouts are placeholders.
#include "base/UnkSList.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkScorpiusListTag_l696258;
using UnkScorpiusList_l696258 = base::UnkSList<UnkScorpiusListTag_l696258>;

struct UnkScorpiusOther_l696258 {
    u8 pad_0x0[0x28];
    UnkScorpiusList_l696258 list;  // 0x28
};

struct UnkScorpiusAI_l696258 {
    u8 pad_0x0[0x1140];
    u32 phase;  // 0x1140
    u8 pad_0x1144[0x139ac];
    u8 attack_0x14af0[0x68];
    u8 attack_0x14b58[0xd1];
    bool field_0x14c29;
    u8 pad_0x14c2a[0xce];
    bool field_0x14cf8;
    bool field_0x14cf9;
    bool field_0x14cfa;
    u8 pad_0x14cfb[0x89];
    u32 state;  // 0x14d84
};
static_assert(__builtin_offsetof(UnkScorpiusAI_l696258, attack_0x14af0) == 0x14af0, "");
static_assert(__builtin_offsetof(UnkScorpiusAI_l696258, field_0x14c29) == 0x14c29, "");
static_assert(__builtin_offsetof(UnkScorpiusAI_l696258, field_0x14cf8) == 0x14cf8, "");
static_assert(__builtin_offsetof(UnkScorpiusAI_l696258, state) == 0x14d84, "");

static inline s32 IndexOf(UnkScorpiusList_l696258* list, u64 v) {
    u32 i = 0;
    for (UnkScorpiusList_l696258::Node* n = list->head; n; n = n->next, ++i)
        if (n->value == v)
            return static_cast<s32>(i);
    return -1;
}

static inline UnkScorpiusList_l696258::Node* Find(UnkScorpiusList_l696258* list, u64 v) {
    for (UnkScorpiusList_l696258::Node* n = list->head; n; n = n->next)
        if (n->value == v)
            return n;
    return nullptr;
}

static inline void Erase(UnkScorpiusList_l696258* list, UnkScorpiusList_l696258::Node* pos) {
    UnkScorpiusList_l696258::Node* prev = nullptr;
    for (UnkScorpiusList_l696258::Node* it = list->head; it;) {
        UnkScorpiusList_l696258::Node* next = it->next;
        if (it == pos) {
            if (prev) {
                prev->next = next;
                if (list->tail == pos)
                    list->tail = prev;
            } else {
                list->head = next;
            }
            if (--list->count == 0) {
                list->head = nullptr;
                list->tail = nullptr;
            }
            list->allocator->vf_0x48(pos);
            return;
        }
        prev = it;
        it = next;
    }
}

static inline void MoveToFront(UnkScorpiusList_l696258* list, u64 v) {
    if (UnkScorpiusList_l696258::Node* n = Find(list, v))
        Erase(list, n);
    list->PushFront(v);
}

static inline void* Current(UnkScorpiusAI_l696258* self) {
    return fn_7101105970(reinterpret_cast<UnkScorpiusAI_b69a4*>(self));
}

void CScorpiusAIComponent_MovePoisonousGasAttackToListFront(UnkScorpiusAI_l696258* self, UnkScorpiusOther_l696258* other, u32 id) {
    fn_710110bc0c(self, other, id, 2, 6);
    if (self->phase != 2)
        return;

    bool first;
    if (self->field_0x14cf8)
        first = !self->field_0x14cf9 && self->state == 3 && Current(self) != self->attack_0x14af0;
    else
        first = self->state != 0;
    if (first) {
        u64 v = reinterpret_cast<u64>(self->attack_0x14af0);
        if (IndexOf(&other->list, v) >= 0) {
            MoveToFront(&other->list, v);
            return;
        }
    }

    if (self->phase != 2 || !self->field_0x14cf8 || self->field_0x14cfa || self->state < 3 || !self->field_0x14cf9)
        return;
    if (Current(self) == self->attack_0x14af0 || self->field_0x14c29)
        return;
    u64 v = reinterpret_cast<u64>(self->attack_0x14b58);
    if (IndexOf(&other->list, v) >= 0)
        MoveToFront(&other->list, v);
}
