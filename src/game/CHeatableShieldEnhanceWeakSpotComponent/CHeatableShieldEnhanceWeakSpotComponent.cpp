// CHeatableShieldEnhanceWeakSpotComponent: its constructor (fn_71007d4f20). The component base
// constructor fn_71010c4148, the flags at +0x3d (set) and +0x3e (clear) as one halfword, the vtable, the
// byte at +0x48, then the class-entry pass of base::core::CBaseObject (fn_7100023854). Names and
// layouts are placeholders.
#include "types.h"

struct UnkStateObject;
struct UnkNode;
void fn_71010c4148(UnkStateObject* self);
void fn_7100023854(UnkNode* node);
extern void* g_unk_7101a72500[];

struct UnkHeatableShieldEnhanceWeakSpot {
    void** vtable;
    u8 pad_0x8[0x35];
    u8 field_0x3d[2];
    u8 pad_0x3f[9];
    bool field_0x48;
};

void fn_71007d4f20(UnkHeatableShieldEnhanceWeakSpot* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->field_0x3d[0] = 1;
    self->field_0x3d[1] = 0;
    self->vtable = g_unk_7101a72500 + 2;
    self->field_0x48 = false;
    fn_7100023854(reinterpret_cast<UnkNode*>(self));
}
