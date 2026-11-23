// CCharClassGrabComponent unit (tools/tu_map.py, the name from reflection): the class's read. After the base
// read, it empties the list of grab descriptions at +0x58 and reads a count of new ones from the file: for each
// its kind ("Default", "Grabber" or "Grabbed"), two names, its flags and two boxes. Names and layouts are
// placeholders.
#include <new>

#include "base/UnkVtableList.h"
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "game/CChozoCommanderKiGrabAttack.h"
#include "game/UnkTU56.h"
#include "types.h"

using base::global::CRntFile;
using base::global::CStrId;

// As src/game/CCharClassCollisionComponent/CCharClassCollisionComponent_b36.cpp declares it.
struct UnkCollisionClassDef;
bool fn_71011aced4(UnkCollisionClassDef* self, CRntFile* file);  // the base read
// As src/game/CCharClassGrabComponent/CCharClassGrabComponent_Read_A3_b72.cpp defines it.
struct UnkGrabRecord_A3_b72;
void fn_7100790cbc(UnkGrabRecord_A3_b72* self, CRntFile* file);

// As src/game/CGrabComponent/CGrabComponent_Grab_ovb0057.cpp defines them: a zero whose constructor isn't
// constexpr, so the static is guarded.
struct UnkGrabStaticZero_ovb0057 {
    u32 value;
    UnkGrabStaticZero_ovb0057() : value(0) {}
};

inline const UnkGrabStaticZero_ovb0057& UnkGrabStaticZero() {
    static UnkGrabStaticZero_ovb0057 s_value asm("g_unk_7172716dcc");
    return s_value;
}

// Local to this unit, so their constructors are inlined into the read.
namespace {

static constexpr f32 kMax_l7906cc = 3.40282347e+38f;

struct UnkGrabFlags_l7906cc {
    bool enabled;
    u32 field_0x4;
    bool field_0x8;
    bool field_0x9;
    bool field_0xa;
    bool field_0xb;
    bool field_0xc;
    f32 field_0x10;
};

// A box and two words, read by fn_7100790cbc.
struct UnkGrabBox_l7906cc {
    f32 min[3];
    f32 max[3];
    f32 field_0x18;  // unset in the original
    f32 field_0x1c;  // unset in the original
    u32 field_0x20;
    u32 field_0x24;

    UnkGrabBox_l7906cc() {
        min[0] = -kMax_l7906cc;
        min[1] = -kMax_l7906cc;
        min[2] = -kMax_l7906cc;
        max[0] = kMax_l7906cc;
        max[1] = kMax_l7906cc;
        max[2] = kMax_l7906cc;
        field_0x20 = UnkGrabStaticZero().value;
        field_0x24 = UnkGrabStaticZero().value;
    }
};

// A grab description.
struct UnkGrabInfo_l7906cc {
    s32 kind;
    CStrId field_0x8;
    CStrId field_0x10;
    CStrId field_0x18;
    UnkGrabFlags_l7906cc flags;  // 0x20
    UnkGrabBox_l7906cc box_0x34;
    UnkGrabBox_l7906cc box_0x5c;

    UnkGrabInfo_l7906cc()
        : kind(1), field_0x8("", true), field_0x10("", true),
          field_0x18(base::global::UnkStrIdInline(), nullptr, true) {
        flags.enabled = false;
        flags.field_0x4 = 0;
        flags.field_0x8 = false;
        flags.field_0x9 = false;
        flags.field_0xa = true;
        flags.field_0xb = true;
        flags.field_0xc = true;
        flags.field_0x10 = 0.0f;
    }
};
static_assert(sizeof(UnkGrabInfo_l7906cc) == 0x88, "UnkGrabInfo_l7906cc layout");

struct UnkGrabNode_l7906cc {
    UnkGrabInfo_l7906cc info;
    UnkGrabNode_l7906cc* next;  // 0x88

    explicit UnkGrabNode_l7906cc(const UnkGrabInfo_l7906cc& info) : info(info), next(nullptr) {}
};

}  // namespace

struct UnkGrabClass_l7906cc {
    u8 pad_0x0[0x58];
    base::UnkVtableList<UnkGrabNode_l7906cc> list;  // 0x58
};

// The kind a name in the file stands for; 0 for none of them.
static inline s32 GrabKind_l7906cc(const char* name) {
    if (!__builtin_strcmp(name, "Default"))
        return 1;
    if (!__builtin_strcmp(name, "Grabber"))
        return 2;
    if (!__builtin_strcmp(name, "Grabbed"))
        return 3;
    return 0;
}

bool fn_71007906cc(UnkGrabClass_l7906cc* self, CRntFile* file) {
    bool result = fn_71011aced4(reinterpret_cast<UnkCollisionClassDef*>(self), file);
    if (!result)
        return result;
    s32 count = 0;
    CRntFile_Read(file, &count, 4, 1);

    base::UnkLinkedList<UnkGrabNode_l7906cc>& list = self->list;
    for (UnkGrabNode_l7906cc* node = list.head; node;) {
        UnkGrabNode_l7906cc* next = node->next;
        fn_7100431cec(node);
        list.allocator->vf_0x48(node);
        node = next;
    }
    list.count = 0;
    list.head = nullptr;
    list.tail = nullptr;

    for (s32 i = 0; i < count; ++i) {
        UnkGrabInfo_l7906cc info;
        const char* kind;
        const char* name;
        const char* other;
        CRntFile_ReadCString(file, &kind);
        CRntFile_ReadCString(file, &name);
        CRntFile_ReadCString(file, &other);
        CRntFile_Read(file, &info.flags.field_0x4, 4, 1);
        CRntFile_Read(file, &info.flags.field_0x8, 1, 1);
        CRntFile_Read(file, &info.flags.field_0x9, 1, 1);
        CRntFile_Read(file, &info.flags.field_0xb, 1, 1);
        CRntFile_Read(file, &info.flags.field_0xc, 1, 1);
        info.field_0x8 = CStrId(name, true);
        info.field_0x10 = CStrId(other, true);
        info.kind = GrabKind_l7906cc(kind);
        fn_7100790cbc(reinterpret_cast<UnkGrabRecord_A3_b72*>(&info.box_0x34), file);
        fn_7100790cbc(reinterpret_cast<UnkGrabRecord_A3_b72*>(&info.box_0x5c), file);
        void* memory = list.allocator->vf_0x40(sizeof(UnkGrabNode_l7906cc), 8, list.flags);
        if (memory)
            list.PushBack(new (memory) UnkGrabNode_l7906cc(info));
        fn_7100431cec(&info);
    }
    return result;
}
