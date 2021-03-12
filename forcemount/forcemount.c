#include <psp2kern/kernel/modulemgr.h>
#include <taihen.h>

static int hook = -1;
static tai_hook_ref_t ref_hook;


static int return1() {
	ksceDebugPrintf("ForceMount // return 1;\n");
	return 1;
}	


void _start() __attribute__ ((weak, alias ("module_start")));
int module_start(SceSize argc, const void *args)
{
    tai_module_info_t info;
	info.size = sizeof(tai_module_info_t);
	taiGetModuleInfoForKernel(KERNEL_PID, "SceAppMgr", &info);
	
	hook = taiHookFunctionOffsetForKernel(KERNEL_PID, &ref_hook, info.modid, 0, 0x15D54,1, return1);
	ksceDebugPrintf("ForceMount // return1 HOOK: %x %x !\n",hook,ref_hook);
	return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args)
{
	if (hook >= 0) taiHookReleaseForKernel(hook, ref_hook);
	return SCE_KERNEL_STOP_SUCCESS;
}
