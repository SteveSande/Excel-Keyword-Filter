// FILE				: Keyword Filter Functions.cpp
// PROJECT			: Excel File Keyword Filter
// PROGRAMMER		: Steve Sande
// FIRST VERSION	: July 2, 2022
// DESCRIPTION		:
//	Refer to Keyword Filter Main.cpp


#include "Keyword Filter Header.h"


// FUNCTION    : displayProgramInformation()
// DESCRIPTION : Tells the user what the program can do.
// PARAMETERS  : void
// RETURNS     : void

void displayProgramInformation(void)
{
	printf("This program searches an CSV file for rows containing custom keyword(s).\n");
	printf("New CSV files will be created that are named after the keyword(s).\n");
	printf("Rows containing an exact case-sensitive keyword match will be copied into their respective file.\n");
	printf("Rows containing no keyword matches will be copied into a special file.\n");
	printf("To account for case sensitivity include multiple keywords with the expected case variations.\n");
	printf("Your inputs are limited to 100 characters per input.\n");
	printf("Keyword(s) can include spaces, but not commas. You can include up to 10 keywords.\n\n");
}



// FUNCTION    : collectAndProcessKeywords()
// DESCRIPTION : 
//   Collects custom keywords from the user and modifies them for effective use in searchFile().
// PARAMETERS  :
//   struct keywordTracking* keywords : the pointer to the keyword storage array
//   int* numberOfKeywords : the pointer to where to store the number of keywords the user wants
// RETURNS     : void

void collectAndProcessKeywords(struct keywordTracking* keywords, int* numberOfKeywords)
{
	bool validInputFlag = false;
	std::string numberOfKeywordsS = "";

	// keyword collection and processing
	// if the number of keywords is invalid then the process restarts
	while (false == validInputFlag)
	{
		printf("\nEnter the number of keywords: ");
		getline(std::cin, numberOfKeywordsS);
		*numberOfKeywords = std::stoi(numberOfKeywordsS);

		// check that number of keywords inputted is valid
		if (*numberOfKeywords >= 1 && *numberOfKeywords <= kMaxKeywords)
		{
			validInputFlag = true;
			for (int currentKeyword = 0; currentKeyword < *numberOfKeywords; ++currentKeyword)
			{
				printf("Enter a keyword: ");
				getline(std::cin, (keywords + currentKeyword)->keyword);
				(keywords + currentKeyword)->searchableKeyword = (keywords + currentKeyword)->keyword;
				
				// modify the keyword so it has spaces at the beginning and end
				(keywords + currentKeyword)->searchableKeyword.insert(0, " ");
				(keywords + currentKeyword)->searchableKeyword.push_back(' ');
			}
		}
		else
		{
			printf("The number of keywords must be between 1 and 10.\n");
		}
	}
}



// FUNCTION    : searchFile()
// DESCRIPTION : 
//   Scans the file one line at a time and checks each line for all keywords before moving on to the next line.
//   The line string is modified to be space delimited instead of comma delimited for effective searching.
// PARAMETERS  :
//   struct keywordTracking* keywords : the pointer to the keyword storage array
//   int numberOfKeywords : the number of keywords the user designated
//   std::string& fileToSearch : the file to search
//   std::string& whereToSave : where to save created files
// RETURNS     : void

void searchFile(struct keywordTracking* keywords, int numberOfKeywords, std::string& fileToSearch, std::string& whereToSave, int* rowsAnalyzed)
{
	FILE* pFileToSearch = NULL;

	fopen_s(&pFileToSearch, fileToSearch.c_str(), "r");
	
	if (pFileToSearch == NULL)
	{
		printf("Couldn't open file to search.\n");
	}
	else
	{
		// the file to search was created or opened successfully
		// search one line at a time for each keyword
		// all keyword searches happen before moving on to next line
		while (!feof(pFileToSearch))
		{
			int keywordFoundFlag = false;
			char currentLineC[kMaxStringLength] = "";
			
			// collect and store a line in a C string and in a string object
			fgets(currentLineC, kMaxStringLength, pFileToSearch);
			std::string currentLine(currentLineC);

			// remove newline character from the line string object
			if (currentLine.find('\n') != std::string::npos)
			{
				currentLine.erase(currentLine.find('\n'));
			}

			// insert spaces at the start and end of the line string object
			currentLine.insert(0, " ");
			currentLine.push_back(' ');
			size_t commaPlaceholder = currentLine.find_first_of(',');

			// replace all commas with spaces for the line string object
			if (commaPlaceholder != std::string::npos)
			{
				while (commaPlaceholder != std::string::npos)
				{
					currentLine.replace(commaPlaceholder, 1, 1, ' ');
					commaPlaceholder = currentLine.find_first_of(',');
				}
			}

			// search the modified line string object for each keyword
			for (int currentKeyword = 0; currentKeyword < numberOfKeywords; ++currentKeyword)
			{
				// if a keyword is found then update tracking and send to be appended
				if (currentLine.find((keywords + currentKeyword)->searchableKeyword) != std::string::npos)
				{
					keywordFoundFlag = true;
					(keywords + currentKeyword)->linesContainingKeyword++;

					appendKeywordFile(whereToSave, currentLineC, keywords, currentKeyword);
				}
			}

			// if no keywords were found then send to be appended
			if (keywordFoundFlag == false)
			{
				appendKeywordFile(whereToSave, currentLineC, keywords);
			}

			++*rowsAnalyzed;
		}
		
		if (fclose(pFileToSearch) != 0)
		{
			printf("Keyword file failed to close.");
		}
	}
}



