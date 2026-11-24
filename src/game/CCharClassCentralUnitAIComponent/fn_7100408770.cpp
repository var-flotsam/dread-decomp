// Asks the global registry (created on first use) for the value named by a fixed string id and the
// string id at +0x510 (each use counted on its string data at 0x44; the first text defaults to
// the empty string) and returns it; 0 when nothing is found.
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkTunableRegistry.h"
#include "game/CCharClassLifeComponent.h"
#include "game/UnkNamedString.h"
#include "types.h"

using base::reflection::UnkTunableRegistry;

struct UnkSelf_408770 {
    u8 unk_0x0[0x510];
    UnkNamedString* name;
};

void fn_710013c7e0(UnkTunableRegistry* registry, const char* group, const char* name, f32* value);

f32 CCharClassCentralUnitAIComponent_GetArmorLifeTunableValue(UnkSelf_408770* self) {
    f32 found = 0.0f;
    UnkTunableRegistry* registry = g_unk_7101d47350;
    if (!registry) {
        registry = static_cast<UnkTunableRegistry*>(fn_71000839a0(0x58, 0x3f));
        CTunableManager_Construct(registry);
        g_unk_7101d47350 = registry;
    }
    ++g_unk_717272fd38.group->uses;
    const char* group = g_unk_717272fd38.group->text ? g_unk_717272fd38.group->text : "";
    ++self->name->uses;
    const char* name = self->name->text;
    fn_710013c7e0(registry, group, name, &found);
    return found;
}
