// The timeline component's resource entry for an id (game/UnkTimelineComponent.h): through the owner
// (+0x30) and its character class (+0x28, when it is a CCharClass), the class's action set (+0xf8, a
// counted resource held for the lookup) is searched for the id (remapped first through the owner's
// +0x240 object when there is one) paired with a constant second key. Names and layouts are
// placeholders.
#include "base/UnkCountedResource.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTimelineComponent.h"

struct UnkActionSetMap;
void* fn_7100b8c61c(UnkActionSetMap* self, const u64* key1, const u64* key2);  // src/game/CActionSet.cpp
const u64* fn_7101111d7c(void* remap, const u64* id);  // the remapped id (its object's vf_0x650)
extern const u64 g_unk_710164b510 __attribute__((visibility("hidden")));  // the second key

struct UnkTimelineCharClass {
    u8 pad_0x0[0xf8];
    UnkCountedResource* actions;
};

struct UnkTimelineOwner {
    u8 pad_0x0[0x28];
    base::reflection::UnkReflObject* char_class;
    u8 pad_0x30[0x210];
    void* remap;  // 0x240
};

struct UnkTimelineComponentOwner {
    u8 pad_0x0[0x30];
    UnkTimelineOwner* owner;
};

UnkTimelineResourceEntry* fn_7100a2c13c(UnkTimelineComponent* self, const u64* id) {
    UnkTimelineOwner* owner = reinterpret_cast<UnkTimelineComponentOwner*>(self)->owner;
    base::reflection::UnkReflObject* object = owner->char_class;
    if (!fn_710009d50c(object->vf_0x0(), fn_71011abbd4()))
        object = nullptr;
    // The cast's result is used unchecked; the pin keeps Clang from assuming it is not null.
    asm("" : "+r"(object));
    UnkCountedResource* actions = reinterpret_cast<UnkTimelineCharClass*>(object)->actions;
    if (!actions)
        return nullptr;
    ++actions->ref_count;
    if (void* remap = reinterpret_cast<UnkTimelineComponentOwner*>(self)->owner->remap)
        id = fn_7101111d7c(remap, id);
    void* entry = fn_7100b8c61c(reinterpret_cast<UnkActionSetMap*>(actions), id, &g_unk_710164b510);
    if (--actions->ref_count == 0)
        fn_710001737c(actions);
    return static_cast<UnkTimelineResourceEntry*>(entry);
}
