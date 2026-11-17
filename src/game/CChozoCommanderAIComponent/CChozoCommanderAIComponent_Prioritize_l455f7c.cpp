// CChozoCommanderAIComponent unit (tools/tu_map.py unit 665): fills the other object's attack list
// (+0x28): optionally empties it, runs the base handler (fn_710110bc0c), then moves the attacks that
// should go first to its front: the one at +0x14b90 when listed (and nothing else), the one at +0x14f58
// when listed and allowed, and the one at +0x149a8 when the current attack is one of two and the
// distance to the target is inside its range (fn_71003624f4). Names and layouts are placeholders.
#include <math.h>
#include "base/UnkSList.h"
#include "game/UnkActor.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkAttack_b0086;
void fn_71003624f4(UnkAttack_b0086* self, u32 value, f32* first, f32* second);
void fn_710110bc0c(void* self, void* arg, u32 id, u32 kind, s32 amount);

struct UnkChozoListTag_l455f7c;
using UnkChozoList_l455f7c = base::UnkSList<UnkChozoListTag_l455f7c>;

struct UnkChozoOther_l455f7c {
    u8 pad_0x0[0x28];
    UnkChozoList_l455f7c list;  // 0x28
};

struct UnkChozoCommanderAI_l455f7c {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x1108];
    u32 field_0x1140;
    u8 pad_0x1144[0x2c];
    u32 field_0x1170;
    u8 pad_0x1174[0x13834];
    u8 attack_0x149a8[0x170];
    u8 attack_0x14b18[0x78];
    u8 attack_0x14b90[0x3c8];
    u8 attack_0x14f58[0x120];
    u8 attack_0x15078[0xca0];
    bool field_0x15d18;
    u8 pad_0x15d19[0x57];
    bool field_0x15d70;
};
static_assert(__builtin_offsetof(UnkChozoCommanderAI_l455f7c, field_0x1170) == 0x1170, "");
static_assert(__builtin_offsetof(UnkChozoCommanderAI_l455f7c, attack_0x149a8) == 0x149a8, "");
static_assert(__builtin_offsetof(UnkChozoCommanderAI_l455f7c, attack_0x15078) == 0x15078, "");
static_assert(__builtin_offsetof(UnkChozoCommanderAI_l455f7c, field_0x15d70) == 0x15d70, "");

static inline s32 IndexOf(UnkChozoList_l455f7c* list, u64 v) {
    u32 i = 0;
    for (UnkChozoList_l455f7c::Node* n = list->head; n; n = n->next, ++i)
        if (n->value == v)
            return static_cast<s32>(i);
    return -1;
}

static inline UnkChozoList_l455f7c::Node* Find(UnkChozoList_l455f7c* list, u64 v) {
    for (UnkChozoList_l455f7c::Node* n = list->head; n; n = n->next)
        if (n->value == v)
            return n;
    return nullptr;
}

static inline void Erase(UnkChozoList_l455f7c* list, UnkChozoList_l455f7c::Node* pos) {
    UnkChozoList_l455f7c::Node* prev = nullptr;
    for (UnkChozoList_l455f7c::Node* it = list->head; it;) {
        UnkChozoList_l455f7c::Node* next = it->next;
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

static inline void MoveToFront(UnkChozoList_l455f7c* list, u64 v) {
    if (UnkChozoList_l455f7c::Node* n = Find(list, v))
        Erase(list, n);
    list->PushFront(v);
}

void fn_7100455f7c(UnkChozoCommanderAI_l455f7c* self, UnkChozoOther_l455f7c* other, u32 id) {
    if (self->field_0x15d70)
        other->list.Clear();
    fn_710110bc0c(self, other, id, 2, 5);
    UnkChozoList_l455f7c* list = &other->list;

    u64 first = reinterpret_cast<u64>(self->attack_0x14b90);
    if (IndexOf(list, first) >= 0) {
        MoveToFront(list, first);
        return;
    }
    if (!self->field_0x15d18 && self->field_0x1170 >= 2) {
        u64 second = reinterpret_cast<u64>(self->attack_0x14f58);
        if (IndexOf(list, second) >= 0)
            MoveToFront(list, second);
    }

    UnkActor* target = fn_7100d7c990(self->owner);
    if (!target)
        return;
    f32 target_x = target->field_0x44;
    f32 owner_x = self->owner->field_0x44;
    void* current = fn_7101105970(reinterpret_cast<UnkScorpiusAI_b69a4*>(self));
    if (current != self->attack_0x14b18 && current != self->attack_0x15078)
        return;
    f32 dist = fabsf(target_x - owner_x);
    u64 ranged = reinterpret_cast<u64>(self->attack_0x149a8);
    if (IndexOf(list, ranged) < 0)
        return;
    f32 min = -1.0f;
    f32 max = -1.0f;
    fn_71003624f4(reinterpret_cast<UnkAttack_b0086*>(self->attack_0x149a8), self->field_0x1140, &min, &max);
    if (!(min < 0.0f) && !(dist >= min))
        return;
    if (!(max < 0.0f) && !(dist <= max))
        return;
    MoveToFront(list, ranged);
}
