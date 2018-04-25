
#pragma once

#include <set>
using std::set;
#include <list>
using std::list;


class LSlot;

/// @brief 没有参数的链接基类模板
class LConnectionBase0
{
public:
    virtual ~LConnectionBase0() {}

    /// @brief 获取槽对象
    virtual LSlot* GetDest() const = 0;

    /// @brief 发送信号
    virtual void EmitSignal() = 0;
};


/// @brief 带有一个参数的链接基类模板
template<typename TParam>
class LConnectionBase1
{
public:
    virtual ~LConnectionBase1() {}

    /// @brief 获取槽对象
    virtual LSlot* GetDest() const = 0;

    /// @brief 发送信号
    virtual void EmitSignal(TParam) = 0;
};

/// @brief 没有参数的链接类模板
template<typename TObject>
class LConnection0 : public LConnectionBase0
{
    typedef void (TObject::*TFunc)();
public:
    /// @brief 构造函数
    LConnection0(TObject* pObject, TFunc pFunc)
    {
        m_pObject = pObject;
        m_pFunc = pFunc;
    }

    /// @brief 获取槽对象
    LSlot* GetDest() const
    {
        return m_pObject;
    }

    /// @brief 发送信号
    void EmitSignal()
    {
        (m_pObject->*m_pFunc)();
    }

private:
    TObject* m_pObject;     // 链接对象
    TFunc m_pFunc;          // 链接对象成员函数
};

/// @brief 带有1个参数的链接类模板
template<typename TObject, typename TParam>
class LConnection1 : public LConnectionBase1<TParam>
{
    typedef void (TObject::*TFunc)(TParam);
public:
    /// @brief 构造函数
    LConnection1(TObject* pObject, TFunc pFunc)
    {
        m_pObject = pObject;
        m_pFunc = pFunc;
    }

    /// @brief 获取槽对象
    LSlot* GetDest() const
    {
        return m_pObject;
    }

    /// @brief 发送信号
    void EmitSignal(TParam param)
    {
        (m_pObject->*m_pFunc)(param);
    }

private:
    TObject* m_pObject;     // 链接对象
    TFunc m_pFunc;          // 链接对象成员函数
};


/// @brief 信号基类
class LSignalBase
{
public:
    virtual ~LSignalBase() {}

    /// @brief 槽要求断开链接
    virtual void SlotDisconnect(LSlot* pSlot) = 0;
};


/// @brief 槽类
/// 
/// 槽对象中保存有一个信号集合, 以便在槽对象析构时, 
/// 调用信号对象断开链接, 防止槽对象删除后, 
/// 信号对象依然呼叫槽函数
/// 
/// 抽象出基类LSignalBase供槽类使用更方便
class LSlot
{
    typedef set<LSignalBase*> LSignalsSet;

public:
    /// @brief 构造函数
    LSlot()
    {

    }

    /// @brief 析构函数
    ~LSlot()
    {
        this->DisconnectAll();
    }

    /// @brief 信号要求进行链接
    void SignalConnect(LSignalBase* pSignal)
    {
        m_signalsSet.insert(pSignal);
    }

    /// @brief 信号要求断开链接
    void SignalDisconnect(LSignalBase* pSignal)
    {
        m_signalsSet.erase(pSignal);
    }

    /// @brief 断开所有连接
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


/// @brief 没有参数的信号类
/// 
/// 信号对象中需要保存槽对象和槽函数
/// 槽对象和槽函数可以为不同类实例
/// 所以我们需要使用链接类对它们进行封装
class LSignal0 : public LSignalBase
{
    typedef list<LConnectionBase0*> LConnectionsList;

public:
    /// @brief 构造函数
    LSignal0()
    {

    }

    /// @brief 析构函数
    ~LSignal0()
    {
        this->DisconnectAll();
    }

    /// @brief 链接槽
    template<typename TObject>
    void Connect(TObject* pObject, void (TObject::*pFunc)())
    {
        LConnection0<TObject>* pConn = new LConnection0<TObject>(pObject, pFunc);
        m_connsList.push_back(pConn);

        pObject->SignalConnect(this);
    }

    /// @brief 激发信号
    void Emit()
    {
        for (auto iter = m_connsList.begin(); iter != m_connsList.end(); iter++)
        {
            (*iter)->EmitSignal();
        }
    }

    /// @brief 断开指定槽
    void Disconnect(LSlot* pSlot)
    {
        /*
        信号主动断开链接时, 需要在槽中也断开与信号的链接,
        并且删除链接对象
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

    /// @brief 断开所有链接
    void DisconnectAll()
    {
        /*
        信号主动断开链接时, 需要在槽中也断开与信号的链接
        并且删除链接对象
        */

        for (auto iter = m_connsList.begin(); iter != m_connsList.end(); iter++)
        {
            (*iter)->GetDest()->SignalDisconnect(this);

            delete *iter;
        }

        m_connsList.clear();
    }

    /// @brief 槽要求断开链接
    void SlotDisconnect(LSlot* pSlot)
    {
        /*
        槽要求断开链接时, 槽自己会处理断开与信号的链接
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

private:
    LConnectionsList m_connsList; // 链接列表
};


/// @brief 带有一个参数的信号类模板
/// 
/// 信号对象中需要保存槽对象和槽函数
/// 槽对象和槽函数可以为不同类实例
/// 所以我们需要使用链接类对它们进行封装
template<typename TParam>
class LSignal1 : public LSignalBase
{
    typedef list<LConnectionBase1<TParam>*> LConnectionsList;

public:
    /// @brief 构造函数
    LSignal1()
    {

    }

    /// @brief 析构函数
    ~LSignal1()
    {
        this->DisconnectAll();
    }

    /// @brief 链接槽
    template<typename TObject>
    void Connect(TObject* pObject, void (TObject::*pFunc)(TParam))
    {
        LConnection1<TObject, TParam>* pConn = new LConnection1<TObject, TParam>(pObject, pFunc);
        m_connsList.push_back(pConn);

        pObject->SignalConnect(this);
    }

    /// @brief 激发信号
    void Emit(TParam param)
    {
        for (auto iter = m_connsList.begin(); iter != m_connsList.end(); iter++)
        {
            (*iter)->EmitSignal(param);
        }
    }

    /// @brief 断开指定槽
    void Disconnect(LSlot* pSlot)
    {
        /*
        信号主动断开链接时, 需要在槽中也断开与信号的链接,
        并且删除链接对象
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

    /// @brief 断开所有链接
    void DisconnectAll()
    {
        /*
        信号主动断开链接时, 需要在槽中也断开与信号的链接
        并且删除链接对象
        */

        for (auto iter = m_connsList.begin(); iter != m_connsList.end(); iter++)
        {
            (*iter)->GetDest()->SignalDisconnect(this);

            delete *iter;
        }

        m_connsList.clear();
    }

    /// @brief 槽要求断开链接
    void SlotDisconnect(LSlot* pSlot)
    {
        /*
        槽要求断开链接时, 槽自己会处理断开与信号的链接
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

private:
    LConnectionsList m_connsList; // 链接列表
};



