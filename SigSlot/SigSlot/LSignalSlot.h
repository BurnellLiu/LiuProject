
#pragma once

#include <set>
using std::set;
#include <list>
using std::list;


class LSlot;


template<typename TParam>
class LConnectionBase
{
public:
    virtual ~LConnectionBase() {}
    virtual LSlot* GetDest() const = 0;
    virtual void EmitSignal(TParam) = 0;
};


template<typename TObject, typename TParam>
class LConnection : public LConnectionBase<TParam>
{
    typedef void (TObject::*TFunc)(TParam);
public:
    LConnection(TObject* pObject, TFunc pFunc)
    {
        m_pObject = pObject;
        m_pFunc = pFunc;
    }

    void EmitSignal(TParam param)
    {
        (m_pObject->*m_pFunc)(param);
    }

    LSlot* GetDest() const
    {
        return m_pObject;
    }

private:
    TObject* m_pObject;     // 链接对象
    TFunc m_pFunc;          // 链接对象成员函数
};

/// <SUMMARY>
/// 信号基类接口
/// </SUMMARY>
class LSignalBaseI
{
public:
    virtual ~LSignalBaseI() {}

    /// <SUMMARY>
    /// 槽要求断开链接
    /// </SUMMARY>
    virtual void SlotDisconnect(LSlot* pSlot) = 0;
};


/// <SUMMARY>
/// 槽类
/// </SUMMARY>
class LSlot
{
    typedef set<LSignalBaseI*> LSignalsSet;

public:
    LSlot()
    {

    }

    ~LSlot()
    {
        this->DisconnectAll();
    }

    /// <SUMMARY>
    /// 信号要求进行链接
    /// </SUMMARY>
    void SignalConnect(LSignalBaseI* pSignal)
    {
        m_signalsSet.insert(pSignal);
    }

    /// <SUMMARY>
    /// 信号要求断开链接
    /// </SUMMARY>
    void SignalDisconnect(LSignalBaseI* pSignal)
    {
        m_signalsSet.erase(pSignal);
    }

    /// <SUMMARY>
    /// 断开所有连接
    /// </SUMMARY>
    void DisconnectAll()
    {
        for (auto iter = m_signalsSet.begin(); iter != m_signalsSet.end(); iter++)
        {
            (*iter)->SlotDisconnect(this);
        }

        m_signalsSet.clear();

    }
private:
    LSignalsSet m_signalsSet; // 信号集
};


/// <SUMMARY>
/// 信号基类
/// </SUMMARY>
template<typename TParam>
class LSignalBase : public LSignalBaseI
{
    typedef list<LConnectionBase<TParam>*> LConnectionsList;

public:
    LSignalBase()
    {

    }

    ~LSignalBase()
    {
        this->DisconnectAll();
    }

    /// <SUMMARY>
    /// 断开指定槽
    /// </SUMMARY>
    void Disconnect(LSlot* pSlot)
    {
        /*
        主动断开槽时, 需要在槽中也断开信号
        */

        auto iter = m_connsList.begin();
        while (iter != m_connsList.end())
        {
            if ((*iter)->GetDest() == pSlot)
            {
                delete *iter;
                iter = m_connsList.erase(iter);

                pSlot->SignalDisconnect(this);
            }
            else
            {
                iter++;
            }

        }
    }

    /// <SUMMARY>
    /// 断开所有链接
    /// </SUMMARY>
    void DisconnectAll()
    {
        /*
        主动断开槽时, 需要在槽中也断开信号
        */

        for (auto iter = m_connsList.begin(); iter != m_connsList.end(); iter++)
        {
            (*iter)->GetDest()->SignalDisconnect(this);
        }

        m_connsList.clear();
    }

    /// <SUMMARY>
    /// 槽要求断开链接
    /// </SUMMARY>
    void SlotDisconnect(LSlot* pSlot)
    {
        /*
        被槽要求断开时, 槽自己处理断开信号
        */

        auto iter = m_connsList.begin();
        while (iter != m_connsList.end())
        {
            if ((*iter)->GetDest() == pSlot)
            {
                delete *iter;
                iter = m_connsList.erase(iter);
            }
            else
            {
                iter++;
            }
                
        }
    }

protected:
    LConnectionsList m_connsList; // 链接列表
};


template<typename TParam>
class LSignal : public LSignalBase<TParam>
{
public:
    template<typename TObject>
    void Connect(TObject* pObject, void (TObject::*pFunc)(TParam))
    {
        LConnection<TObject, TParam>* pConn = new LConnection<TObject, TParam>(pObject, pFunc);
        m_connsList.push_back(pConn);

        pObject->SignalConnect(this);
    }

    void operator()(TParam param)
    {
        for (auto iter = m_connsList.begin(); iter != m_connsList.end(); iter++)
        {
            (*iter).EmitSignal(param);
        }
    }
};


