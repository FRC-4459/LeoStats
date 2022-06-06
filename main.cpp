#include <iostream>
#include <json.hpp>
#include <curl/curl.h>
#include <string>
#include <vector>
#include "authkey.h"
#include <typeinfo>

/* TODO:
Expected time handling
A "toggle" for displaying all matches in an event rather than just those of your team
Sorting the matches by comp level, then match number (enum the comp levels)
*/

nlohmann::json data;
using json = nlohmann::json;

std::string readBuffer;
std::string getAuthKey();
std::string authkey = getAuthKey();

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void request( std::string url )
{
    CURL* curl;
    CURLcode res;

    std::string eventsByTeam {"https://www.thebluealliance.com/api/v3/team/frc4459/events/2022/simple"};

    //Create a list of our headers; One for authorization, the other to tell the program to accept a JSON response
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, authkey.c_str());
    headers = curl_slist_append(headers, "accept: application/json");

    curl = curl_easy_init();
    
    if (curl) 
    {
        //URL to request to
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        //Tell curl to output our json to a string called readBuffer
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        //Set headers to the list we made earlier
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        
        res = curl_easy_perform(curl);
        
        // Check for errors
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));

        curl_easy_cleanup(curl); 
    } 
}

class game 
{
    private:
        int matchNumber {0};
        long int matchTime {0};
        std::string blue[3] { "0", "0", "0" };
        std::string red[3] { "0", "0", "0" };
        std::string myAlliance { "RED" };
        std::string matchType {"match"};

        template <typename T>
        void grabFromJSON(T &var, nlohmann::json value) 
        {
            if ( value.is_null() )
                { return; }
            else
                { var = value.get<T>(); }
        }
    
    public:
        void getGame( json data, int index, std::string teamNum ) 
        {
            //matchNumber = data[index]["match_number"].get<int>();
            grabFromJSON<int>(matchNumber, data[index]["match_number"]);
            //matchTime = data[index]["actual_time"].get<long int>(); //We also need the predicted time!!!!
            grabFromJSON<long int>(matchTime, data[index]["actual_time"]);
            //matchType = data[index]["comp_level"].get<std::string>();
            grabFromJSON<std::string>(matchType, data[index]["comp_level"]);
            for ( int i{0}; i < 3; i++ )
            {
                blue[i] = data[index]["alliances"]["blue"]["team_keys"][i].get<std::string>();
                blue[i].erase(0, 3);
                
                red[i] = data[index]["alliances"]["red"]["team_keys"][i].get<std::string>();
                red[i].erase(0, 3);
                
            }
            
            for ( int i{0}; i < 3; i++ )
                if ( blue[i] == teamNum ) 
                    { myAlliance = "BLUE"; break; }
                else
                    { myAlliance = "RED"; }
        }

        void print() 
        {
            using namespace std;
            cout << "Match number " << matchNumber << ":\n";
            cout << std::asctime(std::localtime(&matchTime));
            cout << "Blue Alliance: " << blue[0] << ", " << blue[1] << ", " << blue[2] << "\n";
            cout << "Red Alliance: " << red[0] << ", " << red[1] << ", " << red[2] << "\n";
            cout << "You need " << myAlliance << " bumpers this match.\n\n\n";
        }

};

std::string inputRequestUrl(std::string &url, std::string& teamNum) 
{
    std::string type {"team"};
    std::cout << "All matches or those for your team: ";
    std::cin >> type;

    if ( type == "team" )
        { 
            url.append("https://www.thebluealliance.com/api/v3/team/");
            std::cout << "Enter your team number: ";
            std::cin >> teamNum;
            url.append("frc");
            url.append(teamNum);
        }
    else if ( type == "all" )
        { url.append("https://www.thebluealliance.com/api/v3/event/"); }
        

    std::string inputEventKey;
    if ( type == "team" )
        { url.append("/event/"); }
    std::cout << "Enter your event code: ";
    std::cin >> inputEventKey;
    url.append(inputEventKey);
    url.append("/matches/simple");

    return url;
}

int main()
{
    using json = nlohmann::json;

    std::vector<game> games;
    game currentGame;

    std::string url;
    std::string* urlPtr = &url;
    std::string teamNum;

    inputRequestUrl(url, teamNum);
    request(url);
    json data = json::parse(readBuffer);

    while ( true )
    {
        if ( data.empty() || data.contains("Error")  ) 
        {
            if ( data.empty() )
                { std::cout << "\nNo results found. Maybe your search was faulty?\n"; }
            else if ( data.contains("Error") )
                { std::cout << "\n" << data.at("Error"); }

            url.clear();
            inputRequestUrl(url, teamNum); 
            readBuffer.clear();
            request(url);
            data = json::parse(readBuffer);
        }

        else 
            { break; }
    }

    for ( int i{0}; i < data.size(); i++ )
    {
        currentGame.getGame(data, i, teamNum);
        games.push_back(currentGame);
    }

    for ( int i{0}; i < games.size(); i++ )
        { games.at(i).print(); }

    // std::cout << "\n\n\n" << readBuffer << "\n\n\n";
    
    return 0;
}