// CAIComponent unit (tools/tu_map.py unit 2702): reorders a list of candidate attacks (at least two) for
// `id`: drops the ones with no entry for it, moves one picked at random by weight to the front, drops the
// ones the recent-attack history has repeated too often, sends a boss's overused attack to the back and
// brings forward one not used for long enough. Names and layouts are placeholders.
#include <stdlib.h>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCharClassAttack.h"
#include "types.h"

using base::global::CStrId;

struct UnkCancelAttack_ovb0128;
void* CAttack_GetPreset(UnkCancelAttack_ovb0128*, u32);

struct UnkAttackList_l10bc0c;
struct UnkAttackNode_l10bc0c;

// What an attack defines.
struct UnkAttackDef_l10bc0c {
    u8 pad_0x0[0xa4];
    s32 repeat_limit;  // 0xa4: how many times in a row it may come (the caller's default when not positive)
    s32 min_gap;       // 0xa8: how many other attacks must come between (likewise)
    u8 pad_0xac[0x1c];
    CStrId group;  // 0xc8
};

struct UnkAttack_l10bc0c {
    u8 pad_0x0[0x20];
    UnkAttackDef_l10bc0c* def;  // 0x20
};

// An attack's entry for an id: its weight.
struct UnkAttackEntry_l10bc0c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual f32 Weight(UnkAttackList_l10bc0c* list);
};

// A boss's limit for an attack: at most `max_uses` among the last `window` attacks.
struct UnkAttackUse_l10bc0c {
    u8 pad_0x0[0x48];
    u32 max_uses;
    u32 window;
};

// The attacks done so far, newest first.
struct UnkAttackHistory_l10bc0c {
    u8 pad_0x0[8];
    UnkAttack_l10bc0c* attack;
    u8 pad_0x10[8];
    UnkAttackHistory_l10bc0c* next;
};

struct UnkAttackOwner_l10bc0c {
    u8 pad_0x0[0x10];
    UnkAttackHistory_l10bc0c* history;
};

struct UnkAttackSource_l10bc0c {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkAttackNode_l10bc0c {
    UnkAttack_l10bc0c* value;
    UnkAttackNode_l10bc0c* next;
};

// A singly linked list whose nodes come from its allocator.
struct UnkAttackNodeList_l10bc0c {
    u32 count;
    UnkAttackNode_l10bc0c* head;    // 0x8
    UnkAttackNode_l10bc0c* tail;    // 0x10
    base::IAllocator* allocator;  // 0x18
    u32 flags;                      // 0x20

