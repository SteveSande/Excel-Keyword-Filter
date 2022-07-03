// FILE				: Keyword Filter Main.cpp
// PROJECT			: Excel File Keyword Filter
// PROGRAMMER		: Steve Sande
// FIRST VERSION	: July 2, 2022
// DESCRIPTION		:
//	This program searches a CSV file for rows containing specific keywords.
//  Rows with keyword matches will be copied into a file associated witht the keyword that matched.
//  Rows without keyword matches will also be copied into a distinct file.
//  The user specifies the file to search, where to save created files, and up to 10 keywords.


#include "Keyword Filter Header.h"


int main(void)
{
	displayProgramInformation();

	std::string fileToSearch = "";
	printf("Enter the filepath for the file to be searched:\n");
	getline(std::cin, fileToSearch);

	std::string whereToSave = "";
	printf("\nEnter the filepath for where created files are to be saved:\n");
	getline(std::cin, whereToSave);

	// variables for keyword tracking
	struct keywordTracking keywords[kMaxKeywords - 1] = {"", "", 0};
	int numberOfKeywords = 0;
	int rowsAnalyzed = -1;

	collectAndProcessKeywords(keywords, &numberOfKeywords);

	searchFile(keywords, numberOfKeywords, fileToSearch, whereToSave, &rowsAnalyzed);

	displaySearchStatistics(keywords, numberOfKeywords, &rowsAnalyzed);
}