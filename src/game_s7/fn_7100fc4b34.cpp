// Reads a set of tuning values from two property sets: each value is looked up by its
// definition's qualified id in a section (a key from the id table) of one of the sets, after
// being reset. A few values are combined: a difference of two floats, flags read only when the
// previous one is set, two enum values read under named sub-ids of one definition, and flags
// set only when the first set has them and the second doesn't. Names and layouts are
// placeholders.
#include "UnkPropS7.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"

using namespace unk_prop;

// A definition's qualified id (game/UnkGameBlackboard.h), built on first use (as in fn_710092f204).
struct UnkDef_7100fc4b34 : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedId qid;
};

static inline const UnkQualifiedId& Qid(UnkDef_7100fc4b34& def) {
    if ((u64)def.qid.id.data == (u64)g_unk_7101d47108 + 0x10)
        fn_7100ae9a10(&def.qid, &def, g_unk_7101d470e0);
    return def.qid;
}

// A definition whose qualified id is kept as a string (built by fn_7100ae9d7c), with named
// sub-ids looked up in a map (fn_71003327a4).
struct UnkSubId_7100fc4b34 {
    CStrId id;
    u64 field_0x8;
};
using UnkQualifiedName_7100fc4b34 = UnkQualifiedName;
const UnkSubId_7100fc4b34& fn_71003327a4(UnkQualifiedName_7100fc4b34* self, const CStrId* name);

struct UnkNamedDef_7100fc4b34 : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedName_7100fc4b34 name;
};

static inline UnkSubId_7100fc4b34 SubId(UnkNamedDef_7100fc4b34& def, const CStrId& name) {
    if (!def.name.str || !def.name.str[0])
        fn_7100ae9d7c(&def.name, &def, g_unk_7101d470e0);
    return fn_71003327a4(&def.name, &name);
}

// Typed reads of a section's value (refl_GetKeyedValue copies).
bool fn_7100395b34(CBlackboard::CSection* section, const void* name, f32* out);
bool fn_710006d11c(CBlackboard::CSection* section, const void* name, bool* out);
bool fn_7100395a30(CBlackboard::CSection* section, const void* name, s32* out);
bool fn_7100a68fb0(CBlackboard::CSection* section, const void* name, u32* out);

struct UnkString_7100fc4b34 {
    u8 unk_0x0[0x30];
};
void fn_710007eba0(UnkString_7100fc4b34* self, u32 size);
bool fn_7100ec50e4(CBlackboard::CSection* section, const void* name, UnkString_7100fc4b34* out);

template <auto Reader, class T, class N>
__attribute__((always_inline)) inline void Get(CBlackboard* set, const CStrId& section,
                                               const N& name, T* out) {
    if (CBlackboard::CSection* s = fn_7101047270(set, section))
        Reader(s, &name, out);
}

