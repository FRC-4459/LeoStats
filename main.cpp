# define JSON_DIAGNOSTICS 1

#include <iostream>
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <string>
#include <vector>
#include "authkey.h"

std::string getAuthKey();

// Parsing the JSON instead is a much better solution.

// void findAllOccurances(std::vector<size_t>& vec, std::string data, std::string toSearch)
// {
//     // Get the first occurrence
//     size_t pos = data.find(toSearch);
//     // Repeat till end is reached
//     while (pos != std::string::npos)
//     {
//         // Add position to the vector
//         vec.push_back(pos);
//         // Get the next occurrence from the current position
//         pos = data.find(toSearch, pos + toSearch.size());
//     }
// }


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

int main()
{

    std::string keyword{"},"};
    std::vector<size_t> occurances;

    using json = nlohmann::json;

    request();
    
    //Parse our output (readBuffer) into json
    json data = json::parse(readBuffer);
    
    

    try
    {

        //This prints the raw response unparsed. Useful because it is formatted whereas the JSON is not.
        std::cout << readBuffer << std::endl;

        
        int i {0};
        for (auto& x : data.items()) 
        {
            
            std::cout << "Match Number " << data[i]["match_number"] << ":\n";
            std::cout << "Blue: " << data[i]["alliances"]["blue"]["team_keys"] << std::endl;
            std::cout << "Red: " << data[i]["alliances"]["red"]["team_keys"] << std::endl << std::endl;
            i++;
        }

    }
    catch (json::exception& e)
    {
        std::cout << e.what() << '\n';
    }

    


    return 0;
}