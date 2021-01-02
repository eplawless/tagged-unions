#include <iostream>
#include "tagged-unions.h"

TAGGED_UNION(Connection,
    (Disconnected, {
        int LastConnectionTime;
    }),
    (Connecting, {
        int LastConnectionTime;
        int ConnectionStartTime;
        const char *URL;
    }),
    (Connected, {
        int LastConnectionTime;
        const char *URL;
    })
)

void Handle(const FConnection &Connection) {
    // NOTE: You can't access e.g. Connection.Value.Connected here, it's private.
    // You have to go through the Match function, which prevents people from
    // ever accessing the wrong union member.
    Match<void>(Connection,
        [](const FConnection_Disconnected &Members) {
            std::cout << "Disconnected (last time " << Members.LastConnectionTime << ")\n";
        },
        +[](const FConnection_Connecting &Members) {
            std::cout << "Connecting to " << Members.URL 
                << " (last time " << Members.LastConnectionTime 
                << ", started " << Members.ConnectionStartTime << ")\n";
        },
        [](const FConnection_Connected &Members) {
            std::cout << "Connected to " << Members.URL 
                << " (last time " << Members.LastConnectionTime << ")\n";
        });
}

int GetLastConnectionTime(const FConnection &Connection) {
    auto Handler = [](const auto &Value) -> int { return Value.LastConnectionTime; };
    return Match<int>(Connection, Handler, Handler, Handler);
}

int main(int argc, char **argv) {
    Handle(FConnection_Disconnected {
        .LastConnectionTime = 20
    });
    Handle(FConnection_Connecting {
        .LastConnectionTime = 20,
        .ConnectionStartTime = 30,
        .URL = "google.com"
    });
    Handle(FConnection_Connected {
        .LastConnectionTime = 40,
        .URL = "google.com"
    });

    int PrevConnectionTime = GetLastConnectionTime(FConnection_Disconnected {
        .LastConnectionTime = 20
    });
    int ConnectionTime = GetLastConnectionTime(FConnection_Connected {
        .LastConnectionTime = 40,
        .URL = "google.com"
    });
    std::cout << "Connections at " << PrevConnectionTime << " and then " << ConnectionTime << "\n";
    return 0;
}
