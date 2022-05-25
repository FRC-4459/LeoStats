#include <iostream>
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <string>
#include <vector>
#include "authkey.h"

nlohmann::json data;
using json = nlohmann::json;

std::string readBuffer; //String that the JSON will be stored in
std::string getAuthKey();  //Forward declare so the compiler knows to search our headers (authkey.h)
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

class info 
{
    private:

    struct game 
    {
        int matchNumber {0};
        long int matchTime {0};
        int blue[3] { 0, 0, 0 };
        int red[3] { 0, 0, 0 };
        bool isBlue {false};
        std::string compLevel {"qm"};
    };
    
    std::vector<game> games;

    void printGame(int i) 
    {
        if ( games.at(i).compLevel == "qm" )
                { std::cout << "Qualifier "; }
        else if ( games.at(i).compLevel == "qf" )
                { std::cout << "Quarterfinal "; }
        else if ( games.at(i).compLevel == "sf" )
                { std::cout << "Semifinal "; }
        else if ( games.at(i).compLevel == "f" )
                { std::cout << "Final "; }
        std::cout << games.at(i).matchNumber << ":  ";

        long int* timePtr = &games.at(i).matchTime;
        std::time_t result = std::time(timePtr);
        std::cout << "\n" << std::asctime(std::localtime(&result));

        //First two team numbers print with a comma and last without
        std::cout << "Blue: ";
        for (int x {0}; x < 3; x++ )
            { 
                std::cout << games.at(i).blue[x];
                
                if ( x < 2 )
                    { std::cout << ", "; }
                else
                    { std::cout << "\n"; }
            }

        std::cout << "Red: ";
        for (int x {0}; x < 3; x++ )
        {
            { 
                std::cout << games.at(i).red[x];
                
                if ( x < 2 )
                    { std::cout << ", "; }
                else
                    { std::cout << "\n"; }
            }
                    
        }

    if ( games.at(i).isBlue == true )
        { std::cout << "You need BLUE bumpers this match.";  }
    else 
        { std::cout << "You need RED bumpers this match."; }
    std::cout << "\n\n\n";
    }
    
    public:

    void getData( json data, std::string inputTeamNum ) 
    {
        std::string blueTempNum;
        std::string redTempNum;
        game currentGame;

        for ( int i {0}; i < data.size(); i++ ) 
            {
                currentGame.matchNumber = data[i]["match_number"];
                currentGame.matchTime = data[i]["actual_time"];
                currentGame.compLevel = data[i]["comp_level"];

                for (int x{0}; x < data[i]["alliances"]["blue"]["team_keys"].size(); x++) 
                    { 
                        blueTempNum = data[i]["alliances"]["red"]["team_keys"][x];
                        currentGame.blue[x] = stoi(blueTempNum.erase(0, 3));
                        
                        if ( blueTempNum == inputTeamNum )
                            { currentGame.isBlue = true; }
                    }
                
                for (int x{0}; x < data[i]["alliances"]["red"]["team_keys"].size(); x++) 
                    { 
                        redTempNum = data[i]["alliances"]["blue"]["team_keys"][x];
                        currentGame.red[x] = std::stoi(redTempNum.erase(0, 3)); 
                        
                        if ( redTempNum == inputTeamNum )
                            { currentGame.isBlue = false; }
                    }

                    games.push_back(currentGame);
            } 
    }

    void printingLogic( json data ) 
    {  
        for (int i{0}; i < data.size(); i++ ) 
        {
            if ( games.at(i).compLevel == "qm" )
                { printGame(i); }
        }
        
        for (int i{0}; i < data.size(); i++ ) 
        {
            if ( games.at(i).compLevel == "qf" )
                { printGame(i); }
        }

        for (int i{0}; i < data.size(); i++ ) 
        {
            if ( games.at(i).compLevel == "sf" )
                { printGame(i); }
        }
    
        for (int i{0}; i < data.size(); i++ ) 
        {
            if ( games.at(i).compLevel == "f" )
                { printGame(i); }
        }
    }
};

info inf;

std::string inputRequestUrl(std::string url, std::string& inputTeamNum) 
{
    url.append("https://www.thebluealliance.com/api/v3/team/");    
    
    std::cout << "Enter your team number: ";
    std::cin >> inputTeamNum;
    url.append("frc");
    url.append(inputTeamNum);

    std::string inputEventKey;
    url.append("/event/");
    std::cout << "Enter your event number: ";
    std::cin >> inputEventKey;
    url.append(inputEventKey);
    url.append("/matches/simple");

    return url;
}

int main()
{
    using json = nlohmann::json;
    
    std::string url;
    std::string inputTeamNum;

    url = inputRequestUrl(url, inputTeamNum);
    request(url);
    json data = json::parse(readBuffer);
    inf.getData(data, inputTeamNum);
    inf.printingLogic( data );
    
    return 0;
}