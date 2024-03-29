#pragma once

#undef max
#undef min

#include "uuid_v4/uuid_v4.h"

namespace Duckvil {

    static UUIDv4::UUIDGenerator<std::mt19937_64> g_GeneratorUUID;

    class UUID
    {
    private:
        UUIDv4::UUID m_uuid;

    public:
        UUID() :
            m_uuid(g_GeneratorUUID.getUUID())
        {

        }

        UUID(const char* _aBytes) :
            m_uuid(_aBytes)
        {

        }

        ~UUID()
        {

        }

        inline void GetBytes(char* _cBytes) const { return m_uuid.bytes(_cBytes); }
        inline size_t Hash() const { return m_uuid.hash(); }

        friend bool operator==(const UUID &lhs, const UUID &rhs)
        {
            return lhs.m_uuid == rhs.m_uuid;
        }

        friend bool operator<(const UUID &lhs, const UUID &rhs)
        {
            return lhs.m_uuid == rhs.m_uuid;
        }

        friend bool operator!=(const UUID &lhs, const UUID &rhs)
        {
            return lhs.m_uuid != rhs.m_uuid;
        }

        friend bool operator>(const UUID &lhs, const UUID &rhs)
        {
            return lhs.m_uuid > rhs.m_uuid;
        }

        friend bool operator<=(const UUID &lhs, const UUID &rhs)
        {
            return lhs.m_uuid <= rhs.m_uuid;
        }

        friend bool operator>=(const UUID &lhs, const UUID &rhs)
        {
            return lhs.m_uuid >= rhs.m_uuid;
        }

        friend std::ostream& operator<<(std::ostream& _stream, const UUID& _uuid)
        {
            return _stream << _uuid.m_uuid.str();
        }

        friend std::istream& operator>>(std::istream& _stream, UUID& _uuid)
        {
            std::string _s;

            _stream >> _s;

            _uuid.m_uuid = UUIDv4::UUID::fromStrFactory(_s);

            return _stream;
        }

        friend UUID& operator<<(UUID& _stream, const std::string& _uuid)
        {
            _stream.m_uuid = UUIDv4::UUID(_uuid);

            return _stream;
        }

        friend const UUID& operator>>(const UUID& _stream, std::string& _uuid)
        {
            _uuid = _stream.m_uuid.bytes();

            return _stream;
        }
    };

}