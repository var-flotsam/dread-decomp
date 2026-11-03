// CCharClassActivatableEntitiesEvent (unit 1736 of tools/tu_map.py, the name from reflection): when
// the owner entity has the component fn_7100b96bb8 finds, passes it a copy of the event's string
// id (+0x10) with fn_71008910a0. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkActivatableEntity;
struct UnkActivatableComponent;
UnkActivatableComponent* fn_7100b96bb8(UnkActivatableEntity* entity);
void fn_71008910a0(UnkActivatableComponent* component, CStrId* name, u32 mode);

struct UnkActivatableEntitiesEvent {
    u8 unk_0x0[0x10];
    CStrId name;
};

void fn_7100b96b18(UnkActivatableEntitiesEvent* self, UnkActivatableEntity** owner) {
    if (!*owner)
        return;
    UnkActivatableComponent* component = fn_7100b96bb8(*owner);
    if (!component)
        return;
    CStrId name(self->name);
    fn_71008910a0(component, &name, 0);
}
