// CElectricGeneratorComponent's unit (tools/tu_map.py unit 1587): after the base call, reads the usable's
// property from the current scenario's blackboard section, switches the sub-area records named in the id
// list at +0x540 on or off, and sets the render parameter BLACKOUT_LEVEL's x or y from it. Names and
// layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "game/CBlackboard.h"
#include "game/CScenario.h"
#include "game/CUsableComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2577.h"
#include "game/UnkTU315.h"
#include "game/UnkTU365.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector4D;

struct UnkTeleporterSelf_b0233;
void fn_7100a9f300(UnkTeleporterSelf_b0233* self);

base::UnkStrIdMember* fn_71000ce740(base::global::CRntVector<base::UnkStrIdMember>* self, u32 index);

struct UnkGeneratorRecord_ovb0120;
UnkGeneratorRecord_ovb0120* fn_7100a7cd10(base::global::CRntVector<UnkGeneratorRecord_ovb0120>* self, u32 index);

struct UnkSubArea;
const CStrId* fn_7100fac818(UnkSubArea* area, u32 index);

struct UnkScenarioLayer_ovb0032;
struct UnkScenarioKey_ovb0032;
void* fn_71011a3234(UnkScenarioLayer_ovb0032* layers, u32 index, UnkScenarioKey_ovb0032* const* key);

struct UnkBreakableTarget {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

// What fn_7100a7cee8 finds for a part: switched on or off through its vtable.
class UnkGeneratorSwitch_la7c8e8 {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void SetActive(bool active);  // 0x168
};
UnkGeneratorSwitch_la7c8e8* fn_7100a7cee8(void* object);

struct UnkGeneratorFade_la7c8e8;
UnkGeneratorFade_la7c8e8* fn_7100a7cfc4(void* object);
void fn_710079fc24(UnkGeneratorFade_la7c8e8* self, f32 time, f32 delay, u32 mode);

// A 0x40-byte part of a scenario layer's entry.
struct UnkGeneratorPart_la7c8e8 {
    u8 data[0x40];
};
struct UnkGeneratorParts_la7c8e8 {
    UnkGeneratorPart_la7c8e8* items;
    u32 count;
};

struct UnkGeneratorValue_la7c8e8 {
    CStrId name;
    u64 field_0x8;
};

struct UnkGeneratorSubAreas_la7c8e8 {
    u8 pad_0x0[0x860];
    base::global::CRntVector<UnkGeneratorRecord_ovb0120> records;  // 0x860
};

struct UnkElectricGenerator_la7c8e8 {
    u8 pad_0x0[0x4d0];
    UnkGeneratorValue_la7c8e8* values;  // 0x4d0
    u64* keys;  // 0x4d8
    u32 count;  // 0x4e0
    u8 pad_0x4e4[0x5c];
    base::global::CRntVector<base::UnkStrIdMember> ids;  // 0x540
    u8 pad_0x568[0x10];
    s32 mode;  // 0x578
    CVector4D blackout;  // 0x57c
};
static_assert(__builtin_offsetof(UnkElectricGenerator_la7c8e8, ids) == 0x540, "layout");
static_assert(__builtin_offsetof(UnkElectricGenerator_la7c8e8, blackout) == 0x57c, "layout");

static inline UnkGeneratorSubAreas_la7c8e8* SubAreaManager() {
    void* manager = g_unk_7101d47360;
    if (!manager) {
        manager = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = static_cast<UnkManager*>(manager);
    }
    return static_cast<UnkGeneratorSubAreas_la7c8e8*>(manager);
}

// The parts of a sub-area record under its key `which`, in scenario layer `layer`.
static inline UnkGeneratorParts_la7c8e8* RecordParts(UnkGeneratorRecord_ovb0120* record, u32 layer, u32 which) {
    UnkScenarioLayer_ovb0032* layers = reinterpret_cast<UnkScenarioLayer_ovb0032*>(g_unk_7101d47310->field_0x608);
    const CStrId* key = fn_7100fac818(reinterpret_cast<UnkSubArea*>(record), which);
    return static_cast<UnkGeneratorParts_la7c8e8*>(
        fn_71011a3234(layers, layer, reinterpret_cast<UnkScenarioKey_ovb0032* const*>(key)));
}

static inline void* PartObject(UnkGeneratorPart_la7c8e8* part) {
    UnkBreakableTarget* target = *fn_71002a76a4(reinterpret_cast<UnkBreakablePart*>(part));
    return target ? target->field_0x10 : nullptr;
}

// Reads the usable's property from the current scenario's blackboard section into `used`.
static inline void ReadUsed(UnkElectricGenerator_la7c8e8* self, bool* used) {
    UnkGameManager* manager = g_unk_7101d47310;
    if (!manager->field_0x608)
        return;
    CBlackboard::CSection* section = fn_71010472e0(&manager->field_0x968, fn_71011a2fbc(manager->field_0x608));
    if (!section)
        return;
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)reinterpret_cast<UnkProperty*>(g_unk_7172753148)->name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    fn_710006d11c(section, &self->values[index].name, used);
}

void fn_7100a7c8e8(UnkElectricGenerator_la7c8e8* self) {
    fn_7100a9f300(reinterpret_cast<UnkTeleporterSelf_b0233*>(self));

    bool used = false;
    ReadUsed(self, &used);

    UnkGeneratorSubAreas_la7c8e8* sub_areas = SubAreaManager();
    for (u32 i = 0; i < self->ids.count; ++i) {
        for (u32 j = 0; j < sub_areas->records.count; ++j) {
            UnkGeneratorRecord_ovb0120* record = fn_7100a7cd10(&sub_areas->records, j);
            CStrId name = *reinterpret_cast<const CStrId*>(record);
            if (fn_71000ce740(&self->ids, i)->data != name.data)
                continue;

            UnkGeneratorParts_la7c8e8* switches = RecordParts(record, 0, 4);
            for (u64 k = 0; k < switches->count; ++k) {
                UnkBreakableTarget* target =
                    *fn_71002a76a4(reinterpret_cast<UnkBreakablePart*>(&switches->items[k]));
                if (target && target->field_0x10) {
                    if (UnkGeneratorSwitch_la7c8e8* object = fn_7100a7cee8(target->field_0x10))
                        object->SetActive(used);
                }
            }
            UnkGeneratorParts_la7c8e8* fades = RecordParts(record, 2, 1);
            for (u64 k = 0; k < fades->count; ++k) {
                if (UnkGeneratorFade_la7c8e8* fade = fn_7100a7cfc4(PartObject(&fades->items[k])))
                    fn_710079fc24(fade, used ? 0.3f : 0.02f, 0.0f, 0);
            }
            break;
        }
    }

    // The blackout follows the property: full when used, none otherwise.
    bool blackout = used;
    u64 hash = fn_7100001570("BLACKOUT_LEVEL", 14);
    fn_710021a0bc(g_unk_7101d47300, &hash, &self->blackout);
    switch (self->mode) {
    case 0:
        if (blackout)
            self->blackout.x = 1.0f;
        else
            self->blackout.x = 0.0f;
        break;
    case 1:
        if (blackout)
            self->blackout.y = 1.0f;
        else
            self->blackout.y = 0.0f;
        break;
    }
    fn_710021a0b4(g_unk_7101d47300, &hash, &self->blackout);
}
