
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#define PCAP_VERSION_MAJOR 2
#define PCAP_VERSION_MINOR 4

#define PCAP_HEADER_DATA(linktype, snaplen)   \
char __pcap_header_buff[] = {                 \
    0xd4, 0xc3, 0xb2, 0xa1, /*magic*/           \
    PCAP_VERSION_MAJOR, 0, PCAP_VERSION_MINOR, 0, /*major, minor*/ \
    0,0,0,0,                /*gmt to local correction*/            \
    0,0,0,0,                /*accuracy of timestamps*/             \
    (snaplen)&0xff, (snaplen>>8)&0xff, (snaplen>>16)&0xff, (snaplen>>24)&0xff,  /*data link type*/  \
    (linktype)&0xff, (linktype>>8)&0xff, (linktype>>16)&0xff, (linktype>>24)&0xff       /*max length saved portion of each pkt*/ \
}

struct pcap_pkt_hdr {  
    struct timeval ts;      /* time stamp */  
    unsigned int caplen;    /* length of portion present */  
    unsigned int len;       /* length this packet (off wire) */  
};  

static char buff[1<<20] = {0};
static time_t now = 0;

static FILE *pcap_file_open(const char *filename, unsigned int linktype, unsigned int snaplen )
{
    PCAP_HEADER_DATA(linktype, snaplen);
    FILE *fp =fopen(filename, "wb");
    if(fp == NULL) {
        return NULL;
    }
    
    fwrite(__pcap_header_buff, 1, sizeof(__pcap_header_buff), fp);
    return fp;
}

static unsigned int pcap_packet_write(FILE* fp, struct timeval tm, const char *pkt, unsigned int len)
{
    struct pcap_pkt_hdr hdr = {tm, len, len};
    fwrite((const char*)&hdr, 1, sizeof(hdr), fp);
    len = fwrite(pkt, 1, len, fp);
    return len+sizeof(hdr);
}

static int pcap_find_pkthdr(FILE *fp, struct timeval *tm)
{
    char *lptmp = NULL;
    
    if(now == 0) {
        time_t timep = time(NULL);
        struct tm* p = localtime(&timep);
        struct tm tmp = *p;
        tmp.tm_sec  = 0;
        tmp.tm_min  = 0;
        tmp.tm_hour = 0;
        now = mktime(&tmp);
    }
    
    while(!feof(fp) && fgets(buff, sizeof(buff), fp)) {
        if(buff[0] == '\r' || buff[0] == '\n' || buff[1] == '\r' || buff[1] == '\n') {
            continue;
        }
        
        if(strstr(buff, " > ") == NULL) {
            continue;
        }
        
        if((lptmp = strchr(buff, ' ')) == NULL) {
            continue;
        }
        
        *lptmp = 0;
        
        //hh:mm:ss.uuuuuu format
        if(strchr(buff, ':') != NULL) {
            unsigned int hour = 0, min = 0, sec = 0, us = 0;
            sscanf(buff, " %d:%d:%d.%d", &hour, &min, &sec, &us);
            tm->tv_sec  = now + hour*3600+min*60+sec;
            tm->tv_usec = us;
            printf("now=%d,buf=%s,h:%d,m=%d,s:%d,us:%d\n", now, buff, hour, min, sec, us);
        } else {
            //ss.uuuuuu format
            sscanf(buff, "%d.%d", &tm->tv_sec, &tm->tv_usec);
        }
        
        return 1;
    }
    
    return 0;
}

static int pcap_find_pktdat(FILE *fp, unsigned char *data, unsigned int max)
{
    unsigned char *lptmp = NULL;
    int num = 0, len = 0;
    
    while(!feof(fp) && fgets(buff, sizeof(buff), fp)) {
        if(buff[0] == '\r' || buff[0] == '\n' || buff[1] == '\r' || buff[1] == '\n') {
            continue;
        }
        
        if((lptmp = (unsigned char *)strchr(buff, ':')) == NULL) {
            continue;
        }
        
        lptmp++;
        
        if(max < 16) {
            break;
        }
        
        num = sscanf(lptmp, "%02X%02X %02X%02X %02X%02X %02X%02X %02X%02X %02X%02X %02X%02X %02X%02X", 
            data+0, data+1, data+2, data+3, data+4, data+5, data+6, data+7, 
            data+8, data+9, data+10, data+11, data+12, data+13, data+14, data+15);
        data += num;
        len += num;
        max -= 16;
        
        if(num < 16) {
             break;
        }
    }
    
    return len;
}

static void pcap_file_close(FILE *fp)
{
    if(fp != NULL) {
        fclose(fp);
    }
}

int main()
{
    struct timeval tm = {0};
    char data[64<<10] = {0};
    int size = 0;
    FILE *fp = pcap_file_open("test.pcap", 1, 0xffff);
    FILE *from = fopen("data.log", "rb");
    if(fp == NULL || from == NULL) {
        return -1;
    }
    
    while(pcap_find_pkthdr(from, &tm)) {
        size = pcap_find_pktdat(from, data, sizeof(data));
        if(size > 0) {
            pcap_packet_write(fp, tm, data, size);
        }
    }
    
    pcap_file_close(fp);
    return 0;
}

