// CEntity's unit (tools/tu_map.py unit 2523): after the base initialisation (fn_7100232160) succeeds,
// takes the source's two flags, copies the character class's physical settings, and keeps a copy of
// every sound path the class lists that the sound system takes. Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/snd/CSoundSystem.h"
#include "game/UnkTU23.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::reflection::UnkReflObject;

// As src/game/UnkTU341/UnkTU341_Actor_ovb0226.cpp defines them.
struct UnkActor_7100232160;
struct UnkSource_7100232160;
bool fn_7100232160(UnkActor_7100232160* self, const UnkSource_7100232160* other);
void fn_71002337c4(UnkActor_7100232160* self, bool flag);

// A sound path the character class lists.
struct UnkPathNode_ld77f54 {
    CFilePathStrId path;
    UnkPathNode_ld77f54* next;
};

// The character class's members read here.
struct UnkCharClass_ld77f54 {
    u8 pad_0x0[0x110];
    UnkPathNode_ld77f54* paths;  // 0x110
    u8 pad_0x118[0x24];
    f32 field_0x13c;
    f32 field_0x140;
    f32 scale;  // 0x144
    f32 field_0x148;
    base::math::CVector3D field_0x14c;
    f32 field_0x158;
};
static_assert(__builtin_offsetof(UnkCharClass_ld77f54, field_0x158) == 0x158, "layout");

struct UnkSource_ld77f54 {
    u8 pad_0x0[0x10];
    bool field_0x10;
    bool field_0x11;
};

struct UnkPathList_ld77f54 {
    u32 count;
    UnkPathNode_ld77f54* head;
    UnkPathNode_ld77f54* tail;
    base::UnkAllocator* allocator;
    u32 flags;

    void PushBack(const CFilePathStrId& path) {
        auto* node = static_cast<UnkPathNode_ld77f54*>(allocator->vf_0x40(sizeof(UnkPathNode_ld77f54), 8, flags));
        if (node) {
            new (&node->path) CFilePathStrId(path);
            node->next = nullptr;
            if (count == 0) {
                head = node;
                tail = node;
            } else {
                tail->next = node;
                node->next = nullptr;
                tail = node;
            }
            count++;
        }
    }
};

struct UnkEntity_ld77f54 {
    u8 pad_0x0[0x28];
    UnkReflObject* char_class;  // 0x28
    u8 pad_0x30[0x64];
    base::math::CVector3D scale;  // 0x94
    u8 pad_0xa0[0x2b0];
    UnkPathList_ld77f54 paths;  // 0x350
    u8 pad_0x378[0x4];
    f32 field_0x37c;
    f32 field_0x380;
    f32 field_0x384;
    base::math::CVector3D field_0x388;
    f32 field_0x394;
    u8 pad_0x398[0x38];
    bool field_0x3d0;
};
static_assert(__builtin_offsetof(UnkEntity_ld77f54, paths) == 0x350, "layout");
static_assert(__builtin_offsetof(UnkEntity_ld77f54, field_0x3d0) == 0x3d0, "layout");

static inline UnkCharClass_ld77f54* AsCharClass(UnkReflObject* object) {
    return fn_710009d50c(object->vf_0x0(), fn_71011abbd4()) ? reinterpret_cast<UnkCharClass_ld77f54*>(object)
                                                            : nullptr;
}

// fn_710010808c, which base/snd/CSoundSystem.h declares void: the original reads its result, whether
// the sound system took the path.
static inline bool TakePath(const CFilePathStrId* path) {
    using TakeFn = bool (*)(UnkSoundMgr*, UnkPathNode*);
    return reinterpret_cast<TakeFn>(fn_710010808c)(g_unk_7101d4d820,
                                                   reinterpret_cast<UnkPathNode*>(const_cast<CFilePathStrId*>(path)));
}

bool fn_7100d77f54(UnkEntity_ld77f54* self, const UnkSource_ld77f54* source) {
    bool ok = fn_7100232160(reinterpret_cast<UnkActor_7100232160*>(self),
                            reinterpret_cast<const UnkSource_7100232160*>(source));
    if (ok) {
        fn_71002337c4(reinterpret_cast<UnkActor_7100232160*>(self), source->field_0x10);
        self->field_0x3d0 = source->field_0x11;
        if (UnkCharClass_ld77f54* char_class = AsCharClass(self->char_class)) {
            self->field_0x37c = char_class->field_0x13c;
            self->field_0x380 = char_class->field_0x140;
            self->field_0x384 = char_class->field_0x148;
            self->field_0x388 = char_class->field_0x14c;
            self->field_0x394 = char_class->field_0x158;
            f32 scale = char_class->scale;
            self->scale.x = scale;
            self->scale.y = scale;
            self->scale.z = scale;
        }
        for (UnkPathNode_ld77f54* node = AsCharClass(self->char_class)->paths; node; node = node->next) {
            if (TakePath(&node->path))
                self->paths.PushBack(node->path);
        }
    }
    return ok;
}
