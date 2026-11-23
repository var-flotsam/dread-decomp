// Static initialiser of the unit of the CObsydomithonAttack::EState enum: the header ids, the
// class registration, the enum, the animation ids and two vectors. Names and layouts are placeholders.
#include "../../../src/game_5821e4/UnkStaticInit.h"
#include "../../../src/game_s7/UnkStaticInitS7.h"
#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"

using namespace unk_static_init;

struct UnkObsydomithonEntry;
using UnkObsydomithonVector = base::global::CRntVector<UnkObsydomithonEntry>;

struct __attribute__((packed, aligned(4))) UnkStatics_710062ae30 {
    HeaderIds4 ids;
    u8 pad_0x14[0x8];
    UnkStrIdBase field_0x1c;
    UnkStrIdBase field_0x24;
    UnkStrIdBase field_0x2c;
    UnkStrIdBase field_0x34;
    UnkStrIdBase field_0x3c;
    UnkStrIdBase field_0x44;
    UnkStrIdBase field_0x4c;
    UnkStrIdBase field_0x54;
    UnkStrIdBase field_0x5c;
    UnkStrIdBase field_0x64;
    UnkObsydomithonVector field_0x6c;
    UnkObsydomithonVector field_0x94;
    u8 field_0xbc[8];
};
static_assert(__builtin_offsetof(UnkStatics_710062ae30, field_0xbc) == 0xbc, "layout");
extern UnkStatics_710062ae30 g_unk_71726f8dcc __attribute__((visibility("hidden")));
extern char g_unk_71726f8c60[];
extern UnkEnumType g_unk_71726f8c68;
void fn_71006284d4(UnkEnumType* type, const CStrId& name, void (*factory)(void*), void* arg);
void fn_71006281a8(void* e);
extern char g_unk_71726f8d80[];
extern UnkConversion g_unk_71726f8d88;
extern UnkConversion g_unk_71726f8da8;
void fn_710062a990() __attribute__((visibility("hidden")));
void fn_710062a99c() __attribute__((visibility("hidden")));
void fn_71003da2a0(void* self);

// A vector of the unit built with allocation flags 0xf, its destructor registered.
static inline void InitEntryVector(UnkObsydomithonVector* vector) {
    new (vector) UnkObsydomithonVector(0xf);
    fn_7100000250(fn_71003da2a0, vector, g_unk_7101a19000);
}

void fn_710062ae30() {
    InitHeaderIds(&g_unk_71726f8dcc.ids);
    fn_7100627f2c();
    fn_710009ec00(g_unk_71726f8c60, g_unk_71726f8dcc.field_0xbc);
    InitEnum(&g_unk_71726f8c68, "CObsydomithonAttack::EState", fn_71006284d4, fn_71006281a8,
             g_unk_71726f8d80, &g_unk_71726f8d88, fn_710062a990, &g_unk_71726f8da8, fn_710062a99c);
    InitId(&g_unk_71726f8dcc.field_0x1c, "relax");
    InitId(&g_unk_71726f8dcc.field_0x24, "walk_to_charge");
    InitId(&g_unk_71726f8dcc.field_0x2c, "relax_to_charge");
    InitId(&g_unk_71726f8dcc.field_0x34, "charge_init");
    InitId(&g_unk_71726f8dcc.field_0x3c, "attack_init");
    InitId(&g_unk_71726f8dcc.field_0x44, "attack_end");
    InitId(&g_unk_71726f8dcc.field_0x4c, "attack_to_relax");
    InitId(&g_unk_71726f8dcc.field_0x54, "attack_to_relax_turn");
    InitId(&g_unk_71726f8dcc.field_0x5c, "blend_to_relax");
    InitEntryVector(&g_unk_71726f8dcc.field_0x6c);
    InitEntryVector(&g_unk_71726f8dcc.field_0x94);
    InitId(&g_unk_71726f8dcc.field_0x64, "attack_heat");
}
