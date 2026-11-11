// CCharClassStopForceEmmyChasePulseEvent's unit (tools/tu_map.py): the event's run: when the actor
// argument is set and the event's id (+0x10) is not empty, the central unit of the lazily created
// registry g_unk_71726bee10 (fn_7100ddb768) stops the forced chase pulse with that id
// (fn_7100775610, off). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/CAutsharpAIComponent.h"
#include "game/UnkTU2547.h"
#include "types.h"

using base::global::CStrId;

struct UnkDoorRegistry_b61;
struct UnkUnit2547;
struct UnkCentralUnit;
void fn_7100775610(UnkCentralUnit* unit, bool on, const CStrId* id);

struct UnkChasePulseEvent_ovb0069 {
    u8 pad_0x0[0x10];
    CStrId id;  // 0x10
};

void fn_7100be5f6c(UnkChasePulseEvent_ovb0069* self, void** actor) {
    if (!*actor)
        return;
    if (self->id.data == &g_unk_7101d47108->empty)
        return;
    UnkDoorRegistry_b61* reg = g_unk_71726bee10;
    if (!reg) {
        reg = static_cast<UnkDoorRegistry_b61*>(fn_71000839a0(4, 0x3f));
        fn_7100ddb6dc(reg);
        g_unk_71726bee10 = reg;
    }
    UnkCentralUnit* unit = fn_7100ddb768(reinterpret_cast<UnkUnit2547*>(reg));
    if (!unit)
        return;
    fn_7100775610(unit, false, &self->id);
}
