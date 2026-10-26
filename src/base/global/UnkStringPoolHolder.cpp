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
using base::UnkVector;
using base::global::UnkPoolNode;
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
    state->nodes_vtable = g_unk_7101a1a4c8 + 0x10;
    state->nodes_parent = nullptr;
    fn_710008377c(&state->nodes_blocks);
    state->nodes_field_0x38 = 0;
    state->nodes_field_0x3c = false;
    char* set = reinterpret_cast<char*>(&state->set);
    fn_7100080578(set, a, a);
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
