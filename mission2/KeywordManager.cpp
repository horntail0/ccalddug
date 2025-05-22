
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "fix_keyword.h"
#include "KeywordManager.h"
#include "IBaseAlgorithm.h"
#include "Levenshtein.h"
using namespace std;

// Public
void KeywordManager::SetAlgorithm(IBaseAlgorithm* pAlgorithm)
{
	pBaseAlgorithm = pAlgorithm;
}

void KeywordManager::ManageUZ()
{
	UZ++;
	if (UZ >= MAX_SCORE)
	{
		RearrangeKeywords();
	}

}

string KeywordManager::Recommended_keyword(string input_keyword, string weekday)
{
	if (input_keyword == "")
	{
		return "Input Keyword!\n";
	}

	int weekday_index = GetWeekdayIndex(weekday);
	
	if (weekday_index >= 7 || weekday_index < 0) {
		return "Invalid weekday input!\n";
	}


	bool IsCorrectKeyword = false;
	ManageUZ();


	//평일 / 주말
	int daytype_index = 0;
	if (weekday_index >= 0 && weekday_index <= 4) daytype_index = 0;
	else daytype_index = 1;

	int point = UZ;

	//관리 목록에 존재하는지 확인
	//관리되는 키워드이면 점수가 증가


	//완벽 HIT
	if (CheckCorrectKeyword(weekday_index, daytype_index, input_keyword) == true)
	{
		return input_keyword;
	}

	//찰떡 HIT
	string SimilarKeyword = CheckSimilarKeyword(weekday_index, daytype_index, input_keyword);
	if (SimilarKeyword != "")
	{
		return SimilarKeyword;
	}

	//완벽 HIT / 찰떡 HIT 둘다 아닌경우
	CheckNewKeyword(weekday_index, daytype_index, input_keyword, point);



	return input_keyword;
}






// Private
int KeywordManager::GetWeekdayIndex(string weekday)
{
	if (weekday == "monday") return 0;
	else if (weekday == "tuesday") return 1;
	else if (weekday == "wednesday") return 2;
	else if (weekday == "thursday") return 3;
	else if (weekday == "friday") return 4;
	else if (weekday == "saturday") return 5;
	else if (weekday == "sunday") return 6;
	return 0xFFFF;
}

void KeywordManager::RearrangeKeywords()
{
	UZ = 10;
	for (int i = 0; i < MAX_DAYS; i++) {
		int num = 1;
		for (KeywordNode& node : DailyBest[i]) {
			node.point = num;
			num++;
		}
	}
	for (int i = 0; i < MAX_DAYTYPES; i++) {
		int num = 1;
		for (KeywordNode& node : DaytypeBest[i]) {
			node.point = num;
			num++;
		}
	}
}

bool KeywordManager::IsSimilar(const std::string& a, const std::string& b)
{
	if (a.empty() && b.empty()) return true;
	if (a.empty() || b.empty()) return false;
	int dist = 0;
	if (pBaseAlgorithm != nullptr)
	{
		dist = pBaseAlgorithm->Calculate_Similarity(a, b);
	}
	else
	{
		return false;
	}

	int max_len = std::max(a.length(), b.length());
	// 유사도 비율 (1.0: 완전히 같음, 0.0: 전혀 다름)
	double similarity = 1.0 - (double)dist / max_len;

	int score = 1 + static_cast<int>(similarity * 99);

	if (score >= 80) return true;
	return false;
}

