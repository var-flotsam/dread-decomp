#pragma once

// The 0x68-byte value the music manager keeps under a name (its map at +0x1c0, filled by fn_7100fdca88; unit
// 2602's dictionary, fn_7100f899a8; the presets' first map, fn_7100f89678): the name, a list of 0x50-byte
// layers, another id and a list of polymorphic 0x20-byte items. Built by fn_7100f85a78 and destroyed by
// fn_7100d90304 (defined in src/base/reflection/reflection_environment_ovb0056.cpp). Names and layouts are
// placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "types.h"

// A layer (0x50 bytes), holding a list of path entries released by fn_7100d90460.
struct UnkMusicLayer {
    u8 unk_0x0[0x50];
};

// A polymorphic item (0x20 bytes), destroyed through its vf_0x8.
struct UnkMusicItem {
    virtual void vf_0x0();
    virtual void vf_0x8();
    u8 unk_0x8[0x18];
};

// The layers as a member: its destructor truncates them through fn_7100d90460, then frees the array.
struct UnkMusicLayers : base::global::CRntVector<UnkMusicLayer> {
    UnkMusicLayers() = default;
    UnkMusicLayers(UnkMusicLayers&& other) = default;
    ~UnkMusicLayers();
};

// The items as a member: its destructor destroys each in order, then frees the array.
struct UnkMusicItems : base::global::CRntVector<UnkMusicItem> {
    UnkMusicItems() = default;
    UnkMusicItems(UnkMusicItems&& other) = default;
    ~UnkMusicItems();
};

// The layers with an id of their own (0x30 bytes). One member, not two: fn_7100d90304 computes the pair's
// address once and releases the id through it.
struct UnkMusicLayerSet {
    UnkMusicLayers layers;      // 0x0
    base::UnkStrIdMember name;  // 0x28
};

struct UnkMusicMapValue {
    base::UnkStrIdMember name;    // 0x0
    UnkMusicLayerSet layer_set;   // 0x8
    u32 field_0x38;               // 0x38
    UnkMusicItems items;          // 0x40

    UnkMusicMapValue() asm("fn_7100f85a78");
    UnkMusicMapValue(UnkMusicMapValue&& other) = default;
    ~UnkMusicMapValue() asm("fn_7100d90304");
};
static_assert(sizeof(UnkMusicMapValue) == 0x68, "UnkMusicMapValue layout");

// Adds the layers of the name `name` (a string id) to a value's layer list.
void fn_7100fed650(char* layers, char* name);

// Destroys the value at p: the destroy step the maps of these values take as a function.
inline void DestroyMusicMapValue(void* p) {
    static_cast<UnkMusicMapValue*>(p)->~UnkMusicMapValue();
}
