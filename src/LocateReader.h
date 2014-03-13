#ifndef __LOCATE_READER_H_
#define __LOCATE_READER_H_

#include "Locate.h"

// location.loc�ļ���ȡ��
class LocateReader : public LocateBase
{
public:
    LocateReader(const wchar_t *path);            // ���ļ�
    ~LocateReader();

    bool Reload(const wchar_t *path);             // ���������ļ�

    LocateItem* GetItem(uint32_t index);          // ����������ѯ��ַ
    LocateItem* GetLocate(const char *ip);        // �����ַ���IP��ѯ��ַ
    LocateItem* GetLocate(uint32_t ip);           // ����С������IP��ѯ��ַ
private:
    void Load(const wchar_t *path);
    void Clearup();
};

#endif // __LOCATE_READER_H_
