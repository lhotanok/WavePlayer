#include "LyricsHandler.h"
#include "FileHandler.h"

using namespace std;

using map_t = unordered_map<wstring, vector<wstring>>; // key: songName, value:lyrics paths for the matching songName

LyricsDatabase::LyricsDatabase()
{
	databaseFilename = L"Lyrics.csv";
	CacheDatabaseFromFile();
}

void LyricsDatabase::SetLyricsDatabaseDestination(const wstring& destination)
{
	databaseFilename = destination;
}

bool LyricsDatabase::SongInDatabase(const wstring& songName)
{
	return lyrics.find(songName) != lyrics.end();
}

vector<wstring>& LyricsDatabase::GetLyricsCollection(const wstring& songName)
{
	return lyrics.at(songName);
}

size_t LyricsDatabase::GetMaxLyricsLineLength(size_t startIndex, size_t linesToCount, const std::vector<std::wstring>& lyricsLines)
{
	size_t maxLength = 0;
	for (size_t i = startIndex; i < linesToCount; i++)
	{
		if (lyricsLines[i].size() > maxLength)
		{
			maxLength = lyricsLines[i].size();
		}
	}
	return maxLength;
}

vector<wstring> LyricsDatabase::SplitIntoTokens(const wstring &line, const char delimiter)
{
	vector<wstring> tokens;
	size_t startIndex;
	size_t endIndex = 0;

	while ((startIndex = line.find_first_not_of(delimiter, endIndex)) != string::npos)
	{
		endIndex = line.find(delimiter, startIndex);
		tokens.push_back(line.substr(startIndex, endIndex - startIndex));
	}
	return tokens;
}

void LyricsDatabase::CacheDatabaseFromFile()
{
	wifstream wistrm(databaseFilename);

	if (wistrm)
	{
		wstring line;
		while (getline(wistrm, line))
		{
			vector<wstring> lineParts = SplitIntoTokens(line, ';');
			if (lineParts.size() > 0)
			{
				auto songName = lineParts[0];
				vector<wstring> lyricsPaths;

				for (size_t i = 1; i < lineParts.size(); i++)
				{
					lyricsPaths.push_back(lineParts[i]);
				}
				lyrics.insert(pair<wstring, vector<wstring>>(songName, lyricsPaths));
			}
		}
	}
}

void LyricsDatabase::UpdateDatabaseFile()
{
	wofstream wostrm(databaseFilename);
	for (auto&& songWithLyrics : lyrics)
	{
		auto songName = songWithLyrics.first;
		auto lyricsCollection = songWithLyrics.second;
		wostrm << songName << ";";
		for (auto&& lyricsPaths : lyricsCollection)
		{
			wostrm << lyricsPaths << ";";
		}
		wostrm << endl;
	}
	wostrm.flush();
	wostrm.close();
}

void LyricsDatabase::AddLyricsToDatabase(const wstring& songName, const wstring& filePath)
{
	if (SongInDatabase(songName))
	{
		vector<wstring>& songLyricsCollection = GetLyricsCollection(songName);
		bool fileInDatabase = false;
		for (auto&& lyrics : songLyricsCollection)
		{
			if (lyrics == filePath)
			{
				fileInDatabase = true;
				break;
			}
		}
		if (!fileInDatabase)
		{
			songLyricsCollection.push_back(filePath);
			UpdateDatabaseFile();
		}
	}
	else
	{
		vector<wstring> lyricsPaths{ filePath };
		lyrics.insert(pair<wstring, vector<wstring>>(songName, lyricsPaths));
		UpdateDatabaseFile();
	}
}

vector<wstring> LyricsDatabase::GetLyricsFromFile(const wstring& filePath, const wstring& songName)
{
	wifstream lyricsFile(filePath);
	vector<wstring> textLines;

	if (lyricsFile) // file exists
	{
		wstring line;
		while (getline(lyricsFile, line))
		{
			wstringstream ws;
			ws << line << endl;
			textLines.push_back(ws.str());
		}
	}
	else
	{
		// remove lyrics path from known file paths
		auto& songLyrics = GetLyricsCollection(songName);
		if (songLyrics.size() == 1)
		{
			// remove whole song record from known songs with lyrics
			lyrics.erase(songName);
		}
		else
		{
			// remove 1 lyrics file from collection
			for (size_t i = 0; i < songLyrics.size(); i++)
			{
				if (songLyrics[i] == filePath)
				{
					songLyrics.erase(songLyrics.begin() + i);
				}
			}
		}
		UpdateDatabaseFile();
	}
	return textLines;
}

std::wstring LyricsDatabase::JoinLyricsLines(size_t startIndex, size_t linesToJoin, const std::vector<std::wstring>& lines)
{
	wstringstream ws;
	for (size_t i = startIndex; i < startIndex + linesToJoin; i++)
	{
		ws << lines[i];
	}
	return ws.str();
}