    // Unlinks and frees `node`; returns the node after it (null when `node` is not in the list).
    UnkAttackNode_l10bc0c* Erase(UnkAttackNode_l10bc0c* node) {
        UnkAttackNode_l10bc0c* prev = nullptr;
        UnkAttackNode_l10bc0c* it = head;
        UnkAttackNode_l10bc0c* next;
        do {
            if (!it)
                return nullptr;
            UnkAttackNode_l10bc0c* cur = it;
            next = it->next;
            if (cur == node)
                break;
            prev = cur;
            it = next;
        } while (true);
        if (prev) {
            prev->next = next;
            if (tail == node)
                tail = prev;
        } else {
            head = next;
        }
        if (--count == 0) {
            head = nullptr;
            tail = nullptr;
        }
        allocator->vf_0x48(node);
        return next;
    }
    void Remove(UnkAttack_l10bc0c* value) {
        for (UnkAttackNode_l10bc0c* node = head; node; node = node->next) {
            if (node->value == value) {
                Erase(node);
                break;
            }
        }
    }
    UnkAttackNode_l10bc0c* NewNode(UnkAttack_l10bc0c* value) {
        UnkAttackNode_l10bc0c* node =
            static_cast<UnkAttackNode_l10bc0c*>(allocator->vf_0x40(sizeof(UnkAttackNode_l10bc0c), 8, flags));
        if (node) {
            node->value = value;
            node->next = nullptr;
        }
        return node;
    }
    void PushFront(UnkAttack_l10bc0c* value) {
        if (UnkAttackNode_l10bc0c* node = NewNode(value)) {
            if (count) {
                node->next = head;
                head = node;
                ++count;
            } else {
                head = node;
                tail = node;
                count = 1;
            }
        }
    }
    void PushBack(UnkAttack_l10bc0c* value) {
        if (UnkAttackNode_l10bc0c* node = NewNode(value)) {
            if (count) {
                tail->next = node;
                node->next = nullptr;
                ++count;
                tail = node;
            } else {
                head = node;
                tail = node;
                count = 1;
            }
        }
    }
};

struct UnkAttackList_l10bc0c {
    UnkAttackSource_l10bc0c* source;
    u8 pad_0x8[0x18];
    UnkAttackOwner_l10bc0c* owner;  // 0x20
    u8 pad_0x28[8];
    UnkAttackNodeList_l10bc0c attacks;  // 0x30
};

struct UnkBossData_l10bc0c {
    virtual const base::reflection::CType* GetType();
    u8 pad_0x8[0x504];
    bool field_0x50c;
};

struct UnkAIComponent_l10bc0c {
    virtual const base::reflection::CType* GetType();
    u8 pad_0x8[0x1608];
    UnkBossData_l10bc0c* boss;  // 0x1610
};

static inline UnkAttackEntry_l10bc0c* EntryFor(UnkAttack_l10bc0c* attack, u32 id) {
    return static_cast<UnkAttackEntry_l10bc0c*>(CAttack_GetPreset(reinterpret_cast<UnkCancelAttack_ovb0128*>(attack), id));
}

// Whether two history entries count as the same attack: the same one, or both in one named group.
static inline bool SameAttack(UnkAttack_l10bc0c* a, UnkAttack_l10bc0c* b) {
    if (a == b)
        return true;
    return a->def->group.data != &g_unk_7101d47108->empty && a->def->group.data == b->def->group.data;
}

static inline f32 RandomRange(f32 min, f32 max) {
    return (max - min) * (f32)((f64)(rand() & 0x7fff) * (1.0 / 32767.0)) + min;
}

void fn_710110bc0c(void* component, void* arg, u32 id, u32 kind, s32 amount) {
    UnkAIComponent_l10bc0c* self = static_cast<UnkAIComponent_l10bc0c*>(component);
    UnkAttackList_l10bc0c* list = static_cast<UnkAttackList_l10bc0c*>(arg);
    if (!list->source || !list->source->field_0x10)
        return;
    UnkAttackNodeList_l10bc0c& attacks = list->attacks;
    if (attacks.count < 2)
        return;

    // The attacks with an entry for `id`, and their total weight.
    f32 total = 0.0f;
    for (UnkAttackNode_l10bc0c* node = attacks.head; node;) {
        UnkAttackEntry_l10bc0c* entry = node->value ? EntryFor(node->value, id) : nullptr;
        UnkAttackNode_l10bc0c* next;
        if (entry) {
            total += entry->Weight(list);
            next = node->next;
        } else {
            next = attacks.Erase(node);
        }
        node = next;
    }

    // One picked at random by weight goes first.
    f32 pick = RandomRange(0.0f, total);
    f32 sum = 0.0f;
    for (UnkAttackNode_l10bc0c* node = attacks.head; node;) {
        UnkAttack_l10bc0c* attack = node->value;
        UnkAttackEntry_l10bc0c* entry = attack ? EntryFor(attack, id) : nullptr;
        if (!entry) {
            node = attacks.Erase(node);
            continue;
        }
        f32 weight = entry->Weight(list);
        if (!(weight > 0.0f)) {
            node = attacks.Erase(node);
            continue;
        }
        sum += weight;
        if (sum > pick) {
            attacks.Erase(node);
            attacks.PushFront(attack);
            break;
        }
        node = node->next;
    }

    // Drop the ones done too many times in a row.
    for (UnkAttackNode_l10bc0c* node = attacks.head; node;) {
        UnkAttack_l10bc0c* attack = node->value;
        s32 limit = attack->def->repeat_limit > 0 ? attack->def->repeat_limit : (s32)kind;
        s32 repeats = 0;
        if (limit >= 1) {
            for (UnkAttackHistory_l10bc0c* h = list->owner->history; h; h = h->next) {
                if (!SameAttack(h->attack, attack))
                    break;
                if (++repeats >= limit)
                    break;
            }
        }
        if (repeats < limit)
            node = node->next;
        else
            node = attacks.Erase(node);
    }

    // A boss sends an attack it has used too often lately to the back.
    if (self && fn_710009d50c(self->GetType(), fn_71003ab524())) {
        UnkBossData_l10bc0c* boss = self->boss;
        UnkBossData_l10bc0c* data = fn_710009d50c(boss->GetType(), fn_71004186ec()) ? boss : nullptr;
        if (!data->field_0x50c && attacks.count != 0) {
            UnkAttackNode_l10bc0c* node = attacks.head;
            for (u32 i = 0; i < attacks.count; ++i, node = node->next) {
                UnkAttack_l10bc0c* attack = node->value;
                UnkAttackUse_l10bc0c* use = static_cast<UnkAttackUse_l10bc0c*>(
                    fn_71003646b0(reinterpret_cast<UnkAttackTarget_A2_b68*>(attack->def), id));
                if (!attack || !use || !use->window)
                    continue;
                u32 uses = 0;
                u32 left = use->window;
                for (UnkAttackHistory_l10bc0c* h = list->owner->history; h; h = h->next) {
                    if (h->attack == attack)
                        ++uses;
                    if (--left == 0)
                        break;
                }
                if (uses >= use->max_uses) {
                    attacks.Remove(attack);
                    attacks.PushBack(attack);
                    break;
                }
            }
        }
    }

    // One not done for long enough comes first.
    for (UnkAttackNode_l10bc0c* node = attacks.head; node; node = node->next) {
        UnkAttack_l10bc0c* attack = node->value;
        if (!attack)
            continue;
        s32 gap = 0;
        for (UnkAttackHistory_l10bc0c* h = list->owner->history; h && !SameAttack(h->attack, attack); h = h->next)
            ++gap;
        s32 min_gap = attack->def->min_gap > 0 ? attack->def->min_gap : amount;
        if (gap >= min_gap) {
            attacks.Remove(attack);
            attacks.PushFront(attack);
            return;
        }
    }
}
