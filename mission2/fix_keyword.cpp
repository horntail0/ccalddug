#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "fix_keyword.h"
#include "KeywordManager.h"
#include "Levenshtein.h"
using namespace std;




#ifndef _DEBUG

int main() {
	KeywordManager* KM = KeywordManager::GetInstance();
	KM->SetAlgorithm(Levenshtein::GetInstance());
	ifstream fin{ "keyword_weekday_500.txt" }; //500개 데이터 입력
	for (int i = 0; i < 500; i++) {
		string keyword, day;
		fin >> keyword >> day;


		string ret = KM->Recommended_keyword(keyword, day);
		std::cout << ret << "\n";
	}

}
#endif