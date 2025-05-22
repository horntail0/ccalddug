#pragma once
#include "IBaseAlgorithm.h"
#include <string>
class Levenshtein : public IBaseAlgorithm
{
	Levenshtein() {}
	~Levenshtein() {}
public:
	static Levenshtein* GetInstance()
	{
		static Levenshtein Lev;
		return &Lev;
	}
	int Calculate_Similarity(const std::string& a, const std::string& b);
};