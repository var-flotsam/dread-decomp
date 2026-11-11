#include "base/UnkBlockFreeList.h"
#include "base/UnkVector.h"
#include "base/input/UnkRumbleManager.h"
#include "types.h"

using base::input::UnkPooledChainedMap;

extern char g_unk_7101a1e2a8[];  // vtable
void fn_710019e58c(UnkPooledChainedMap* map, u32 buckets, u32 nodes);

struct UnkMapOwner_b260 {
    char* vtable;
    UnkPooledChainedMap map;        // 0x8
    base::UnkVector<void*> list;    // 0x90
};
static_assert(__builtin_offsetof(UnkMapOwner_b260, list) == 0x90, "layout");

void fn_710019dcac(UnkMapOwner_b260* self) {
    typedef u64 __attribute__((may_alias)) word;
    self->vtable = g_unk_7101a1e2a8 + 0x10;
    UnkPooledChainedMap* m = &self->map;
    m->alloc = fn_7100083990();
    m->flags = 0xe;
    char* mb = reinterpret_cast<char*>(m);
    *reinterpret_cast<word*>(mb + 0xc) = 0;
    *reinterpret_cast<word*>(mb + 0x14) = 0;
    *reinterpret_cast<u32*>(mb + 0x1c) = 0;
    *reinterpret_cast<u32*>(mb + 0x20) = 10;  // the growth step
    m->field_0x24 = true;
    m->field_0x28 = 0xe;
    base::UnkAllocator* a = fn_7100083990();
    m->field_0x38 = 0;
    m->field_0x40 = 0;
    m->alloc_0x30 = a;
    m->field_0x50 = 0;
    m->pool_vtable = g_unk_7101a1a4c8 + 0x10;
    fn_710008377c(&m->pool);
    m->field_0x80 = 0;
    m->field_0x84 = false;
    fn_710019e58c(m, 0x400, 0x4000);
    new (&self->list) base::UnkVector<void*>(0xf);
    self->list.Reserve(0x1000);
    if (*reinterpret_cast<u32*>(mb + 0xc)) {
        char* node = reinterpret_cast<char*>(m->field_0x40);
        while (node) {
            char* next = *reinterpret_cast<char**>(node + 0x10);
            m->alloc->vf_0x48(node);
            node = next;
        }
        __builtin_memset(*reinterpret_cast<void**>(mb + 0x10), 0, *reinterpret_cast<u32*>(mb + 0x1c) * u64(8));
        *reinterpret_cast<u32*>(mb + 0xc) = 0;
        m->field_0x38 = 0;
        m->field_0x40 = 0;
    }
}
