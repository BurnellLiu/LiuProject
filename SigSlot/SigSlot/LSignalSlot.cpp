
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
    /// 槽断开链接
    /// </SUMMARY>
    virtual void SlotDisconnect(LSlot* pSlot) = 0;
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

    }

    void Disconnect(LSlot)

protected:
    LConnectionsList m_connectionsList; // 链接列表
};

// class LSlot
// {
//     typedef set<>
// public:
// private:
// };
