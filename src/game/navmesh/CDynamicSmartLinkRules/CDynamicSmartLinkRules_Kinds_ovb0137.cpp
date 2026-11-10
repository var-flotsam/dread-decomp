// navmesh::CDynamicSmartLinkRules's unit (tools/tu_map.py): the smart-link kinds parsed from text (as src/game/CCharClassAIComponent/CCharClassAIComponent_DoorKinds_ovb0113.cpp parses the door kinds).
// Names and layouts are placeholders.
#include <new>

#include "base/UnkAtExit.h"
#include "base/UnkBlockFreeList.h"
#include "base/UnkNodeOwner.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "types.h"

using base::global::CRntString;

using UnkSetupStrList = UnkStrList30;
void fn_71000730a0(const char* str, const char* separators, UnkSetupStrList* out);
extern void* g_unk_7101a1a070[];

// The list the rules text is split into, kept between calls (a guarded static, destroyed by
// fn_71000732d4).
struct UnkSmartLinkParts_ovb0137 {
    void** vtable;
    u32 count;  // 0x8
    u32 pad_0xc;
    UnkListNode<0x30>* head;  // 0x10
    UnkListNode<0x30>* tail;  // 0x18
    base::UnkAllocator* allocator;  // 0x20
    u32 field_0x28;
    u32 pad_0x2c;
    void* field_0x30;
    base::UnkAllocator* field_0x38;
    UnkBlockFreeList field_0x40;
    u32 field_0x68;
    u8 field_0x6c;

    UnkSmartLinkParts_ovb0137() {
        vtable = g_unk_7101a1a070 + 2;
        count = 0;
        head = nullptr;
        tail = nullptr;
        allocator = fn_7100083990();
        field_0x28 = 4;
        vtable = g_unk_7101a1a4a8 + 2;
        field_0x30 = g_unk_7101a1a4c8 + 0x10;
        field_0x38 = nullptr;
        fn_710008377c(&field_0x40);
        field_0x68 = 0;
        field_0x6c = 0;
    }
};

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);
extern UnkSmartLinkParts_ovb0137 g_unk_71726a37e0;  // the list
extern u64 g_unk_71726a3850;                        // its guard

static inline UnkSmartLinkParts_ovb0137& SmartLinkParts_ovb0137() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726a3850), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726a3850)) {
            new (&g_unk_71726a37e0) UnkSmartLinkParts_ovb0137();
            __cxa_atexit(reinterpret_cast<void (*)(void*)>(fn_71000732d4), &g_unk_71726a37e0, g_unk_7101a19000);
            __cxa_guard_release(&g_unk_71726a3850);
        }
    }
    return g_unk_71726a37e0;
}

static inline s32 SmartLinkKind(const CRntString* part) {
    if (fn_710007f140(part, "Tunnel"))
        return 1;
    if (fn_710007f140(part, "Water"))
        return 2;
    return fn_710007f140(part, "All") ? -1 : 0;
}

// The smart-link kinds named in `text` (split at "|"): "Tunnel" is bit 0, "Water" bit 1, "All" every bit.
s32 fn_710026d2e4(const char* text) {
    UnkSmartLinkParts_ovb0137& parts = SmartLinkParts_ovb0137();
    for (UnkListNode<0x30>* node = parts.head; node;) {
        UnkListNode<0x30>* next = node->next;
        fn_710007ecd4(reinterpret_cast<CRntString*>(node));
        parts.allocator->vf_0x48(node);
        node = next;
    }
    parts.count = 0;
    parts.head = nullptr;
    parts.tail = nullptr;
    fn_71000730a0(text, "|", reinterpret_cast<UnkSetupStrList*>(&parts));
    s32 kinds = 0;
    for (UnkListNode<0x30>* node = parts.head; node; node = node->next) {
        const CRntString* part = reinterpret_cast<const CRntString*>(node);
        if (fn_710007f140(part, "All"))
            kinds = -1;
        else
            kinds |= SmartLinkKind(part);
    }
    return kinds;
}
