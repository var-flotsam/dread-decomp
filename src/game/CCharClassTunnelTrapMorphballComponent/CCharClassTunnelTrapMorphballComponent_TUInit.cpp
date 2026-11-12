// CCharClassTunnelTrapMorphballComponent's static initialiser: the unit header, the char class's
// and the component's registrations, the component's id, two bool properties and four names.
#include "base/reflection/UnkClassGetters.h"
#include "game/CCharClassTunnelTrapMorphballComponent.h"
#include "game/UnkTUInit.h"
#include "game/UnkTUProperty.h"

// The unit's data (its statics, merged into one block).
struct UnkTunnelTrapMorphballTU {
    UnkTUHeader header;
    u64 field_0x18;
    u64 field_0x20;
    CStrId activation_trap;     // 0x28
    CStrId trap_block_initial;  // 0x30
    CStrId trap_block_final;    // 0x38
    CStrId forced_movement;     // 0x40
    u8 field_0x48[0x10];
    UnkReflType char_class_type;  // 0x58
    u8 field_0x58_rest[0x190 - 0x58 - sizeof(UnkReflType)];
    UnkReflType type;  // 0x190
};

extern UnkTunnelTrapMorphballTU g_unk_717274a6b0 __attribute__((visibility("hidden")));
extern char g_unk_717274a610;
extern char g_unk_717274a611;
extern CStrId g_unk_717274a618;
extern char g_unk_7101a8e8c0[];
void fn_7100a44ff4(UnkProperty* p);
void fn_7100a43538(void* p);

void fn_7100a45160() {
    UnkTunnelTrapMorphballTU& tu = g_unk_717274a6b0;
    InitHeader(tu.header);
    fn_7100a42d74();
    fn_710009ec00(&g_unk_717274a610, &tu.char_class_type);
    fn_7100a430bc();
    fn_710009ec00(&g_unk_717274a611, &tu.type);
    new (&g_unk_717274a618) CStrId("TUNNEL_TRAP", true);
    fn_7100000250(fn_710000051c, &g_unk_717274a618, &g_unk_7101a19000);
    InitPropertyVtableFirst(&g_unk_717274a620, reinterpret_cast<void**>(g_unk_7101a8e8c0), g_unk_71726db788,
                            "Activated", refl_TypeOf<fn_710000d228>(), fn_7100a44ff4, fn_7100a43538);
    InitProperty(&g_unk_717274a648, reinterpret_cast<void**>(g_unk_7101a8e8c0), g_unk_71726db788, "ForcedOpened",
                 refl_TypeOf<fn_710000d228>(), fn_7100a44ff4, fn_7100a43538);
    new (&tu.activation_trap) CStrId("activationtrap", true);
    fn_7100000250(fn_710000051c, &tu.activation_trap, &g_unk_7101a19000);
    new (&tu.trap_block_initial) CStrId("trapblockinitial", true);
    fn_7100000250(fn_710000051c, &tu.trap_block_initial, &g_unk_7101a19000);
    new (&tu.trap_block_final) CStrId("trapblockfinal", true);
    fn_7100000250(fn_710000051c, &tu.trap_block_final, &g_unk_7101a19000);
    new (&tu.forced_movement) CStrId("ForcedMovementAreaComponent", true);
    fn_7100000250(fn_710000051c, &tu.forced_movement, &g_unk_7101a19000);
}
