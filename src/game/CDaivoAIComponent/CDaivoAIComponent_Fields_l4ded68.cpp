// CDaivoAIComponent's unit (tools/tu_map.py unit 766): its reflected member fields. The three swarm links
// (eLinkTarget), the base class's wpPathToFollow shown as "Swarm path" (with the flag at +0xa9 of its field
// descriptor set), and fChaseForcedDistanceToWall (default -1). Names and layouts are placeholders.
#include "base/global/CName.h"
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedFields.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkTU2536.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::UnkReflField;

struct UnkEnumType;
extern UnkEnumType g_unk_717269bb48;

// A field descriptor with the flag after its two attribute containers.
struct UnkDaivoField_l4ded68 : UnkReflField {
    u8 field_0xa8;
    bool field_0xa9;
};

static inline const base::reflection::CType* TypeOf(UnkReflType& t) {
    return reinterpret_cast<const base::reflection::CType*>(&t);
}
static inline const base::reflection::CType* TypeOf(UnkEnumType& t) {
    return reinterpret_cast<const base::reflection::CType*>(&t);
}

void fn_71004ded68(CClass* cls) {
    namespace attr = base::reflection::attr;
    UnkReflField* actor = AddReflectedField(cls, CStrId("wpSwarmActor", true), TypeOf(g_unk_717269bb48), 0x14840, -1, -1);
    actor->attrs.Add<s32>(attr::eLinkTarget(), TypeOf(g_unk_71727bc110), 0);
    UnkReflField* begin = AddReflectedField(cls, CStrId("wpSwarmAOIBegin", true), TypeOf(g_unk_717269bb48), 0x147c0, -1, -1);
    begin->attrs.Add<s32>(attr::eLinkTarget(), TypeOf(g_unk_71727bc110), 0);
    UnkReflField* end = AddReflectedField(cls, CStrId("wpSwarmAOIEnd", true), TypeOf(g_unk_717269bb48), 0x14800, -1, -1);
    end->attrs.Add<s32>(attr::eLinkTarget(), TypeOf(g_unk_71727bc110), 0);

    auto* path = static_cast<UnkDaivoField_l4ded68*>(
        base::reflection::refl_FindField(cls, base::global::CName("wpPathToFollow")));
    path->field_0xa9 = true;
    path->attrs.Add<CStrId>(attr::DisplayName(), CStrId("Swarm path", true));

    UnkReflField* distance = AddReflectedField(cls, CStrId("fChaseForcedDistanceToWall", true),
                                               base::reflection::UnkReflTypeOf<f32>::Get(), 0x14888, -1, -1);
    distance->attrs.Add<f32>(attr::DefaultValue(), -1.0f);
}
