// GUI::CAmiiboComposition's unit (unit 1650): showing the composition. When the options blackboard already
// has the amiibo first-use flag, it shows the composition's child and plays its "Rotate" animation; the first
// time, it stores the flag and opens the "#GUI_AMIIBO_FTUE" message instead. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU0.h"
#include "game/UnkTU1645.h"
#include "game/UnkTU2768.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CStrId;

struct UnkAmiiboSingleton_laf0d38;
extern UnkAmiiboSingleton_laf0d38* g_unk_717275c378;
void fn_710002a028(UnkAmiiboSingleton_laf0d38* self);  // constructor
void fn_710002a560(UnkAmiiboSingleton_laf0d38* self);

struct UnkAmiiboComposition_laf0d38;
void fn_7100af1d78(UnkAmiiboComposition_laf0d38* self);

// A function bound to its object, called when the message closes. It is passed by value; its copy
// constructor is out of line, so it goes in memory.
struct UnkAmiiboCallback_laf0d38 {
    UnkAmiiboComposition_laf0d38* object;
    void (*function)(UnkAmiiboComposition_laf0d38*);

    UnkAmiiboCallback_laf0d38(UnkAmiiboComposition_laf0d38* object, void (*function)(UnkAmiiboComposition_laf0d38*))
        : object(object), function(function) {}
    UnkAmiiboCallback_laf0d38(const UnkAmiiboCallback_laf0d38& other);
};

struct UnkAmiiboMessages_laf0d38;
void fn_7100b5fd9c(UnkAmiiboMessages_laf0d38* self, const CStrId& text, UnkAmiiboCallback_laf0d38 callback,
                   bool a, bool b);

struct UnkAmiiboMessages_laf0d38 {
    u8 pad_0x0[0x2e3];
    bool field_0x2e3;
    u8 pad_0x2e4[0x14];
    u32 field_0x2f8;
};

struct UnkAmiiboGuiRef_laf0d38 {
    u8 pad_0x0[0x10];
    UnkAmiiboMessages_laf0d38* messages;  // 0x10
};

struct UnkAmiiboChild_laf0d38 {
    void** vtable;

    void PlayAnimation(const char* name, bool flag) {
        reinterpret_cast<void (*)(UnkAmiiboChild_laf0d38*, const char*, bool)>(vtable[0x1e8 / 8])(this, name, flag);
    }
};

struct UnkAmiiboComposition_laf0d38 {
    u8 pad_0x0[0x230];
    bool field_0x230;
    u8 pad_0x231[7];
    UnkAmiiboChild_laf0d38* child;  // 0x238
};

static inline UnkAmiiboMessages_laf0d38* Messages() {
    return static_cast<UnkAmiiboGuiRef_laf0d38*>(g_unk_7101d47310->field_0x2258)->messages;
}

static inline const UnkQualifiedId& DefId_laf0d38(UnkPropertyEx<>& def) {
    UnkQualifiedId& qid = ItemQualifiedId(def);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(def), g_unk_7101d470e0);
    return qid;
}

// Stores the flag `value` points to under `name` in the section `section`, unless the blackboard is locked.
static inline void SetFlag_laf0d38(CBlackboard* blackboard, const CStrId& section, const UnkQualifiedId& name,
                                   bool* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = value;
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(const_cast<UnkQualifiedId*>(&name)), &v);
        }
    }
}

static inline UnkAmiiboSingleton_laf0d38* Singleton_laf0d38() {
    UnkAmiiboSingleton_laf0d38* p = g_unk_717275c378;
    if (!p) {
        p = static_cast<UnkAmiiboSingleton_laf0d38*>(fn_71000839a0(0x18, 0x3f));
        fn_710002a028(p);
        g_unk_717275c378 = p;
    }
    return p;
}

static inline UnkUserDataOwner* UserData_laf0d38() {
    UnkUserDataOwner* p = g_unk_7101d47308;
    if (!p) {
        p = static_cast<UnkUserDataOwner*>(fn_71000839a0(0x2b8, 0x3f));
        fn_710120320c(p);
        g_unk_7101d47308 = p;
    }
    return p;
}

void fn_7100af0d38(UnkAmiiboComposition_laf0d38* self) {
    self->field_0x230 = false;
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x820;
    if (fn_71010474f4(blackboard, base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0xb4],
                      DefId_laf0d38(g_unk_717275b0c8).id)) {
        if (Messages()->field_0x2f8 != 0)
            return;
        fn_710002a560(Singleton_laf0d38());
        fn_71011cdce8(reinterpret_cast<UnkGuiDisplayObject*>(self->child), true);
        self->child->PlayAnimation("Rotate", false);
        return;
    }
    const CStrId& section = base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0xb4];
    const UnkQualifiedId& name = DefId_laf0d38(g_unk_717275b0c8);
    bool seen = true;
    SetFlag_laf0d38(blackboard, section, name, &seen);
    fn_7101203854(UserData_laf0d38());
    UnkAmiiboMessages_laf0d38* messages = Messages();
    messages->field_0x2e3 = true;
    fn_7100b5fd9c(messages, CStrId("#GUI_AMIIBO_FTUE", true), UnkAmiiboCallback_laf0d38(self, fn_7100af1d78), true,
                  false);
}
