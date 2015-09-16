
#include "LSMARTPaser.h"

#include <cstring>

#define MAX_SMART_ATTRIBUTES 30

/// @brief SMART����ID
enum SMART_ATTRIB_ID
{
    POWN_ON_HOURS = 0x09, ///< ͨ��ʱ��
    TEMPERATURE = 0xC2 //�¶�
};

// ���½ṹȡ���ڴ����
#pragma pack (1) // ȡ���ڴ����

/// @brief SMARTÿ�����Խṹ
struct SSMARTAttribute
{
    unsigned char Id; // ����ID
    unsigned short StatusFlags; // ״ֵ̬
    unsigned char Current; // ��ǰֵ
    unsigned char Worst; // ���ֵ
    unsigned char RawValue[6]; // ��ʵֵ
    unsigned char Reserved; // ����
};

/// @brief SMART���ݽṹ
struct SSMARTData
{
    unsigned short Version; // SMART�汾
    SSMARTAttribute AttributeArray[MAX_SMART_ATTRIBUTES]; // SMART��������, ���30��
};

#pragma pack () // �ָ��ڴ����

/// @brief SMART����ʵ����
class CSMARTParser
{
public:
    CSMARTParser(IN unsigned short smartData[SMART_DATA_LENGTH]);
    ~CSMARTParser();

    /// @brief ��ȡ�¶�ֵ, ��λ���϶�
    /// @param[out] temp �洢�¶�ֵ
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetTemperature(OUT int& temp);

    /// @brief ��ȡͨ��ʱ��, ��λСʱ
    ///  
    /// @param[out] hours �洢ͨ��ʱ��
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetPowerOnHours(OUT unsigned long& hours);

private:
    SSMARTData m_smartData; ///< �洢SMART����
};

CSMARTParser::CSMARTParser(IN unsigned short smartData[SMART_DATA_LENGTH])
{
    memcpy(&m_smartData, smartData, SMART_DATA_LENGTH);
}

CSMARTParser::~CSMARTParser()
{

}

bool CSMARTParser::GetTemperature(OUT int& temp)
{
    bool bRet = false;

    for (int i = 0; i < MAX_SMART_ATTRIBUTES; i++)
    {
        if (m_smartData.AttributeArray[i].Id == TEMPERATURE)
        {
            bRet = true;
            temp = m_smartData.AttributeArray[i].RawValue[5];
            break;
        }
    }

    return bRet;
}