/// @file LMemory.h
/// @brief ���ļ����������ڴ������ 
/// 
/// Detail:
/// @author Burnell_Liu  
/// @version   
/// @date 9:11:2015

#ifndef _LMEMORY_H_
#define _LMEMORY_H_


    /// @brief �ڴ���
class LMemory
{
public:
    LMemory();
    ~LMemory();

    /// @brief ��ȡ�ڴ�ʹ�ðٷֱ�
    /// @return �ɹ����ذٷֱ�, ʧ�ܷ���-1
    int GetLoadPercent();

    /// @brief ��ȡ���ڴ��С(��λM)
    /// @return �ɹ������ڴ��С, ʧ�ܷ���-1
    int GetTotalSize();

    /// @brief ��ȡ�����ڴ��С
    /// @return �ɹ����ؿ����ڴ��С, ʧ�ܷ���-1
    int GetAvailableSize();
};

#endif