// SCameraRailNode's unit (tools/tu_map.py): copy-constructs a cached (index, node) value from
// another, which owns its pair (state 1) or refers to an index and a node (state 2); either way
// the copy owns its own. Names and layouts are placeholders.
#include <new>

#include "game/SCameraRailNode.h"
#include "game/UnkCallbackSet_7101a20950.h"
#include "types.h"

struct UnkCameraRailNode_c08704 {
    UnkCameraRailNode_c08704(const UnkCameraRailNode_c08704& other)
        : vtable(g_unk_7101aa2888 + 0x10), field_0x8(other.field_0x8), field_0xc(other.field_0xc),
          field_0x10(other.field_0x10), set(other.set) {}
    void* vtable;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    UnkCallbackSet_7101a20950 set;
};

struct UnkCameraRailValue_c08704 {
    struct Value {
        u32 index;
        UnkCameraRailNode_c08704 node;
    };
    struct Ref {
        u32* index;
        UnkCameraRailNode_c08704* node;
    };
    alignas(8) u8 storage[sizeof(Value)];
    u32 state;
};

void fn_7100c08704(UnkCameraRailValue_c08704* self, const UnkCameraRailValue_c08704* other) {
    self->state = 0;
    auto* value = reinterpret_cast<UnkCameraRailValue_c08704::Value*>(self->storage);
    switch (other->state) {
    case 1: {
        self->state = 1;
        auto* from = reinterpret_cast<const UnkCameraRailValue_c08704::Value*>(other->storage);
        value->index = from->index;
        new (&value->node) UnkCameraRailNode_c08704(from->node);
        break;
    }
    case 2: {
        self->state = 1;
        auto* ref = reinterpret_cast<const UnkCameraRailValue_c08704::Ref*>(other->storage);
        const UnkCameraRailNode_c08704& node = *ref->node;
        value->index = *ref->index;
        new (&value->node) UnkCameraRailNode_c08704(node);
        break;
    }
    }
}
