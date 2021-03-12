#include <string.h>
#include <vitasdk.h>
#include <stdlib.h>
#include <stdio.h>
#include <taihen.h>

int ldUser = 0;
int ldKernel = 0;

int getFileSize(const char *file) {
	SceUID fd = sceIoOpen(file, SCE_O_RDONLY, 0);
	if (fd < 0)
		return fd;
	int fileSize = sceIoLseek(fd, 0, SCE_SEEK_END);
	sceIoClose(fd);
	return fileSize;
}

int fileExists(const char *file)
{
	if(getFileSize(file) <= 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int ReadFile(char *file, void *buf, int size) {
	SceUID fd = sceIoOpen(file, SCE_O_RDONLY, 0);
	if (fd < 0)
		return fd;

	int written = sceIoRead(fd, buf, size);

	sceIoClose(fd);
	return written;
}

int WriteFile(char *file, void *buf, int size) {
	SceUID fd = sceIoOpen(file, SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, 0777);
	if (fd < 0)
		return fd;

	int written = sceIoWrite(fd, buf, size);

	sceIoClose(fd);
	return written;
}


int CopyFile(char *src, char *dst)
{
	int size = getFileSize(src);
	if (size <= -1){
		return size;
	}
	else
	{
		char *buffer = malloc(size);
		int ret = ReadFile(src,buffer,size);
		if (ret <= -1)
		{
			return ret;
		}
			
		ret = WriteFile(dst,buffer,size);
		if (ret <= -1)
		{
			return ret;
		}
	}
	return 0;
}


void _start() __attribute__ ((weak, alias ("module_start"))); 
int module_start(SceSize argc, const void *args) {	
	sceClibPrintf("LD_PRELOAD // Loading modules\n");

	if(fileExists("app0:/PRELOAD.suprx"))
	{
		sceClibPrintf("LD_PRELOAD // Loading PRELOAD.SUPRX\n");
		int ret = sceKernelLoadStartModule("app0:/PRELOAD.suprx", 0, NULL, 0, NULL, NULL);
		if(ret < 0)
		{
			sceClibPrintf("LD_PRELOAD // Error Loading PRELOAD.SUPRX : %x\n",ret);
			return ret;
		}
	}
	
	if(fileExists("app0:/PRELOAD.skprx"))
	{
		sceClibPrintf("LD_PRELOAD // Loading PRELOAD.SKPRX\n");
		CopyFile("app0:/PRELOAD.skprx","ur0:/temp/screenshot/capture.jpg");
		int ret = taiLoadStartKernelModule("ur0:/temp/screenshot/capture.jpg",0,NULL,0);
		if(ret < 0)
		{
			sceClibPrintf("LD_PRELOAD // Error Loading PRELOAD.SKPRX : %x\n",ret);
			return ret;
		}
	}
	
	
	return SCE_KERNEL_START_SUCCESS;

}

int module_stop(SceSize argc, const void *args) {
	if(ldUser > 0)
		sceKernelStopUnloadModule(ldUser, 0, NULL, 0, NULL, NULL);
	if(ldKernel > 0)
		taiStopUnloadKernelModule(ldKernel, 0, NULL, 0, NULL, NULL); 
	
	return SCE_KERNEL_STOP_SUCCESS;
}
