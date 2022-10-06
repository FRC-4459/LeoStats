#include <string>
#include <iostream>
#include <vector>

#include <json.hpp>
using json = nlohmann::json;

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
        
        std::string setType(std::string type)
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
