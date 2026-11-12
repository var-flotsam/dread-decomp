// Unit 2185: which of the ten material constant colors a name hash selects, and the sampler
// handler that sends a clamped color to a particle target. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CFXComponent.h"
#include "types.h"

struct UnkSampleParams;
[[noreturn]] void fn_7100c7b820();

struct UnkColorTarget_b260 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void SetColor(u32 slot, f32* color, u32 kind);                   // 0xe0
    virtual void SetColorIndexed(u64 index, u32 slot, f32* color, u32 kind);  // 0xe8
};

struct UnkColorKey_b260 {
    u64 index;     // 0x0
    u64 index2;    // 0x8
    u64 slot_hash; // 0x10
    u64 kind_hash; // 0x18
};
struct UnkColorOwner_b260 {
    u8 unk_0x0[0x280];
    void* world;
};

struct UnkColorParams_b260 {
    f32* color;               // 0x0
    u8 unk_0x8[8];
    UnkColorKey_b260* key;    // 0x10
    u8 unk_0x18[0x18];
    UnkColorOwner_b260* owner;  // 0x30
    u32 failed;               // 0x38
};

struct UnkPart;
struct UnkWorld;

u32 fn_7100c7b648(u64 hash) {
    if (fn_7100001570("MATERIAL_CONSTANT_COLOR_0", 25) == hash)
        return 0;
    if (fn_7100001570("MATERIAL_CONSTANT_COLOR_1", 25) == hash)
        return 1;
    if (fn_7100001570("MATERIAL_CONSTANT_COLOR_2", 25) == hash)
        return 2;
    if (fn_7100001570("MATERIAL_CONSTANT_COLOR_3", 25) == hash)
        return 3;
    if (fn_7100001570("MATERIAL_CONSTANT_COLOR_4", 25) == hash)
        return 4;
    if (fn_7100001570("MATERIAL_CONSTANT_COLOR_5", 25) == hash)
        return 5;
    if (fn_7100001570("MATERIAL_CONSTANT_COLOR_6", 25) == hash)
        return 6;
    if (fn_7100001570("MATERIAL_CONSTANT_COLOR_7", 25) == hash)
        return 7;
    if (fn_7100001570("MATERIAL_CONSTANT_COLOR_8", 25) == hash)
        return 8;
    if (fn_7100001570("MATERIAL_CONSTANT_COLOR_9", 25) == hash)
        return 9;
    return fn_7100001570("EMATERIALCONSTANTCOLOR_COUNT", 28) == hash ? 10 : 0x7fffffff;
}


void fn_7100c7b4ec(UnkSampleParams* p_) {
    UnkColorParams_b260* params = reinterpret_cast<UnkColorParams_b260*>(p_);
    if (params->failed)
        fn_7100c7b820();
    void* world = params->owner->world;
    if (!world || params->key->index == (u64)-1)
        return;
    UnkColorTarget_b260* target =
        reinterpret_cast<UnkColorTarget_b260*>(fn_7101152b90(static_cast<UnkWorld*>(world), params->key->index));
    if (!target)
        return;
    f32 r = params->color[0], g = params->color[1], b = params->color[2], a = params->color[3];
    f32 c[4];
    c[0] = r <= 0.0f ? 0.0f : r;
    c[1] = g <= 0.0f ? 0.0f : g;
    c[2] = b <= 0.0f ? 0.0f : b;
    c[3] = a <= 0.0f ? 0.0f : a;
    u64 h = params->key->kind_hash;
    u32 kind;
    if (h == 0xbd2673e0bce97b56ull)
        kind = 1;
    else if (h == 0x61839512038ac025ull)
        kind = 0;
    else
        kind = fn_7100001570("E_GEOMETRY", 10) == h ? 2 : 0x7fffffff;
    u32 slot = fn_7100c7b648(params->key->slot_hash);
    if (params->key->index2 != (u64)-1)
        target->SetColorIndexed(params->key->index2, slot, c, kind);
    else
        target->SetColor(slot, c, kind);
}
