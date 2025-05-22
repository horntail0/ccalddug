#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "gmock/gmock.h"
#include "fix_keyword.h"
#include "Levenshtein.h"
#ifdef _DEBUG
using namespace testing;
class MockAlgorithm : public IBaseAlgorithm
{
public:
	MOCK_METHOD(int, Calculate_Similarity, (const std::string& a, const std::string& b));
};
class MockKeywordManager : public KeywordManager
{
public:
	MOCK_METHOD(int, GetWeekdayIndex, (string weekday));
};
TEST(TestSuite1, TestCase1)
{
	KeywordManager* KM = KeywordManager::GetInstance();
	string ret = KM->Recommended_keyword("", "monday");
	EXPECT_EQ("Input Keyword!\n", ret);
}
TEST(TestSuite1, TestCase2)
{
	KeywordManager* KM = KeywordManager::GetInstance();
	string ret = KM->Recommended_keyword("Apple", "munday");
	EXPECT_EQ("Invalid weekday input!\n", ret);
}
TEST(TestSuit2, TC1)
{
	MockKeywordManager mockKM;
	EXPECT_CALL(mockKM, GetWeekdayIndex(_)).WillRepeatedly(Return(0));
	ifstream fin{ "keyword_weekday_500.txt" }; //500개 데이터 입력
	int tempUZ;
	for (int i = 0; i < 10; i++) {
		string keyword, day;
		fin >> keyword >> day;
		mockKM.ManageUZ();
		tempUZ = mockKM.GetUZ();
		mockKM.CheckNewKeyword(0, 0, keyword, tempUZ);
		
	}
	vector<KeywordNode>* dailybest = mockKM.GetDailyBest();
	int tempindex = 0;
	for (KeywordNode& node : dailybest[0])
	{

		EXPECT_EQ(node.point, tempindex + 10);
		tempindex++;
	}
	//mockKM.SetUZ(KeywordManager::MAX_SCORE - 1);
}

TEST(TestSuit2, TC2)
{
	MockKeywordManager mockKM;
	EXPECT_CALL(mockKM, GetWeekdayIndex(_)).WillRepeatedly(Return(0));
	ifstream fin{ "keyword_weekday_500.txt" }; //500개 데이터 입력
	int tempUZ;
	string keyword, day;
	string worststring;
	int worstscore;
	for (int i = 0; i < 11; i++) {
		
		fin >> keyword >> day;

		if (i == 10)
		{
			worstscore = mockKM.GetWorstDailyWord(0, &worststring);
			break;
		}
		mockKM.ManageUZ();
		tempUZ = mockKM.GetUZ();
		mockKM.CheckNewKeyword(0, 0, keyword, tempUZ);

	}


	mockKM.ManageUZ();
	tempUZ = mockKM.GetUZ();
	mockKM.CheckNewKeyword(0, 0, keyword, tempUZ);



	vector<KeywordNode>* dailybest = mockKM.GetDailyBest();
	int tempindex = 0;
	bool flag = false;
	for (KeywordNode& node : dailybest[0])
	{

		EXPECT_EQ(node.point, tempindex + 11);
		tempindex++;
		if (node.name == keyword)
		{
			flag = true;
		}
	}
	string secondworststring;
	int secondworstscore = mockKM.GetWorstDailyWord(0, &secondworststring);
	EXPECT_EQ(flag, true);
	EXPECT_NE(worstscore, secondworstscore);

}
TEST(TestSuit2, TC3_MAXUZ)
{
	MockKeywordManager mockKM;
	EXPECT_CALL(mockKM, GetWeekdayIndex(_)).WillRepeatedly(Return(0));
	ifstream fin{ "keyword_weekday_500.txt" }; //500개 데이터 입력
	int tempUZ;
	string keyword, day;
	string keywordstore[10];
	string worststring;

	mockKM.SetUZ(KeywordManager::MAX_SCORE-10);
	for (int i = 0; i < 10; i++) {

		fin >> keyword >> day;
		keywordstore[i] = keyword;
		mockKM.ManageUZ();
		tempUZ = mockKM.GetUZ();
		mockKM.CheckNewKeyword(0, 0, keyword, tempUZ);

	}
	vector<KeywordNode>* dailybest = mockKM.GetDailyBest();
	int tempindex = 0;
	bool flag = false;

	for (KeywordNode& node : dailybest[0])
	{

		EXPECT_EQ(node.point, tempindex + 1);
		tempindex++;
		if (node.name == keyword)
		{
			flag = true;
		}
	}
	string resultstring;
	int resultscore;
	mockKM.Recommended_keyword(keywordstore[8], "monday");
	mockKM.Recommended_keyword(keywordstore[8], "monday");
	mockKM.Recommended_keyword(keywordstore[0], "monday");
	resultscore = mockKM.GetBestDailyWord(0, &resultstring);
	
	EXPECT_EQ(resultstring, keywordstore[8]);


}
TEST(TestSuit3, TestCase1_100Words)
{
	KeywordManager KM;
	ifstream fin{ "keyword_weekday_500.txt" }; //500개 데이터 입력
	for (int i = 0; i < 100; i++) {
		string keyword, day;
		fin >> keyword >> day;
		KM.Recommended_keyword(keyword, day);
	}
	for (int i = 0; i < 7; i++)
	{
		int bestscore = 0;
		string bestkeyword;
		int worstscore = INT32_MAX;

		int score_besttry = 0;
		int score_worsttry = 0;
		string worstkeyword;
		string keyword_besttry;
		string keyword_worsttry;
		score_besttry = KM.GetBestDailyWord(i, &keyword_besttry);
		score_worsttry = KM.GetWorstDailyWord(i, &keyword_worsttry);
		EXPECT_TRUE(score_besttry >= score_worsttry);		
	}
}

