// Switches the object's current source: unregisters its listener from every entry of the old source,
// registers it with every entry of the new one (skipping entries that already have it), then refreshes.
#include "types.h"

struct UnkAllocator {
    virtual void vf0();
    virtual void vf8();
    virtual void vf10();
    virtual void vf18();
    virtual void vf20();
    virtual void vf28();
    virtual void vf30();
    virtual void vf38();
    virtual void* Alloc(u64 size, u64 align, u32 flags);
    virtual void Free(void* p);
};

template <class T>
struct UnkList {
    struct Node {
        T value;
        Node* next;
    };
    u32 count;
    Node* head;
    Node* tail;
    UnkAllocator* allocator;
    u32 allocFlags;

    s32 IndexOf(const T& v) const {
        u32 i = 0;
        for (Node* n = head; n; n = n->next, ++i)
            if (n->value == v)
                return i;
        return -1;
    }

    bool Contains(const T& v) const { return IndexOf(v) >= 0; }

    void Remove(const T& v) {
        if (!Contains(v))
            return;
        Node* node = head;
        while (node && node->value != v)
            node = node->next;
        if (!node)
            return;
        Node* prev = nullptr;
        Node* cur = nullptr;
        Node* it = head;
        while (it) {
            prev = cur;
            cur = it;
            it = it->next;
            Node* next = it;
            if (cur == node) {
                if (prev) {
                    prev->next = next;
                    if (tail == node)
                        tail = prev;
                } else {
                    head = next;
                }
                if (--count == 0)
                    head = tail = nullptr;
                allocator->Free(node);
                return;
            }
        }
    }

    void PushBack(const T& v) {
        Node* n = static_cast<Node*>(allocator->Alloc(sizeof(Node), 8, allocFlags));
        if (!n)
            return;
        n->value = v;
        n->next = nullptr;
        if (count == 0) {
            head = n;
            tail = n;
            count = 1;
        } else {
            __builtin_memcpy(&tail->next, &n, sizeof(n));
            n->next = nullptr;
            tail = n;
            ++count;
        }
    }
};

struct UnkListener;

struct UnkEntry {
    char pad_0x0[0x130];
    UnkList<UnkListener*> field_0x130;
};

struct UnkSource {
    virtual void vf0();
    virtual void vf8();
    virtual void vf10();
    virtual void vf18();
    virtual UnkEntry* vf20(u32 index);
    char pad_0x8[0x30];
    u32 field_0x38;
    char pad_0x3c[0x10];
    u32 field_0x4c;
};

struct UnkListener {
    char pad_0x0[0x8];
};

struct UnkOwner {
    char pad_0x0[0x40];
    UnkListener field_0x40;
    char pad_0x48[0x138];
    UnkSource* field_0x180;
};

void fn_71009c6220(UnkSource* src);
void fn_71009c5d6c(UnkSource* src);
void fn_71009bffdc(UnkOwner* self, bool arg);

// NON_MATCHING(minor): the loop that finds the node's predecessor rotates its registers differently
// (one extra move per iteration); same loads, stores and calls.
void fn_71009bfda8(UnkOwner* self, UnkSource* src) {
    if (src && src->field_0x38 == 0)
        return;
    UnkSource* old = self->field_0x180;
    self->field_0x180 = src;
    if (old) {
        fn_71009c6220(old);
        for (u32 i = 0; i < old->field_0x4c; ++i) {
            UnkEntry* e = old->vf20(i);
            if (e)
                e->field_0x130.Remove(&self->field_0x40);
        }
    }
    if (self->field_0x180) {
        fn_71009c5d6c(self->field_0x180);
        for (u32 i = 0; i < self->field_0x180->field_0x4c; ++i) {
            UnkEntry* e = self->field_0x180->vf20(i);
            if (e && !e->field_0x130.Contains(&self->field_0x40))
                e->field_0x130.PushBack(&self->field_0x40);
        }
    }
    fn_71009bffdc(self, false);
}
