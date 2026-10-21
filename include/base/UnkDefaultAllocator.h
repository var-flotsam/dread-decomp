#pragma once

// The default allocator (the one fn_7100083990 returns and operator new allocates through):
// one object, created on first use by fn_7100083870 and named "Default", that hands each request
// to one of four 0x38-byte sub-allocators held in place. Names and layouts are placeholders
// inferred from the code; only the slots used so far are named.

#include <new>

#include <nn/os.h>

#include "types.h"

// Mutex helpers shared by the engine (recursive init, finalize, lock, unlock).
bool fn_71000668f8(nn::os::MutexType* mutex);
void fn_7100066918(nn::os::MutexType* mutex);
void fn_710006691c(nn::os::MutexType* mutex);
void fn_7100066920(nn::os::MutexType* mutex);

namespace base {

// One of the default allocator's sub-allocators (vtable via GOT 0x7101cbc2c0): a heap over a
// fixed range, probably a dlmalloc mspace (fn_71000815b0 creates it over the range; its
// neighbours look like dlmalloc's).
struct UnkSubAllocator {
    UnkSubAllocator();   // fn_7100081304
    ~UnkSubAllocator();  // fn_7100081328

    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(bool value);
    virtual void vf_0x28();
    virtual void vf_0x30(u64 value);
    virtual void vf_0x38();
    virtual void* vf_0x40(u64 size, u64 align, u32 flags);
    virtual void vf_0x48(void* ptr);
    virtual u64 vf_0x50(void* ptr);   // the usable size of an allocation
    virtual bool vf_0x58(void* ptr);  // whether ptr is one of its blocks
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual u32 vf_0x88();
    virtual u64 vf_0x90();  // the free size (an upper bound for the largest block)
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8(void* ptr);  // called before ptr is freed

    void* space = nullptr;  // +0x8, from fn_71000815b0
    u64 size = 0;
    char* begin = nullptr;
    char* end = nullptr;
    s64 field_0x28 = -1;
    bool ready = false;  // +0x30, set once space is created
    bool field_0x31 = false;
};

// Its slots 0x40 and 0x48 are the ones UnkAllocator names: the default allocator is what
// fn_7100083990 hands out as an UnkAllocator. Vtable at 0x7101a1ac28 (via GOT 0x7101cbc2c8):
// 0x0/0x8 fn_7100082cc8/fn_7100082d1c (destructors), 0x10 fn_7100082d78, 0x40 fn_71000831a8,
// 0x48 fn_71000832d8, 0x50 fn_7100082ea0, 0x58 fn_71000835a0, 0x88 fn_7100083374.
struct UnkDefaultAllocator {
    UnkDefaultAllocator();   // fn_7100082c38
    ~UnkDefaultAllocator();  // fn_7100082cc8

    virtual void vf_0x0();
    virtual void vf_0x8();
    // Hands it the memory it carves its sub-allocators from.
    virtual void vf_0x10(void* begin, void* end, const char* name, u64 arg);
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void* vf_0x40(u64 size, u64 align, u32 flags);
    virtual void vf_0x48(void* ptr);
    virtual u64 vf_0x50(void* ptr);  // the usable size of an allocation

    UnkSubAllocator* subs[4];  // +0x8
    u64 field_0x28;
    s32 last = -1;  // index of the last sub-allocator in use
    nn::os::MutexType mutex;  // +0x38
    UnkSubAllocator sub0;  // +0x58, "Main"
    UnkSubAllocator sub1;  // "Secondary"
    UnkSubAllocator sub2;  // "Extra"
    UnkSubAllocator sub3;  // "Pack"
};

}  // namespace base

// The heap behind a sub-allocator, apparently dlmalloc's mspace functions.
void* fn_71000815b0(void* base, u64 capacity, int locked);  // create_mspace_with_base
void* fn_7100081860(void* space, u64 size);                 // mspace_malloc
void fn_7100082370(void* space, void* ptr);                 // mspace_free
void* fn_71000829f0(void* space, u64 align, u64 size);      // mspace_memalign

// The sub-allocator's own functions that the default allocator calls directly.
void fn_7100081330(base::UnkSubAllocator* self, char* begin, char* end, const char* name, u64 arg);  // set up over [begin, end)
void fn_7100081380(base::UnkSubAllocator* self, bool value);  // sets field_0x31

// Lock, forward to sub-allocator `index` (0 to 3), unlock.
void fn_71000830f0(base::UnkDefaultAllocator* self, u32 index, bool value);  // slot 0x20
void fn_710008314c(base::UnkDefaultAllocator* self, u32 index, u64 value);   // slot 0x30
