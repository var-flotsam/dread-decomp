// CGooplotAIComponent's unit (tools/tu_map.py): fn_710059bdec runs the base handler (fn_7101108c9c)
// and then, unless the owner's +0x218 object is in state 3 (fn_71010cc3a4), builds a damage request
// (fn_71008c0c74) with flags 0x800, or 0x40000 when the argument's +0x2f8 object passes
// fn_710092b6f8, copies it (fn_71008c1090) and hands the copy to fn_710059bea4. Names and layouts
// are placeholders.
#include "game/CEnemyMovement.h"
#include "game/CMeleeComponent.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkDaivoArg_b46a1;
struct UnkDaivoAIComponent_b46a1;

struct UnkStruct_71008c0c74;
struct UnkBase_71003cc474;

struct UnkGooplotRequest_b63a3 {
    u8 pad_0x0[0xb4];
    u32 flags;  // 0xb4
    u8 pad_0xb8[0x100 - 0xb8];
};

struct UnkGooplotOwner_b63a3 {
    u8 pad_0x0[0x218];
    void* field_0x218;
};

struct UnkGooplotArg_b63a3 {
    u8 pad_0x0[0x2f8];
    void* field_0x2f8;
};

struct UnkGooplotAIMsg_b63a3 {
    u8 pad_0x0[0x30];
    UnkGooplotOwner_b63a3* owner;
};
void fn_710059bea4(UnkGooplotAIMsg_b63a3* self, UnkGooplotRequest_b63a3* request);

bool fn_710059bdec(UnkGooplotAIMsg_b63a3* self, UnkGooplotArg_b63a3* arg) {
    bool result = fn_7101108c9c(reinterpret_cast<UnkDaivoAIComponent_b46a1*>(self),
                                reinterpret_cast<UnkDaivoArg_b46a1*>(arg));
    void* other = arg ? arg->field_0x2f8 : nullptr;
    UnkGooplotOwner_b63a3* owner = self->owner;
    if (owner && owner->field_0x218 && fn_71010cc3a4(owner->field_0x218) != 3) {
        UnkGooplotRequest_b63a3 request;
        fn_71008c0c74(reinterpret_cast<UnkStruct_71008c0c74*>(&request));
        u32 flags = 0x800;
        if (other && fn_710092b6f8(other))
            flags = 0x40000;
        request.flags = flags;
        UnkGooplotRequest_b63a3 copy;
        fn_71008c1090(&copy, &request);
        fn_710059bea4(self, &copy);
        fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(&copy));
        fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(&request));
    }
    return result;
}
