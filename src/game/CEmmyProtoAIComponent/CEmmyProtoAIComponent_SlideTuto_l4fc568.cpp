// CEmmyProtoAIComponent's unit (tools/tu_map.py unit 791): when `show` is set and the game manager has a
// tutorial composition, marks the floor-slide tutorial as shown, shows it, and logs its message (kind 2)
// to the game manager's callback set at +0x2128. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CChozoCommanderAIComponent.h"
#include "game/GUI/CTutorialComposition.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

struct UnkTutorialComposition;
struct UnkMlogMessage_b42a1;

struct UnkMlogMessage_l4fc568 {
    u32 kind;
    base::global::CStrId text;
    base::global::CRntVector<base::UnkStrIdMember> args;

    UnkMlogMessage_l4fc568(u32 k, const CStrId& t) : kind(k), text(t), args(0xc) {}
};

struct UnkTutorialHolder_l4fc568 {
    u8 pad_0x0[0x10];
    UnkTutorialComposition* composition;  // 0x10
};

struct UnkEmmyProto_l4fc568 {
    u8 pad_0x0[0x2a708];
    bool slideTutoShown;  // 0x2a708
};

void CEmmyProtoAIComponent_ShowFloorSlideTutorial(UnkEmmyProto_l4fc568* self, bool show) {
    if (!show)
        return;
    UnkTutorialHolder_l4fc568* holder = static_cast<UnkTutorialHolder_l4fc568*>(g_unk_7101d47310->field_0x2270);
    if (!holder)
        return;
    UnkTutorialComposition* composition = holder->composition;
    if (!composition)
        return;
    self->slideTutoShown = true;
    static CStrId s_tutorial("#CAVE_EMMY_PROTO_FLOOR_SLIDE_TUTO", true);
    CTutorialComposition_ShowWithText(composition, &s_tutorial);
    UnkMlogMessage_l4fc568 message(2, CStrId("#MLOG_TUTO_EMMY_SLIDE", true));
    fn_7100456b7c(reinterpret_cast<u8*>(g_unk_7101d47310) + 0x2128,
                  reinterpret_cast<const UnkMlogMessage_b42a1*>(&message));
    message.args.Free();
}
