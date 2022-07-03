// FILE				: Keyword Filter Header.h
// PROJECT			: Excel File Keyword Filter
// PROGRAMMER		: Steve Sande
// FIRST VERSION	: July 2, 2022
// DESCRIPTION		:
//	Refer to Keyword Filter Main.cpp

#pragma once
#include "stdio.h"
#include "string.h"
#include <iostream>
#include <string>
#include <vector>

void displayProgramInformation(void);
void collectAndProcessKeywords(std::vector<struct keywordTracking>& keywords, int* numberOfKeywords);
void searchFile(std::vector<struct keywordTracking>& keywords, int numberOfKeywords, std::string& fileToSearch, std::string& whereToSave, int* rowsAnalyzed);
void appendKeywordFile(std::string& whereToSave, char* whatToAppend, std::vector<struct keywordTracking>& keywords);
void appendKeywordFile(std::string& whereToSave, char* whatToAppend, std::vector<struct keywordTracking>& keywords, int currentKeyword);
void displaySearchStatistics(std::vector<struct keywordTracking>& keywords, int numberOfKeywords, int* rowsAnalyzed);

const int kMaxStringLength = 100;
const int kMaxKeywords = 10;

struct keywordTracking
{
	std::string keyword = "";
	std::string searchableKeyword = "";
	int linesContainingKeyword = 0;
};