# define JSON_DIAGNOSTICS 1

#include <iostream>
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <string>
#include <vector>
#include "authkey.h"

std::string getAuthKey();

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string authkey = getAuthKey();
std::string readBuffer;

void request()
{
    CURL* curl;
    CURLcode res;

    //Create a list of our headers
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, authkey.c_str());
    headers = curl_slist_append(headers, "accept: application/json");


    curl = curl_easy_init();
    if (curl) {
        //URL to request to
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.thebluealliance.com/api/v3/team/frc4459/event/2022gacar/matches/simple");

        //Tell curl to output our json to a string called readBuffer
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        //Set headers; One for authorization, the other to tell the program to accept a JSON response
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        //Set verbose to 1L for extra information
            //Switched off to not display authkey.
        curl_easy_setopt(curl, CURLOPT_VERBOSE, NULL);


        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));


        curl_easy_cleanup(curl);

    }

}

class jstuff 
{
    public:
    using json = nlohmann::json;
    json data = json::parse(readBuffer);
};
jstuff jbj;

class info 
{
    private:

    int amountMatches {0};
    std::vector <std::string> redList;
    std::vector <std::string> blueList;
    std::vector <int> matchNumbers;
    
    public:
        
    void countMatches() { for ( auto x : jbj.data.items() ) { amountMatches++; } }

    void getData() 
    {
    for ( int i {0}; i < amountMatches; i++ ) 
        {
            matchNumbers.push_back(jbj.data[i]["match_number"]);

            for (int x{0}; x < jbj.data[i]["alliances"]["blue"]["team_keys"].size(); x++) 
                { blueList.push_back(jbj.data[i]["alliances"]["blue"]["team_keys"][x]); }
            
            for (int x{0}; x < jbj.data[i]["alliances"]["red"]["team_keys"].size(); x++) 
                { redList.push_back(jbj.data[i]["alliances"]["red"]["team_keys"][x]); }
        } 
    }

};

info inf;

int main()
{
    using json = nlohmann::json;

    
    request();
    inf.countMatches();
    inf.getData();
    
    try
    {
        //This prints the raw response unparsed. Useful because it is formatted whereas the JSON is not.
        //std::cout << readBuffer << std::endl;

    
    
    }
    
    catch (json::exception& e)
    {
        std::cout << e.what() << '\n';
    }

    


    return 0;
}