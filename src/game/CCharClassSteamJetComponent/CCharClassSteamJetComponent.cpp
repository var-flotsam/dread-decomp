// In the CCharClassSteamJetComponent unit: fn_710089ff24, a reflected setter. It stores its float
// argument at +0xc70 of the object and, when the object has the node holder at +0xc58, scales that
// holder's node (+0x28) uniformly by it (fn_7100208fa4). Returns the empty value. Names and layouts
// are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::reflection::UnkReflValue;

void fn_7100208fa4(UnkXformNode* self, const UnkVec3f* v);

struct UnkSteamJetNodeHolder {
    u8 pad_0x0[0x28];
    UnkXformNode node;
};

struct UnkSteamJet {
    u8 pad_0x0[0xc58];
    UnkSteamJetNodeHolder* field_0xc58;
    u8 pad_0xc60[0x10];
    f32 field_0xc70;
};

UnkReflValue fn_710089ff24(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* arg = args[0];
    UnkSteamJet* jet = *reinterpret_cast<UnkSteamJet**>(obj + 8);
    f32 scale = *base::reflection::refl_ArgValue<f32>(arg);
    jet->field_0xc70 = scale;
    if (UnkSteamJetNodeHolder* holder = jet->field_0xc58) {
        UnkVec3f v(scale, scale, scale);
        fn_7100208fa4(&holder->node, &v);
    }
    return base::reflection::refl_EmptyValue();
}
