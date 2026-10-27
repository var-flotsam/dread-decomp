// The CXParasiteAIComponent unit (tools/tu_map.py: 0x71006e5f24-0x71006ebf9c, whose reflected
// class is CXParasiteAIComponent). The names and layouts are placeholders.
// Holds fn_71006ea3fc (a flag setter that mirrors the flag into the blackboard and the owner's
// timeline component).
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CBlackboard.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkPropertyName;

// A typed value as the section setter takes it (as in src/game_aca57c/fn_7100dc8828.cpp).
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

struct UnkXParasiteAI {
    u8 pad_0x0[0x30];
    UnkActor* field_0x30;  // the owner
    u8 pad_0x38[0x1228];
    CBlackboard field_0x1260;
    u8 pad_0x1350[0x13398];
    bool field_0x146e8;
};
static_assert(__builtin_offsetof(UnkXParasiteAI, field_0x146e8) == 0x146e8, "UnkXParasiteAI placeholder layout");

extern base::global::CStrId g_unk_71726b7cb8;                                    // the blackboard section
extern UnkPropertyName g_unk_717270b8d8 __attribute__((visibility("hidden")));  // the property in it
extern const u64 g_unk_710164a838;                                               // the timeline id

// Sets the flag at +0x146e8. On a change the flag is stored in the blackboard section (unless the
// blackboard is locked), and the owner's timeline component drops the id while the flag is set or
// gets it (with the type id, after a delay of -1) when it is clear and unknown.
void fn_71006ea3fc(UnkXParasiteAI* self, bool value) {
    bool& flag = self->field_0x146e8;
    if (flag == value)
        return;
    flag = value;
    if (CBlackboard::CSection* section = fn_71010472e0(&self->field_0x1260, g_unk_71726b7cb8)) {
        if (!self->field_0x1260.field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = &flag;
            fn_7100332c80(section, &g_unk_717270b8d8, &v);
        }
    }
    UnkTimelineComponent* timeline = static_cast<UnkTimelineComponent*>(self->field_0x30->field_0x2e8);
    if (!timeline)
        return;
    bool known = fn_7100a2be94(timeline, &g_unk_710164a838);
    if (flag && known)
        fn_7100a2b7b8(timeline, &g_unk_710164a838);
    if (!flag && !known) {
        base::reflection::UnkReflTypeIdArg arg;
        arg.id = base::reflection::UnkReflTypeId<base::reflection::UnkReflTypeIdTag>::id();
        fn_7100a2cb7c(timeline, &g_unk_710164a838, &arg.id, false, -1.0f);
    }
}
