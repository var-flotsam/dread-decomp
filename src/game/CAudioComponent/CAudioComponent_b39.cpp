// CAudioComponent unit (tools/tu_map.py): updating the listener. When the component's actor (+0x30) is
// the player, the sound manager gets the actor's position (+0x44, +0x54, +0x64, the matrix's translation)
// with the vectors at +0xd0 and +0xdc (fn_710010807c); then it always gets those with the one at +0xc4
// (fn_7100108084). Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager* manager);

struct UnkSoundMgr;
extern UnkSoundMgr* g_unk_7101d4d820;
void fn_710010807c(UnkSoundMgr* mgr, const base::math::CVector3D* pos, const base::math::CVector3D* a,
                   const base::math::CVector3D* b);
void fn_7100108084(UnkSoundMgr* mgr, const base::math::CVector3D* pos, const base::math::CVector3D* a,
                   const base::math::CVector3D* b);

struct UnkAudioActor_acm {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkAudioComponent_acm {
    u8 pad_0x0[0x30];
    UnkAudioActor_acm* actor;  // 0x30
    u8 pad_0x38[0x8c];
    base::math::CVector3D field_0xc4;
    base::math::CVector3D field_0xd0;
    base::math::CVector3D field_0xdc;
};

void fn_710071e4ac(UnkAudioComponent_acm* self) {
    if (reinterpret_cast<UnkPlayer*>(self->actor) == fn_7100dec788(g_unk_7101d47310)) {
        UnkAudioActor_acm* actor = self->actor;
        f32 x = actor->x;
        f32 y = actor->y;
        f32 z = actor->z;
        base::math::CVector3D pos;
        pos.z = z;
        pos.x = x;
        pos.y = y;
        fn_710010807c(g_unk_7101d4d820, &pos, &self->field_0xd0, &self->field_0xdc);
    }
    fn_7100108084(g_unk_7101d4d820, &self->field_0xc4, &self->field_0xd0, &self->field_0xdc);
}
