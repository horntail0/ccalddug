#pragma once
#include <string>
using namespace std;

class IBaseAlgorithm
{

public:
	virtual int Calculate_Similarity(const std::string& a, const std::string& b) = 0;
};