#ifndef __QQWRY_READER_H_
#define __QQWRY_READER_H_

#include "Locate.h"

// QQWry.dat�ļ���ȡ��
class QQWryReader : public LocateBase
{
public:
    QQWryReader(const wchar_t *path);             // ���ļ�
    ~QQWryReader();

    LocateItem* GetItem(uint32_t index);          // ����������ѯ��ַ
    LocateItem* GetLocate(const char *ip);        // �����ַ���IP��ѯ��ַ
    LocateItem* GetLocate(uint32_t ip);           // ����С������IP��ѯ��ַ
};

#endif // __QQWRY_READER_H_
