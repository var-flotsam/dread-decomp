// UnkTU811 (CEmmyAIComponent's unit): starts a move on the AI's navigation object with a kind-7
// parameter block (the two callbacks, the target or the AI's own one at +0x14718, an optional third
// callback). Returns whether the move was handed over (UnkTU811.h declares it void). Names and layouts
// are placeholders.
#include "game/UnkAIComponent.h"
#include "game_s2/UnkParams.h"
#include "types.h"

extern void* g_unk_7101a265a8[];

struct UnkEmmySearchingAction_b0068;

// A callback bound to the action: the object and the function called with it.
struct UnkEmmySearchCallback_b0068 {
    UnkEmmySearchingAction_b0068* self;
    void (*fn)(UnkEmmySearchingAction_b0068*);
};

struct UnkEmmyAI_b0068;

// A callback that may be unset.
struct UnkOptionalCallback_l556c00 {
    UnkEmmySearchCallback_b0068 value;
    bool has;
    u8 pad_0x11[7];

    void Set(const UnkEmmySearchCallback_b0068& callback) {
        value = callback;
        has = true;
    }
};

// The move's target: something that says whether it can be moved to.
class UnkMoveTarget_l556c00 {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual u32 IsValid();
};

struct UnkEmmyAIComponent_l556c00 {
    u8 pad_0x0[0x14718];
    UnkMoveTarget_l556c00 ownTarget;  // 0x14718
};

// The kind-7 parameter block (game_s2/UnkParams7.h) as this function fills it.
struct UnkMoveParams_l556c00 {
    void** vtable;
    u8 field_0x8[13];
    u8 pad_0x15[3];
    f32 speed;
    bool field_0x1c;
    u8 pad_0x1d[3];
    UnkEmmySearchCallback_b0068 onArrive;
    bool hasOnArrive;
    u8 pad_0x31[3];
    u32 kind;
    UnkMoveTarget_l556c00* target;
    bool hasTarget;
    u8 pad_0x41[7];
    void* field_0x48;
    bool field_0x50;
    u8 pad_0x51[7];
    bool field_0x58;
    bool field_0x59;
    bool field_0x5a;
    bool pending;
    bool field_0x5c;
    u8 pad_0x5d;
    bool field_0x5e;
    u8 pad_0x5f[5];
    bool field_0x64;
    u8 pad_0x65[3];
    UnkOptionalCallback_l556c00 onFail;   // 0x68
    UnkOptionalCallback_l556c00 onExtra;  // 0x80
    bool field_0x98;
    u8 pad_0x99[7];
};
static_assert(sizeof(UnkMoveParams_l556c00) == 0xa0, "UnkMoveParams_l556c00 size");

bool fn_7100556c00(UnkEmmyAI_b0068* ai, bool pending, const UnkEmmySearchCallback_b0068* a,
                   const UnkEmmySearchCallback_b0068* b, u32 c, void* data, const UnkEmmySearchCallback_b0068& d) {
    UnkNavigation* nav = fn_71010fccf8(reinterpret_cast<UnkAIComponent*>(ai));
    if (!nav)
        return false;
    UnkMoveTarget_l556c00* target = static_cast<UnkMoveTarget_l556c00*>(data);
    if (!target)
        target = &reinterpret_cast<UnkEmmyAIComponent_l556c00*>(ai)->ownTarget;
    if (!target->IsValid())
        return false;

    UnkMoveParams_l556c00 params;
    __builtin_memset(params.field_0x8, 0, sizeof(params.field_0x8));
    params.vtable = &g_unk_7101a265a8[2];
    params.onExtra.value = UnkEmmySearchCallback_b0068();
    params.kind = 7;
    params.speed = 1.0f;
    params.pending = pending;
    params.field_0x50 = false;
    params.field_0x58 = false;
    params.field_0x5e = false;
    params.field_0x64 = false;
    params.field_0x98 = false;
    params.onExtra.has = false;
    params.target = target;
    params.field_0x1c = true;
    params.hasTarget = true;
    params.field_0x5c = true;
    params.field_0x59 = true;
    params.field_0x5a = true;
    params.onArrive = *a;
    params.hasOnArrive = true;
    params.onFail.Set(*b);
    if (d.fn)
        params.onExtra.Set(d);
    fn_71006f4b0c(nav, &params, 1);
    return true;
}