extern UnkDef_7100fc4b34 g_unk_717275a118;
extern UnkDef_7100fc4b34 g_unk_717275a058;
extern UnkDef_7100fc4b34 g_unk_7172759ad0;
extern UnkDef_7100fc4b34 g_unk_7172759c10;
extern UnkDef_7100fc4b34 g_unk_7172759c50;
extern UnkDef_7100fc4b34 g_unk_7172759c90;
extern UnkDef_7100fc4b34 g_unk_7172759d50;
extern UnkDef_7100fc4b34 g_unk_7172759d90;
extern UnkDef_7100fc4b34 g_unk_717275af40;
extern UnkDef_7100fc4b34 g_unk_717275af80;
extern UnkDef_7100fc4b34 g_unk_717275afc0;
extern UnkDef_7100fc4b34 g_unk_717275b000;
extern UnkDef_7100fc4b34 g_unk_717275b040;
extern UnkDef_7100fc4b34 g_unk_717275b080;
extern UnkDef_7100fc4b34 g_unk_7172758cf0;
extern UnkDef_7100fc4b34 g_unk_7172758d30;
extern UnkDef_7100fc4b34 g_unk_7172758db0;
extern UnkDef_7100fc4b34 g_unk_71729dbc00;
extern UnkDef_7100fc4b34 g_unk_7172759cd0;
extern UnkDef_7100fc4b34 g_unk_7172759d10;
extern UnkDef_7100fc4b34 g_unk_7172759dd0;
extern UnkDef_7100fc4b34 g_unk_717275a1d8;
extern UnkDef_7100fc4b34 g_unk_717275a218;
extern UnkDef_7100fc4b34 g_unk_717275a258;
extern UnkDef_7100fc4b34 g_unk_717275a298;
extern UnkDef_7100fc4b34 g_unk_717275a818;
extern UnkDef_7100fc4b34 g_unk_717275a2d8;
extern UnkDef_7100fc4b34 g_unk_717275a318;
extern UnkDef_7100fc4b34 g_unk_717275a398;
extern UnkDef_7100fc4b34 g_unk_717275a358;
extern UnkDef_7100fc4b34 g_unk_717275a3d8;
extern UnkDef_7100fc4b34 g_unk_717275a018;
extern UnkDef_7100fc4b34 g_unk_7172759fd8;
extern UnkDef_7100fc4b34 g_unk_71727594b8;
extern UnkDef_7100fc4b34 g_unk_71729dbc40;
extern UnkDef_7100fc4b34 g_unk_71729dbc80;
extern UnkDef_7100fc4b34 g_unk_71729dbcc0;
extern UnkDef_7100fc4b34 g_unk_71729dbd00;
extern UnkDef_7100fc4b34 g_unk_71729dbd40;
extern UnkDef_7100fc4b34 g_unk_71729dbd80;
extern UnkDef_7100fc4b34 g_unk_717275b108;
extern UnkDef_7100fc4b34 g_unk_717275b148;
extern UnkNamedDef_7100fc4b34 g_unk_7172759e10;

struct UnkTuning_7100fc4b34 {
    u8 unk_0x0[0x4];
    f32 field_0x4;
    f32 field_0x8;
    f32 field_0xc;
    f32 field_0x10;
    f32 field_0x14;
    bool field_0x18;
    bool field_0x19;
    bool field_0x1a;
    bool field_0x1b;
    bool field_0x1c;
    bool field_0x1d;
    bool field_0x1e;
    bool field_0x1f;
    bool field_0x20;
    bool field_0x21;
    bool field_0x22;
    bool field_0x23;
    bool field_0x24;
    bool field_0x25;
    bool field_0x26;
    bool field_0x27;
    bool field_0x28;
    u8 unk_0x29[0x3];
    s32 field_0x2c;
    u8 unk_0x30[0x9];
    u8 field_0x39;
    u8 field_0x3a;
    u8 unk_0x3b[0x9];
    u8 field_0x44;
    u8 field_0x45;
    u8 unk_0x46[0x2];
    s32 field_0x48;
    bool field_0x4c;
    bool field_0x4d;
    bool field_0x4e;
    u8 unk_0x4f;
    u32 field_0x50;
    f32 field_0x54;
    f32 field_0x58;
    f32 field_0x5c;
    f32 field_0x60;
    f32 field_0x64;
    bool field_0x68;
    bool field_0x69;
    u8 unk_0x6a[0x2];
    f32 field_0x6c;
    f32 field_0x70;
    f32 field_0x74;
    f32 field_0x78;
    f32 field_0x7c;
    UnkString_7100fc4b34 field_0x80;
};

struct UnkOwner_7100fc4b34;

