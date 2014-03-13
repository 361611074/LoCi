#ifndef __UTILITY_H_
#define __UTILITY_H_

#pragma warning (disable:4200)

#include <string.h>

extern "C"
{
#include "lzma/LzmaLib.c"
#include "lzma/LzmaDec.c"
#include "lzma/LzmaEnc.c"
#include "lzma/Threads.c"
#include "lzma/LzFindMt.c"
#include "lzma/LzFind.c"
#include "lzma/Alloc.c"
}


////////////////////////////////////////////////////////////////////////////////////
// loc�ļ���ʽ����

#define LOCATE_MAGIC            0x636F6C00
#define LOCATE_VERISON          0x01

// ������Ŀ
typedef struct _LOCATE
{
    uint32_t begin_ip;          // ��ʼIP
    uint32_t table1;            // ����ƫ��
    uint32_t table2;            // ��ַƫ��
} LOCATE, *PLOCATE;

// �ļ�ͷ
typedef struct _HEADER
{
    uint32_t magic;             // �ļ�ǩ������ԶΪLOCATE_MAGIC
    uint16_t version;           // ���ɰ汾
    uint16_t compress;          // ѹ����ʽ 0δѹ�� 1 LZMAѹ��

    uint32_t total;             // ������
    uint32_t time;              // ���ݿ����ʱ��

    uint32_t table1;            // �����ַ�����ʼλ�ã�δѹ��ǰ��
    uint32_t table2;            // ��ַ�ַ�����ʼλ�ã�δѹ��ǰ��
    uint32_t size;              // ��������С��δѹ��ǰ��
    uint32_t crc32;             // ǰ�����ݵ�CRC32У����
    LOCATE data[0];             // ��������ʹ��LZMAѹ������ռ��ʵ�ʴ�С
} HEADER, *PHEADER;


////////////////////////////////////////////////////////////////////////////////////
// dif�ļ���ʽ����

#define DIFF_MAGIC              0x66696400

// �޸ķ�������
typedef enum _DIFF_METHOD
{
    INSERT = 0,                 // ׷��
    REMOVE = 1,                 // ɾ��
    MODIFY = 2                  // �޸�
} DIFF_METHOD;

// ������Ŀ
typedef struct _DIFFITEM
{
    uint32_t line;              // ƥ����
    uint32_t method;            // �޸ķ��� DIFF_METHOD
    uint32_t begin_ip;          // ��ʼIP
    uint32_t table1;            // ����ƫ��
    uint32_t table2;            // ��ַƫ��
} DIFFITEM, *PDIFFITEM;

// �ļ�ͷ
typedef struct _DIFFHEADER
{
    uint32_t magic;             // �ļ�ǩ������ԶΪDIFF_MAGIC

    uint32_t total1;            // �����ݿ�������
    uint32_t total2;            // �����ݿ�������

    uint32_t time1;             // �����ݿ����ʱ��
    uint32_t time2;             // �����ݿ����ʱ��

    uint32_t table1;            // �����ַ�����ʼλ�ã�δѹ��ǰ��
    uint32_t table2;            // ��ַ�ַ�����ʼλ�ã�δѹ��ǰ��

    uint32_t size;              // ��������С��δѹ��ǰ��
    uint32_t crc32;             // ǰ�����ݵ�CRC32У����
    DIFFITEM data[0];           // ��������ʹ��LZMAѹ������ռ��ʵ�ʴ�С
} DIFFHEADER, *PDIFFHEADER;


////////////////////////////////////////////////////////////////////////////////////

uint32_t str2ip(const char *lp)
{
    uint32_t ret = 0;
    uint8_t now = 0;

    while(*lp)
    {
        if('.' == *lp)
        {
            ret = 256 * ret + now;
            now = 0;
        }
        else
        {
            now = 10 * now + *lp - '0';
        }

        ++lp;
    }
    ret = 256 * ret + now;

    return ret;
}

////////////////////////////////////////////////////////////////////////////////////

#include <map>
#include <string>
#include <vector>
using namespace std;

typedef map <string, uint32_t> StringPool;
typedef vector <uint8_t> Buffer;

// �ַ�����
class StringTable
{
public:
    StringTable()
    {
        offset = 0;
    }

    uint32_t Append(const char *string)
    {
        if ( string_pool.count(string)==0 )
        {
            string_pool[string] = offset;

            size_t len = strlen(string);
            AppendBuffer((uint8_t*)string, len + 1);

            offset = offset + len + sizeof(uint8_t);
        }

        return string_pool.find(string)->second;
    }

    operator Buffer* const()
    {
        return &string_buffer;
    }
private:
    void AppendBuffer(const uint8_t *ptr, size_t len)
    {
        std::copy(ptr, ptr + len, std::back_inserter(string_buffer));
    }

    uint32_t offset;
    StringPool string_pool;
    Buffer string_buffer;
};

void AppendBuffer(Buffer &buffer, const uint8_t *ptr, size_t len)
{
    std::copy(ptr, ptr + len, std::back_inserter(buffer));
}


////////////////////////////////////////////////////////////////////////////////////

uint32_t CRC32_MEM(const uint8_t* buffer, uint32_t len)
{
    //����CRC32�Ĳ�ѯ��
    static uint32_t CRC32Table[256] = {0};
    if ( CRC32Table[1]==0 )
    {
        for (uint32_t i = 0; i < 256; i++)
        {
            CRC32Table[i] = i;
            for (uint32_t j = 0; j < 8; j++)
            {
                if (CRC32Table[i] & 1)
                    CRC32Table[i] = (CRC32Table[i] >> 1) ^ 0xEDB88320;
                else
                    CRC32Table[i] >>= 1;
            }
        }
    }

    //��ʼ����CRC32У��ֵ
    uint32_t crc32 = ~0;
    for ( uint32_t i = 0 ; i < len ; i++ )
    {
        crc32 = (crc32 >> 8) ^ CRC32Table[(crc32 & 0xFF) ^ buffer[i]];
    }

    return ~crc32;
}

#endif // __UTILITY_H_
