
#include <cmath>
#include <cstdlib>
#include <time.h>

#include "LNeuralNetwork.h"

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) \
do\
{\
    if ((p) != 0)\
    {\
        delete p;\
        p = 0;\
    }\
}while(0)
#endif

void SRandTime()
{
    srand((int)time(0));
}

/// @brief 产生随机小数, 范围0~1
/// @return 随机小数
static float RandFloat()		   
{
    return (rand())/(RAND_MAX + 1.0f);
}

/// @brief 产生随机小数, 范围-1~1
/// @return 随机小数
static float RandClamped()	   
{
    return RandFloat() - RandFloat();
}

LNeuronNet::SNeuron::SNeuron(IN int inputNum)
	: InputNum(inputNum), WeightList(0)
{
	WeightList.Reset(InputNum);
	PrevUpdate.Reset(InputNum);
	for (int i = 0; i < InputNum; i++)
	{
		WeightList.Data[i] = 0.0f;
		PrevUpdate.Data[i] = 0.0f;
	}

	Error = 0.0f;
	Output = 0.0f;
	
}

LNeuronNet::SNeuron::~SNeuron()
{

}

LNeuronNet::SNeuronLayer::SNeuronLayer(IN int neuronNum, IN int neuronInputNum)
	: NeuronNum(neuronNum)
{
	InputList.Reset(neuronInputNum);
	// 要为偏移值也附加一个输出, 因此应将输出数目加1
	OutputList.Reset(neuronNum + 1);
	NeuronList.Reset(neuronNum);

	for (int i = 0; i < neuronNum; i++)
	{
		NeuronList.Data[i] = new SNeuron(neuronInputNum);
	}
}

LNeuronNet::SNeuronLayer::~SNeuronLayer()
{
	for (int i = 0; i < NeuronNum; i++)
	{
		SAFE_DELETE(NeuronList.Data[i]);
	}
}

LNeuronNet::LNeuronNet()
{
	m_inputNum = 0;
	m_outputNum = 0;
	m_neuronNumPerHiddenLayer = 0;
	m_weightNumTotal = 0;
	m_layerNum = 0;
	m_biasInput = -1.0f;
}

LNeuronNet::~LNeuronNet()
{
	this->CleanUp();
}

void LNeuronNet::Init(IN int inputNum, IN int outputNum, 
	IN int hiddenLayerNum, IN int neuronNumPerHiddenLayer)
{
	if (inputNum <= 0 || outputNum <= 0 || neuronNumPerHiddenLayer <= 0 || hiddenLayerNum < 0)
		return;

	this->CleanUp();

	// 要为偏移值也附加一个输入, 因此应将输入数目加1
	m_inputNum = inputNum + 1;
	m_outputNum = outputNum;
	m_neuronNumPerHiddenLayer = neuronNumPerHiddenLayer;

	m_layerNum = hiddenLayerNum + 1;
	m_layerList.Reset(m_layerNum);
	
	// 只有输出层
	if (hiddenLayerNum == 0)
	{
		m_layerList.Data[0] = new SNeuronLayer(m_outputNum, m_inputNum);
		m_weightNumTotal = m_outputNum * m_inputNum;
		return;
	}

	// 创建 第一个隐藏层
	m_layerList.Data[0] = new SNeuronLayer(m_neuronNumPerHiddenLayer, m_inputNum);
	m_weightNumTotal += m_neuronNumPerHiddenLayer * m_inputNum;
	for (int i = 1; i <  hiddenLayerNum; i++)
	{
		m_layerList.Data[i] = new SNeuronLayer(m_neuronNumPerHiddenLayer, m_neuronNumPerHiddenLayer + 1);
		m_weightNumTotal += m_neuronNumPerHiddenLayer * (m_neuronNumPerHiddenLayer + 1);
	}

	// 创建输出层
	 m_layerList.Data[m_layerNum - 1] = new SNeuronLayer(m_outputNum, m_neuronNumPerHiddenLayer + 1);
	 m_weightNumTotal += m_outputNum * (m_neuronNumPerHiddenLayer + 1);
}

