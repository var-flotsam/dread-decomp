// Static initialiser of the unit of the EAnimationTag, TAnimationTagFlagSet enums.
#include "UnkStaticInit.h"

using namespace unk_static_init;

struct UnkStatics_7100710350 {
    HeaderIds ids;
};
extern UnkStatics_7100710350 g_unk_717270e480 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_717270e1e8;
void fn_710070ff68(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_710070f99c();
extern char g_unk_717270e300[];
extern UnkConversion g_unk_717270e308;
extern UnkConversion g_unk_717270e328;
void fn_7100710238() __attribute__((visibility("hidden")));
void fn_7100710244() __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_717270e348;
void fn_71007100e8(UnkEnumType* type, const CStrId& name, void* arg);
extern char g_unk_717270e438[];
extern UnkConversion g_unk_717270e440;
extern UnkConversion g_unk_717270e460;
void fn_7100710250() __attribute__((visibility("hidden")));
void fn_710071025c() __attribute__((visibility("hidden")));
void fn_710009a368(void* type);  // the type's destructor, as registered

void fn_7100710350() {
    InitHeaderIds(&g_unk_717270e480.ids);
    InitEnum(&g_unk_717270e1e8, "EAnimationTag", fn_710070ff68, fn_710070f99c,
             g_unk_717270e300, &g_unk_717270e308, fn_7100710238, &g_unk_717270e328, fn_7100710244);
    InitFlagSet(&g_unk_717270e348, "TAnimationTagFlagSet", fn_71007100e8, fn_710009a368, g_unk_717270e438, TypeOf<fn_7100005178>,
                &g_unk_717270e440, fn_7100710250, &g_unk_717270e460, fn_710071025c);
}
