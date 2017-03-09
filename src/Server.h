//
//  rtmp_relay
//

#pragma once

#include <vector>
#include "Connection.h"

namespace relay
{
    class Server
    {
    public:
        Server(Relay& aRelay, cppsocket::Network& aNetwork);

        void start(const std::vector<Connection::Description>& aConnectionDescriptions);

        void update(float delta);

        void startStreaming(Connection& connection);
        void stopStreaming(Connection& connection);

        void startReceiving(Connection& connection);
        void stopReceiving(Connection& connection);

        void removeConnection(Connection& connection);

        const std::vector<Connection::Description>& getConnectionDescriptions() const { return connectionDescriptions; }

        void sendAudioHeader(const std::vector<uint8_t>& headerData);
        void sendVideoHeader(const std::vector<uint8_t>& headerData);
        void sendAudio(uint64_t timestamp, const std::vector<uint8_t>& audioData);
        void sendVideo(uint64_t timestamp, const std::vector<uint8_t>& videoData);
        void sendMetaData(const amf0::Node& newMetaData);
        void sendTextData(uint64_t timestamp, const amf0::Node& textData);

    private:
        Relay& relay;
        cppsocket::Network& network;
        std::vector<Connection::Description> connectionDescriptions;

        Connection* inputConnection = nullptr;
        std::vector<Connection*> outputConnections;

        std::string applicationName;
        std::string streamName;

        bool streaming = false;
        std::vector<uint8_t> audioHeader;
        std::vector<uint8_t> videoHeader;
        amf0::Node metaData;

        std::vector<std::unique_ptr<Connection>> connections;
    };
}
