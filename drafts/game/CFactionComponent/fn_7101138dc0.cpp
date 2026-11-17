// CFactionComponent's destructor: takes the component out of its faction's list (by its kind at +0x70, as
// CFactionComponent_Register_ovb0139.cpp adds it) and out of the list of all, destroys its name and runs the
// component base's destructor. Names and layouts are placeholders.
#include "base/UnkSList.h"
#include "base/global/CRntString.h"
#include "game/UnkComponent.h"
#include "types.h"

using base::global::CRntString;

struct UnkFactionListTag_ovb0139;
using UnkFactionList_ovb0139 = base::UnkSList<UnkFactionListTag_ovb0139>;
using UnkFactionNode_l138dc0 = UnkFactionList_ovb0139::Node;

extern UnkFactionList_ovb0139 g_unk_7172a08ad8;  // players
extern UnkFactionList_ovb0139 g_unk_7172a08b48;  // neutral
extern UnkFactionList_ovb0139 g_unk_7172a08bb8;  // enemies
extern UnkFactionList_ovb0139 g_unk_7172a08c28;  // all

extern char g_unk_7101cb2ee0[];  // CFactionComponent's vtable

struct UnkFactionComponent {
    char* vtable;
    u8 base_0x8[0x38];
    CRntString name;  // 0x40
    u32 field_0x70;

    void vf_0x18() { (*reinterpret_cast<void (**)(UnkFactionComponent*)>(vtable + 0x18))(this); }
};
static_assert(__builtin_offsetof(UnkFactionComponent, field_0x70) == 0x70, "UnkFactionComponent layout");

static inline UnkFactionNode_l138dc0* Find(UnkFactionList_ovb0139* list, u64 value) {
    for (UnkFactionNode_l138dc0* n = list->head; n; n = n->next) {
        if (n->value == value)
            return n;
    }
    return nullptr;
}

static inline void Erase(UnkFactionList_ovb0139* list, UnkFactionNode_l138dc0* node) {
    UnkFactionNode_l138dc0* prev = nullptr;
    UnkFactionNode_l138dc0* it = list->head;
    while (it) {
        UnkFactionNode_l138dc0* cur = it;
        UnkFactionNode_l138dc0* next = it->next;
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

static inline void Remove(UnkFactionList_ovb0139* list, UnkFactionComponent* obj) {
    if (UnkFactionNode_l138dc0* node = Find(list, reinterpret_cast<u64>(obj)))
        Erase(list, node);
}

// The counterpart of fn_710113915c (CFactionComponent_Register_ovb0139.cpp).
static inline void Unregister(UnkFactionComponent* self) {
    s32 kind = self->field_0x70;
    if (kind == 0 || kind > 7)
        return;
    switch (kind) {
    case 1:
        Remove(&g_unk_7172a08ad8, self);
        break;
    case 2:
        Remove(&g_unk_7172a08b48, self);
        break;
    case 4:
        Remove(&g_unk_7172a08bb8, self);
        break;
    }
    Remove(&g_unk_7172a08c28, self);
}

void fn_7101138dc0(UnkFactionComponent* self) {
    self->vtable = g_unk_7101cb2ee0 + 0x10;
    Unregister(self);
    self->name.~CRntString();
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}
