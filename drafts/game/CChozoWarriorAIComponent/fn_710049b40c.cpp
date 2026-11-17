// CChozoWarriorAIComponent unit (tools/tu_map.py unit 733): after the base handler, picks the next attack
// (by range and facing, or at random) and moves the chosen attack's entry to the front of the other object's
// list. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/UnkSList.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CCharacterMovement.h"
#include "game/UnkTU2702.h"
#include "types.h"

extern "C" int rand();

// As src/game/CAIComponent/CAIComponent_ReachRadius.cpp defines it.
struct UnkAIReach {
    f32 radius;
    f32 field_0x4;

    UnkAIReach() : radius(0.0f), field_0x4(0.0f) {}
    UnkAIReach(const UnkAIReach& other);
};

struct UnkChozoWarriorTuning_l49b40c {
    u8 pad_0x0[0x2c];
    f32 chance;  // 0x2c
};

extern UnkChozoWarriorTuning_l49b40c g_unk_71726d05a0;

struct UnkChozoWarriorState_l49b40c {
    u8 pad_0x0[0x8];
    u32 field_0x8;
};

u32 fn_7100ed7268(void* sounds);
UnkChozoWarriorState_l49b40c* fn_7100aba0a8();

struct UnkChozoWarriorListTag_l49b40c;
using UnkChozoWarriorList_l49b40c = base::UnkSList<UnkChozoWarriorListTag_l49b40c>;

struct UnkChozoWarriorActive_l49b40c {
    u8 pad_0x0[0x8];
    void* attack;  // 0x8
};

struct UnkChozoWarriorTarget_l49b40c {
    u8 pad_0x0[0x10];
    UnkChozoWarriorActive_l49b40c* active;  // 0x10
};

struct UnkChozoWarriorOther_l49b40c {
    u8 pad_0x0[0x20];
    UnkChozoWarriorTarget_l49b40c* target;  // 0x20
    UnkChozoWarriorList_l49b40c list;       // 0x28
};

struct UnkChozoWarriorOwner_l49b40c {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x1f8 - 0x78];
    void* tracked;  // 0x1f8
};

struct UnkChozoWarriorAI_l49b40c;

struct UnkChozoWarriorAIVtbl_l49b40c {
    u8 pad_0x0[0x3c8];
    f32 (*vf_0x3c8)(UnkChozoWarriorAI_l49b40c* self);
};

struct UnkChozoWarriorAttack_l49b40c {
    u8 pad_0x0[0x50];
};

struct UnkChozoWarriorAI_l49b40c {
    UnkChozoWarriorAIVtbl_l49b40c* vtable;
    u8 pad_0x8[0x28];
    UnkChozoWarriorOwner_l49b40c* owner;  // 0x30
    u8 pad_0x38[0x1128 - 0x38];
    f32 field_0x1128;
    u8 pad_0x112c[0x14a78 - 0x112c];
    u8 entry_0x14a78[0x68];
    UnkChozoWarriorAttack_l49b40c attack_0x14ae0;
    UnkChozoWarriorAttack_l49b40c* current;  // 0x14b30
};
static_assert(__builtin_offsetof(UnkChozoWarriorAI_l49b40c, current) == 0x14b30, "layout");

static inline f32 Random01() {
    return (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
}

static inline f32 RandomRange(f32 min, f32 max) {
    return min + Random01() * (max - min);
}

static inline s32 IndexOf(UnkChozoWarriorList_l49b40c* list, u64 v) {
    u32 i = 0;
    for (UnkChozoWarriorList_l49b40c::Node* n = list->head; n; n = n->next, ++i)
        if (n->value == v)
            return static_cast<s32>(i);
    return -1;
}

static inline UnkChozoWarriorList_l49b40c::Node* Find(UnkChozoWarriorList_l49b40c* list, u64 v) {
    for (UnkChozoWarriorList_l49b40c::Node* n = list->head; n; n = n->next)
        if (n->value == v)
            return n;
    return nullptr;
}

static inline void Erase(UnkChozoWarriorList_l49b40c* list, UnkChozoWarriorList_l49b40c::Node* pos) {
    UnkChozoWarriorList_l49b40c::Node* prev = nullptr;
    for (UnkChozoWarriorList_l49b40c::Node* it = list->head; it;) {
        UnkChozoWarriorList_l49b40c::Node* next = it->next;
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

static inline void Remove(UnkChozoWarriorList_l49b40c* list, u64 v) {
    if (UnkChozoWarriorList_l49b40c::Node* n = Find(list, v))
        Erase(list, n);
}

// The near and far ends of the attack's reach along the facing axis.
static inline f32 Back(UnkChozoWarriorAI_l49b40c* self) {
    f32 base;
    fn_71010fc31c(reinterpret_cast<UnkChozoWarriorAI_b66a2*>(self), &base);
    UnkAIReach reach = fn_7100977b90(self->owner->tracked);
    f32 offset = self->vtable->vf_0x3c8(self) - reach.radius * 0.5f;
    return base - offset;
}

static inline f32 Front(UnkChozoWarriorAI_l49b40c* self) {
    f32 base;
    fn_71010fc284(reinterpret_cast<UnkChozoWarriorAI_b66a2*>(self), &base);
    UnkAIReach reach = fn_7100977b90(self->owner->tracked);
    f32 offset = self->vtable->vf_0x3c8(self) - reach.radius * 0.5f;
    return base + offset;
}

void fn_710049b40c(UnkChozoWarriorAI_l49b40c* self, UnkChozoWarriorOther_l49b40c* other, u32 id) {
    fn_710110bb4c(reinterpret_cast<UnkQuetzoaXAI_ovb0134*>(self), reinterpret_cast<UnkQuetzoaOther_ovb0134*>(other),
                  id, false);
    UnkChozoWarriorList_l49b40c* list = &other->list;
    if (!self->current) {
        u64 entry = reinterpret_cast<u64>(self->entry_0x14a78);
        if (IndexOf(list, entry) >= 0) {
          f32 roll = Random01();
          if (g_unk_71726d05a0.chance > roll) {
            f32 x = self->owner->transform.m[3];
            f32 back = Back(self);
            f32 front = Front(self);
            if ((fn_710002614c(&self->owner->transform).m[2] >= 0.0f && front - x < x - back) ||
                (fn_710002614c(&self->owner->transform).m[2] < 0.0f && x - back < front - x))
                Remove(list, entry);
          }
        }
        f32 f = self->field_0x1128;
        if ((f > -1.0f && f < 1.0f) || fn_7100ed7268(g_unk_71726af0b0) != 0 || fn_7100aba0a8()->field_0x8 != 0) {
            UnkChozoWarriorActive_l49b40c* active = other->target->active;
            if (!(active && active->attack == &self->attack_0x14ae0) &&
                IndexOf(list, reinterpret_cast<u64>(&self->attack_0x14ae0)) >= 0 && RandomRange(0.0f, 1.0f) > 0.5f)
                self->current = &self->attack_0x14ae0;
        }
    }
    if (self->current) {
        u64 current = reinterpret_cast<u64>(self->current);
        if (IndexOf(list, current) >= 0) {
            Remove(list, current);
            list->PushFront(current);
        }
        self->current = nullptr;
    }
}
