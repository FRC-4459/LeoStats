#include <iostream>
#include <json.hpp>
#include <curl/curl.h>
#include <string>
#include <vector>
#include "authkey.h"


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
        //MATCHTYPE HERE
    
    public:
        void getGame( json data, int index, std::string teamNum ) 
        {
            matchNumber = data[index]["match_number"];
            matchTime = data[index]["actual_time"]; //We also need the predicted time!!!!
            
            for ( int i{0}; i < 3; i++ )
            {
                blue[i] = data[index]["alliances"]["blue"]["team_keys"][i];
                blue[i].erase(0, 3);
                
                red[i] = data[index]["alliances"]["red"]["team_keys"][i];
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

std::string inputRequestUrl(std::string url, std::string& teamNum) 
{
    url.append("https://www.thebluealliance.com/api/v3/team/");    
    
    std::cout << "Enter your team number: ";
    std::cin >> teamNum;
    url.append("frc");
    url.append(teamNum);

    std::string inputEventKey;
    url.append("/event/");
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
    std::string teamNum;

    url = inputRequestUrl(url, teamNum);
    request(url);
    json data = json::parse(readBuffer);

    for ( int i{0}; i < data.size(); i++ )
    {
        currentGame.getGame(data, i, teamNum);
        games.push_back(currentGame);
    }

    for ( int i{0}; i < games.size(); i++ )
        { games.at(i).print(); }

    //std::cout << "\n\n\n" << readBuffer << "\n\n\n";
    
    return 0;
}