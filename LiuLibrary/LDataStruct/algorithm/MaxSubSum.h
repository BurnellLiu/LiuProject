#ifndef _MAXSUBSUM_H_
#define _MAXSUBSUM_H_

#include <vector>
using std::vector;

/// @brief 最大连续子序列和算法
class MaxSubSum
{

public:
	/// @brief 立方算法
	///
	/// 该算法通过枚举所有子序列来得到结果，时间复杂度为O(N3), 立方
	/// @param[in] data 输入数据序列
	/// @return 最大连续子序列和
	static int CubicAlgorithm(const vector<int>& data)
	{
		// begin 子序列开始位置
		// end 子序列结束位置
		
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
	
	/// @brief 平方算法
	/// 
	/// 该算法为立方算法的改进版本
	/// @param[in] data 输入数据序列
	/// @return 最大连续子序列和
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

	
	/// @brief 递归算法
	/// 
	/// 该算法的核心思想为：最大子序列和可能出现在三处地方.
	/// 1.整个出现在输入数据的左半部。
	/// 2.整个出现在输入数据的右半部。
	/// 3.跨越输入数据的中部。
	/// 前两种情况可以递归求解
	/// 该算法时间复杂度O(NlogN)
	/// @param[in] data 输入数据序列
	/// @param[in] left 左边界
	/// @param[in] right 右边界
	/// @return 出错返回-1，否则返回最大连续子序列和
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

	/// @brief 线性时间算法
	///
	/// 该算法的核心思想是任何负的子序列不可能是最优子序列的前缀
	/// @param[in] data 数据序列
	/// @return 最大连续子序列和
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