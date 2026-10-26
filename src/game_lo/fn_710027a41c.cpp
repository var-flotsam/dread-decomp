// Finds the region (0x70-byte entry) containing a 2D point, through the bounding-box tree when
// there is one or by testing every region otherwise, and returns its index if the region
// accepts the argument (0xffff otherwise).
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkRegion_710027a41c {
    u8 data[0x70];
};

// A node of the flattened bounding-box tree: leaves have a negative link (bit 31 set, the low
// 16 bits are the region index); other nodes link to the node after their subtree.
struct UnkNode_710027a41c {
    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;
    s32 link;
};

struct UnkOwner_710027a41c {
    u8 pad_0x0[0x290];
    UnkRegion_710027a41c* regions;  // 0x290
    u32 region_count;               // 0x298
    u8 pad_0x29c[0x400 - 0x29c];
    UnkNode_710027a41c* nodes;      // 0x400
    u32 node_count;                 // 0x408
};

struct UnkBox2D_710027a41c {
    CVector2D min;
    CVector2D max;
};

struct UnkQuery_710027a41c {
    CVector2D point;
    UnkOwner_710027a41c* owner;
    u16 result;
};

bool fn_710027a5f0(UnkOwner_710027a41c* self, const CVector2D* point, u16 index);
bool fn_71002af6fc(UnkRegion_710027a41c* region, u32 arg);

u16 fn_710027a41c(UnkOwner_710027a41c* self, const CVector2D* point, u32 arg) {
    if (self->node_count == 0) {
        for (u16 i = 0; i < self->region_count; i++) {
            if (fn_710027a5f0(self, point, i))
                return fn_71002af6fc(&self->regions[i], arg) ? i : 0xffff;
        }
        return 0xffff;
    }

    UnkQuery_710027a41c query = {{point->x, point->y}, self, 0xffff};
    UnkBox2D_710027a41c box;
    CVector3D segment[2] = {{point->x, point->y, -3.4028235e38f}, {point->x, point->y, 3.4028235e38f}};
    fn_710007b430(&box.min, &segment[0]);
    fn_710007b430(&box.max, &segment[1]);

    bool found = false;
    u32 count = self->node_count;
    for (u32 i = 0; i < count;) {
        UnkNode_710027a41c* node = &self->nodes[i];
        if (!(box.max.x >= node->min_x && box.min.x <= node->max_x && box.max.y >= node->min_y &&
              box.min.y <= node->max_y)) {
            i = node->link >= 0 ? node->link : i + 1;
            continue;
        }
        s32 link = node->link;
        if (link < 0 && fn_710027a5f0(self, &query.point, link)) {
            query.result = link;
            found = true;
            break;
        }
        i++;
    }

    u16 index = found ? query.result : 0xffff;
    if (index != 0xffff && fn_71002af6fc(&self->regions[index], arg))
        return index;
    return 0xffff;
}
