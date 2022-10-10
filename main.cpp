#include <string>
#include <vector>
#include <iostream>

#include <json.hpp>
#include <curl/curl.h>

#include "authkey.h"
#include "request.cpp"
#include "game.cpp"

class game;

/* TODO:
    Expected time handling
    Sorting the matches by comp level, then match number
*/

nlohmann::json data;
using json = nlohmann::json;

std::string getUrl(std::string &url, std::string &teamNum)
{

    std::string requestType {"team"};
    std::cout << "Would you like all matches or only those your team participates in? enter all/team  ";
    std::cin >> requestType;

    std::cout << "Enter your FRC team number: ";
    std::cin >> teamNum;

    std::string eventKey;
    std::cout << "Enter a valid TBA event code: ";
    std::cin >> eventKey;

    if ( requestType == "team" )
        {
            url.append("https://www.thebluealliance.com/api/v3/team/");
            url.append("frc");
            url.append(teamNum);
            url.append("/event/");
            url.append(eventKey);
            url.append("/matches/simple");
        }

    else if ( requestType == "all" )
        {
            url.append("https://www.thebluealliance.com/api/v3/event/");
            url.append(eventKey);
            url.append("/matches/simple");
        }

    return url;
}

int main()
{
    using json = nlohmann::json;

    std::vector<game> games;
    game currentGame;

    std::string readBuffer;
    std::string url;
    std::string teamNum;


    getUrl(url, teamNum); //This function modifies the team number.
    request(url, readBuffer);
    json data = json::parse(readBuffer);

    
    while (data.empty() || data.contains("Error"))
    {
        if ( data.empty() )
            { std::cout << "\nNo results found. Maybe your search was faulty?\n"; }
        else if ( data.contains("Error") )
            { std::cout << "\n" << data.at("Error"); }

        url.clear();
        getUrl(url, teamNum);
        readBuffer.clear();
        request(url, readBuffer);
        data = json::parse(readBuffer);
    }

    for ( unsigned int i{0}; i < data.size(); i++ )
    {
        currentGame.reset();
        currentGame.getGame(data, i, teamNum);
        games.push_back(currentGame);
    }

    std::cout << "\n";
    for ( unsigned long i{0}; i < games.size(); i++ )
        { games.at(i).print(); }
}