void LNeuronNet::Active(IN LNNInputList& inputList, INOUT LNNOutputList& outputList)
{
	static LNNInputList inputListRevise; // 使用静态变量, 提高程序效率

	if (inputList.Length != (m_inputNum - 1))
		return;

	// 将偏移输入值加入到输入列表中
	inputListRevise.Reset(m_inputNum);
	for (int i = 0; i < m_inputNum - 1; i++)
	{
		inputListRevise.Data[i] = inputList.Data[i];
	}
	inputListRevise.Data[m_inputNum - 1] = m_biasInput;

	// 每一层
	for (int i = 0; i < m_layerNum; i++)
	{
		if (i == 0)
			m_layerList.Data[i]->InputList = inputListRevise;
		else
			m_layerList.Data[i]->InputList = m_layerList.Data[i - 1]->OutputList;

		LNNInputList& layerInputList = m_layerList.Data[i]->InputList;
		SNeuronLayer& neuronLayer = (*m_layerList.Data[i]);

		// 每个神经元
		for (int j = 0; j < neuronLayer.NeuronNum; j++)
		{
			SNeuron& neuron = *(neuronLayer.NeuronList.Data[j]);
			float neuronActivation = 0.0f; // 神经元激励值

			//每个权重
			for (int k = 0; k < neuron.InputNum; k++)
			{
				neuronActivation += neuron.WeightList.Data[k] * (layerInputList.Data[k]);
			}

			neuron.Output = this->Sigmoid(neuronActivation, 1);
			neuronLayer.OutputList.Data[j] = neuron.Output;
		}
		neuronLayer.OutputList.Data[neuronLayer.NeuronNum] = m_biasInput;

	}

	if (m_layerNum >= 1)
	{
		outputList.Reset(m_outputNum);
		for (int i = 0; i < m_outputNum; i++)
		{
			outputList.Data[i] = m_layerList.Data[m_layerNum - 1]->OutputList.Data[i];
		}
	}

}

float LNeuronNet::Sigmoid(IN float inputA, IN float inputP)
{
	return ( 1.0f / ( 1.0f + exp(-inputA/inputP)));
}

void LNeuronNet::CleanUp()
{
	for (int i = 0; i < m_layerNum; i++)
	{
		SAFE_DELETE(m_layerList.Data[i]);
	}
}

void LUnSuperviseNN::GetWeightTotal(INOUT LNNWeightList& weightList)
{
	if (weightList.Length != m_weightNumTotal)
		weightList.Reset(m_weightNumTotal);

	int index = 0;

	//每一层
	for (int i = 0; i < m_layerNum; i++)
	{
		SNeuronLayer& neuronLayer = (*m_layerList.Data[i]);

		// 每个神经元
		for (int j = 0; j < neuronLayer.NeuronNum; j++)
		{
			SNeuron& neuron = *(neuronLayer.NeuronList.Data[j]);

			// 每个权重
			for (int k = 0; k < neuron.InputNum; k++)
			{
				weightList.Data[index++] = neuron.WeightList.Data[k];
			}
		}
	}
}

void LUnSuperviseNN::PutWeightTotal(IN LNNWeightList& weightList)
{
	if (weightList.Length != m_weightNumTotal)
		return;

	int totalIndex = 0;

	//每一层
	for (int i = 0; i < m_layerNum; i++)
	{
		SNeuronLayer& neuronLayer = (*m_layerList.Data[i]);

		// 每个神经元
		for (int j = 0; j < neuronLayer.NeuronNum; j++)
		{
			SNeuron& neuron = *(neuronLayer.NeuronList.Data[j]);

			// 每个权重
			for (int k = 0; k < neuron.InputNum; k++)
			{
				neuron.WeightList.Data[k] = weightList.Data[totalIndex++];
			}
		}
	}
}

