// animtree::CLayerNode (unit 414): advancing the layer by `dt`: the object at +0x28 first, then the
// node itself (fn_71002dfaf0 with both flags set). Names are placeholders.
#include "game/animtree/CAnimTreeNode.h"
#include "types.h"

struct UnkLayerChild;

struct UnkLayerNode {
    u8 pad_0x0[0x28];
    UnkLayerChild* field_0x28;
};

void fn_710014ac2c(UnkLayerChild* child, f32 dt);

void fn_71002e465c(UnkLayerNode* self, f32 dt) {
    fn_710014ac2c(self->field_0x28, dt);
    fn_71002dfaf0(self, true, dt, true);
}
