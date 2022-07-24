#include "HotReloader/Network/HotObjectSync.h"

#include <fstream>
#include <iterator>
#include <algorithm>
#include <iostream>

#include "Network/IServer.h"

namespace Duckvil { namespace HotReloader { namespace Network {

    HotObjectSync::HotObjectSync(const Memory::FreeList& _heap, Duckvil::Network::IClient* _pClient, Duckvil::Network::IServer* _pServer, HotReloader::RuntimeCompilerSystem* _pRCS) :
        m_pServer(_pServer),
        m_pClient(_pClient),
        m_pRCS(_pRCS)
    {

    }

    HotObjectSync::~HotObjectSync()
    {

    }

    bool HotObjectSync::Init()
    {
        return true;
    }

    void HotObjectSync::Update(double _dDelta)
    {

    }

    void HotObjectSync::OnEvent(const SwapEvent& _event)
    {
        if(m_owner == Duckvil::Network::IConnection::Owner::SERVER && m_pServer)
        {
            Duckvil::Network::Message _message(NetworkCommands::SendHotObject);

            std::vector<char> _data;

            std::ifstream _moduleFile(std::filesystem::path(DUCKVIL_SWAP_OUTPUT) / (_event.m_sModuleName + ".dll").m_sText, std::ios::binary);

            uint32_t _size = 0;

            _moduleFile.seekg(0, std::ios::end);

            _size = _moduleFile.tellg();

            _moduleFile.seekg(0, std::ios::beg);

            std::copy(
                std::istreambuf_iterator<char>(_moduleFile),
                std::istreambuf_iterator<char>(),
                std::back_inserter(_data)
            );

            _moduleFile.close();

            _message << _data;

            m_pServer->MessageAllClients(_message);
        }
        else if(m_owner == Duckvil::Network::IConnection::Owner::CLIENT && m_pClient)
        {
            Duckvil::Network::Message _message(NetworkCommands::SendHotObject);

            std::vector<char> _data;

            std::ifstream _moduleFile(std::filesystem::path(DUCKVIL_SWAP_OUTPUT) / (_event.m_sModuleName + ".dll").m_sText, std::ios::binary);

            uint32_t _size = 0;

            _moduleFile.seekg(0, std::ios::end);

            _size = _moduleFile.tellg();

            _moduleFile.seekg(0, std::ios::beg);

            std::copy(
                std::istreambuf_iterator<char>(_moduleFile),
                std::istreambuf_iterator<char>(),
                std::back_inserter(_data)
            );

            _moduleFile.close();

            _message << _event.m_pTrackKeeper->GetTypeHandle().m_ID << _data;

            m_pClient->Send(_message);
        }
    }

    void HotObjectSync::SetOwner(Duckvil::Network::IConnection::Owner _owner)
    {
        m_owner = _owner;
    }

    bool HotObjectSync::OnMessage(const Duckvil::Network::Message& _message, std::shared_ptr<Duckvil::Network::IConnection> _pClient)
    {
        Duckvil::Network::Message _msg = _message;

        if(_message == NetworkCommands::SendHotObject)
        {
            std::vector<char> _data;

            _msg >> _data;

            std::filesystem::path _moduleFilename = std::tmpnam(nullptr);

            _moduleFilename = _moduleFilename.filename();

            std::filesystem::path _modulePath = DUCKVIL_SWAP_OUTPUT;

            std::ofstream _file(_modulePath / (_moduleFilename.string() + ".dll"), std::ios::binary);
            std::ostream_iterator<char> _outputIterator(_file);

            std::copy(
                _data.begin(),
                _data.end(),
                _outputIterator
            );

            _file.close();

            m_pRCS->HotReload(_moduleFilename);

            if(m_owner == Duckvil::Network::IConnection::Owner::SERVER)
            {
                m_pServer->MessageAllClients(_message, _pClient);
            }

            return true;
        }

        return false;
    }

}}}