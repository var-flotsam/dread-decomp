// CInfesterAIComponent unit (tools/tu_map.py): fn_71005ccb5c is a message handler override. After the base
// handler (fn_7101111dc8), a message with its flag at +0x20 set clears the bool property "bScared" of the
// blackboard section g_unk_71726b7cb8 (blackboard at +0x1260, unless locked). Names are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/UnkTU2702.h"
#include "game/UnkTUProperty.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"
#include "game/UnkTypedValue.h"

using base::global::CStrId;

struct UnkNailongAI_b46a2;
struct UnkNailongMsg_b46a2;

void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

struct UnkInfesterAI_A3_b68 {
    u8 pad_0x0[0x1260];
    CBlackboard blackboard;
};

struct UnkInfesterMsg_A3_b68 {
    u8 pad_0x0[0x20];
    bool field_0x20;
};

u32 fn_71005ccb5c(UnkInfesterAI_A3_b68* self, UnkInfesterMsg_A3_b68* msg) {
    u32 result =
        fn_7101111dc8(reinterpret_cast<UnkNailongAI_b46a2*>(self), reinterpret_cast<UnkNailongMsg_b46a2*>(msg));
    if (msg->field_0x20) {
        CBlackboard* blackboard = &self->blackboard;
        CStrId name("bScared", true);
        UnkTypedValue v;  // declared here: the original's slot order is name, v, value
        bool value = false;
        if (CBlackboard::CSection* s = fn_71010472e0(blackboard, g_unk_71726b7cb8)) {
            if (!blackboard->field_0xe8) {
                v.type = fn_710000d228();
                v.value = &value;
                fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(&name), &v);
            }
        }
    }
    return result;
}
