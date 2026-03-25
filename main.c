#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "uart.h"
#include "spi.h"
#include "sdcard.h"
#include "sdprint.h"
#include "firepins.h"


#define BAUD_RATE 9600
#define P1_OFFSET 446

#define ATTR_READ_ONLY 0x01
#define ATTR_HIDDEN 0x02
#define ATTR_SYSTEM 0x04
#define ATTR_VOLUME_ID 0x08
#define ATTR_DIRECTORY 0x10
#define ATTR_ARCHIVE 0x20
#define ATTR_LONG_NAME 0x0F
#define LAST_LONG_ENTRY 0x01

int main(void)
{
    
    _Bool FileFound = 0;
    
    // array to hold responses
    uint8_t res[5], buf[512], token;
    uint32_t addr = 0;
    
    // Partition Data
    uint8_t Partition_Type_Code;
    uint32_t Partition_LBA_Begin, Number_of_Sectors;
    
    // LBA Data
    uint8_t BPB_SecPerClus, BPB_NumFATs;
    uint16_t BPB_BytsPerSec, BPB_RsvdSecCnt;
    uint32_t BPB_FATSz32, BPB_RootClus, BPB_TotSec32;
    
    
    
    
    // initialize UART
    UART_init();

    // initialize SPI
    SPI_init(SPI_MASTER | SPI_FOSC_128 | SPI_MODE_0);

    // initialize sd card
    if(SD_init() != SD_SUCCESS)
    {
        UART_pputs("Error initializing SD CARD\r\n");
    }
    else
    {
        UART_pputs("SD Card initialized\r\n");

        // read sector 0
        UART_pputs("\r\nReading Address: 0x");
        UART_puthex8((uint8_t)(addr >> 24));
        UART_puthex8((uint8_t)(addr >> 16));
        UART_puthex8((uint8_t)(addr >> 8));
        UART_puthex8((uint8_t)addr);
        res[0] = SD_readSingleBlock(addr, buf, &token);
        UART_pputs("\r\nResponse:\r\n");
        SD_printR1(res[0]);

        if(!(token & 0xF0))
        {
            UART_pputs("Error token:\r\n");
            SD_printDataErrToken(token);
        }
        
        Partition_Type_Code = buf[P1_OFFSET+4];
        Partition_LBA_Begin = ((uint32_t)buf[P1_OFFSET+11] << 24) |((uint32_t)buf[P1_OFFSET+10] << 16) |((uint32_t)buf[P1_OFFSET+9] << 8) | (uint32_t)buf[P1_OFFSET+8];
        Number_of_Sectors = ((uint32_t)buf[P1_OFFSET+15] << 24) |((uint32_t)buf[P1_OFFSET+14] << 16) |((uint32_t)buf[P1_OFFSET+13] << 8) | (uint32_t)buf[P1_OFFSET+12];
        
        UART_pputs("\r\nPartition Type Code: 0x");
        UART_puthex8(Partition_Type_Code);
        
        UART_pputs("\r\nNumber of Sectors: 0x");
        UART_puthex32(Number_of_Sectors);
        
        addr = Partition_LBA_Begin;
        
        res[0] = SD_readSingleBlock(addr, buf, &token);
        
        // read sector LBA
        UART_pputs("\r\nReading Address: 0x");
        UART_puthex8((uint8_t)(addr >> 24));
        UART_puthex8((uint8_t)(addr >> 16));
        UART_puthex8((uint8_t)(addr >> 8));
        UART_puthex8((uint8_t)addr);
        res[0] = SD_readSingleBlock(addr, buf, &token);
        UART_pputs("\r\nResponse:\r\n");
        SD_printR1(res[0]);
        
        if(!(token & 0xF0))
        {
            UART_pputs("Error token:\r\n");
            SD_printDataErrToken(token);
        }
        
        BPB_BytsPerSec = (buf[12] << 8) | buf[11];
        BPB_SecPerClus = buf[13];
        BPB_FATSz32 = ((uint32_t)buf[39] << 24) |((uint32_t)buf[38] << 16) |((uint32_t)buf[37] << 8) | (uint32_t)buf[36];
        BPB_NumFATs = buf[16];
        BPB_RsvdSecCnt = (buf[15] << 8) | buf[14];
        BPB_RootClus = ((uint32_t)buf[47] << 24) |((uint32_t)buf[46] << 16) |((uint32_t)buf[45] << 8) | (uint32_t)buf[44];
        BPB_TotSec32 = ((uint32_t)buf[35] << 24) |((uint32_t)buf[34] << 16) |((uint32_t)buf[33] << 8) | (uint32_t)buf[32];
        
        
        UART_pputs("\r\nBytes Per Sector: 0x");
        UART_puthex16(BPB_BytsPerSec);
        
        UART_pputs("\r\nSectors Per Cluster: 0x");
        UART_puthex8(BPB_SecPerClus);
        
        UART_pputs("\r\nNumber of Reserved Sectors: 0x");
        UART_puthex16(BPB_RsvdSecCnt);
        
        UART_pputs("\r\nNumber of FATs: 0x");
        UART_puthex8(BPB_NumFATs);
        
        UART_pputs("\r\nSectors Per FAT: 0x");
        UART_puthex32(BPB_FATSz32);
        
        UART_pputs("\r\nRoot Directory First Clusters: 0x");
        UART_puthex32(BPB_RootClus);
        
        UART_pputs("\r\nTotal Sector Count: 0x");
        UART_puthex32(BPB_TotSec32);
        
        
        // SD working variables
        uint32_t FAT32ClusEntryVal, FirstDataSector, FirstSectorofCluster, N, FATOffset,ThisFATSecNum,ThisFATEntOffset ;
        
        
        N = BPB_RootClus;
        
        FirstDataSector = BPB_RsvdSecCnt + (BPB_NumFATs * BPB_FATSz32);
        FirstSectorofCluster = ((N - 2) * BPB_SecPerClus) + FirstDataSector;
        
        FATOffset = N * 4;
        ThisFATSecNum = BPB_RsvdSecCnt + (FATOffset/BPB_BytsPerSec);
        ThisFATEntOffset = FATOffset % BPB_BytsPerSec;
            
        UART_pputs("\r\nRoot Directory...");
        addr = FirstDataSector + Partition_LBA_Begin;
        res[0] = SD_readSingleBlock(addr, buf, &token);
        if((res[0] == 0x00) && (token == SD_START_TOKEN))
            SD_printBufAscii(buf);
            SD_printBuf(buf);
            
        UART_pputs("\r\n");
            
        uint32_t DIR_FileSize;
        uint8_t DIR_Name[11], DIR_Attr, LDIR_Ord;
        uint8_t LDIR_Attr, TargetFile[]="TEXT    TXT";
        uint8_t LFN[255], LFN_Buf[255];
        int i = 0;
        
        while (i<SD_BLOCK_LEN){
            LDIR_Attr = buf[11+i];
            LDIR_Ord = buf[i];
            DIR_Attr = buf[11+i];
            
            if ((LDIR_Attr == ATTR_LONG_NAME) && (LDIR_Ord != 0xE5)){
                for (int m = 0; m < 255; m++){ // reset LFN
                    LFN[m] = '\0';
                    LFN_Buf[m] = '\0';
                }
                int k = 0;
                while ((LDIR_Ord >= 0x01) && (LDIR_Ord <= 0x42)){
                    for (int j = 0; j<10;j++){
                        LFN[(k)*26 + j]=buf[i+j+1];
                    }
                    for (int j = 0; j<12;j++){
                        LFN[(k)*26+10+ j]=buf[i+j+14];
                    }
                    for (int j = 0; j<4;j++){
                        LFN[(k)*26+22+j]=buf[i+j+28];
                    }
                    k+=1;
                    i+=32;
                    LDIR_Ord = buf[i];
                }
                
                UART_pputs("\r\nLFN: ");
                

                for (int j = 0; j <= k; j++){
                    for (int m = 0; m<26; m++){
                        LFN_Buf[m+26*j] = LFN[m+26*(k-j)];
                    }
                }
                
                for (int m = 0; m < 255; m++) {
                    LFN[m] = LFN_Buf[m];
                    UART_putc(LFN[m]);
                }
                
            } 
            else if (LDIR_Ord == 0) {
                i=1000;
            }
            else
            {
                if ((DIR_Attr & (ATTR_DIRECTORY | ATTR_VOLUME_ID)) == 0x00){
                    /* Found a file. */
                    for (int j = 0; j<11;j++){
                        DIR_Name[j] = buf[i+j];
                        UART_putc(DIR_Name[j]);
                    }
                    if (strcmp((const char*)DIR_Name, (const char*)TargetFile) ==0){
                        UART_pputs("\r\nFound text file: ");
                        N = ((uint32_t)buf[i+21] << 24) |((uint32_t)buf[i+20] << 16) |((uint32_t)buf[i+27] << 8) | (uint32_t)buf[i+26];
                        DIR_FileSize = ((uint32_t)buf[i+21] << 31) |((uint32_t)buf[i+30] << 16) |((uint32_t)buf[i+29] << 8) | (uint32_t)buf[i+28];
                        UART_pputs("\r\n    Size: 0x");
                        UART_puthex32(DIR_FileSize);
                        FileFound =1;
                    }
                    
                    i+=32;
                }
                else if ((DIR_Attr & (ATTR_DIRECTORY | ATTR_VOLUME_ID)) == ATTR_DIRECTORY){
                    /* Found a directory. */
                    UART_pputs("\r\nFound a directory: ");
                    for (int j = 0; j<11;j++){
                        DIR_Name[j] = buf[i+j];
                        UART_putc(DIR_Name[j]);
                    }
                    i+=32;
                }
                else if ((DIR_Attr & (ATTR_DIRECTORY | ATTR_VOLUME_ID)) == ATTR_VOLUME_ID){
                    /* Found a volume label. */
                    UART_pputs("\r\nFound a volume label: ");
                    for (int j = 0; j<11;j++){
                        DIR_Name[j] = buf[i+j];
                        UART_putc(DIR_Name[j]);
                    }
                    i+=32;
                }
                else{
                    /* Found an invalid directory entry. */
                    UART_pputs("\r\nInvalid");
                    i=1000;
                    i+=32;
                }
            }
            
            
            
        }
      
        if (FileFound){
            FirstDataSector = BPB_RsvdSecCnt + (BPB_NumFATs * BPB_FATSz32);
            FirstSectorofCluster = ((N - 2) * BPB_SecPerClus) + FirstDataSector;

            FATOffset = N * 4;
            ThisFATSecNum = BPB_RsvdSecCnt + (FATOffset/BPB_BytsPerSec);
            ThisFATEntOffset = FATOffset % BPB_BytsPerSec;

            addr = (ThisFATSecNum+BPB_FATSz32) + Partition_LBA_Begin;
            res[0] = SD_readSingleBlock(addr, buf, &token);

            FAT32ClusEntryVal = ((uint32_t)buf[ThisFATEntOffset+3] << 24) |((uint32_t)buf[ThisFATEntOffset+2] << 16) |((uint32_t)buf[ThisFATEntOffset+1] << 8) | (uint32_t)buf[ThisFATEntOffset];
            FAT32ClusEntryVal &= 0x0FFFFFFF;

            UART_pputs("\r\nREADING BEGIN :\r\n");

            for (int m = 0; m <= DIR_FileSize/BPB_BytsPerSec; m++){
                addr = FirstSectorofCluster + Partition_LBA_Begin + m;
                res[0] = SD_readSingleBlock(addr, buf, &token);
                
                SD_printBufAscii(buf);

//                UART_pputs("\r\n20L Test Buf :\r\n");
                fire(buf, DIR_FileSize);
                
                while(1){}
            }
        }
        
        
    }

    while(1) ;
}