bool KeywordManager::CheckCorrectKeyword(int weekday_index, int daytype_index, string input_keyword)
{
	long long int DailyMaxpoint = 0;
	long long int DaytypeMaxpoint = 0;

	bool matched = false;
	for (KeywordNode& node : DailyBest[weekday_index]) {
		if (node.name == input_keyword) {
			if (node.point < 10)
			{
				DailyMaxpoint = UZ;
			}
			else
			{
				DailyMaxpoint = node.point + (node.point * 0.1);
			}
			node.point = DailyMaxpoint;
			std::sort(DailyBest[weekday_index].begin(), DailyBest[weekday_index].end());
			matched = true;
			break;
		}
	}

	for (KeywordNode& node : DaytypeBest[daytype_index]) {
		if (node.name == input_keyword) {
			if (node.point < 10)
			{
				DaytypeMaxpoint = UZ;
			}
			else
			{
				DaytypeMaxpoint = node.point + (node.point * 0.1);
			}
			node.point = DaytypeMaxpoint;
			std::sort(DaytypeBest[daytype_index].begin(), DaytypeBest[daytype_index].end());
			matched = true;
			break;
		}
	}

	//재정렬 작업
	if (DailyMaxpoint >= MAX_SCORE || DaytypeMaxpoint >= MAX_SCORE) {
		RearrangeKeywords();
	}

	return matched;
}

string KeywordManager::CheckSimilarKeyword(int weekday_index, int daytype_index, string input_keyword)
{
	//찰떡 HIT
	for (KeywordNode& node : DailyBest[weekday_index]) {
		if (IsSimilar(node.name, input_keyword)) {
			return node.name;
		}
	}

	for (KeywordNode& node : DaytypeBest[daytype_index]) {
		if (IsSimilar(node.name, input_keyword)) {
			return node.name;
		}
	}
	return "";
}

#include <algorithm> // Ensure this is included for std::sort

void KeywordManager::CheckNewKeyword(int weekday_index, int daytype_index, string input_keyword, int point)
{
    if (DailyBest[weekday_index].size() < 10) {
        DailyBest[weekday_index].push_back({ input_keyword, point });
        std::sort(DailyBest[weekday_index].begin(), DailyBest[weekday_index].end());
    }
    else if (DailyBest[weekday_index].size() == 10) {
        if (DailyBest[weekday_index].front().point < point) {
            DailyBest[weekday_index].erase(DailyBest[weekday_index].begin()); // Replace pop_front with erase
            DailyBest[weekday_index].push_back({ input_keyword, point });
            std::sort(DailyBest[weekday_index].begin(), DailyBest[weekday_index].end());
        }
    }

    if (DaytypeBest[daytype_index].size() < 10) {
        DaytypeBest[daytype_index].push_back({ input_keyword, point });
        std::sort(DaytypeBest[daytype_index].begin(), DaytypeBest[daytype_index].end());
    }
    else if (DaytypeBest[daytype_index].size() == 10) {
        if (DaytypeBest[daytype_index].front().point < point) {
            DaytypeBest[daytype_index].erase(DaytypeBest[daytype_index].begin()); // Replace pop_front with erase
            DaytypeBest[daytype_index].push_back({ input_keyword, point });
            std::sort(DaytypeBest[daytype_index].begin(), DaytypeBest[daytype_index].end());
        }
    }
}
#ifdef _DEBUG
void KeywordManager::SetUZ(int score)
{
	UZ = score;
}
int KeywordManager::GetUZ()
{
	return UZ;
}
int KeywordManager::GetBestDailyWord(int weekday_index, string* str)
{
	if (DailyBest[weekday_index].empty())
	{
		return 0;
	}
	*str = DailyBest[weekday_index].back().name;
	return DailyBest[weekday_index].back().point;
}

int KeywordManager::GetWorstDailyWord(int weekday_index, string* str)
{
	if (DailyBest[weekday_index].empty())
	{
		return 0;
	}
	*str = DailyBest[weekday_index].front().name;
	return DailyBest[weekday_index].front().point;
}

int KeywordManager::GetBestDaytypeWord(int daytype_index, string* str)
{
	*str = DaytypeBest[daytype_index].back().name;
	return DaytypeBest[daytype_index].back().point;
}


vector<KeywordNode>* KeywordManager::GetDailyBest()
{
	return DailyBest;
}

#endif