#include "ff.h"
#include "diskio.h"
#include "sdcard.h"
#include "uart.h"

#define DEV_MMC 0

DSTATUS disk_status(BYTE pdrv)
{
    uint8_t res[2];

    if (pdrv != DEV_MMC)
        return STA_NOINIT;

    SD_sendStatus(res);

    if (res[0] == 0x00)
        return 0;
    else
        return STA_NOINIT;
}

DSTATUS disk_initialize(BYTE pdrv)
{
    
    if (pdrv != DEV_MMC)
        return STA_NOINIT;

    if (SD_init() == SD_SUCCESS)
        return 0;
    else
        return STA_NOINIT;
}

DRESULT disk_read(BYTE pdrv, BYTE *buff, LBA_t sector, UINT count)
{
    uint8_t token;

    if (pdrv != DEV_MMC)
        return RES_PARERR;

    while(count--)
    {
        if (SD_readSingleBlock(2048 + sector * 512, buff, &token) != 0x00 || token != 0xFE)
            return RES_ERROR;

        buff += 512;
        sector++;
    }

    return RES_OK;
}

#if FF_FS_READONLY == 0
DRESULT disk_write(BYTE pdrv, const BYTE *buff, LBA_t sector, UINT count)
{
    uint8_t token;

    if (pdrv != DEV_MMC)
        return RES_PARERR;

    while(count--)
    {
        if (SD_writeSingleBlock(2048 + sector * 512, (uint8_t*)buff, &token) != 0x00 || token != 0x05)
            return RES_ERROR;

        buff += 512;
        sector++;
    }

    return RES_OK;
}
#endif

DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buff)
{
    // For now, just return OK
    if(pdrv != DEV_MMC)
        return RES_PARERR;

    return RES_OK;
}
