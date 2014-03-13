#ifndef __LOCATE_H_
#define __LOCATE_H_

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#include <stdio.h>
#include <string.h>

#ifndef uint32_t
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned uint32_t;
#endif

#ifndef PURE
#define PURE = 0
#endif

// ���ݿ���Ϣ
typedef struct LocateInfo
{
    uint32_t count;             // ������                  ���磺444382
    uint32_t time;              // ���ݿ����ʱ��          ���磺20130515
} LocateInfo;

// ��ѯ���
typedef struct LocateItem
{
    uint32_t begin_ip;          // ��ʼIP          С��ģʽ�����IP��ַ
    uint32_t end_ip;            // ����IP          ͬ��
    const char *region;         // ����            ���磺"�Ĵ�����"
    const char *address;        // ��ַ            ���磺"�����Territory Technology Solutions��˾"
} LocateItem;

// ��IP��ַת�����ַ�����ʽ��strΪ16���ֽ�
inline uint32_t ip2str(char *str, uint32_t ip)
{
    return sprintf(str, "%d.%d.%d.%d", (uint8_t)(ip>>24), (uint8_t)(ip>>16), (uint8_t)(ip>>8), (uint8_t)ip);
}

// Locate IP SDK����
class LocateBase
{
public:
    // �Ƿ���سɹ�
    bool IsAvailable()
    {
        return available;
    }

    // ���Ϊtxt�ļ������������ƺʹ���IP���ݿ�����߽�ѹ������ͬ
    bool Save2Text(const wchar_t *path)
    {
        FILE *fp = _wfopen(path, L"wb");
        if(fp)
        {
            char ip1[16];
            char ip2[16];
            for(uint32_t i=0; i<GetInfo()->count; i++)
            {
                LocateItem *item = GetItem(i + 1);
                ip2str(ip1, item->begin_ip);
                ip2str(ip2, item->end_ip);
                fprintf(fp, "%-16s%-16s%s%s%s\r\n", ip1, ip2, item->region, strlen(item->address)>0?" ":"", item->address);
            }

            fprintf(fp, "\r\n\r\nIP���ݿ⹲������ �� %d ��\r\n", GetInfo()->count);

            fclose(fp);
            return true;
        }

        return false;
    }

    // ��ȡ������Ϣ
    LocateInfo* GetInfo()
    {
        return &info;
    }

    // �������ʵ���������ӿ�
    virtual LocateItem* GetItem(uint32_t index) PURE;        // ����������ѯ��ַ 1 ~ GetInfo()->count
    virtual LocateItem* GetLocate(const char *ip) PURE;      // �����ַ���IP��ѯ��ַ
    virtual LocateItem* GetLocate(uint32_t ip) PURE;         // ����С������IP��ѯ��ַ
protected:
    bool available;
    const uint8_t* buffer;
    LocateInfo info;
    LocateItem item;
};

#endif // __LOCATE_H_
