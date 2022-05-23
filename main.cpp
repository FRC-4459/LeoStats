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

    int amountMatches = data.size();
    std::vector <std::string> redList;
    std::vector <std::string> blueList;
    std::vector <int> matchNumbers;
    std::vector <long int> matchTimes;
    
    public:

    void getData( json data, std::string inputTeamNum ) 
    {
        std::string blueTempNum;
        std::string redTempNum;

        for (int i{1}; i < data.size(); i++ ) 
            { 
                amountMatches++; 
            }
        
        for ( int i {0}; i < amountMatches; i++ ) 
            {
                matchNumbers.push_back(data[i]["match_number"]);

                matchTimes.push_back(data[i]["actual_time"]);

                for (int x{0}; x < data[i]["alliances"]["blue"]["team_keys"].size(); x++) 
                    { 
                        blueTempNum = data[i]["alliances"]["red"]["team_keys"][x];
                        blueTempNum.erase(0, 3);
                        blueList.push_back(blueTempNum); 
                    }
                
                for (int x{0}; x < data[i]["alliances"]["red"]["team_keys"].size(); x++) 
                    { 
                        redTempNum = data[i]["alliances"]["blue"]["team_keys"][x];
                        redTempNum.erase(0, 3);
                        redList.push_back(redTempNum); 
                    }
            } 
    }

    void printData() 
    {  
        int blueIndex {0};
        int redIndex {0};
        
        for (int i{0}; i < amountMatches; i++ ) 
        {
            std::cout << "Match number " << matchNumbers.at(i) << ":  ";

            long int* timePtr = &matchTimes.at(i);
            std::time_t result = std::time(timePtr);
            std::cout << "\n" << std::asctime(std::localtime(&result));

            //First two team numbers print with a comma and last without
            std::cout << "\033[34;mBlue:\033[0m ";
            for (int i {0}; i < 3; i++ )
                { 
                    if ( i < 2 )
                    {
                        std::cout << blueList.at(blueIndex) << ", ";
                        blueIndex++; 
                    }

                    else
                    {
                        std::cout << blueList.at(blueIndex);
                        blueIndex++;
                        std::cout << "\n";
                    }
                }


            std::cout << "Red: ";
            for (int i {0}; i < 3; i++ )
                { 
                    if ( i < 2 )
                    {
                        std::cout << redList.at(redIndex) << ", ";
                        redIndex++;
                    }

                    else 
                    {
                        std::cout << redList.at(redIndex);
                        redIndex++;
                        std::cout << "\n\n";
                    }
                }
        }
    
        std::cout << "\n";

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
    inf.printData();
    
    return 0;
}