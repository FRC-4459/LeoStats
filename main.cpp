#include <string>
#include <vector>
#include <iostream>

#include <json.hpp>
#include <curl.h>

#include "mainwindow.h"
#include <QApplication>
#include <QPushButton>

#include "authkey.h"

class game;

/* TODO:
Expected time handling
Sorting the matches by comp level, then match number
*/

nlohmann::json data;
using json = nlohmann::json;

std::string readBuffer;
std::string getAuthKey();
std::string authkey = getAuthKey();

std::string setType( std::string type )
{
    if ( type == "qm" )
        { return "Qualifier"; }
    else if ( type == "ef" )
        { return "Elimination"; }
    else if ( type == "qf" )
        { return "Quarterfinal"; }
    else if ( type == "sf" )
        { return "Semifinal"; }
    else if ( type == "f" )
        { return "Final"; }
    return "N/A";
}

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
        bool userParticipating { false };
        std::string blue[3] { "0", "0", "0" };
        std::string red[3] { "0", "0", "0" };
        std::string myAlliance { "RED" };
        std::string matchType {"N/A"};

        template <typename T>

        void grabFromJSON(T &var, nlohmann::json value)
        {
            if ( value.is_null() || value.empty() )
                { return; }
            else
                { var = value.get<T>(); }
        }

    public:

        void reset()
        {
            matchNumber = 0;
            matchTime = 0;
            blue[0] = "0"; blue[1] = "0"; blue[2] = "0";
            red[0] = "0"; red[1] = "0"; red[2] = "0";
            myAlliance = "N/A";
            matchType = "N/A";
            userParticipating = false;
        }

        void checks()
        {
            for (int i{0}; i < 3; i++ )
            {
                if ( blue[i].empty() )
                    { blue[i] = "N/A"; }
                if ( red[i].empty() )
                    { red[i] = "N/A";  }
            }
        }

        void getGame( json data, int index, std::string teamNum )
        {
            grabFromJSON<int>(matchNumber, data[index]["match_number"]);
            grabFromJSON<long int>(matchTime, data[index]["actual_time"]);
            grabFromJSON<std::string>(matchType, data[index]["comp_level"]);

            matchType = setType(matchType);

            for ( int i{0}; i < 3; i++ )
            {
                grabFromJSON<std::string>(blue[i], data[index]["alliances"]["blue"]["team_keys"][i]);
                blue[i].erase(0, 3);

                grabFromJSON<std::string>(red[i], data[index]["alliances"]["red"]["team_keys"][i]);
                red[i].erase(0, 3);
            }

            for ( int i{0}; i < 3; i++ )
            {
                if ( blue[i] == teamNum )
                    {
                        myAlliance = "BLUE";
                        userParticipating = true;
                        break;
                    }
                else if ( red[i] == teamNum )
                    {
                        myAlliance = "RED";
                        userParticipating = true;
                        break;
                    }
            }
        }

        void print()
        {
            using std::cout;

            cout << matchType << " " << matchNumber << ":\n";

            if ( matchTime == 0 )
                { cout << "N/A"; }
            else
                { cout << std::asctime(std::localtime(&matchTime)); }

            cout << "Blue Alliance: " << blue[0] << ", " << blue[1] << ", " << blue[2] << "\n";
            cout << "Red Alliance: " << red[0] << ", " << red[1] << ", " << red[2] << "\n";

            if ( userParticipating == true )
                { cout << "You need " << myAlliance << " bumpers this match."; }

            cout << "\n\n";
        }
};

std::string getUrl(std::string &url, std::string &teamNum)
{

    std::string requestType {"team"};
    std::cout << "All matches or those for your team: ";
    std::cin >> requestType;

    std::cout << "Enter your team number: ";
    std::cin >> teamNum;

    std::string eventKey;
    std::cout << "Enter your event code: ";
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

int main(int argc, char **argv)
{
    using json = nlohmann::json;

    std::vector<game> games;
    game currentGame;

    std::string url;
    std::string teamNum;


    getUrl(url, teamNum); //This function modifies the team number.
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
            getUrl(url, teamNum);
            readBuffer.clear();
            request(url);
            data = json::parse(readBuffer);
        }

        else
            { break; }
    }

    for ( unsigned int i{0}; i < data.size(); i++ )
    {
        currentGame.reset();
        currentGame.getGame(data, i, teamNum);
        games.push_back(currentGame);
    }

    for ( unsigned long i{0}; i < games.size(); i++ )
        { games.at(i).print(); }

    std::cout << "\n\n\n" << readBuffer << "\n\n\n";

    QApplication app(argc, argv);

    QPushButton button ("Hello World !");
    button.setToolTip("Tooltip");
    button.show();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();


    return app.exec();
}
