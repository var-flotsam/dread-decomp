// CCharClassActivatableComponent's static initialiser: the unit header, the component's id, two
// bool properties, and the char class's and the component's registrations.
#include "base/reflection/UnkClassGetters.h"
#include "game/CCharClassActivatableComponent.h"
#include "game/UnkTUInit.h"
#include "game/UnkTUProperty.h"

// The unit's data (its statics, merged into one block).
struct UnkCharClassActivatableTU {
    UnkTUHeader header;
    u64 field_0x18;
    u64 field_0x20;
    UnkReflType char_class_type;  // 0x28
    u8 field_0x28_rest[0x160 - 0x28 - sizeof(UnkReflType)];
    UnkReflType type;  // 0x160
};

extern UnkCharClassActivatableTU g_unk_717272a488 __attribute__((visibility("hidden")));
extern CStrId g_unk_717272a3e8;
extern char g_unk_7101a7ada8[];
void fn_7100878ac0(UnkProperty* p);
void fn_7100877b90(void* p);
extern char g_unk_717272a440;
extern char g_unk_717272a441;

void fn_7100878e20() {
    UnkCharClassActivatableTU& tu = g_unk_717272a488;
    InitHeader(tu.header);
    new (&g_unk_717272a3e8) CStrId("ACTIVATABLE", true);
    fn_7100000250(fn_710000051c, &g_unk_717272a3e8, &g_unk_7101a19000);
    InitPropertyVtableFirst(&g_unk_717272a3f0, reinterpret_cast<void**>(g_unk_7101a7ada8), g_unk_71726db788,
                            "Activated", refl_TypeOf<fn_710000d228>(), fn_7100878ac0, fn_7100877b90);
    InitProperty(&g_unk_717272a418, reinterpret_cast<void**>(g_unk_7101a7ada8), g_unk_71726db788, "Enabled",
                 refl_TypeOf<fn_710000d228>(), fn_7100878ac0, fn_7100877b90);
    fn_7100877c0c();
    fn_710009ec00(&g_unk_717272a440, &tu.char_class_type);
    fn_7100877e8c();
    fn_710009ec00(&g_unk_717272a441, &tu.type);
}