TEST(Testsuite4, samecase1)
{
	KeywordManager KM;
	string keyword, day;
	string resultstring[2];
	string keyword_besttry;
	keyword = "apple";
	day = "wednesday";
	resultstring[0] = KM.Recommended_keyword(keyword, day);
	resultstring[1] = KM.Recommended_keyword(keyword, day);
	EXPECT_EQ(resultstring[0], resultstring[1]);
	int score_besttry = KM.GetBestDailyWord(2, &keyword_besttry);
	EXPECT_EQ(11, score_besttry);


}
TEST(Testsuite4, samecase2)
{
	KeywordManager KM;
	string keyword, day;
	string resultstring[2];
	string keyword_besttry;
	keyword = "apple";
	day = "sunday";
	resultstring[0] = KM.Recommended_keyword(keyword, day);
	resultstring[1] = KM.Recommended_keyword(keyword, day);
	EXPECT_EQ(resultstring[0], resultstring[1]);
	int score_besttry = KM.GetBestDaytypeWord(1, &keyword_besttry);
	EXPECT_EQ(11, score_besttry);
}
TEST(Testsuite4, samecase3)
{
	KeywordManager KM;
	string keyword, day1, day2;
	string resultstring[2];
	string keyword_besttry;
	keyword = "apple";
	KM.SetUZ(KeywordManager::MAX_SCORE - 3);
	day1 = "sunday";
	day2 = "saturday";
	resultstring[0] = KM.Recommended_keyword(keyword, day1);
	resultstring[1] = KM.Recommended_keyword(keyword, day2);
	EXPECT_EQ(resultstring[0], resultstring[1]);
	int score_besttry = KM.GetBestDaytypeWord(1, &keyword_besttry);
	EXPECT_EQ(1, score_besttry);
}
TEST(Testsuite4, samecase4)
{
	KeywordManager KM;
	KM.SetUZ(1);
	string keyword, day;
	keyword = "rabbit";
	day = "tuesday";
	KM.Recommended_keyword(keyword, day);
	KM.SetUZ(9);
	KM.Recommended_keyword(keyword, day);
	int result = KM.GetBestDailyWord(1, &keyword);
	EXPECT_EQ(result, 10);

}
TEST(Testsuite5, similarcase1)
{
	KeywordManager KM;
	MockAlgorithm MAL;
	KM.SetAlgorithm(&MAL);
	EXPECT_CALL(MAL, Calculate_Similarity(_, _)).WillOnce(Return(0));
	string keyword, day;
	string inputstring[2];
	inputstring[0] = "apple";
	inputstring[1] = "aplet";
	string resultstring[2];
	string keyword_besttry;

	day = "sunday";
	resultstring[0] = KM.Recommended_keyword(inputstring[0], day);
	resultstring[1] = KM.Recommended_keyword(inputstring[1], day);
	EXPECT_EQ(resultstring[0], resultstring[1]);
	int score_besttry = KM.GetBestDaytypeWord(1, &keyword_besttry);
	EXPECT_EQ(10, score_besttry);
}

