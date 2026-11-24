// Unit 683 (CChozoRobotSoldierDashSlashAttack): sends the owner's animation component the value 1 under one of
// two hashes, the dash slash's own id when the target is in reach (200 units at most), with a local parameter
// block (a null pointer and 4.0). Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/CCharClassChozoRobotSoldierDashSlashAttack.h"
#include "game/CChozoRobotSoldierCannonShotAttack.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkGoliathAI_b58a3;

struct UnkDashSlashSendOwner_l468f70 {
    u8 pad_0x0[0x220];
    void* field_0x220;
    u8 pad_0x228[0x18];
    UnkGoliathAI_b58a3* field_0x240;
};

struct UnkDashSlashSend_l468f70 {
    void** vtable;
    UnkDashSlashSendOwner_l468f70* owner;  // 0x8
};

struct UnkDashSlashParams_l468f70 {
    void* ptr;
    f32 value;
};

struct UnkStrIdData_l468f70 {
    u8 pad_0x0[0x30];
    u64 hash;
};

void fn_7100468f70(UnkDashSlashSend_l468f70* self) {
    void* target = self->owner->field_0x220;
    if (!target)
        return;
    f32 limit = *reinterpret_cast<f32*>(g_unk_71726cb118 + 0x2c);
    if (UnkGoliathAI_b58a3* ai = self->owner->field_0x240) {
        f32 value = 0.0f;
        fn_71010fc68c(ai, &value);
        limit = (value < limit || limit < 0.0f) ? value : limit;
    }
    UnkDashSlashParams_l468f70 params;
    switch (static_cast<int>(limit <= 200.0f)) {
    case 1:
        // In reach: the dash slash's own id.
        u64 hash;
        hash = (*reinterpret_cast<UnkStrIdData_l468f70**>(g_unk_71726cbb88))->hash;
        base::reflection::UnkReflTypeIdArg near_value;
        near_value.id = 1;
        params.ptr = nullptr;
        params.value = 4.0f;
        base::reflection::UnkReflTypeIdArg near_arg;
        near_arg.id = base::reflection::UnkReflTypeId<base::reflection::UnkReflTypeIdTag>::id();
        CAnimationComponent_SendActionToTrack(target, hash, 0xf4061ff25a2163d6ull, &near_value, &g_unk_710164b958, &params, &near_arg);
        break;
    default:
        base::reflection::UnkReflTypeIdArg far_value;
        far_value.id = 1;
        params.ptr = nullptr;
        params.value = 4.0f;
        base::reflection::UnkReflTypeIdArg far_arg;
        far_arg.id = base::reflection::UnkReflTypeId<base::reflection::UnkReflTypeIdTag>::id();
        CAnimationComponent_SendActionToTrack(target, 0xd25de804095aad99ull, 0xf4061ff25a2163d6ull, &far_value, &g_unk_710164b958,
                      &params, &far_arg);
        break;
    }
}
