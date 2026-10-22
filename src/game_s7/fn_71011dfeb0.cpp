// Initialises a GUI panel: after the base part (fn_71011d9b3c) it keeps handles to 18 child
// objects found by name hash, then four children of known classes (GUI::CDisplayObjectContainer,
// GUI::CLabel, names from reflection), sets up the time label and the alarm sound. The names in
// comments are strings whose CRC-64 is the hash; the others aren't in the binary. Layouts are
// placeholders.
#include <new>

#include "base/UnkDestructors.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "UnkHandleS7.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::global::UnkStrIdInline;
using namespace unk_handle;

struct UnkClass_71011dfeb0;
bool fn_710009d50c(const UnkClass_71011dfeb0* type, const UnkClass_71011dfeb0* base);
const UnkClass_71011dfeb0* fn_71011d7b84();  // GUI::CDisplayObjectContainer::refl_GetClass
const UnkClass_71011dfeb0* fn_71011e8dfc();  // GUI::CLabel::refl_GetClass

struct UnkGuiObj_71011dfeb0 {
    virtual const UnkClass_71011dfeb0* GetClass();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual UnkRefPtr* GetHandleSlot();
};

struct UnkLabel_71011dfeb0 : UnkGuiObj_71011dfeb0 {
    u8 unk_0x8[0x2f0];
    u32 field_0x2f8;
};

// A string id and a file-path id for an assignment (fn_71000267f8), the path released as a
// two-step member.
struct UnkIdPairValue_71011dfeb0 {
    union {
        CStrId id;
    };
    union {
        CFilePathStrId path_init;
        base::UnkStrIdMemberOf<g_unk_7101d47200> path;
    };

    UnkIdPairValue_71011dfeb0(const CStrId& a, const CFilePathStrId& b) : id(a) {
        new (&path_init) CFilePathStrId(b);
    }
    ~UnkIdPairValue_71011dfeb0() {
        path.~UnkStrIdMemberOf();
        id.~CStrId();
    }
};

struct UnkPanel_71011dfeb0;
UnkGuiObj_71011dfeb0* fn_71011d98ec(UnkPanel_71011dfeb0* self, u64 id);  // finds a child
void fn_71011d9b3c(UnkPanel_71011dfeb0* self);
void fn_71011e1404(UnkPanel_71011dfeb0* self);
void fn_71011ec3fc(UnkLabel_71011dfeb0* label, bool value);
void fn_71000267f8(void* pair, const UnkIdPairValue_71011dfeb0& value);
extern CStrId g_unk_71727b9d30;

struct UnkPanel_71011dfeb0 {
    u8 unk_0x0[0x260];
    UnkRefPtr field_0x260;
    UnkRefPtr field_0x268;
    UnkRefPtr field_0x270;
    UnkRefPtr field_0x278;
    UnkRefPtr field_0x280;
    UnkRefPtr field_0x288;
    UnkRefPtr field_0x290;
    UnkRefPtr field_0x298;
    UnkRefPtr field_0x2a0;
    UnkRefPtr field_0x2a8;
    UnkRefPtr field_0x2b0;
    UnkRefPtr field_0x2b8;
    UnkRefPtr field_0x2c0;
    UnkRefPtr field_0x2c8;
    UnkRefPtr field_0x2d0;
    UnkRefPtr field_0x2d8;
    UnkRefPtr field_0x2e0;
    UnkRefPtr field_0x2e8;
    u8 unk_0x2f0[0x28];
    UnkGuiObj_71011dfeb0* field_0x318;
    UnkGuiObj_71011dfeb0* field_0x320;
    UnkLabel_71011dfeb0* field_0x328;
    UnkLabel_71011dfeb0* field_0x330;
    u8 unk_0x338[0x170];
    u8 field_0x4a8[0x18];

    // Keeps a handle to the child with this name hash, if there is one.
    __attribute__((always_inline)) void Bind(UnkRefPtr& ref, u64 id) {
        if (UnkGuiObj_71011dfeb0* child = fn_71011d98ec(this, id))
            SetRef(ref, Handle(child));
    }

    // The child with this name when it is of the class `get_class` returns, else null.
    template <class T>
    __attribute__((always_inline)) T* FindAs(const char* name, const UnkClass_71011dfeb0* (*get_class)()) {
        UnkGuiObj_71011dfeb0* child = fn_71011d98ec(this, fn_7100001570(name, __builtin_strlen(name)));
        return child && fn_710009d50c(child->GetClass(), get_class()) ? static_cast<T*>(child)
                                                                      : nullptr;
    }
};

void fn_71011dfeb0(UnkPanel_71011dfeb0* self) {
    fn_71011d9b3c(self);
    self->Bind(self->field_0x260, 0xa85c4752f3e59787);  // "LifeLabel"
    self->Bind(self->field_0x268, 0x4fee8e10d17093ff);  // "MissileLabel"
    self->Bind(self->field_0x270, 0x5903b8357e71ecf5);  // "BombLabel"
    self->Bind(self->field_0x278, 0xb7cf12c6a466c5ad);
    self->Bind(self->field_0x280, 0x0fc45cb8e98d58ef);  // "MissileIcon"
    self->Bind(self->field_0x288, 0x098361e2bbd052ce);
    self->Bind(self->field_0x290, 0xd3d63f4c991b8138);
    self->Bind(self->field_0x298, 0x7c0caca73e7f739d);  // "BombIcon"
    self->Bind(self->field_0x2a0, 0x3bb273b1a23af00a);
    self->Bind(self->field_0x2a8, 0xe1e72d1f80f123fc);
    self->Bind(self->field_0x2b0, 0x6c8f558668cf9753);
    self->Bind(self->field_0x2c8, 0x6b2c8fa02cae947e);
    self->Bind(self->field_0x2d0, 0x75926c0e21c461d6);  // "EnergyTanksComposition"
    self->Bind(self->field_0x2d8, 0x938b9a7e4c71aee9);
    self->Bind(self->field_0x2e0, 0xafa76b4f7a94f250);
    self->Bind(self->field_0x2e8, 0x552f1b3315ab9707);
    self->Bind(self->field_0x2b8, 0x1abb09308fc78bde);
    self->Bind(self->field_0x2c0, 0xe9eeb59ec3e52cd6);
    self->field_0x318 = self->FindAs<UnkGuiObj_71011dfeb0>("BossRushPanel", fn_71011d7b84);
    self->field_0x320 = self->FindAs<UnkGuiObj_71011dfeb0>("TimeBG", fn_71011d7b84);
    self->field_0x328 = self->FindAs<UnkLabel_71011dfeb0>("TimeLabel", fn_71011e8dfc);
    self->field_0x330 = self->FindAs<UnkLabel_71011dfeb0>("BossCount", fn_71011e8dfc);
    fn_71011ec3fc(self->field_0x328, true);
    self->field_0x328->field_0x2f8 = 3;
    fn_71000267f8(self->field_0x4a8,
                  UnkIdPairValue_71011dfeb0(g_unk_71727b9d30,
                                            CFilePathStrId(UnkStrIdInline(),
                                                           "system/snd/presets/gui/timealarm_01", true)));
    fn_71011e1404(self);
}
