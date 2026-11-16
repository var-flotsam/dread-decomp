// Unit 2553 (CGameManager::CTunableGameManager): on event 1, re-points the weak handle at +0x4a8 at the
// component its target's +0x1f8 object provides; without one, flags +0x4a5 and runs the script callback
// named at +0x560. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2730.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

void fn_71007aa7e4(void* target);
void fn_71007aa7d0(void* target);

struct UnkScriptNameData_ldf2f78 {
    u8 pad_0x0[0x10];
    const char* str;  // 0x10
    u8 pad_0x18[0x2c];
    union {
        s32 use_count;  // 0x44
    };
};

// A pooled name: reading its text counts a use.
struct UnkScriptName_ldf2f78 {
    UnkScriptNameData_ldf2f78* data;

    bool IsEmpty() const {
        return data == reinterpret_cast<UnkScriptNameData_ldf2f78*>(&g_unk_7101d47108->empty);
    }
    const char* CStr() const {
        ++data->use_count;
        return data->str ? data->str : g_unk_710162e2ed;
    }
};

struct UnkAttachTarget_ldf2f78 {
    u8 pad_0x0[0x1f8];
    void* field_0x1f8;
};

struct UnkAttachComponent_ldf2f78 {
    u8 pad_0x0[0x216];
    u8 field_0x216;
};

struct UnkAttachOwner_ldf2f78 {
    u8 pad_0x0[0x4a5];
    u8 detached;     // 0x4a5
    bool field_0x4a6;  // 0x4a6
    bool field_0x4a7;  // 0x4a7
    UnkWeakHandle* handle;  // 0x4a8
    u8 pad_0x4b0[0xb0];
    UnkScriptName_ldf2f78 callback;  // 0x560
};

void fn_7100df2f78(UnkAttachOwner_ldf2f78* self, void* arg, s32 event) {
    if (event != 1)
        return;

    UnkWeakTarget* component = nullptr;
    if (UnkWeakHandle* current = self->handle) {
        auto* target = reinterpret_cast<UnkAttachTarget_ldf2f78*>(current->target);
        if (target && target->field_0x1f8)
            component = static_cast<UnkWeakTarget*>(fn_71004067a4(target->field_0x1f8));
    }

    if (!component) {
        self->detached = 1;
        ResetRef(&self->handle);
        fn_7101199818(g_unk_7101d47330, self);
        if (!self->callback.IsEmpty())
            fn_7100000790(self->callback.CStr(), g_unk_710162e2ed, false);
        return;
    }

    UnkWeakHandle* handle = GetWeakHandle(component);
    AssignRef(&self->handle, handle);
    if (handle && handle->target) {
        if (self->field_0x4a6)
            fn_71007aa7e4(handle->target);
        if (self->field_0x4a7)
            reinterpret_cast<UnkAttachComponent_ldf2f78*>(self->handle->target)->field_0x216 = 1;
        fn_71007aa7d0(self->handle ? self->handle->target : nullptr);
    }
}
