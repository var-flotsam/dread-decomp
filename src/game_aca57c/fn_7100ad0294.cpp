// Whether two entities can interact through their models: unless `force` is set, neither may
// be excluded by fn_71002b005c; the other entity must pass fn_7100ad00ac, and both need a model
// (at 0x68 of their object at 0x18). The answer comes from the navmesh manager
// (game/UnkNavMeshManager.h), created on first use.
#include "game/UnkNavMeshManager.h"
#include "types.h"

struct UnkModel;
struct UnkEntityObj {
    u8 unk_0x0[0x68];
    UnkModel* model;
};
struct UnkEntity {
    u8 unk_0x0[0x18];
    UnkEntityObj* obj;
};

void* fn_71000839a0(u64 size, u32 flags);
bool fn_71002b005c(UnkEntityObj* obj);
bool fn_7100ad00ac(UnkEntity* entity, u32 mode);
bool fn_7100ec1454(UnkNavMeshManager* manager, UnkModel* a, UnkModel* b, bool flag_a, bool flag_b);

static inline bool IsExcluded(UnkEntity* entity) { return entity->obj && fn_71002b005c(entity->obj); }

bool fn_7100ad0294(UnkEntity* self, bool force, u32 mode, UnkEntity* other) {
    if (!other)
        return false;
    if (!force && (IsExcluded(self) || IsExcluded(other)))
        return false;
    if (!fn_7100ad00ac(other, mode))
        return false;
    if (!self->obj || !other->obj || !self->obj->model || !other->obj->model)
        return false;
    UnkNavMeshManager* registry = g_unk_71726a4510;
    if (!registry) {
        registry = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(registry);
        g_unk_71726a4510 = registry;
    }
    return fn_7100ec1454(registry, self->obj->model, other->obj->model, true, false);
}
