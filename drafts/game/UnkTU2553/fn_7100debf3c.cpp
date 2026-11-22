// Unit 2553 (CGameManager::CTunableGameManager): walks the manager's list of objects and, for each one
// whose type is not the "MainCamera" one, calls its vf_0xe0 and unlinks it. Names and layouts are
// placeholders.
#include "base/UnkNamedRegistry.h"
#include "base/global/CRntVector.h"
#include "types.h"

struct UnkTU2553TypeInfo {
    u8 pad_0x0[0x30];
    u64 field_0x30;
    u8 pad_0x38[0xc];
    // A union member: the original reloads the pointer to this record after each increment.
    union {
        s32 field_0x44;
    };
};

struct UnkTU2553Listed {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    u8 pad_0x8[0x28];
    UnkTU2553TypeInfo* info;
};

struct UnkTU2553Node {
    UnkTU2553Listed* value;
    UnkTU2553Node* next;
};

struct UnkTU2553Key {
    u8 pad_0x0[0x20];
    u64 field_0x20;
    u8 pad_0x28[8];
};

struct UnkTU2553Manager {
    u8 pad_0x0[0x2690];
    u32 count;
    UnkTU2553Node* head;
    UnkTU2553Node* tail;
    base::UnkAllocator* allocator;
};

// Unlinks and frees `node`; false when it is not in the list.
static bool Unlink(UnkTU2553Manager* self, UnkTU2553Node* node, UnkTU2553Node*& after) {
    UnkTU2553Node* prev = nullptr;
    UnkTU2553Node* cur = self->head;
    UnkTU2553Node* next;
    for (;;) {
        if (!cur)
            return false;
        next = cur->next;
        if (cur == node)
            break;
        prev = cur;
        cur = next;
    }
    if (prev) {
        prev->next = next;
        if (self->tail == node)
            self->tail = prev;
    } else {
        self->head = next;
    }
    if (--self->count == 0) {
        self->head = nullptr;
        self->tail = nullptr;
    }
    self->allocator->vf_0x48(node);
    after = next;
    return true;
}

void fn_7100debf3c(UnkTU2553Manager* self) {
    for (UnkTU2553Node* node = self->head; node;) {
        UnkTU2553Listed* obj = node->value;
        UnkTU2553Key key;
        CRntString_ConstructFromCString(reinterpret_cast<base::UnkNamedEntry*>(&key), "MainCamera");
        obj->info->field_0x44++;
        obj->info->field_0x44++;
        u64 id = obj->info->field_0x30;
        u64 keyId = key.field_0x20;
        CRntString_Destroy(reinterpret_cast<base::global::CRntString*>(&key));
        if (id == keyId) {
            node = node->next;
            continue;
        }
        node->value->vf_0xe0();
        if (!Unlink(self, node, node))
            return;
    }
}
