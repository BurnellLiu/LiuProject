
#include "LSMARTPaser.h"

#include <cstring>

#define MAX_SMART_ATTRIBUTES 30

/// @brief SMART属性ID
enum SMART_ATTRIB_ID
{
    POWN_ON_HOURS = 0x09, ///< 通电时间
    TEMPERATURE = 0xC2 //温度
};

// 以下结构取消内存对齐
#pragma pack (1) // 取消内存对齐

/// @brief SMART每项属性结构
struct SSMARTAttribute
{
    unsigned char Id; // 属性ID
    unsigned short StatusFlags; // 状态值
    unsigned char Current; // 当前值
    unsigned char Worst; // 最差值
    unsigned char RawValue[6]; // 真实值
    unsigned char Reserved; // 保留
};

/// @brief SMART数据结构
struct SSMARTData
{
    unsigned short Version; // SMART版本
    SSMARTAttribute AttributeArray[MAX_SMART_ATTRIBUTES]; // SMART属性数组, 最多30项
};

#pragma pack () // 恢复内存对齐

/// @brief SMART解析实现类
class CSMARTParser
{
public:
    CSMARTParser(IN unsigned short smartData[SMART_DATA_LENGTH]);
    ~CSMARTParser();

    /// @brief 获取温度值, 单位摄氏度
    /// @param[out] temp 存储温度值
    /// @return 成功返回true, 失败返回false
    bool GetTemperature(OUT int& temp);

    /// @brief 获取通电时间, 单位小时
    ///  
    /// @param[out] hours 存储通电时间
    /// @return 成功返回true, 失败返回false
    bool GetPowerOnHours(OUT unsigned long& hours);

private:
    SSMARTData m_smartData; ///< 存储SMART数据
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