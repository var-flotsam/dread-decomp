// The rumble manager's unit (tools/tu_map.py unit 51): splits a string at a separator into a list of
// strings (base/UnkNodeOwner.h's UnkStrList30), first emptying the list and, when its node pool is too
// small for the parts, rebuilding the pool for them. Names and layouts are placeholders.
#include <string.h>

#include "base/UnkNodeOwner.h"
#include "base/global/CRntStringAssign.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;

using UnkSetupStrList = UnkStrList30;
using UnkStrNode_l0730a0 = UnkListNode<0x30>;

// The allocation flags the list allocates its nodes with (the word at +0x28).
static inline u32& NodeFlags(UnkSetupStrList* list) {
    return *reinterpret_cast<u32*>(list->pad_0x28);
}

// Makes the node pool hold `count` nodes, taking a new buffer when the one it has is too small.
inline void ReservePool(UnkSetupStrList* list, u32 count) {
    if (list->field_0x40.size / sizeof(UnkStrNode_l0730a0) >= count)
        return;
    base::IAllocator* allocator = list->field_0x38 ? list->field_0x38 : list->field_0x20;
    u32 flags = NodeFlags(list);
    if (void* block = list->field_0x40.buffer) {
        fn_71000837f4(&list->field_0x40);
        list->field_0x38->vf_0x48(block);
        list->field_0x38 = nullptr;
    }
    if (count) {
        if (!allocator)
            allocator = GetDefaultAllocator();
        list->field_0x38 = allocator;
        u64 size = (u64)count * sizeof(UnkStrNode_l0730a0);
        void* buffer = allocator->vf_0x40(size, 8, flags);
        fn_710008378c(&list->field_0x40, buffer, size, sizeof(UnkStrNode_l0730a0));
    }
    list->field_0x20 = reinterpret_cast<base::IAllocator*>(&list->field_0x30);
}

void fn_71000730a0(const char* str, const char* separators, UnkSetupStrList* out) {
    out->FreeNodes();
    out->field_0x8 = 0;
    out->field_0x10 = nullptr;
    out->field_0x18 = nullptr;

    u32 length = strlen(str);
    u32 separator_length = strlen(separators);

    // One part more than there are separators.
    u32 count = 0;
    if (str) {
        const char* p = str;
        do {
            const char* found = strstr(p, separators);
            ++count;
            p = found ? found + separator_length : nullptr;
        } while (p);
    }

    ReservePool(out, count);

    if (!length)
        return;
    u32 pos = 0;
    do {
        const char* end = strstr(str, separators);
        if (!end) {
            u64 rest = strlen(str);
            if (!rest)
                return;
            end = str + rest;
        }
        auto* node = static_cast<UnkStrNode_l0730a0*>(out->field_0x20->vf_0x40(sizeof(UnkStrNode_l0730a0), 8, NodeFlags(out)));
        if (node) {
            fn_710007e6c4(node);
            node->next = nullptr;
        }
        if (!out->field_0x8) {
            out->field_0x10 = node;
            out->field_0x18 = node;
            out->field_0x8 = 1;
        } else {
            out->field_0x18->next = node;
            node->next = nullptr;
            out->field_0x18 = node;
            ++out->field_0x8;
        }
        u32 part_length = end - str;
        CRntString_AssignPrefix(reinterpret_cast<CRntString*>(node), str, part_length);
        u32 step = part_length + separator_length;
        pos += step;
        str += step;
    } while (pos < length);
}
