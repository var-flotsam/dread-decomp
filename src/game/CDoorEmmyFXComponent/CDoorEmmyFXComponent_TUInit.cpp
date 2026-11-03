// CDoorEmmyFXComponent's static initialiser: the unit header, the char class's and the component's
// registrations, two parameter names, the component's id and two event names.
#include "base/UnkStrIdRegistry.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUInit.h"

// The unit's data (its statics, merged into one block).
struct UnkDoorEmmyFXTU {
    UnkTUHeader header;
    u64 field_0x18;
    u64 field_0x20;
    CStrId position;    // 0x28
    CStrId opening;     // 0x30
    CStrId id;          // 0x38
    CStrId magma_event; // 0x40
    CStrId emmy_chase;  // 0x48
    u8 field_0x50[0x20];
    UnkReflType char_class_type;  // 0x70
    u8 field_0x70_rest[0x1a8 - 0x70 - sizeof(UnkReflType)];
    UnkReflType type;   // 0x1a8
};

extern UnkDoorEmmyFXTU g_unk_717271a550 __attribute__((visibility("hidden")));
extern char g_unk_717271a548;
extern char g_unk_717271a549;

// The id lives in an inline function: its guard and object are reached through the GOT.
inline const CStrId& DoorEmmyFXId() {
    static CStrId s_id("DOOREMMYFX", true);
    return s_id;
}

void fn_71007b7190() {
    UnkDoorEmmyFXTU& tu = g_unk_717271a550;
    InitHeader(tu.header);
    fn_71007b4fb0();
    fn_710009ec00(&g_unk_717271a548, &tu.char_class_type);
    fn_71007b55e4();
    fn_710009ec00(&g_unk_717271a549, &tu.type);
    new (&tu.position) CStrId("fPosition", true);
    fn_7100000250(fn_710000051c, &tu.position, &g_unk_7101a19000);
    new (&tu.opening) CStrId("fOpening", true);
    fn_7100000250(fn_710000051c, &tu.opening, &g_unk_7101a19000);
    new (&tu.id) CStrId(fn_7100de8f78(DoorEmmyFXId()));
    fn_7100000250(fn_710000051c, &tu.id, &g_unk_7101a19000);
    new (&tu.magma_event) CStrId("MagmaEvent", true);
    fn_7100000250(fn_710000051c, &tu.magma_event, &g_unk_7101a19000);
    new (&tu.emmy_chase) CStrId("EmmyChase", true);
    fn_7100000250(fn_710000051c, &tu.emmy_chase, &g_unk_7101a19000);
}
