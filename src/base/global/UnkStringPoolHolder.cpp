// The string pool's constructor and destructor (base::global::UnkStringPoolHolder, the weak
// function-local static the string-id constructors create when there's no pool yet). The
// constructor builds the pool in place: the empty string, the hash set with its node allocator,
// and the empty string's node in the set; it publishes the pool in its global last. The destructor
// frees the nodes and clears the global. Layout in base/global/UnkStringPoolBody.h.
#include <string.h>

#include "base/UnkNamedRegistry.h"
#include "base/global/CStrId.h"
#include "base/global/UnkStringPool.h"
#include "base/global/UnkStringPoolBody.h"

using base::UnkNamedEntry;
using base::UnkAllocator;
using base::UnkVector;
using base::global::UnkPoolNode;
using base::global::UnkPoolSetWithNodes;
using base::global::UnkStringPool;
using base::global::UnkStringPoolBody;
using base::global::UnkStringPoolHolder;
using base::global::UnkStringPoolState;

static_assert(sizeof(UnkStringPoolHolder) == sizeof(UnkStringPoolBody), "the holder's storage is the pool body");

// Clang 9, the matching compiler, ignores the asm labels on the holder's constructor and destructor
// (base/global/CStrId.h) and emits their mangled symbols, so these aliases give the definitions the
// addresses' names; the callers keep referring to the members. A newer Clang, the port's, names the
// definitions by their labels, and an alias to a symbol it doesn't emit is an error there.
// check.py reads the NON_MATCHING note below as the constructor alias's: keep the note directly
// above the #if and the alias declaration over two lines, as they are.
// NON_MATCHING(minor): scheduling. Clang puts the move of the 0x25 flags constant one instruction
// ahead of the pre-indexed store of the empty string's heap copy; the original has it after.
#if defined(__clang__) && __clang_major__ <= 9
void fn_710007fe68(UnkStringPoolHolder*, u32, UnkStringPool**)
    __attribute__((alias("_ZN4base6global19UnkStringPoolHolderC1EjPPNS0_13UnkStringPoolE")));
void fn_7100080004(UnkStringPoolHolder*)
    __attribute__((alias("_ZN4base6global19UnkStringPoolHolderD1Ev")));
#endif

// The constructor (fn_710007fe68; its match status is annotated on the alias above).
UnkStringPoolHolder::UnkStringPoolHolder(u32 a, UnkStringPool** pool) {
    UnkStringPoolBody* self = reinterpret_cast<UnkStringPoolBody*>(this);
    self->slot = pool;
    self->field_0x8 = false;
    self->state.empty.ref_count = 0;
    fn_710007ecb4(reinterpret_cast<char*>(&self->state.empty.buffer), "", 0, -1);
    // The state is reached through one register, the pre-indexed store's write-back, up to the
    // node allocation; the barrier keeps the stores that follow on that register.
    UnkStringPoolState* state = &self->state;
    state->empty.heap_copy = nullptr;
    asm("" : "+r"(state));
    state->empty.tail.copy = true;
    state->empty.tail.field_0x44 = 0;
    new (&state->set.buckets) UnkVector<char*>(0x25);
    state->set.first_node = nullptr;
    state->set.first_bucket = -1;
    state->set.node_allocator = fn_7100083990();
    state->set.node_flags = 0x25;
    state->set.node_count = 0;
    state->set.version = 1;
    state->set.nodes.vtable = g_unk_7101a1a4c8 + 0x10;
    state->set.nodes.parent = nullptr;
    fn_710008377c(&state->set.nodes.blocks);
    state->set.nodes.field_0x38 = 0;
    state->set.nodes.field_0x3c = false;
    char* set = reinterpret_cast<char*>(&state->set);
    fn_7100080578(&state->set, a, a);
    self->state.empty.ref_count = 1;

    // The empty string's node, under the key -1.
    ++state->set.version;
    UnkPoolNode* node =
        static_cast<UnkPoolNode*>(state->set.node_allocator->vf_0x40(0x58, 8, state->set.node_flags));
    if (node) {
        node->key = -1;
        // Both read before either is stored, as the original does.
        char* heap_copy = state->empty.heap_copy;
        s32 ref_count = state->empty.ref_count;
        node->data.ref_count = ref_count;
        node->data.heap_copy = heap_copy;
        fn_710007ea60(reinterpret_cast<UnkNamedEntry*>(&node->data.buffer),
                      reinterpret_cast<const UnkNamedEntry*>(&self->state.empty.buffer));
        node->data.tail = self->state.empty.tail;
        node->next = nullptr;
    }
    UnkStringPoolState& s = self->state;
    u32 bucket = s.set.buckets.count ? (u32)-1 % s.set.buckets.capacity : -1;
    u32& first = s.set.first_bucket;
    first = bucket < first ? bucket : first;
    if (first == bucket)
        s.set.first_node = node;
    UnkPoolNode** head = reinterpret_cast<UnkPoolNode**>(fn_7100080824(set, bucket));
    node->next = *head;
    *head = node;
    ++s.set.node_count;
    *self->slot = reinterpret_cast<UnkStringPool*>(self);
}

