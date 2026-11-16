// CCollider's unit (2503): sets a node's matrix when it differs from the current one, keeping the old one
// as the previous matrix (the new one on the first set), marks the children dirty and updates each of them,
// then updates the node itself (virtual slot 0x38). Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "types.h"

struct UnkMatrixNode_ld36e9c;

// A fixed-capacity list of child nodes.
struct UnkNodeList_ld36e9c {
    u32 capacity;
    u32 count;
    UnkMatrixNode_ld36e9c* nodes[56];

    UnkMatrixNode_ld36e9c** begin() { return nodes; }
    UnkMatrixNode_ld36e9c** end() { return begin() + count; }
};

struct UnkMatrixNode_ld36e9c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();

    u8 pad_0x8[0x60];
    bool first_set;                       // 0x68
    UnkMatrix44 matrix;                   // 0x6c
    UnkMatrix44 previous_matrix;          // 0xac
    u8 pad_0xec[0x74];
    UnkNodeList_ld36e9c children;  // 0x160
    u8 dirty;                      // 0x328
};
static_assert(__builtin_offsetof(UnkMatrixNode_ld36e9c, children) == 0x160, "layout");
static_assert(__builtin_offsetof(UnkMatrixNode_ld36e9c, dirty) == 0x328, "layout");

void fn_7100d36e9c(UnkMatrixNode_ld36e9c* self, const UnkMatrix44* m) {
    if (__builtin_memcmp(m, &self->matrix, sizeof(UnkMatrix44)) == 0)
        return;
    if (self->first_set) {
        UnkCopyMatrix(&self->previous_matrix, *m);
        self->first_set = false;
    } else {
        UnkCopyMatrix(&self->previous_matrix, self->matrix);
    }
    UnkCopyMatrix(&self->matrix, *m);
    for (UnkMatrixNode_ld36e9c** it = self->children.begin(); it != self->children.end(); ++it) {
        (*it)->dirty = 1;
        (*it)->vf_0x38();
    }
    self->vf_0x38();
}
