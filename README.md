# WavePlayer <img src=".\WavePlayer\WavePlayer.png" width="50" height="50" />

`Windows`	`Win32 API`	`C++ 17`

**Audio player for wav files with lyrics support**

* Minimalistic
* Intuitive
* Easy to use

### Find song

`WavePlayer` provides button with `Find song` label for song searching. Once you click this button you are prompted to navigate to the desired `wav` file using the standard open dialog. If you choose file of a different format than `wav` a message box is shown but no error arises so you can search for a new file immediately after closing the message box.

If you provided a valid `wav` file path, a song is played automatically. New buttons appear under `Find song` button. These are:

- `Show lyrics`
- `Add lyrics`
- `Stop playing`
- `Play again`

`Stop playing` button exits song playing completely.

`Play again` button enables song replay. It can be clicked during the time the current song is still playing as well. In this case current song is stopped as it would be with `Stop playing` button used and it is played from the beginning.

Don't like the currently playing song or just want to skip the ending part and move to another song? Simply click `Find song` button again and choose a new song. `WavePlayer` only lets you play one song at once so the new song playback overrides the old one. 

### Add lyrics

If you find a new song and it starts playing you can bind it with the lyrics file. Click `Add lyrics` button and navigate to the lyrics file. A song file is now bounded with the lyrics file and you can display the lyrics in the application window using `Show lyrics` button. The application saves the file path to its `csv` database file (named `Lyrics`) so next time you play the same song you can skip the adding lyrics part and jump directly to showing it on the screen.

You can add the lyrics and display it right after adding it. No application restart needed. 

#### Multiple lyrics support

Sometimes you only wish to read the plain lyrics for karaoke, other time you would appreciate the lyrics with chords to have a guitar lesson? No problem, `WavePlayer` comes with the support of unlimited number of lyrics. Simply click `Add lyrics` button while the song is playing and add new file to the database. To browse through the available lyrics just press `Show lyrics` button repeatedly.

### Show lyrics

Displays lyrics binded to the currently chosen song. Song name is displayed on the first line automatically (it is derived from the song file name) so there is no need to specify it inside the lyrics file.

If no lyrics path is found in the database, the message `No lyrics found` is displayed instead of the actual lyrics. Use `Add lyrics` button to associate current song with one or more lyrics files.

If lyrics path is saved in the database but the file is found missing (it was renamed / deleted), the record of this lyrics file is erased from the database and if no other lyrics is saved with the current song, the message `No lyrics found` is again displayed.

#### Adjust the lyrics screen

Resize the application window and see how the lyrics reacts. By changing the window size you can easily customize the number of columns to your preference. But stay careful if there is still enough space for the whole lyrics to be displayed - otherwise some lines or their parts may not be visible.
