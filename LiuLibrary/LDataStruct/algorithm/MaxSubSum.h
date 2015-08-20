#ifndef _MAXSUBSUM_H_
#define _MAXSUBSUM_H_

#include <vector>
using std::vector;

/// @brief ������������к��㷨
class MaxSubSum
{

public:
	/// @brief �����㷨
	///
	/// ���㷨ͨ��ö���������������õ������ʱ�临�Ӷ�ΪO(N3), ����
	/// @param[in] data ������������
	/// @return ������������к�
	static int CubicAlgorithm(const vector<int>& data)
	{
		// begin �����п�ʼλ��
		// end �����н���λ��
		
		int maxSum = 0;
		for (unsigned int begin = 0; begin < data.size(); begin++)
		{
			for (unsigned int end = begin; end < data.size(); end++)
			{
				int thisSum = 0;

				for (unsigned int i = begin; i <= end; i++)
					thisSum += data[i];

				if (thisSum > maxSum)
					maxSum = thisSum;

			}
		}

		return maxSum;
	}
	
	/// @brief ƽ���㷨
	/// 
	/// ���㷨Ϊ�����㷨�ĸĽ��汾
	/// @param[in] data ������������
	/// @return ������������к�
	static int QuadraticAlgorithm(const vector<int>& data)
	{
		int maxSum = 0;

		for (unsigned int begin = 0; begin < data.size(); begin++)
		{
			int thisSum = 0;
			for (unsigned int end = begin; end < data.size(); end++)
			{
				thisSum += data[end];

				if (thisSum > maxSum)
					maxSum = thisSum;

			}
		}

		return maxSum;
	}

	
	/// @brief �ݹ��㷨
	/// 
	/// ���㷨�ĺ���˼��Ϊ����������кͿ��ܳ����������ط�.
	/// 1.�����������������ݵ���벿��
	/// 2.�����������������ݵ��Ұ벿��
	/// 3.��Խ�������ݵ��в���
	/// ǰ����������Եݹ����
	/// ���㷨ʱ�临�Ӷ�O(NlogN)
	/// @param[in] data ������������
	/// @param[in] left ��߽�
	/// @param[in] right �ұ߽�
	/// @return ������-1�����򷵻�������������к�
	static int RecursiveAlgorithm(const vector<int>& data, int left, int right)
	{
		if (right >= (int)data.size() || left < 0)
			return -1;

		if (left == right)
		{
			if (data[left] < 0)
				return 0;
			else
				return data[left];
		}

		int center = (left + right)/2;
		int maxLeftSum = RecursiveAlgorithm(data, left, center);
		int maxRightSum = RecursiveAlgorithm(data, center + 1, right);

		int maxLeftBorderSum = 0;
		int leftBorderSum = 0;
		for (int i = center; i >= left; i--)
		{
			leftBorderSum += data[i];
			if (leftBorderSum > maxLeftBorderSum)
				maxLeftBorderSum = leftBorderSum;
		}

		int maxRigntBorderSum = 0;
		int rightBorderSum = 0;
		for (int i = center + 1; i <= right; i++)
		{
			rightBorderSum += data[i];
			if (rightBorderSum > maxRigntBorderSum)
				maxRigntBorderSum = rightBorderSum;
		}

		int maxBorderSum = maxLeftBorderSum + maxRigntBorderSum;

		if (maxLeftSum > maxBorderSum)
		{
			if (maxLeftSum > maxRightSum)
				return maxLeftSum;
			else
				return maxRightSum;
		}
		else
		{
			if (maxBorderSum > maxRightSum)
				return maxBorderSum;
			else
				return maxRightSum;
		}

	}

	/// @brief ����ʱ���㷨
	///
	/// ���㷨�ĺ���˼�����κθ��������в����������������е�ǰ׺
	/// @param[in] data ��������
	/// @return ������������к�
	static int LinearTimeAlgorithm(const vector<int>& data)
	{
		int maxSubSum = 0;
		int thisSubSum = 0;

		for (unsigned int i = 0; i < data.size(); i++)
		{
			thisSubSum += data[i];
			if (thisSubSum > maxSubSum)
				maxSubSum = thisSubSum;
			else if (thisSubSum < 0)
				thisSubSum = 0;
		}

		return maxSubSum;
	}

};
#endif