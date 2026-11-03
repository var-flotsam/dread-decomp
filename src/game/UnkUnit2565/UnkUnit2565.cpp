// Unit 2565 (tools/tu_map.py; no reflected class): a grid of linked cells. Names and layouts are
// placeholders inferred from the code.
#include "types.h"

struct UnkLinkedCell {
    u8 unk_0x0[0x18];
    UnkLinkedCell* link0;  // +0x18
    UnkLinkedCell* link1;  // +0x20
    UnkLinkedCell* link2;  // +0x28
    UnkLinkedCell* link3;  // +0x30
    bool enabled;          // +0x38
    bool blocked;          // +0x39
    u8 unk_0x3a[0x5];
    u8 open_links;  // +0x3f
    u8 unk_0x40;
    bool dirty;  // +0x41
};

static inline bool IsOpen(const UnkLinkedCell* cell) {
    return cell && cell->enabled && !cell->blocked;
}

// Recomputes the mask of open neighbours (and their links) and clears the dirty flag.
void fn_7100e78180(UnkLinkedCell* self) {
    u32 mask = 0;
    self->open_links = 0;
    if (UnkLinkedCell* cell = self->link0) {
        if (IsOpen(cell))
            self->open_links = mask = 1;
        if (IsOpen(cell->link2))
            self->open_links = mask |= 0x10;
        if (IsOpen(cell->link3))
            self->open_links = mask |= 0x20;
    }
    if (UnkLinkedCell* cell = self->link1) {
        if (IsOpen(cell))
            self->open_links = mask |= 2;
        if (IsOpen(cell->link2))
            self->open_links = mask |= 0x40;
        if (IsOpen(cell->link3))
            self->open_links = mask |= -0x80;  // the top bit, as a signed byte
    }
    if (IsOpen(self->link2))
        self->open_links = mask |= 4;
    if (IsOpen(self->link3))
        self->open_links = mask |= 8;
    self->dirty = false;
}