TEST(Testsuite5, similarcase2)
{
	KeywordManager KM;
	MockAlgorithm MAL;
	KM.SetAlgorithm(&MAL);
	EXPECT_CALL(MAL, Calculate_Similarity(_, _)).WillRepeatedly(Return(5));
	string keyword, day;
	string inputstring[2];
	inputstring[0] = "eeeee";
	inputstring[1] = "aplet";
	string resultstring[2];
	string keyword_besttry;

	string day1 = "sunday";
	string day2 = "tuesday";
	resultstring[0] = KM.Recommended_keyword(inputstring[0], day1);
	resultstring[1] = KM.Recommended_keyword(inputstring[1], day2);
	EXPECT_NE(resultstring[0], resultstring[1]);
	int score_besttry = KM.GetBestDaytypeWord(1, &keyword_besttry);
	EXPECT_EQ(10, score_besttry);
}
TEST(Testsuite5, similarcase3)
{
	KeywordManager KM;
	KM.SetAlgorithm(nullptr);
	string keyword, day;
	string inputstring[2];
	inputstring[0] = "apple";
	inputstring[1] = "aplet";
	string resultstring[2];
	string keyword_besttry;

	string day1 = "sunday";
	string day2 = "sunday";
	resultstring[0] = KM.Recommended_keyword(inputstring[0], day1);
	resultstring[1] = KM.Recommended_keyword(inputstring[1], day2);
	EXPECT_NE(resultstring[0], resultstring[1]);
	int score_besttry = KM.GetBestDaytypeWord(1, &keyword_besttry);
	EXPECT_EQ(11, score_besttry);
}

TEST(Testsuite5, similarcase4)
{
	KeywordManager KM;
	KM.SetAlgorithm(Levenshtein::GetInstance());
	string keyword, day;
	string inputstring[2];
	inputstring[0] = "apple";
	inputstring[1] = "eeexx";
	string resultstring[2];
	string keyword_besttry;

	string day1 = "sunday";
	string day2 = "tuesday";
	KM.Recommended_keyword(inputstring[0], day2);
	resultstring[0] = KM.Recommended_keyword(inputstring[0], day1);
	resultstring[1] = KM.Recommended_keyword(inputstring[1], day2);
	EXPECT_NE(resultstring[0], resultstring[1]);
}
TEST(Testsuite5, similarcase5)
{
	KeywordManager KM;
	MockAlgorithm MAL;
	KM.SetAlgorithm(&MAL);
	EXPECT_CALL(MAL, Calculate_Similarity(_, _)).WillOnce(Return(0));
	string keyword, day1, day2;
	string inputstring[2];
	inputstring[0] = "apple";
	inputstring[1] = "aplet";
	string resultstring[2];
	string keyword_besttry;

	day1 = "sunday";
	day2 = "saturday";
	resultstring[0] = KM.Recommended_keyword(inputstring[0], day1);
	resultstring[1] = KM.Recommended_keyword(inputstring[1], day2);
	EXPECT_EQ(resultstring[0], resultstring[1]);
	int score_besttry = KM.GetBestDaytypeWord(1, &keyword_besttry);
	EXPECT_EQ(10, score_besttry);
}
int main()
{

	testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
#endif

