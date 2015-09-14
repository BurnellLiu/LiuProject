

#ifndef _LWIFINetwork_H_
#define _LWIFINetwork_H_

#include <string>
using std::wstring;

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif


class CWifiNetwork;

/// <SUMMARY>
/// WIFI������
/// </SUMMARY>
class LWifiNetwork
{
public:
    LWifiNetwork();
    ~LWifiNetwork();

    /// <SUMMARY>
    /// WLAN�Ƿ����
    /// </SUMMARY>
    /// <RETURNS>
    /// ���ڷ���true, �����ڷ���false
    /// </RETURNS>
    bool IsWLANExist();

    /// <SUMMARY>
    /// ��ȡWLAN����
    /// </SUMMARY>
    /// <PARAM name = "desc" dir = "OUT">
    /// ����
    /// </PARAM>
    /// <RETURNS>
    ///  �ɹ���ȡ����true, ��ȡʧ����false
    /// </RETURNS>
    bool GetWLANDesc(OUT wstring& desc);

    /// <SUMMARY>
    /// ��ȡWLAN GUID
    /// </SUMMARY>
    /// <PARAM name = "guid" dir = "OUT">
    /// strGUID
    /// </PARAM>
    /// <RETURNS>
    /// �ɹ���ȡ����true, ��ȡʧ����false
    /// </RETURNS>
    bool GetWLANGUID(OUT wstring& strGUID);

private:
    CWifiNetwork* m_pWifiNetwork;
};

#endif