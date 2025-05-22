#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "fix_keyword.h"

using namespace std;


vector<KeywordNode> DailyBest[7]; //월 ~ 금
vector<KeywordNode> DaytypeBest[2]; //평일, 주말
const int MAX_KEYWORD = 10;
const int MAX_DAYS = 7;
const int MAX_DAYTYPES = 2;
const int MAX_SCORE = 2100000000;
int UZ = 9;

// 레벤슈타인 거리 계산 알고리즘 (문자열 유사도 검사)
int levenshtein(const std::string& a, const std::string& b) {
	const size_t len_a = a.size();
	const size_t len_b = b.size();

	std::vector<std::vector<int>> d(len_a + 1, std::vector<int>(len_b + 1));

	for (size_t i = 0; i <= len_a; ++i) d[i][0] = i;
	for (size_t j = 0; j <= len_b; ++j) d[0][j] = j;

	for (size_t i = 1; i <= len_a; ++i) {
		for (size_t j = 1; j <= len_b; ++j) {
			if (a[i - 1] == b[j - 1])
				d[i][j] = d[i - 1][j - 1];
			else
				d[i][j] = 1 + std::min({ d[i - 1][j], d[i][j - 1], d[i - 1][j - 1] });
		}
	}
	return d[len_a][len_b];
}

// 점수 환산
bool IsSimilar(const std::string& a, const std::string& b) {
	if (a.empty() && b.empty()) return true;
	if (a.empty() || b.empty()) return false;

	int dist = levenshtein(a, b);
	int max_len = std::max(a.length(), b.length());
	// 유사도 비율 (1.0: 완전히 같음, 0.0: 전혀 다름)
	double similarity = 1.0 - (double)dist / max_len;

	int score = 1 + static_cast<int>(similarity * 99);

	if (score >= 80) return true;
	return false;
}


int GetWeekdayIndex(string weekday)
{
	if (weekday == "monday") return 0;
	else if (weekday == "tuesday") return 1;
	else if (weekday == "wednesday") return 2;
	else if (weekday == "thursday") return 3;
	else if (weekday == "friday") return 4;
	else if (weekday == "saturday") return 5;
	else if (weekday == "sunday") return 6;
	else {
		return 0xFFFF;
	}
}
void RearrangeKeywords()
{
	UZ = 9;
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
bool CheckCorrectKeyword(int weekday_index, int daytype_index, string input_keyword)
{
	long long int DailyMaxpoint = 0;
	long long int DaytypeMaxpoint = 0;

	int flag = 0;
	for (KeywordNode& node : DailyBest[weekday_index]) {
		if (node.name == input_keyword) {
			if (node.point < 10)
			{
				DailyMaxpoint = node.point + 1;
			}
			else
			{
				DailyMaxpoint = node.point + (node.point * 0.1);
			}
			node.point = DailyMaxpoint;
			flag = 1;
			break;
		}
	}

	for (KeywordNode& node : DaytypeBest[daytype_index]) {
		if (node.name == input_keyword) {
			if (node.point < 10)
			{
				DaytypeMaxpoint = node.point + 1;
			}
			else
			{
				DaytypeMaxpoint = node.point + (node.point * 0.1);
			}
			node.point = DaytypeMaxpoint;
			flag = 1;
			break;
		}
	}

	//재정렬 작업
	if (DailyMaxpoint >= MAX_SCORE || DaytypeMaxpoint >= MAX_SCORE) {
		RearrangeKeywords();
	}

	if (flag == 1) {
		return true;
	}
	return false;

}

string CheckSimilarKeyword(int weekday_index, int daytype_index, string input_keyword)
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

void CheckNewKeyword(int weekday_index, int daytype_index, string input_keyword, int point)
{
	if (DailyBest[weekday_index].size() < 10) {
		DailyBest[weekday_index].push_back({ input_keyword, point });
		std::sort(DailyBest[weekday_index].begin(), DailyBest[weekday_index].end());
	}
	else if (DailyBest[weekday_index].size() == 10) {
		if (DailyBest[weekday_index].front().point < point) {
			DailyBest[weekday_index].erase(DailyBest[weekday_index].begin());
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
			DaytypeBest[daytype_index].erase(DaytypeBest[weekday_index].begin());
			DaytypeBest[daytype_index].push_back({ input_keyword, point });
			std::sort(DaytypeBest[daytype_index].begin(), DaytypeBest[daytype_index].end());
		}
	}

}


string Recommended_keyword(string input_keyword, string weekday) {
	UZ++;
	if (UZ >= MAX_SCORE)
	{
		RearrangeKeywords();
	}
	int weekday_index = GetWeekdayIndex(weekday);
	bool IsCorrectKeyword = false;
	if (weekday_index >= 7 || weekday_index < 0) {
		std::cout << "Invalid weekday input!\n";
		return "";
	}

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

void input() {
	ifstream fin{ "keyword_weekday_500.txt" }; //500개 데이터 입력
	for (int i = 0; i < 500; i++) {
		string keyword, day;
		fin >> keyword >> day;
		if (keyword == "")
		{
			std::cout << "Input Keyword!\n";
			continue;
		}
		
		string ret = Recommended_keyword(keyword, day);
		std::cout << ret << "\n";
	}
}
#ifndef _DEBUG

int main() {
	input();

}
#endif