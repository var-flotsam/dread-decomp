// CCharClassPlayRandomTimelineEvent (unit from tools/tu_map.py, the name from reflection): when the
// actor has a timeline component (+0x2e8) and the event's mode condition holds (fn_7100bc77ec),
// picks one of up to seven timeline ids (+0x28...) at random (count at +0xc, the first when there
// is only one, the empty id past the seventh) and sets it to the all-flags mask after a random
// delay between the floats at +0x10 and +0x14. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

extern "C" int rand();

template <u32 Offset>
struct UnkModeCondition;
struct UnkModeActor;
bool fn_7100bc77ec(const UnkModeCondition<0x18>* self, UnkModeActor* actor);

struct UnkTimelineNameData_b60c {
    base::global::UnkStringData base;
    u8 unk_0xc[0x24];
    u64 hash;
};

struct UnkPlayRandomTimelineEvent_b60 {
    u8 unk_0x0[0xc];
    u32 count;
    f32 min_delay;
    f32 max_delay;
    u8 unk_0x18[0x10];
    UnkTimelineNameData_b60c* ids[7];
};

struct UnkPlayRandomTimelineTarget_b60 {
    UnkActor* actor;
};

void fn_7100bc7628(UnkPlayRandomTimelineEvent_b60* self, UnkPlayRandomTimelineTarget_b60* target) {
    UnkActor* actor = target->actor;
    auto* timeline = static_cast<UnkTimelineComponent*>(actor->field_0x2e8);
    if (!timeline)
        return;
    if (!fn_7100bc77ec(reinterpret_cast<const UnkModeCondition<0x18>*>(self),
                       reinterpret_cast<UnkModeActor*>(actor)))
        return;
    u32 count = self->count;
    if (count == 0)
        count = 1;
    if (count >= 8)
        count = 8;
    UnkTimelineNameData_b60c* const* id;
    if (count == 1) {
        id = &self->ids[0];
    } else {
        switch ((rand() & 0x7fff) % (count - 1)) {
        case 0:
            id = &self->ids[0];
            break;
        case 1:
            id = &self->ids[1];
            break;
        case 2:
            id = &self->ids[2];
            break;
        case 3:
            id = &self->ids[3];
            break;
        case 4:
            id = &self->ids[4];
            break;
        case 5:
            id = &self->ids[5];
            break;
        case 6:
            id = &self->ids[6];
            break;
        default:
            id = reinterpret_cast<UnkTimelineNameData_b60c* const*>(&g_unk_7101d470e0);
            break;
        }
    }
    alignas(8) u32 mask;
    u64 key = (*id)->hash;
    f32 lo = self->min_delay;
    f32 hi = self->max_delay;
    f32 r = (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
    f32 delay = lo + (hi - lo) * r;
    mask = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, &key, &mask, false, delay);
}