// The destructor (fn_7100080004): frees every node, empties the set, clears the global the pool
// was published in and destroys the empty string.
UnkStringPoolHolder::~UnkStringPoolHolder() {
    UnkStringPoolBody* self = reinterpret_cast<UnkStringPoolBody*>(this);
    UnkStringPoolState& s = self->state;
    char* set = reinterpret_cast<char*>(&s.set);
    if (s.set.node_count) {
        u32 n = s.set.buckets.count;
        for (u32 i = 0; i != n; ++i) {
            UnkPoolNode* node = reinterpret_cast<UnkPoolNode*>(*fn_7100080824(set, i));
            while (node) {
                UnkPoolNode* next = node->next;
                fn_710007ecd4(&node->data.buffer);
                s.set.node_allocator->vf_0x48(node);
                ++s.set.version;
                node = next;
            }
        }
        memset(s.set.buckets.data, 0, (u64)s.set.buckets.capacity * 8);
        s.set.node_count = 0;
        s.set.first_node = nullptr;
        s.set.first_bucket = -1;
    }
    *self->slot = nullptr;
    fn_71000800dc(set);
    fn_710007ecd4(&s.empty.buffer);
}

// Sets the set up (fn_7100080578; see base/global/UnkStringPoolBody.h).
// NON_MATCHING(minor): the bucket copy loop compares its counter with the count as 64-bit values
// where the original compares the low halves (one instruction); the same behaviour. The barriers
// on `set` make each use of the node pool's address a fresh computation, as in the original.
void fn_7100080578(UnkPoolSetWithNodes* set, u32 buckets, u32 nodes) {
    u32 flags = set->node_flags;
    UnkAllocator* node_allocator = set->node_allocator;
    void* old_memory = set->nodes.blocks.buffer;
    if (old_memory) {
        fn_71000837f4(&set->nodes.blocks);
        set->nodes.parent->vf_0x48(old_memory);
        set->nodes.parent = nullptr;
    }
    // The pool's address is formed afresh at each use rather than kept in a register.
    if (nodes) {
        if (!node_allocator)
            node_allocator = fn_7100083990();
        set->nodes.parent = node_allocator;
        void* memory = node_allocator->vf_0x40((u64)nodes * sizeof(UnkPoolNode), 8, flags);
        asm("" : "+r"(set));
        fn_710008378c(&set->nodes.blocks, memory, (u64)nodes * sizeof(UnkPoolNode), sizeof(UnkPoolNode));
    }
    asm("" : "+r"(set));
    set->node_allocator = reinterpret_cast<UnkAllocator*>(&set->nodes);
    UnkVector<char*>& v = set->buckets;
    u32 count = v.count;
    if (count < buckets) {
        if (v.capacity < buckets) {
            char** grown = static_cast<char**>(v.allocator->vf_0x40((u64)buckets << 3, 8, v.flags));
            char** old = v.data;
            if (old) {
                // Bitwise copies: the original reloads the count after each store.
                if (v.count) {
                    u32 i = 0;
                    do {
                        __builtin_memcpy(&grown[i], &old[i], sizeof(char*));
                        ++i;
                    } while (i < v.count);
                }
                v.allocator->vf_0x48(v.data);
            }
            count = v.count;
            v.data = grown;
            v.capacity = buckets;
        }
        for (u32 i = count; i < buckets; ++i)
            v.data[i] = nullptr;
    } else if (count <= buckets) {
        return;
    }
    v.count = buckets;
}
