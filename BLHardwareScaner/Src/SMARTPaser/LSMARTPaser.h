

#ifndef _LSMARTPASER_H_
#define _LSMARTPASER_H_

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

/// @brief SMART���ݳ���
#ifndef SMART_DATA_LENGTH
#define SMART_DATA_LENGTH 362
#endif



class CSMARTParser;

/// @brief SMART������
class LSMARTParser
{
public:
    /// @brief ���캯��
    /// @param[in] smartData[362] Ҫ������362���ֽڵ�SMART����
    LSMARTParser(IN unsigned char smartData[SMART_DATA_LENGTH]);
    ~LSMARTParser();

    /// @brief ��ȡ�¶�ֵ, ��λ���϶�
    /// @param[out] temp �洢�¶�ֵ
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetTemperature(OUT unsigned int& temp);

    /// @brief ��ȡͨ��ʱ��, ��λСʱ
    ///  
    /// @param[out] hours �洢ͨ��ʱ��, ��λСʱ(hours)
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetPowerOnHours(OUT unsigned long& hours);

private:
    CSMARTParser* m_pSMARTParser; ///< SMART����ʵ�ֶ���
};

#endif