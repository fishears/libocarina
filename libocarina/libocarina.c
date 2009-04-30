
/*
 * File:        libocarina.c
 * Author:      fishears
 * Description: library for implementing ocarina code support into game loaders
 *
 * Created on 14 April 2009, 08:52
 *
 */

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "gccore.h"
#include "sdcard/wiisd_io.h"
#include "libocarina.h"
#define FILEDIR	"sdhc:/codes"

extern const unsigned char kenobiwii[];
extern const int kenobiwii_size;
extern void patchhook(u32 address, u32 len);
extern void vipatch(u32 address, u32 len);

static char gameid[8];

static const u32 viwiihooks[4] = {
	0x7CE33B78,0x38870034,0x38A70038,0x38C7004C
};
static const u32 vipatchcode[3] = {
	0x4182000C,0x4180001C,0x48000018
};

/* call OCARINAPATCHER after apploader is run */
void ocarinapatcher(void *addr, u32 len)
{
    void* dst = addr;
    memset((void*)0x80001800,0,kenobiwii_size);
    memcpy((void*)0x80001800,kenobiwii,kenobiwii_size);
    DCFlushRange((void*)0x80001800,kenobiwii_size);
    memcpy((void*)0x80001800, (char*)0x80000000, 6);

    dogamehooks(dst,len);
    vidolpatcher(dst,len);
    DCFlushRange(dst, len);
}

/* call DO_SD_CODE after you get the gameid */
/* returns 1 for codes found
 * returns 2 for codes not found
 * returns 3 for sd error
*/
/* call APPLY_SD_CODE with bool apply = true to apply the codes */
u32 do_sd_code()
{
	FILE *fp;
	u8 *filebuff;
        u32 filesize;
	u32 ret;
	char filepath[128];
        memset(gameid, 0, 8);
	memcpy(gameid, (char*)0x80000000, 6);

	ret = __io_wiisd.startup();
	fatMountSimple("sdhc", &__io_wiisd);
	if (!ret) {
		/* sd error */
		return 3;
	}

	fflush(stdout);

	sprintf(filepath, FILEDIR "/%s", gameid);
	filepath[18] = 0x2E;
	filepath[19] = 0x67;
	filepath[20] = 0x63;
	filepath[21] = 0x74;
	filepath[22] = 0;

	fp = fopen(filepath, "rb");
	if (!fp) {
		/* no codes found */
		fatUnmount("sdhc");
		__io_wiisd.shutdown();
		return 2;
	}

	fseek(fp, 0, SEEK_END);
	filesize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	filebuff = (u8*) malloc (filesize);
	if(filebuff == 0){
		fclose(fp);
		return 3;
	}

	ret = fread(filebuff, 1, filesize, fp);
	if(ret != filesize){
		/* SD Code Error */
		free(filebuff);
		fclose(fp);
		fatUnmount("sdhc");
		__io_wiisd.shutdown();
		return 3;
	}
        /* sd codes found */
        return 1;

}

/* call to apply the codes and cleanup */
void apply_sd_codes(bool apply) {
    extern u8 *filebuff;
    extern u32 filesize;
    extern FILE *fp;

    if (apply){
            memcpy((void*)0x800027E8,filebuff,filesize);
            *(vu8*)0x80001807 = 0x01;
    }

    free(filebuff);
    fclose(fp);

    fatUnmount("sdhc");
    __io_wiisd.shutdown();

    sleep(2);
}

void dogamehooks(void *addr, u32 len)
{
	void *addr_start = addr;
	void *addr_end = addr+len;

	while(addr_start < addr_end)
	{

            if(memcmp(addr_start, viwiihooks, sizeof(viwiihooks))==0){
                patchhook((u32)addr_start, len);

            }

            addr_start += 4;
        }
}

void vidolpatcher(void *addr, u32 len)
{

	void *addr_start = addr;
	void *addr_end = addr+len;

	while(addr_start < addr_end)
	{
		if(memcmp(addr_start, vipatchcode, sizeof(vipatchcode))==0) {
			vipatch((u32)addr_start, len);
		}
		addr_start += 4;
	}
}

