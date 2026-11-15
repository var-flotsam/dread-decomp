// nn::atk unit: removes an item from the list at +0x890 while holding the lock at +0x8a0 of the
// object fn_71001039f8 returns. The node sits at +0x40 of the item. Names and layouts are placeholders.
#include "game/UnkTU2772.h"
#include "game/UnkTU99.h"
#include "types.h"

struct UnkAtkRemLink {
    UnkAtkRemLink* prev;
    UnkAtkRemLink* next;
};

struct UnkAtkRemItem {
    u8 pad_0x0[0x40];
    UnkAtkRemLink link;
};

struct UnkAtkRemOwner {
    u8 pad_0x0[0x890];
    UnkAtkRemLink head;
};

static inline UnkAtkLock* AtkRemLock() {
    return reinterpret_cast<UnkAtkLock*>(static_cast<char*>(fn_71001039f8()) + 0x8a0);
}

void fn_7101215160(UnkAtkRemOwner* self, UnkAtkRemItem* item) {
    fn_71012392d0(AtkRemLock());
    UnkAtkRemLink* head = &self->head;
    UnkAtkRemLink* node = head->next;
    while (head != node) {
        if (reinterpret_cast<UnkAtkRemItem*>(reinterpret_cast<char*>(node) - 0x40) == item)
            break;
        node = node->next;
    }
    if (head != node) {
        UnkAtkRemLink* prev = node->prev;
        UnkAtkRemLink* next = node->next;
        UnkAtkRemLink* last = next->prev;
        prev->next = next;
        next->prev = node->prev;
        last->next = node;
        node->prev = last;
    }
    fn_71012392e0(AtkRemLock());
}
