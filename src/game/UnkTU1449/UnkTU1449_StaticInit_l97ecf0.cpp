// Static initialiser of unit 1449 of tools/tu_map.py (no name hint): the header ids, then four lists of
// two string ids each (the magnet wall and magnet ceiling grapple transitions), each list's destructor
// registered after it is built. Names and layouts are placeholders.
#include "../../game_5821e4/UnkStaticInit.h"
#include "UnkClone.h"
#include "base/UnkCloneRecords.h"
#include "base/global/CRntVector.h"

using namespace unk_static_init;

struct UnkIdRecordOwner_0xf0;
using UnkIdElem = UnkCloneStrIdElem<UnkIdRecordOwner_0xf0, 0, 8, UnkCloneStrId<UnkIdRecordOwner_0xf0>>;

void ConstructStrIdVectorFromIdArray(base::UnkStrIdVec* self, const base::global::CStrId* ids, u64 size, u32 step,
                   base::UnkAllocator* allocator, u32 flags);
void fn_71008600a8(base::global::CRntVector<UnkIdElem>* self);

struct UnkStatics_l97ecf0 {
    HeaderIds ids;
    base::UnkStrIdVec wall_down;
    base::UnkStrIdVec wall_up;
    base::UnkStrIdVec ceiling_forward;
    base::UnkStrIdVec ceiling_backward;
};
static_assert(__builtin_offsetof(UnkStatics_l97ecf0, wall_up) == 0x40, "layout");
extern UnkStatics_l97ecf0 g_unk_717273ad40 __attribute__((visibility("hidden")));

// Each list: two ids, a growth step of 10, the default allocator, then its destructor registered.
void InitMagnetGrappleTransitionIdStatics() {
    InitHeaderIds(&g_unk_717273ad40.ids);
    [] {
        UnkStrIdTmp ids[] = {UnkStrIdTmp("magnetwallgrapple2magnetwalldown5init", true),
                             UnkStrIdTmp("magnetwallgrapple2magnetwalldown2init", true)};
        ConstructStrIdVectorFromIdArray(&g_unk_717273ad40.wall_down, reinterpret_cast<const base::global::CStrId*>(ids), 2, 10, nullptr, 15);
    }();
    fn_7100000250(reinterpret_cast<void (*)(void*)>(fn_71008600a8), &g_unk_717273ad40.wall_down,
                  g_unk_7101a19000);
    [] {
        UnkStrIdTmp ids[] = {UnkStrIdTmp("magnetwallgrapple2magnetwallup5init", true),
                             UnkStrIdTmp("magnetwallgrapple2magnetwallup2init", true)};
        ConstructStrIdVectorFromIdArray(&g_unk_717273ad40.wall_up, reinterpret_cast<const base::global::CStrId*>(ids), 2, 10, nullptr, 15);
    }();
    fn_7100000250(reinterpret_cast<void (*)(void*)>(fn_71008600a8), &g_unk_717273ad40.wall_up,
                  g_unk_7101a19000);
    [] {
        UnkStrIdTmp ids[] = {UnkStrIdTmp("magnetceilinggrapple2magnetceilingforward5init", true),
                             UnkStrIdTmp("magnetceilinggrapple2magnetceilingforward2init", true)};
        ConstructStrIdVectorFromIdArray(&g_unk_717273ad40.ceiling_forward, reinterpret_cast<const base::global::CStrId*>(ids), 2, 10, nullptr,
                      15);
    }();
    fn_7100000250(reinterpret_cast<void (*)(void*)>(fn_71008600a8), &g_unk_717273ad40.ceiling_forward,
                  g_unk_7101a19000);
    [] {
        UnkStrIdTmp ids[] = {UnkStrIdTmp("magnetceilinggrapple2magnetceilingbackward5init", true),
                             UnkStrIdTmp("magnetceilinggrapple2magnetceilingbackward2init", true)};
        ConstructStrIdVectorFromIdArray(&g_unk_717273ad40.ceiling_backward, reinterpret_cast<const base::global::CStrId*>(ids), 2, 10, nullptr,
                      15);
    }();
    fn_7100000250(reinterpret_cast<void (*)(void*)>(fn_71008600a8), &g_unk_717273ad40.ceiling_backward,
                  g_unk_7101a19000);
}