// FUNCTION    : appendKeywordFile()
// DESCRIPTION : 
//   No keywords were found in a line. The file associated with no matches is opened or created.
//   The file is appended with the comma delimited line string.
// PARAMETERS  :
//   std::string& whereToSave : where to save created files
//   char* whatToAppend : the pointer to the comma delimited line string with the keyword match
//	 struct keywordTracking* keywords : the pointer to the keyword storage array
// RETURNS     : void

void appendKeywordFile(std::string& whereToSave, char* whatToAppend, struct keywordTracking* keywords)
{
	FILE* pNoMatchesFile = NULL;
	
	std::string appendedWhereToSave = whereToSave + "\\No Keywords.csv";
	fopen_s(&pNoMatchesFile, appendedWhereToSave.c_str(), "a");

	if (NULL == pNoMatchesFile)
	{
		printf("Couldn't open file to search.\n");
	}
	else
	{
		// the no matches file was created or opened successfully
		// append the unmodified C string to the file
		fwrite(whatToAppend, sizeof(char), strlen(whatToAppend), pNoMatchesFile);

		if (fclose(pNoMatchesFile) != 0)
		{
			printf("Keyword file failed to close.");
		}
	}
}



// FUNCTION    : appendKeywordFile()
// DESCRIPTION : 
//   A keyword was found in a line. The file associated with that keyword is opened or created.
//   The file is appended with the comma delimited line string.
// PARAMETERS  :
//   std::string& whereToSave : where to save created files
//   char* whatToAppend : the pointer to the comma delimited line string with the keyword match
//	 struct keywordTracking* keywords : the pointer to the keyword storage array
//   int currentKeyword : the index of the keyword array containing the keyword that matched
// RETURNS     : void

void appendKeywordFile(std::string& whereToSave, char* whatToAppend, struct keywordTracking* keywords, int currentKeyword)
{
	FILE* pKeywordFile = NULL;
	std::string appendedWhereToSave = whereToSave + "\\" + (keywords + currentKeyword)->keyword + ".csv";

	fopen_s(&pKeywordFile, appendedWhereToSave.c_str(), "a");

	if (NULL == pKeywordFile)
	{
		printf("Couldn't open file to search.\n");
	}
	else
	{
		// the keyword file was created or opened successfully
		// append the unmodified C string to the file
		fwrite(whatToAppend, sizeof(char), strlen(whatToAppend), pKeywordFile);

		if (fclose(pKeywordFile) != 0)
		{
			printf("Keyword file failed to close.");
		}
	}
}



// FUNCTION    : displaySearchStatistics()
// DESCRIPTION : 
//   Informs the user how many lines had matches for each of their keywords and how many lines didn't have any matches.
// PARAMETERS  :
//	 struct keywordTracking* keywords : the pointer to the keyword storage array
//   int numberOfKeywords : the number of keywords the user designated
// RETURNS     : void

void displaySearchStatistics(struct keywordTracking* keywords, int numberOfKeywords, int* rowsAnalyzed)
{
	printf("\n");
	for (int currentKeyword = 0; currentKeyword < numberOfKeywords; ++currentKeyword)
	{
		if (1 == (keywords + currentKeyword)->linesContainingKeyword)
		{
			printf("The '%s' keyword was found in %d row of the file searched.\n", (keywords + currentKeyword)->keyword.c_str(), (keywords + currentKeyword)->linesContainingKeyword);
		}
		else if (0 == (keywords + currentKeyword)->linesContainingKeyword)
		{
			printf("The '%s' keyword was not found in the file searched.\n", (keywords + currentKeyword)->keyword.c_str());
		}
		else
		{
			printf("The '%s' keyword was found in %d rows of the file searched.\n", (keywords + currentKeyword)->keyword.c_str(), (keywords + currentKeyword)->linesContainingKeyword);
		}
	}
	
	printf("%d rows were analyzed.\n", *rowsAnalyzed);
}