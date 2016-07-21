
#pragma once
#ifndef _MULTITHREAD_H_
#define _MULTITHREAD_H_


/// @brief �ֶ���
class LManualLock
{
public:
	/// @brief ���캯��
	LManualLock();

	/// @brief ��������
	~LManualLock();

	/// @brief ����
	void Lock();

	/// @brief ����
	void UnLock();
private:
	LManualLock(const LManualLock&);
	void operator = (const LManualLock&);

	void* m_pLock; ///< ʵ��������
};

/// @brief �Զ���
class LAutoLock
{
public:
    /// @brief ���캯��
	LAutoLock(LManualLock* pManualLock);

    /// @brief ��������
	~LAutoLock();

private:
	LAutoLock(const LAutoLock& );
	void operator = (const LAutoLock&);

	LManualLock* m_pManualLock; ///< �ֶ�������
};
#endif