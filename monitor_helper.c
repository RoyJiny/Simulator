#include "monitor_helper.h"
#include "defines.h"
#include <stdlib.h>
#include <stdio.h>

unsigned char monitor[MONITOR_WIDTH][MONITOR_HEIGHT] = {0};

void write_pixel(int x, int y, unsigned char shade)
{
    if (x >= MONITOR_WIDTH || y >= MONITOR_HEIGHT) return;
    if (x < 0 || y < 0) return;
    monitor[x][y] = shade;
}

void write_monitor_data()
{
    FILE* monitor_txt = fopen(MONITORTXT, "w");
    FILE* monitor_yuv = fopen(MONITORYUV, "w");
    char line[3];
    for(int y=0; y<MONITOR_HEIGHT; y++){
        for(int x=0; x<MONITOR_WIDTH; x++){
            sprintf(line, "%02X", monitor[x][y]);
            // txt:
            fputs(line, monitor_txt);
            fputs("\n", monitor_txt);
            //yuv:
            fwrite(&(monitor[x][y]), sizeof(unsigned char), 1, monitor_yuv);
        }
    }
    fclose(monitor_txt);
    fclose(monitor_yuv);
}
