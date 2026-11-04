// navmesh::CFixedDynamicSmartLinkRule's unit: the call operators of two lambdas that walk a tree of
// 0x50-byte nodes (fn_7100273f08 and its copy fn_7100273ff0). A leaf hands back its two points (x
// negated when the captured flag is set); an inner node offers its children, last first, to a
// captured std::function-like callable until one accepts. Names and layouts are placeholders.
#include "types.h"

[[noreturn]] void fn_7100273ea0();  // throws bad_function_call

struct UnkPoint_A1_b63 {
    f32 x;
    f32 y;
    UnkPoint_A1_b63(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkPoint_A1_b63(const UnkPoint_A1_b63& o) : x(o.x), y(o.y) {}
    UnkPoint_A1_b63& operator=(const UnkPoint_A1_b63&) = default;
};

struct UnkTreeNode_A1_b63 {
    UnkPoint_A1_b63 a;
    UnkPoint_A1_b63 b;
    UnkTreeNode_A1_b63* children;
    u32 count;
    u8 pad_0x1c[0x38 - 0x1c];
    bool leaf;
    u8 pad_0x39[0x50 - 0x39];
};

struct UnkTreeCallable_A1_b63 {
    virtual void vf_0();
    virtual void vf_1();
    virtual void vf_2();
    virtual void vf_3();
    virtual void vf_4();
    virtual void vf_5();
    virtual bool Call(UnkTreeNode_A1_b63* node, UnkPoint_A1_b63* a, UnkPoint_A1_b63* b);
};

struct UnkTreeFunction_A1_b63 {
    u8 buf[0x20];
    UnkTreeCallable_A1_b63* target;
};

// The two lambdas' captures: the second has no leading capture.
struct UnkTreeVisit_A1_b63 {
    void* field_0x0;
    UnkTreeFunction_A1_b63* visit;
    bool* flip;
};

struct UnkTreeVisit2_A1_b63 {
    UnkTreeFunction_A1_b63* visit;
    bool* flip;
};

static inline UnkPoint_A1_b63 Flipped_A1_b63(const UnkPoint_A1_b63& p, bool flip) {
    return flip ? UnkPoint_A1_b63(-p.x, p.y) : p;
}

template <class Self>
static inline bool Visit_A1_b63(Self* self, UnkTreeNode_A1_b63* node, UnkPoint_A1_b63* a,
                                UnkPoint_A1_b63* b) {
    if (node->leaf) {
        *a = Flipped_A1_b63(node->a, *self->flip);
        *b = Flipped_A1_b63(node->b, *self->flip);
        return true;
    }
    for (u32 i = node->count - 1; i < node->count; --i) {
        UnkTreeCallable_A1_b63* target = self->visit->target;
        if (!target)
            fn_7100273ea0();
        if (target->Call(&node->children[i], a, b))
            return true;
    }
    return false;
}

bool fn_7100273f08(UnkTreeVisit_A1_b63* self, UnkTreeNode_A1_b63* node, UnkPoint_A1_b63* a, UnkPoint_A1_b63* b) {
    return Visit_A1_b63(self, node, a, b);
}

bool fn_7100273ff0(UnkTreeVisit2_A1_b63* self, UnkTreeNode_A1_b63* node, UnkPoint_A1_b63* a, UnkPoint_A1_b63* b) {
    return Visit_A1_b63(self, node, a, b);
}
