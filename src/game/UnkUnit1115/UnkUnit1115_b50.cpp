// Unit 1115 of tools/tu_map.py (no class name). fn_71006fadf0: when the shape at +0xa8 of the
// object at +0x90 (taken when its vf_0x18 for the query reports kind 16) is hit by the query
// (fn_7100116de0, 0, 1, scale 1), the query is handed on (fn_71006f9368). Names and layouts are
// placeholders.
#include "types.h"
#include "UnkLeftoverReg.h"

struct UnkEntry;  // src/game_s2/fn_71005528fc.cpp
bool fn_7100116de0(UnkEntry* entry, void* query, u32 a, u32 b, f32 scale);

struct UnkQueryTarget_b50 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual u32 vf_0x18(void* query, u32 mode);
    u8 pad_0x8[0xa0];
    UnkEntry* shape;  // 0xa8
};

struct UnkQueryHolder_b50 {
    u8 pad_0x0[0x90];
    UnkQueryTarget_b50* target;
};

// An empty tag passed by value: it takes x1 without a value (the original leaves x1 as it is).
struct UnkQueryTag_b50 : UnkLeftoverReg {};
void fn_71006f9368(UnkQueryHolder_b50* self, UnkQueryTag_b50 tag, void* data, u32 mode);

void fn_71006fadf0(UnkQueryHolder_b50* self, char* query, u32 mode) {
    UnkQueryTarget_b50* target = self->target;
    UnkQueryTarget_b50* match = target->vf_0x18(query, mode) == 16 ? target : nullptr;
    if (fn_7100116de0(match->shape, query, 0, 1, 1.0f))
        fn_71006f9368(self, UnkQueryTag_b50{}, query + 8, mode);
}

// fn_71006fae70: the same test; on a hit every entry of the list at +0x68 (count +0x70) gets the
// query (fn_71006f8af4 with its data at +0x10) and the holder is told (fn_71006f9980). Then +0x50 is
// cleared and every entry is reset (fn_71006facbc) and filed by its id (+0x8) in the map at +0x40
// (fn_71006fb514).
struct UnkQueryEntry_b50 {
    u8 pad_0x0[0x8];
    u32 id;
    u8 pad_0xc[0x4];
    u8 data[0x8];  // 0x10
};

struct UnkQueryList_b50 {
    u8 pad_0x0[0x40];
    u8 map[0x10];  // 0x40
    u32 field_0x50;
    u8 pad_0x54[0x14];
    UnkQueryEntry_b50** entries;  // 0x68
    u32 count;                    // 0x70
};

void fn_71006f8af4(UnkQueryEntry_b50* entry, void* data, u32 mode);
void fn_71006f9980(UnkQueryList_b50* self, u32 mode);
void fn_71006facbc(UnkQueryEntry_b50* entry);
void fn_71006fb514(void* map, const u32* id, UnkQueryEntry_b50* const* entry);

void fn_71006fae70(UnkQueryList_b50* self, char* query, u32 mode) {
    UnkQueryTarget_b50* target = reinterpret_cast<UnkQueryHolder_b50*>(self)->target;
    UnkQueryTarget_b50* match = target->vf_0x18(query, mode) == 16 ? target : nullptr;
    if (fn_7100116de0(match->shape, query, 0, 1, 1.0f)) {
        for (UnkQueryEntry_b50 **it = self->entries, **end = self->entries + self->count; it != end; ++it)
            fn_71006f8af4(*it, (*it)->data, mode);
        fn_71006f9980(self, mode);
    }
    self->field_0x50 = 0;
    u8* map = self->map;
    for (UnkQueryEntry_b50 **it = self->entries, **end = self->entries + self->count; it != end; ++it) {
        UnkQueryEntry_b50* entry = *it;
        fn_71006facbc(entry);
        u32 id = entry->id;
        fn_71006fb514(map, &id, &entry);
    }
}
