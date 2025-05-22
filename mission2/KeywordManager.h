#pragma once
#include "IBaseAlgorithm.h"

using namespace std;

struct Node {
	string w;
	string wk;
};

struct KeywordNode {
	string name;
	int point;

	bool operator<(const KeywordNode& other) const {
		return point < other.point;
	}
};


class KeywordManager
{



public:
	KeywordManager()
	{
	}
	~KeywordManager() {}

	static KeywordManager* GetInstance(void)
	{
		static KeywordManager s_KeywordManager;
		return &s_KeywordManager;
	}
	void SetAlgorithm(IBaseAlgorithm* pAlgorithm);
	string Recommended_keyword(string input_keyword, string weekday);
	void ManageUZ();

	int GetWeekdayIndex(string weekday);
	void RearrangeKeywords();
	bool IsSimilar(const std::string& a, const std::string& b);
	bool CheckCorrectKeyword(int weekday_index, int daytype_index, string input_keyword);

	string CheckSimilarKeyword(int weekday_index, int daytype_index, string input_keyword);

	void CheckNewKeyword(int weekday_index, int daytype_index, string input_keyword, int point);
#ifdef _DEBUG
	void SetUZ(int score);
	int GetUZ();
	int GetBestDailyWord(int weekday_index, string* str);
	int GetWorstDailyWord(int weekday_index, string* str);
	int GetBestDaytypeWord(int daytype_index, string* str);

	vector<KeywordNode>* GetDailyBest();

#endif
	static const int MAX_KEYWORD = 10;
	static const int MAX_DAYS = 7;
	static const int MAX_DAYTYPES = 2;
	static const int MAX_SCORE = 2100000000;
private:
	vector<KeywordNode> DailyBest[7]; //월 ~ 금
	vector<KeywordNode> DaytypeBest[2]; //평일, 주말
	int UZ = 9;
	IBaseAlgorithm* pBaseAlgorithm;





};

