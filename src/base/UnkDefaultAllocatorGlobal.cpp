// The default allocator's instance and the free functions around it (operator new and delete,
// which follow, allocate through it too).
#include <string.h>

#include "base/UnkDefaultAllocator.h"
#include "base/UnkVector.h"

using base::UnkDefaultAllocator;

// Defined elsewhere in the engine (reached through the GOT).
extern u64 g_unk_7101d58ea8;
extern nn::os::MutexType g_unk_7101d58e88;
extern char g_unk_7101d58eb0[];
extern char g_unk_7101d58eb8[];
void fn_7100133010(char* a, char* b);

static bool s_initialized;
// Only ever set to the function-local instance below. Clang's GlobalOpt relies on that: wherever
// the pointer is dereferenced, it reads the vtable from the instance itself, as the original does.
static UnkDefaultAllocator* s_current;

// Creates the default allocator over [mem, mem + size) the first time it is called.
void fn_7100083870(void* mem, u64 size) {
    if (s_initialized)
        return;
    char* end = (char*)mem + size;
    static UnkDefaultAllocator s_default;
    s_current = &s_default;
    s_current->vf_0x10(mem, end, "Default", g_unk_7101d58ea8);
    s_initialized = true;
    fn_71000668f8(&g_unk_7101d58e88);
    fn_7100133010(g_unk_7101d58eb0, g_unk_7101d58eb8);
}

base::UnkAllocator* fn_7100083990() {
    return reinterpret_cast<base::UnkAllocator*>(s_current);
}

void fn_71000839c0(bool current_thread) {
    fn_71000830f0(s_current, 2, false);
    fn_710008314c(s_current, 2, current_thread ? (u64)nn::os::GetCurrentThread() : (u64)-1);
}

void fn_7100083a10() {
    fn_71000830f0(s_current, 2, true);
    fn_710008314c(s_current, 2, (u64)-1);
}

// realloc: grows by copying into a new block; never shrinks.
void* fn_7100083a70(void* ptr, u64 size) {
    if (!ptr) {
        if (!size)
            return nullptr;
        return s_current->vf_0x40(size, 8, 6);
    }
    if (!size) {
        s_current->vf_0x48(ptr);
        return nullptr;
    }
    u64 old_size = s_current->vf_0x50(ptr);
    if (old_size >= size)
        return ptr;
    void* p = s_current->vf_0x40(size, 8, 6);
    memcpy(p, ptr, old_size);
    s_current->vf_0x48(ptr);
    return p;
}
