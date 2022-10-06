#include <string>
#include <iostream>
#include <curl/curl.h>
#include "authkey.h"

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void request( std::string url, std::string readBuffer )
{
    CURL* curl;
    CURLcode res;

    std::string eventsByTeam {"https://www.thebluealliance.com/api/v3/team/frc4459/events/2022/simple"};

    //Create a list of our headers; One for authorization, the other to tell the program to accept a JSON response
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, getAuthKey().c_str());
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
