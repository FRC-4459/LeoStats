# define JSON_DIAGNOSTICS 1

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
    
    public:

    void countMatches(json data) 
    {
        for (int i{1}; i < data.size(); i++ ) 
        { 
            amountMatches++; 
        }
    }

    void getData( json data ) 
    {
        for ( int i {0}; i < amountMatches; i++ ) 
            {
                matchNumbers.push_back(data[i]["match_number"]);

                for (int x{0}; x < data[i]["alliances"]["blue"]["team_keys"].size(); x++) 
                    { blueList.push_back(data[i]["alliances"]["blue"]["team_keys"][x]); }
                
                for (int x{0}; x < data[i]["alliances"]["red"]["team_keys"].size(); x++) 
                    { redList.push_back(data[i]["alliances"]["red"]["team_keys"][x]); }
            } 
    }

    void printMatches() { std::cout << amountMatches << "\n"; }

    void printData() 
    {  
        int blueIndex {0};
        int redIndex {0};
        
        for (int i{0}; i < amountMatches; i++ ) 
        {
            std::cout << "Match number " << matchNumbers.at(i) << ":\n";
            
            std::cout << "Blue: ";
            for (int i {0}; i < 3; i++ )
                { 
                    std::cout << blueList.at(i) << ", ";
                    blueIndex++;
                }
                
            std::cout << "\n";
            

            std::cout << "Red: ";
            for (int i {0}; i < 3; i++ )
                { 
                    std::cout << redList.at(i) << ", ";
                    blueIndex++;
                }
            
            std::cout << "\n";
        }
    }
};

info inf;

std::string inputRequestUrl(std::string& url) 
{
    url.append("https://www.thebluealliance.com/api/v3/team/");    
    
    std::string inputTeamNum;
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
    url = inputRequestUrl(url);
    std::cout << url;
    // https://www.thebluealliance.com/api/v3/team/frc4459/event/2022gadal/matches/simple

    request(url);

    json data = json::parse(readBuffer);

    inf.countMatches(data);
    inf.getData(data);
    inf.printData();

    std::cout << "\n";
    
    return 0;
    //This prints the raw response unparsed. Useful because it is formatted whereas the JSON is not.
    //std::cout << readBuffer;
}