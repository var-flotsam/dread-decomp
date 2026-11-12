// CChozoWarriorXUltimateGrabAttack's unit (tools/tu_map.py unit 718): hits the player (fn_7100dec788)
// through its life component (+0x1c0, fn_7101166c6c) with a damage request (fn_71008c0c74) of the tunable
// value at +0xec of the attack's +0x710 object and the attack owner's weak handle (+0x8, fn_7100d78378),
// marks the attack (+0x133), drops the life (+0x54) by what the request leaves (+0xf8, virtual 0x168)
// unless it is guarded (+0x5c) and not above it, calls fn_7100dcdc18 on g_unk_71726bc570 and returns
// whether the life component's state (+0x70) is 2; true without a player or life component. Names and
// layouts are placeholders.
#include "game/CChozoWarriorXUltimateGrabAttack.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkPlayer;

struct UnkRequest;
struct UnkStruct_71008c0c74;
struct UnkBase_71003cc474;
void fn_71008c1538(UnkRequest* request, f32 value);

struct UnkModeFlags;
extern UnkModeFlags* g_unk_71726bc570;  // as CGameManager.cpp declares it
void fn_7100dcdc18(void* self);

struct UnkGrabRequest_A2_b71 {
    u8 pad_0x0[0x8];
    UnkWeakHandle* source;  // 0x8
    u8 pad_0x10[0xe8];
    f32 field_0xf8;
    u8 pad_0xfc[0x4];

    UnkGrabRequest_A2_b71() { fn_71008c0c74(reinterpret_cast<UnkStruct_71008c0c74*>(this)); }
    ~UnkGrabRequest_A2_b71() { fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(this)); }
};
static_assert(sizeof(UnkGrabRequest_A2_b71) == 0x100, "request layout");

struct UnkGrabLife_A2_b71 {
    void** vtable;
    u8 pad_0x8[0x4c];
    f32 life;  // 0x54
    u8 pad_0x58[0x4];
    bool guarded;  // 0x5c
    u8 pad_0x5d[0x13];
    s32 state;  // 0x70

    void vf_0x168(f32 value, bool flag) {
        reinterpret_cast<void (*)(UnkGrabLife_A2_b71*, f32, bool)>(vtable[0x168 / 8])(this, value, flag);
    }
};
void fn_7101166c6c(UnkGrabLife_A2_b71* self, UnkGrabRequest_A2_b71* request);

struct UnkGrabPlayer_A2_b71 {
    u8 pad_0x0[0x1c0];
    UnkGrabLife_A2_b71* life;  // 0x1c0
};

struct UnkGrabTunables_A2_b71 {
    u8 pad_0x0[0xec];
    f32 damage;  // 0xec
};

struct UnkGrabAttack_A2_b71 {
    u8 pad_0x0[0x8];
    UnkWeakTarget* owner;  // 0x8
    u8 pad_0x10[0x123];
    bool field_0x133;
    u8 pad_0x134[0x5dc];
    UnkGrabTunables_A2_b71* tunables;  // 0x710
};

bool fn_710048a070(UnkGrabAttack_A2_b71* self) {
    auto* player = reinterpret_cast<UnkGrabPlayer_A2_b71*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return true;
    UnkGrabLife_A2_b71* life = player->life;
    if (!life)
        return true;
    UnkGrabRequest_A2_b71 request;
    fn_71008c1538(reinterpret_cast<UnkRequest*>(&request), self->tunables->damage);
    AssignRef(&request.source, fn_7100d78378(self->owner));
    fn_7101166c6c(life, &request);
    self->field_0x133 = true;
    if (!life->guarded || !(life->life <= request.field_0xf8))
        life->vf_0x168(life->life - request.field_0xf8, true);
    fn_7100dcdc18(reinterpret_cast<void*>(g_unk_71726bc570));
    return life->state == 2;
}
