// Unit 1095 (behaviortree::CXParasiteGoToPointAction): picks the point to go to, the position of the
// entity the blackboard value at +0x38 names (or the point its AI marks on its model), else the point
// the key at +0x30 names, and keeps it and its offset from the owner; otherwise the owner flees.
// Names and layouts are placeholders.
#include "../../../src/game_aca57c/UnkBufferString.h"
#include "base/UnkMatrix44.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1102.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU54.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

struct UnkMatrix44Words {
    u32 m[16];

    UnkMatrix44Words() {}
    UnkMatrix44Words(const UnkMatrix44Words& other);
};
UnkMatrix44Words fn_7100076dc4(const UnkMatrix44Words* src);

// The address of a temporary, for the callees that take their matrix by pointer.
template <class T>
static inline const T* AddressOf(const T& value) {
    return &value;
}

// The AI component's marked node (its id) and the offset from it.
const CStrId* fn_710110f224(void* ai);
const UnkVec3f* fn_710110f230(void* ai);

struct UnkGoToPoint_l6e2b9c {
    u8 pad_0x0[0x18];
    UnkActor* owner;  // 0x18
    u8 pad_0x20[0x10];
    CStrId point_key;   // 0x30
    CStrId entity_key;  // 0x38
    u8 pad_0x40[0x4];
    CVector3D target;  // 0x44
    CVector3D offset;  // 0x50
};

// A string id built from a string out of line (fn_7100000850).
struct UnkBuiltId_l6e2b9c {
    UnkStringData* data;

    explicit UnkBuiltId_l6e2b9c(const CRntString& str) {
        CStrId_ConstructFromRntString(reinterpret_cast<base::global::UnkStrIdBase*>(this), str, true);
    }
    ~UnkBuiltId_l6e2b9c() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

bool fn_71006e2b9c(UnkGoToPoint_l6e2b9c* self) {
    UnkBufferString<0x100, g_unk_7101a194b8> name;
    if (self->entity_key.data != &g_unk_7101d47108->empty &&
        CBehavior_ReadStringProperty(reinterpret_cast<UnkStringEqualsCondition_A2_b72*>(self), &self->entity_key,
                      reinterpret_cast<UnkInlineString_A2_b72*>(&name))) {
        UnkGameManager* manager = g_unk_7101d47310;
        UnkActor* found;
        {
            u64 key;
            alignas(8) u32 kind;
            UnkBuiltId_l6e2b9c id(*name.Str());
            u32 kindValue = g_unk_71727bebb8;
            key = id.data->Key();
            kind = kindValue;
            found = reinterpret_cast<UnkActor*>(fn_7100debbc8(manager, &key, &kind));
        }
        if (found) {
            f32 x = found->field_0x44;
            f32 y = found->field_0x54;
            f32 z = found->field_0x64;
            self->target.x = x;
            self->target.y = y;
            self->target.z = z;
            if (void* ai = found->field_0x240) {
                UnkEntity* shape = static_cast<UnkEntity*>(found->field_0x228);
                const CStrId* node = fn_710110f224(ai);
                if (shape && node->data != &g_unk_7101d47108->empty) {
                    UnkMatrix44 m = {};
                    m.m[15] = 1.0f;
                    m.m[10] = 1.0f;
                    m.m[5] = 1.0f;
                    m.m[0] = 1.0f;
                    bool placed;
                    {
                        u64 key = fn_710110f224(ai)->data->Key();
                        placed = fn_71011735e8(shape, &key, &m, 0);
                    }
                    if (placed) {
                        self->target.x = m.m[3];
                        self->target.y = m.m[7];
                        self->target.z = m.m[11];
                        // The offset turned by the node's rotation (the matrix without its translation).
                        UnkVec3f moved = fn_7100078170(
                            reinterpret_cast<const UnkMat4f*>(
                                AddressOf(fn_7100076dc4(reinterpret_cast<const UnkMatrix44Words*>(&m)))),
                            fn_710110f230(ai));
                        self->target.x = moved.x + self->target.x;
                        self->target.y = moved.y + self->target.y;
                        self->target.z = moved.z + self->target.z;
                    }
                }
            }
            UnkActor* owner = self->owner;
            self->offset.x = self->target.x - owner->field_0x44;
            self->offset.y = self->target.y - owner->field_0x54;
            self->offset.z = self->target.z - owner->field_0x64;
            return true;
        }
    } else if (self->point_key.data != &g_unk_7101d47108->empty &&
               CBehavior_LoadVector3DValue(reinterpret_cast<UnkSelf*>(self), &self->point_key, &self->target)) {
        return true;
    }
    UnkReflObject* ai = static_cast<UnkReflObject*>(self->owner->field_0x240);
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_71006e5f28()))
        XParasiteAI_FleeFromTarget(reinterpret_cast<UnkXParasiteAI*>(ai));
    return false;
}
