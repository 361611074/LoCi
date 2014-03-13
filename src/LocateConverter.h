#ifndef __LOCATE_CONVERTER_H_
#define __LOCATE_CONVERTER_H_

#include "Locate.h"

// ���Ȼص���������
typedef void (*OnProgress)(double rate);

// ��ʽת���࣬�ṩһϵ��ת������
class LocateConverter
{
public:
    // ת��QQWry.DAT�ļ�Ϊlocation.loc��compress��ʾ�Ƿ�ʹ��LZMA����ѹ��
    bool QQWryToLocate(const wchar_t *qqwry, const wchar_t *locate, bool compress = false, OnProgress progress = NULL);

    // ת��location.loc�ļ�ΪQQWry.DAT
    bool LocateToQQWry(const wchar_t *locate, const wchar_t *qqwry);

    // ת����location.loc�ļ�Ϊ��location.loc��compress��ʾ�Ƿ�ʹ��LZMA����ѹ��
    bool LocateUpdate(const wchar_t *locate, const wchar_t *patch, const wchar_t *path, bool compress = false, OnProgress progress = NULL);

    // ͨ��������ͬ�汾��QQWry.DAT�ļ����ɲ����ļ�
    bool GeneratePatch(const wchar_t *qqwry1_path, const wchar_t *qqwry2_path);
};

#endif // __LOCATE_CONVERTER_H_
