/*==============================================================================

    Random.h - 乱数生成
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/21
==============================================================================*/
#ifndef _RANDOM_H_
#define _RANDOM_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include <random>

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/

// デバイス
class RandomDevice
{
public:
	virtual ~RandomDevice(){}
	virtual int GetInt( void) = 0;
	virtual float GetFloat( void) = 0;
};

//擬似乱数
class RandomMt19937 : public RandomDevice
{
public:
	RandomMt19937( unsigned int seed);
	~RandomMt19937();
	int GetInt( void) override;
	float GetFloat( void) override;
private:
	std::mt19937* m_Mt19937;
};

//一様分布（int）
class RandomUniformIntDistribution : public RandomDevice
{
public:
	RandomUniformIntDistribution( unsigned int seed, int min, int max);
	~RandomUniformIntDistribution();
	int GetInt( void) override;
	float GetFloat( void) override;
private:
	std::mt19937* m_Mt19937;
	std::uniform_int_distribution<int>* m_Uniform;
};

//一様分布（float）
class RandomUniformFloatDistribution : public RandomDevice
{
public:
	RandomUniformFloatDistribution( unsigned int seed, float min, float max);
	~RandomUniformFloatDistribution();
	int GetInt( void) override;
	float GetFloat( void) override;
private:
	std::mt19937* m_Mt19937;
	std::uniform_real_distribution<float>* m_Uniform;
};

//使用クラス
class Random
{
public:
	Random();
	~Random();

	void UninitDevice();

	int GetSeed( void){ return m_Seed;}
	void SetSeed( int seed);
	void ResetSeed( void);
	void SetDefault( void);
	void SetRangeInt( int min, int max);
	void SetRangeFloat( float min, float max);

	int GetInt( void);
	float GetFloat( void);
	
private:
	int m_Seed;
	RandomDevice* m_RandomDevice;
};


#endif