#include "WavePlayer.h"

// AudioPlayer class implementation
std::wstring AudioPlayer::ExtractSongFilename(PWSTR songPath)
{
    auto path = FileHandler::ConvertToWstring(songPath);
    auto format = AudioPlayer::GetSongFormat(songPath);
    if (format == L".wav")
    {
        const size_t wavFormatSuffixLength = 4;
        size_t i = path.size() - 1 - wavFormatSuffixLength;
        std::wstringstream reversedName;
 
        while (i != SIZE_MAX && path[i] != L'\\') // i-- used on size_t i = 0 causes i == SIZE_MAX
        {
            reversedName << path[i];
            i--;
        }

        auto filename = reversedName.str();
        std::reverse(filename.begin(), filename.end());
        return filename;
    }
    return L"";
}
PWSTR AudioPlayer::FindSong()
{
    return FileHandler::GetOpenedFilePath();
}
void AudioPlayer::PlaySong(PWSTR songPath)
{
    PlaySound(songPath, NULL, SND_ASYNC); // for WAV format only (uncompressed)

    if (currentSongPath != songPath)
    {
        currentSong = ExtractSongFilename(songPath);
        currentSongPath = songPath;
    }
}
void AudioPlayer::PlayCurrentSong()
{
    if (currentSongPath != NULL)
    {
        PlaySound(currentSongPath, NULL, SND_ASYNC); // for WAV format only (uncompressed)
    }
}
std::wstring AudioPlayer::GetCurrentSong()
{
    return currentSong;
}

PWSTR AudioPlayer::GetCurrentSongPath()
{
    return currentSongPath;
}

std::vector<std::wstring> AudioPlayer::GetCurrentLyricsLines()
{
    return currentLyrics;
}

size_t AudioPlayer::GetNextLyricsIndex()
{
    return nextLyricsIndex;
}

std::wstring AudioPlayer::GetSongFormat(PWSTR songPath)
{
    auto path = FileHandler::ConvertToWstring(songPath);
    std::wstringstream reversedFormat;
    size_t i = path.size() - 1;
    const size_t wavFormatSuffixLength = 4;

    while (i != path.size() - wavFormatSuffixLength - 1)
    {
        reversedFormat << path[i];
        i--;
    }
    auto format = reversedFormat.str();
    std::reverse(format.begin(), format.end());
    return format;
}

void AudioPlayer::SetCurrentLyrics(const std::vector<std::wstring>& currLyrics, size_t lyricsCollectionSize)
{
    currentLyrics = currLyrics;

    if (lyricsCollectionSize != 0)
    {
        nextLyricsIndex = (nextLyricsIndex + 1) % lyricsCollectionSize;
    }
}

AudioPlayer::AudioPlayer(MainAppWindow &mainWindow) : mainAppWindow(mainWindow)
{
    currentSong = L"";
    currentSongPath = NULL;
    currentLyrics = std::vector<std::wstring>();
    nextLyricsIndex = 0;
    maxLineLength = 0;
}

void AudioPlayer::DisplayLyrics(const std::pair<int, int>& clientAreaSize, const std::vector<std::wstring>& lyricsLines, HDC hdc)
{
    auto [areaWidth, areaHeight] = clientAreaSize;
    auto marginLeft = 170;
    auto marginTop = 60;
    auto frameHeight = areaHeight - marginTop - (areaHeight / 12);
    auto frameWidth = areaWidth - ((areaWidth - marginLeft) / 10) + 10;

    Rectangle(hdc,
        marginLeft - 10, marginTop - 10,
        static_cast<int>(frameWidth),
        static_cast<int>(frameHeight));

    const size_t lineHeight = 19;
    const size_t letterWidth = 15;

    size_t startIndex = 0;
    size_t remainingLines = lyricsLines.size();

    while (remainingLines != 0)
    {
        auto linesToDraw = min(remainingLines, frameHeight / lineHeight);
        auto linesJoined = lyrics.JoinLyricsLines(startIndex, linesToDraw, lyricsLines);
        auto maxLineLen = lyrics.GetMaxLyricsLineLength(startIndex, startIndex + linesToDraw, lyricsLines);
        startIndex += linesToDraw;
        remainingLines -= linesToDraw;

        mainAppWindow.UpdateDrawText(linesJoined,
            marginLeft, marginTop,
            frameWidth - 10, frameHeight - 10,
            hdc);
        marginLeft += 10 + ((maxLineLen * letterWidth) / 2);
    }
}

