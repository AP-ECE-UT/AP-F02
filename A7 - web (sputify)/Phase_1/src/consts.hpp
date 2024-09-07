#ifndef CONSTS_HPP_INCLUDE
#define CONSTS_HPP_INCLUDE

#include <string>

namespace consts {
// clang-format off

const int MUSICS_START_ID = 1;
const int USERS_START_ID  = 1;

namespace necessity {
const bool NECESSARY   = true;
const bool UNNECESSARY = false;
} // namespace necessity

namespace input {
const char CMD_DELIM                  = '?';
const char CMD_ARG_START              = '<';
const char CMD_ARG_END                = '>';
const char TAG_DELIM                  = ';';
const char TIME_DELIM                 = ':';

const std::string GET                 = "GET";
const std::string POST                = "POST";
const std::string PUT                 = "PUT";
const std::string DELETE              = "DELETE";
const std::string NONE                = "";
const std::string ARTIST_MODE         = "artist";
const std::string LISTENER_MODE       = "user";
} // namespace input

namespace responses {
const std::string BAD_REQUEST         = "Bad Request";
const std::string NOT_FOUND           = "Not Found";
const std::string PERMISSION_DENIED   = "Permission Denied";
const std::string EMPTY               = "Empty";
const std::string OK                  = "OK";
} // namespace responses

namespace output {
const std::string ARTIST_MODE         = "artist";
const std::string LISTENER_MODE       = "user";
const std::string INFO_DELIM          = ", ";
const char TAG_DELIM                  = ';';
const std::string ID                  = "ID";
const std::string NAME                = "Name";
const std::string ARTIST              = "Artist";
const std::string YEAR                = "Year";
const std::string ALBUM               = "Album";
const std::string TAGS                = "Tags";
const std::string DURATION            = "Duration";
const std::string MODE                = "Mode";
const std::string USERNAME            = "Username";
const std::string PLAYLISTS           = "Playlists";
const std::string SONGS               = "Songs";
const std::string PLAYLISTS_SONGS_NUM = "Playlists_number/Songs_number";
const std::string PLAYLIST_NAME       = "Playlist_name";
const std::string SONGS_NUMBER        = "Songs_number";
const std::string FOLLOWINGS          = "Followings";
const std::string FOLLOWERS           = "Followers";
const std::string OUTPUT_DELIM        = ", ";
const std::string OUTPUT_INDICATOR    = ": "; 
} // namespace output

namespace arg_names {
const std::string SIGNUP_USERNAME = "username";
const std::string SIGNUP_PASSWORD = "password";
const std::string SIGNUP_MODE     = "mode";

const std::string LOGIN_USERNAME  = "username";
const std::string LOGIN_PASSWORD  = "password";

const std::string ID       = "id";
const std::string NAME     = "name";
const std::string ARTIST   = "artist";
const std::string TAG      = "tag";
const std::string TITLE    = "title";
const std::string PATH     = "path";
const std::string YEAR     = "year";
const std::string ALBUM    = "album";
const std::string TAGS     = "tags";
const std::string DURATION = "duration";
}; // namespace arg_names

namespace cmds {
const std::string SIGNUP            = "signup";
const std::string LOGIN             = "login";
const std::string LOGOUT            = "logout";
const std::string MUSICS            = "musics";
const std::string USERS             = "users";
const std::string PLAYLIST          = "playlist";
const std::string ADD_PLAYLIST      = "add_playlist";
const std::string SEARCH_MUSICS     = "search_music";
const std::string MUSIC             = "music";
const std::string REGISTERED_MUSICS = "registered_musics";
} // namespace cmds

// clang-format on
} // namespace consts

#endif // CONSTS_HPP_INCLUDE
