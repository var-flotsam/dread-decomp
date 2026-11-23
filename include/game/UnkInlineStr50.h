#pragma once

// A string with 0x20 bytes of inline storage (0x50 bytes): a CRntString whose allocator is the object at
// +0x30, its vtable g_unk_7101a479c8. CEmmyGrabSamusAttack's unit keeps several as statics
// (src/game/CEmmyGrabSamusAttack/fn_71005113b0.cpp) and fills an array of four (fn_7100505000). Members by
// their uses; the names and the layout are placeholders.
#include "base/global/CRntString.h"
#include "game/CEmmyGrabSamusAttack.h"
#include "types.h"

struct UnkInlineStr50 {
    u8 str[0x30];
    void** vtable;  // +0x30: the allocator object that owns the inline storage
    u8 pad_0x38[8];
    u64 field_0x40;
    u32 field_0x48;
    u8 pad_0x4c[4];

    base::global::CRntString* Str() { return reinterpret_cast<base::global::CRntString*>(str); }
    const base::global::CRntString* Str() const { return reinterpret_cast<const base::global::CRntString*>(str); }

    // An empty string over the inline storage.
    void Init() {
        CRntString_InitWithAllocator(Str(), reinterpret_cast<base::UnkAllocator*>(&vtable));
        vtable = g_unk_7101a479c8 + 2;
        field_0x40 = 0;
        field_0x48 = 0;
    }

    UnkInlineStr50() { Init(); }
    UnkInlineStr50(const char* text) {
        Init();
        CRntString_AssignCString(Str(), text);
    }
    UnkInlineStr50(const UnkInlineStr50& other) : UnkInlineStr50() { CRntString_Assign(Str(), other.Str()); }
    UnkInlineStr50& operator=(const UnkInlineStr50& other) {
        CRntString_Assign(Str(), other.Str());
        return *this;
    }
    ~UnkInlineStr50() {
        CRntString_Clear(Str());
        CRntString_Destroy(Str());
    }
};
static_assert(sizeof(UnkInlineStr50) == 0x50, "UnkInlineStr50 placeholder layout");