void AudioPlayer::HandleSearchEvent(HWND hWnd, MainAppWindow &mainWindow)
{
    PWSTR songPath = FindSong();
    if (songPath != NULL && GetSongFormat(songPath) == L".wav")
    {
        PlaySong(songPath);
        mainWindow.showLyricsBtn = mainWindow.CreatePushButton(L"Show lyrics", (HMENU)SHOW_LRCS_BTN, 10, 50, 100, 30);
        mainWindow.addLyricsBtn = mainWindow.CreatePushButton(L"Add lyrics", (HMENU)ADD_LRCS_BTN, 10, 90, 100, 30);
        mainWindow.stopPlayingBtn = mainWindow.CreatePushButton(L"Stop playing", (HMENU)STOP_PLAYING_BTN, 10, 130, 100, 30);
        mainWindow.playAgainBtn = mainWindow.CreatePushButton(L"Play again", (HMENU)PLAY_AGAIN_BTN, 10, 170, 100, 30);
    }
    else
    {
        MessageBox(hWnd, L"Invalid file given. Choose wav format files only.", L"Invalid audio file", MB_OK);
    }
}

void AudioPlayer::HandleAddLyricsEvent()
{
    PWSTR lyricsPath = FileHandler::GetOpenedFilePath();

    if (!currentSong.empty() && lyricsPath != NULL)
    {
        lyrics.AddLyricsToDatabase(currentSong, FileHandler::ConvertToWstring(lyricsPath));
    }
}

void AudioPlayer::HandleShowLyricsEvent(HWND hWnd)
{
    if (!currentSong.empty())
    {
        std::vector<std::wstring> lyricsText;
        std::vector<std::wstring> lyricsPathsCollection;

        if (lyrics.SongInDatabase(currentSong)) {
            lyricsPathsCollection = lyrics.GetLyricsCollection(currentSong);
            auto lyricsPath = lyricsPathsCollection.at(GetNextLyricsIndex() % lyricsPathsCollection.size());
            lyricsText = lyrics.GetLyricsFromFile(lyricsPath, currentSong);
        }
        if (lyricsText.empty())
        {
            lyricsText = std::vector<std::wstring>{ L"No lyrics found" };
        }

        SetCurrentLyrics(lyricsText, lyricsPathsCollection.size());
        InvalidateRect(hWnd, NULL, false); // reinvokes window paint
    }
}

void AudioPlayer::HandlePlayAgainEvent()
{
    PlayCurrentSong();
}

void AudioPlayer::HandleStopPlayingEvent()
{
    PlaySound(NULL, 0, 0);
}

// MainAppWindow class implementation
void MainAppWindow::RegisterMainWindow(HWND hWnd)
{
    mainWindow = hWnd;
}
HWND MainAppWindow::CreatePushButton(LPCWSTR btnText, HMENU btnInstance, int left, int top, int width, int height)
{
    return CreateWindow(L"button", btnText,
        WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // styles
        left, top, // position
        width, height, // size
        mainWindow, // parent window
        btnInstance, GetModuleHandle(NULL), NULL);
}

void MainAppWindow::UpdateDrawText(const std::wstring& text, int left, int top, int width, int height, HDC hdc)
{
    auto constText = text.c_str();

    RECT rect;
    SetTextColor(hdc, RGB(0, 0, 0));
    SetRect(&rect, left, top, width, height);
    DrawText(hdc, constText, static_cast<int>(wcslen(constText)), &rect, DT_TOP | DT_LEFT);
}

std::pair<int, int> MainAppWindow::GetClientAreaSize(HWND hWnd)
{
    RECT rect;
    auto widthHeight = std::pair<int, int>();
    if (GetWindowRect(hWnd, &rect))
    {
        widthHeight.first = rect.right - rect.left;
        widthHeight.second = rect.bottom - rect.top;
    }
    return widthHeight;
}