bool fn_7100fc4b34(UnkOwner_7100fc4b34* self, CBlackboard* a, CBlackboard* b, UnkTuning_7100fc4b34* out) {
    out->field_0x4 = {};
    Get<fn_7100395b34>(b, Key(0x1bd8), Qid(g_unk_717275a118), &out->field_0x4);
    out->field_0x8 = {};
    Get<fn_7100395b34>(b, Key(0x1bd8), Qid(g_unk_717275a058), &out->field_0x8);
    out->field_0x18 = {};
    Get<fn_710006d11c>(a, Key(0x2050), Qid(g_unk_7172759ad0), &out->field_0x18);
    out->field_0x19 = {};
    Get<fn_710006d11c>(a, Key(0x2050), Qid(g_unk_7172759c10), &out->field_0x19);
    out->field_0x1a = {};
    Get<fn_710006d11c>(a, Key(0x2050), Qid(g_unk_7172759c50), &out->field_0x1a);
    out->field_0x1b = {};
    Get<fn_710006d11c>(a, Key(0x2050), Qid(g_unk_7172759c90), &out->field_0x1b);
    out->field_0x1c = {};
    Get<fn_710006d11c>(a, Key(0x2050), Qid(g_unk_7172759d50), &out->field_0x1c);
    out->field_0x2c = {};
    Get<fn_7100395a30>(a, Key(0x2050), Qid(g_unk_7172759d90), &out->field_0x2c);
    out->field_0x1d = {};
    Get<fn_710006d11c>(a, Key(0x2050), Qid(g_unk_717275af40), &out->field_0x1d);
    out->field_0x1e = {};
    Get<fn_710006d11c>(a, Key(0x2050), Qid(g_unk_717275af80), &out->field_0x1e);
    out->field_0x1f = {};
    Get<fn_710006d11c>(a, Key(0x2050), Qid(g_unk_717275afc0), &out->field_0x1f);
    out->field_0x20 = {};
    Get<fn_710006d11c>(a, Key(0x2050), Qid(g_unk_717275b000), &out->field_0x20);
    out->field_0x21 = {};
    Get<fn_710006d11c>(a, Key(0x2050), Qid(g_unk_717275b040), &out->field_0x21);
    out->field_0x22 = {};
    Get<fn_710006d11c>(a, Key(0x2050), Qid(g_unk_717275b080), &out->field_0x22);
    {
        UnkString_7100fc4b34 s;
        fn_710007eba0(&s, 0x40);
        fn_710007eaa0(reinterpret_cast<base::global::CRntString*>(&out->field_0x80),
                      reinterpret_cast<const base::global::CRntString*>(&s));
        fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(&s));
    }
    Get<fn_7100ec50e4>(a, Key(0x1148), Qid(g_unk_71727594b8), &out->field_0x80);
    f32 x = 0.0f;
    Get<fn_7100395b34>(b, Key(0x1bd8), Qid(g_unk_717275a018), &x);
    f32 y = 0.0f;
    Get<fn_7100395b34>(b, Key(0x1bd8), Qid(g_unk_7172759fd8), &y);
    out->field_0xc = x - y;
    out->field_0x10 = {};
    Get<fn_7100395b34>(b, Key(0x1148), Qid(g_unk_7172758cf0), &out->field_0x10);
    out->field_0x14 = {};
    Get<fn_7100395b34>(b, Key(0x1148), Qid(g_unk_7172758d30), &out->field_0x14);
    bool flag = false;
    Get<fn_710006d11c>(a, Key(0x2050), Qid(g_unk_71729dbc40), &flag);
    out->field_0x23 = flag;
    flag = false;
    if (out->field_0x23)
        Get<fn_710006d11c>(a, Key(0x2050), Qid(g_unk_71729dbc80), &flag);
    out->field_0x24 = flag;
    flag = false;
    Get<fn_710006d11c>(a, Key(0x2050), Qid(g_unk_71729dbcc0), &flag);
    out->field_0x25 = flag;
    flag = false;
    if (out->field_0x25)
        Get<fn_710006d11c>(a, Key(0x2050), Qid(g_unk_71729dbd00), &flag);
    out->field_0x26 = flag;
    flag = false;
    Get<fn_710006d11c>(a, Key(0x2050), Qid(g_unk_71729dbd40), &flag);
    out->field_0x27 = flag;
    flag = false;
    if (out->field_0x27)
        Get<fn_710006d11c>(a, Key(0x2050), Qid(g_unk_71729dbd80), &flag);
    out->field_0x28 = flag;
    out->field_0x39 = 0;
    s32 mode = 0;
    Get<fn_7100395a30>(a, Key(0x2050), SubId(g_unk_7172759e10, Key(0x1008)), &mode);
    out->field_0x44 = mode;
    out->field_0x3a = out->field_0x39 == 4 && out->field_0x44 == 4 ? 4 : 0;
    mode = 0;
    Get<fn_7100395a30>(a, Key(0x2050), SubId(g_unk_7172759e10, Key(0x1360)), &mode);
    out->field_0x45 = mode;
    if (out->field_0x45 < 4 && out->field_0x44 == 4)
        out->field_0x44 = 0;
    out->field_0x48 = {};
    Get<fn_7100395a30>(b, Key(0x1148), Qid(g_unk_7172758db0), &out->field_0x48);
    out->field_0x4c = {};
    Get<fn_710006d11c>(a, Key(0x2050), Qid(g_unk_71729dbc00), &out->field_0x4c);
    out->field_0x4d = {};
    Get<fn_710006d11c>(a, Key(0x2050), Qid(g_unk_7172759cd0), &out->field_0x4d);
    out->field_0x4e = {};
    Get<fn_710006d11c>(a, Key(0x2050), Qid(g_unk_7172759d10), &out->field_0x4e);
    out->field_0x50 = {};
    Get<fn_7100a68fb0>(a, Key(0x2050), Qid(g_unk_7172759dd0), &out->field_0x50);
    out->field_0x54 = {};
    Get<fn_7100395b34>(b, Key(0x1bd8), Qid(g_unk_717275a1d8), &out->field_0x54);
    out->field_0x58 = {};
    Get<fn_7100395b34>(b, Key(0x1bd8), Qid(g_unk_717275a218), &out->field_0x58);
    out->field_0x5c = {};
    Get<fn_7100395b34>(b, Key(0x1bd8), Qid(g_unk_717275a258), &out->field_0x5c);
    out->field_0x60 = {};
    Get<fn_7100395b34>(b, Key(0x1bd8), Qid(g_unk_717275a298), &out->field_0x60);
    out->field_0x64 = {};
    Get<fn_7100395b34>(b, Key(0x1bd8), Qid(g_unk_717275a818), &out->field_0x64);
    out->field_0x6c = {};
    Get<fn_7100395b34>(b, Key(0x1bd8), Qid(g_unk_717275a2d8), &out->field_0x6c);
    out->field_0x70 = {};
    Get<fn_7100395b34>(b, Key(0x1bd8), Qid(g_unk_717275a318), &out->field_0x70);
    out->field_0x74 = {};
    Get<fn_7100395b34>(b, Key(0x1bd8), Qid(g_unk_717275a398), &out->field_0x74);
    out->field_0x78 = {};
    Get<fn_7100395b34>(b, Key(0x1bd8), Qid(g_unk_717275a358), &out->field_0x78);
    out->field_0x7c = {};
    Get<fn_7100395b34>(b, Key(0x1bd8), Qid(g_unk_717275a3d8), &out->field_0x7c);
    out->field_0x68 = false;
    bool in_a = false;
    bool in_b = false;
    Get<fn_710006d11c>(a, Key(0x5a0), Qid(g_unk_717275b108), &in_a);
    Get<fn_710006d11c>(b, Key(0x5a0), Qid(g_unk_717275b108), &in_b);
    if (in_a && !in_b)
        out->field_0x68 = true;
    out->field_0x69 = false;
    bool in_a_2 = false;
    bool in_b_2 = false;
    Get<fn_710006d11c>(a, Key(0x5a0), Qid(g_unk_717275b148), &in_a_2);
    Get<fn_710006d11c>(b, Key(0x5a0), Qid(g_unk_717275b148), &in_b_2);
    if (in_a_2 && !in_b_2)
        out->field_0x69 = true;
    return true;
}