void LUnSuperviseNN::GetSplitPointList(INOUT LNNSplitPointList& splitPointList)
{
	int splitPointNum = m_neuronNumPerHiddenLayer * (m_layerNum - 1) + m_outputNum;

	if (splitPointList.Length != splitPointNum)
		splitPointList.Reset(splitPointNum);

	int splitPointIndex = 0;
	int weightCount = 0;
	for (int i = 0; i < m_layerNum; i++)
	{
		SNeuronLayer& neuronLayer = *(m_layerList.Data[i]);

		for (int j = 0; j < neuronLayer.NeuronNum; j++)
		{
			SNeuron& neuron = *(neuronLayer.NeuronList.Data[j]);

			for (int k = 0; k < neuron.InputNum; k++)
				weightCount++;

			splitPointList.Data[splitPointIndex++] = weightCount - 1;
		}
	}
}

LSuperviseNN::LSuperviseNN()
{
    SRandTime();
	m_learnRate = 0.5f;
	m_momentum = 0.9f;
}

LSuperviseNN::~LSuperviseNN()
{

}

void LSuperviseNN::SetRandomWeight()
{
	for (int i = 0; i < m_layerNum; i++)
	{
		SNeuronLayer& neuronLayer = *m_layerList.Data[i];

		for (int k = 0; k < neuronLayer.NeuronNum; k++)
		{
			SNeuron& neuron = *neuronLayer.NeuronList.Data[k];

			for (int w = 0; w < neuron.InputNum; w++)
			{
				neuron.WeightList.Data[w] = RandClamped();
			}
		}
	}
}

float LSuperviseNN::Train(IN LNNInputList& inputList, IN LNNOutputList& targetOutputList)
{
	static LNNOutputList outputList; // 使用静态变量, 提高程序效率

	float errorSum = 0.0f; //误差总值

	if (m_layerNum < 1)
		return errorSum;
	if (inputList.Length != (m_inputNum - 1))
		return errorSum;
	if (targetOutputList.Length != m_outputNum)
		return errorSum;

	
	// 计算当前输出
	this->Active(inputList, outputList);

	SNeuronLayer* pOutputLayer = m_layerList.Data[m_layerNum - 1];
	// 为每个输出神经细胞 计算误差并调整相应的权重
	for (int i = 0; i < m_outputNum; i++)
	{
		// 计算误差值
		float targetOutput = targetOutputList.Data[i];
		float output = outputList.Data[i];
		float err = targetOutput - output;
		errorSum += err * err;
		err *= output * (1 - output);

		SNeuron* pNeuron = pOutputLayer->NeuronList.Data[i];
		pNeuron->Error = err;

		for (int j = 0; j < pOutputLayer->InputList.Length; j++)
		{
			float weightUpdate = m_learnRate * err * pOutputLayer->InputList.Data[j];
			// 把动量加入到权重更新中
			pNeuron->WeightList.Data[j] += weightUpdate + pNeuron->PrevUpdate.Data[j] * m_momentum;
			pNeuron->PrevUpdate.Data[j] = weightUpdate;
		}

	}

	// 对每个隐藏层
	for (int i = m_layerNum - 2; i >= 0; i--)
	{
		SNeuronLayer* pHiddenLayer = m_layerList.Data[i];

		// 对每个神经元
		for(int j = 0; j < pHiddenLayer->NeuronNum; j++)
		{
			SNeuron* pHiddenNeuron = pHiddenLayer->NeuronList.Data[j];

			// 计算误差值
			float err = 0.0f;
			SNeuronLayer* pNextLayer = m_layerList.Data[i + 1];
			for (int k = 0; k < pNextLayer->NeuronNum; k++)
			{
				SNeuron* pNextLayerNeuron = pNextLayer->NeuronList.Data[k];
				err += pNextLayerNeuron->WeightList.Data[j] * pNextLayerNeuron->Error;
			}
			err *= pHiddenNeuron->Output * (1 - pHiddenNeuron->Output);
			pHiddenNeuron->Error = err;

			for (int w = 0; w < pHiddenNeuron->InputNum; w++)
			{
				float weightUpdate = m_learnRate * err * pHiddenLayer->InputList.Data[w];
				// 把动量加入到权重更新中
				pHiddenNeuron->WeightList.Data[w] += weightUpdate + pHiddenNeuron->PrevUpdate.Data[w] * m_momentum;
				pHiddenNeuron->PrevUpdate.Data[w] = weightUpdate;
			}

		}

	}

	return errorSum;
}