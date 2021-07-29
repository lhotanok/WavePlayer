#pragma once

#include <windows.h>
#include <algorithm>
#include <fstream>

#include <string>
#include <vector>
#include <unordered_map>

class LyricsDatabase {
private:
	std::unordered_map<std::wstring, std::vector<std::wstring>> lyrics;
	std::wstring databaseFilename;

	void CacheDatabaseFromFile();
	void UpdateDatabaseFile();
	std::vector<std::wstring> SplitIntoTokens(const std::wstring &line, const char delimiter);
public:
	LyricsDatabase();
	void SetLyricsDatabaseDestination(const std::wstring& destination);
	bool SongInDatabase(const std::wstring& songName);
	std::vector<std::wstring>& GetLyricsCollection(const std::wstring& songName);
	size_t GetMaxLyricsLineLength(size_t startIndex, size_t linesToCount, const std::vector<std::wstring>& lyricsLines);
	void AddLyricsToDatabase(const std::wstring& songName, const std::wstring& filePath);
	std::vector<std::wstring> GetLyricsFromFile(const std::wstring& filePath, const std::wstring& songName);
	std::wstring JoinLyricsLines(size_t startIndex, size_t linesToJoin, const std::vector<std::wstring> &lines);
};
