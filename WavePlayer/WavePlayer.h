#pragma once

#include "resource.h"
#include <sstream>
#include <string>
#include <vector>

#include "LyricsHandler.h"
#include "FileHandler.h"
#include "Constants.h"

class MainAppWindow {
private:
    HWND mainWindow;

public:
   HWND searchBtn;
   HWND showLyricsBtn;
   HWND addLyricsBtn;
   HWND stopPlayingBtn;
   HWND playAgainBtn;

    void RegisterMainWindow(HWND hWnd);
    HWND CreatePushButton(LPCWSTR btnText, HMENU btnInstance, int left, int top, int width, int height);
    void UpdateDrawText(const std::wstring& text, int left, int top, int width, int height, HDC hdc);
    std::pair<int, int> GetClientAreaSize(HWND hWnd); // pair<width, height>
};

class AudioPlayer {
private:
    MainAppWindow mainAppWindow;
    LyricsDatabase lyrics;

    std::wstring currentSong; // only 1 song can play at the time
    PWSTR currentSongPath;
    std::vector<std::wstring> currentLyrics; // individual lines stored in vector
    size_t nextLyricsIndex;
    size_t maxLineLength;

    static std::wstring ExtractSongFilename(PWSTR songPath);
    static std::wstring GetSongFormat(PWSTR songPath);

    PWSTR GetCurrentSongPath();

    void PlaySong(PWSTR songPath);
    void PlayCurrentSong();

    size_t GetNextLyricsIndex();
    void SetCurrentLyrics(const std::vector<std::wstring>& lyrics, size_t lyricsCollectionSize);

public:
    AudioPlayer(MainAppWindow &mainWindow);

    PWSTR FindSong();

    std::wstring GetCurrentSong();
    std::vector<std::wstring> GetCurrentLyricsLines();

    void DisplayLyrics(const std::pair<int, int>& clientAreaSize, const std::vector<std::wstring>& lyricsLines, HDC hdc);


    void HandleSearchEvent(HWND hWnd, MainAppWindow& mainWindow);
    void HandleAddLyricsEvent();
    void HandleShowLyricsEvent(HWND hWnd);
    void HandlePlayAgainEvent();
    void HandleStopPlayingEvent();
};