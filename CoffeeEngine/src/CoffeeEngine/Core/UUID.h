#pragma once
#include <cereal/access.hpp>
#include <cstddef>
#include <glm/fwd.hpp>

namespace Coffee {

    class UUID
    {
    public:
        inline static constexpr uint64_t null = 0;

    public:
        UUID();
        UUID(uint64_t uuid);
        UUID(const UUID&) = default;

        operator uint64_t() const { return m_UUID; }
    private:
        friend class cereal::access;

        template <class Archive>
        void serialize(Archive& archive)
        {
            archive(m_UUID);
        }
    private:
        uint64_t m_UUID;
    };

}

namespace std {
    template <typename T> struct hash;

    template<>
    struct hash<Coffee::UUID>
    {
        std::size_t operator()(const Coffee::UUID& uuid) const
        {
            return (glm::uint64_t)uuid;
        }
    };

}