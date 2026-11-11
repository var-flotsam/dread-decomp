// From unit 2703 of tools/tu_map.py (CSpawnGroupComponent): fills a temporary list (vtable
// g_unk_7101a32680, nodes from the default allocator with flags 0x13) through fn_710111b230 and
// frees it again. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkTU604.h"
#include "types.h"

struct UnkList_71011199cc {
    struct Node {
        u64 value;
        Node* next;
    };
    char* vtable;
    u32 count;
    Node* head;
    Node* tail;
    base::UnkAllocator* allocator;
    u32 flags;

    UnkList_71011199cc() {
        vtable = reinterpret_cast<char*>(g_unk_7101a32680) + 0x10;
        count = 0;
        head = nullptr;
        tail = nullptr;
        allocator = fn_7100083990();
        flags = 0x13;
    }
    ~UnkList_71011199cc() {
        vtable = reinterpret_cast<char*>(g_unk_7101a32680) + 0x10;
        Node* n = head;
        if (!n)
            return;
        base::UnkAllocator* a = allocator;
        while (n) {
            Node* next = n->next;
            a->vf_0x48(n);
            n = next;
        }
    }
};

struct UnkComponent_71011199cc;
void fn_710111b230(UnkComponent_71011199cc* self, UnkList_71011199cc* out);

void fn_71011199cc(UnkComponent_71011199cc* self) {
    UnkList_71011199cc list;
    fn_710111b230(self, &list);
}
