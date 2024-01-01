#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <unordered_map>

namespace fs = std::filesystem;

struct Song {
    std::string title;
    std::string filePath;
    size_t index; // Add an index member to track the position in allSongs
};

class MusicPlayer {
public:
    void loadSongsFromDirectory(const std::string& directoryPath) {
        size_t currentIndex = 1;
        for (const auto& entry : fs::directory_iterator(directoryPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".ogg") {
                addSong(entry.path().filename().stem().string(), entry.path().string(), currentIndex);
                currentIndex++;
            }
        }
    }

    void displayAllSongs() const {
        std::cout << "All Songs in Root Directory:\n";
        for (size_t i = 0; i < allSongs.size(); ++i) {
            std::cout << i + 1 << ". " << allSongs[i].title << '\n';
        }
    }

    void playSongFromRootDirectory(size_t index) const {
        if (index < 1 || index > allSongs.size()) {
            std::cout << "Invalid song index.\n";
            return;
        }

        const Song& song = allSongs[index - 1];
        sf::Music music;
        if (!music.openFromFile(song.filePath)) {
            std::cerr << "Failed to load music: " << song.title << '\n';
            return;
        }

        std::cout << "Now playing: " << song.title << '\n';
        music.play();

        while (music.getStatus() == sf::Music::Playing) {
            // Keep the program running while the song is playing
        }
    }

    void addToPlaylist(size_t songIndex, size_t playlistIndex) {
        if (songIndex < 1 || songIndex > allSongs.size() || playlistIndex < 1 || playlistIndex > playlists.size()) {
            std::cout << "Invalid song or playlist index.\n";
            return;
        }

        const Song& song = allSongs[songIndex - 1];
        playlists[playlistIndex - 1].push_back(song);
        std::cout << "Added '" << song.title << "' to playlist " << playlistIndex << ".\n";
    }

    void displayAllPlaylists() const {
        std::cout << "All Playlists:\n";
        for (size_t i = 0; i < playlists.size(); ++i) {
            std::cout << i + 1 << ". Playlist " << i + 1 << '\n';
        }
    }

    void playPlaylist(size_t playlistIndex) const {
        if (playlistIndex < 1 || playlistIndex > playlists.size()) {
            std::cout << "Invalid playlist index.\n";
            return;
        }

        const auto& playlist = playlists[playlistIndex - 1];
        if (playlist.empty()) {
            std::cout << "Playlist is empty. Add songs before playing.\n";
            return;
        }

        std::cout << "Playing Playlist " << playlistIndex << ":\n";
        for (const auto& song : playlist) {
            playSongFromRootDirectory(song.index);
        }
    }

    void deletePlaylist(size_t playlistIndex) {
        if (playlistIndex < 1 || playlistIndex > playlists.size()) {
            std::cout << "Invalid playlist index.\n";
            return;
        }

        playlists.erase(playlists.begin() + playlistIndex - 1);
        std::cout << "Deleted Playlist " << playlistIndex << ".\n";
    }

    void displaySongsInPlaylist(size_t playlistIndex) const {
        if (playlistIndex < 1 || playlistIndex > playlists.size()) {
            std::cout << "Invalid playlist index.\n";
            return;
        }

        const auto& playlist = playlists[playlistIndex - 1];
        std::cout << "Songs in Playlist " << playlistIndex << ":\n";
        for (const auto& song : playlist) {
            std::cout << "- " << song.title << '\n';
        }
    }

private:
    std::vector<Song> allSongs;
    std::vector<std::vector<Song>> playlists;

    void addSong(const std::string& title, const std::string& filePath, size_t index) {
        Song song;
        song.title = title;
        song.filePath = filePath;
        song.index = index; // Set the index
        allSongs.push_back(song);
    }
};

int main() {
    MusicPlayer musicPlayer;

    // Specify the directory containing your songs
    std::string programDirectory = fs::current_path().string();
    musicPlayer.loadSongsFromDirectory(programDirectory);

    while (true) {
        std::cout << "\nMenu:\n"
                  << "1. Display all songs in root directory\n"
                  << "2. Play a song\n"
                  << "3. Add a song to playlist\n"
                  << "4. Choose playlist\n"
                  << "5. Quit\n"
                  << "Choose an option: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                musicPlayer.displayAllSongs();
                break;
            case 2: {
                size_t songIndex;
                std::cout << "Enter the index of the song to play: ";
                std::cin >> songIndex;
                musicPlayer.playSongFromRootDirectory(songIndex);
                break;
            }
            case 3: {
                size_t songIndex, playlistIndex;
                std::cout << "Enter the index of the song to add to the playlist: ";
                std::cin >> songIndex;
                std::cout << "Enter the index of the playlist to add the song: ";
                std::cin >> playlistIndex;
                musicPlayer.addToPlaylist(songIndex, playlistIndex);
                break;
            }
            case 4: {
                size_t playlistChoice;
                std::cout << "Choose a playlist:\n";
                musicPlayer.displayAllPlaylists();
                std::cin >> playlistChoice;

                std::cout << "\nPlaylist Menu:\n"
                          << "1. Play playlist\n"
                          << "2. Delete playlist\n"
                          << "3. Display songs in playlist\n"
                          << "Choose an option: ";

                int playlistOption;
                std::cin >> playlistOption;

                switch (playlistOption) {
                    case 1:
                        musicPlayer.playPlaylist(playlistChoice);
                        break;
                    case 2:
                        musicPlayer.deletePlaylist(playlistChoice);
                        break;
                    case 3:
                        musicPlayer.displaySongsInPlaylist(playlistChoice);
                        break;
                    default:
                        std::cout << "Invalid playlist option.\n";
                }
                break;
            }
            case 5:
                return 0;
            default:
                std::cout << "Invalid option. Try again.\n";
        }
    }

    return 0;
}
