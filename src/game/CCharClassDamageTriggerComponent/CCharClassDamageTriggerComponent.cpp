// CCharClassDamageTriggerComponent's static initialiser: the unit header, a default pair of -1.0
// values and a flag in the unit's data, the class's registration, then the CDamageTriggerConfig
// class type and CDamageTriggerComponent's registration.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUInit.h"

// The unit's data (its statics, merged into one block).
struct UnkCharClassDamageTriggerTU {
    UnkTUHeader header;
    u64 field_0x18;
    u64 field_0x20;
    float range[2];
    bool flag;
    UnkReflType type;  // at 0x38
};

extern UnkCharClassDamageTriggerTU g_unk_717274b038 __attribute__((visibility("hidden")));
extern char g_unk_717274aef0;
extern UnkReflType g_unk_717274aef8;
void fn_7100a4a924(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100a4a5c4(base::reflection::CClass* cls);  // src/reflection/registration_01.cpp
extern char g_unk_717274b030;
extern char g_unk_717274b031;
extern UnkReflType g_unk_717274b1a8 __attribute__((visibility("hidden")));

void fn_7100a4b640() {
    UnkCharClassDamageTriggerTU& tu = g_unk_717274b038;
    InitHeader(tu.header);
    tu.range[0] = -1.0f;
    tu.range[1] = -1.0f;
    tu.flag = true;
    fn_7100a49840();
    fn_710009ec00(&g_unk_717274aef0, &tu.type);
    InitClass(&g_unk_717274aef8, "CDamageTriggerConfig", fn_7100a4a924,
              reinterpret_cast<void (*)()>(fn_7100a4a5c4));
    fn_710009ec00(&g_unk_717274b030, &g_unk_717274aef8);
    fn_7100a4aaac();
    fn_710009ec00(&g_unk_717274b031, &g_unk_717274b1a8);
}
