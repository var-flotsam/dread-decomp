// CThermalDeviceComponent's unit (tools/tu_map.py unit 1601): after the base update (fn_7100aa3264),
// places the actor's scenario object (fn_7100774014) relative to the player: the player's position minus
// the actor's and the difference of their y angles; as a checkpoint under the actor's name, 166.5 units
// further along x, when the flag at +0x59c is set (fn_710116c388), else directly (fn_710116c128). Names
// and layouts are placeholders.
#include "game/CCharClassStartPointComponent.h"
#include "game/CUsableComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1176.h"
#include "game/UnkTU2553.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkScenarioObject_ovb0032;
struct UnkGeneratorTarget_ovb0120;
struct UnkElectricGeneratorUpd_ovb0120;
struct UnkStartPointSaver_l16c388;

bool fn_710116c388(UnkStartPointSaver_l16c388* self, const char* const& a, const UnkVec3f* position,
                   const UnkVec3f* angle, bool flag);

// The actor's name, with a count of its reads.
struct UnkThermalName_la92e58 {
    u8 pad_0x0[0x10];
    const char* name;  // 0x10
    u8 pad_0x18[0x2c];
    s32 reads;  // 0x44
};

struct UnkThermalActor_la92e58 {
    u8 pad_0x0[0x30];
    UnkThermalName_la92e58* name;  // 0x30
    UnkXform xform;                // 0x38

    // The name, counting the read.
    const char* const& Name() {
        ++name->reads;
        return name->name;
    }
};

static inline UnkThermalActor_la92e58* Player() {
    return reinterpret_cast<UnkThermalActor_la92e58*>(fn_7100dec788(g_unk_7101d47310));
}

struct UnkThermalDevice_la92e58 {
    u8 pad_0x0[0x18];
    UnkThermalActor_la92e58* actor;  // 0x18
    u8 pad_0x20[0x57c];
    bool checkpoint;  // 0x59c
};
static_assert(__builtin_offsetof(UnkThermalDevice_la92e58, checkpoint) == 0x59c, "layout");

// Saves the player's place relative to the actor as a checkpoint, 166.5 units further along x.
static void SaveCheckpoint(UnkThermalDevice_la92e58* self, UnkStartPointSaver_l16c388* target) {
    UnkVec3f from = Player()->xform.GetTranslation();
    UnkVec3f to = self->actor->xform.GetTranslation();
    UnkVec3f offset(from.x - to.x, from.y - to.y, from.z - to.z);
    f32 a = Player()->xform.GetAngle().y;
    f32 b = self->actor->xform.GetAngle().y;
    UnkVec3f angle(0.0f, a - b, 0.0f);
    offset.x -= 166.5f;
    fn_710116c388(target, self->actor->Name(), &offset, &angle, true);
}

// Places the target at the player's place relative to the actor.
static void Place(UnkThermalDevice_la92e58* self, UnkGeneratorTarget_ovb0120* target) {
    UnkVec3f from = Player()->xform.GetTranslation();
    UnkVec3f to = self->actor->xform.GetTranslation();
    UnkVec3f offset(from.x - to.x, from.y - to.y, from.z - to.z);
    f32 a = Player()->xform.GetAngle().y;
    f32 b = self->actor->xform.GetAngle().y;
    UnkVec3f angle(0.0f, a - b, 0.0f);
    fn_710116c128(target, &offset, &angle, true);
}

void fn_7100a92e58(UnkThermalDevice_la92e58* self) {
    fn_7100aa3264(reinterpret_cast<UnkElectricGeneratorUpd_ovb0120*>(self));
    void* target = fn_7100774014(reinterpret_cast<UnkScenarioObject_ovb0032*>(self->actor));
    if (!target)
        return;
    if (self->checkpoint)
        SaveCheckpoint(self, static_cast<UnkStartPointSaver_l16c388*>(target));
    else
        Place(self, static_cast<UnkGeneratorTarget_ovb0120*>(target));
}
