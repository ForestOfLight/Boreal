#pragma once

#include <entt/entt.hpp>

struct EntityId {
    std::uint32_t rawId;

    [[nodiscard]] constexpr bool operator==(const EntityId& other) const = default;

    [[nodiscard]] constexpr operator std::uint32_t() const {
        return this->rawId;
    }
};

struct EntityIdTraits {
    using value_type = EntityId;

    using entity_type = std::uint32_t;
    using version_type = std::uint16_t;

    static constexpr entity_type entity_mask = 0x3FFFF; // lower 18 bits of raw id
    static constexpr entity_type version_mask = 0x3FFF; // upper 14 bits of raw id
};
template<>
struct entt::entt_traits<EntityId> : entt::basic_entt_traits<EntityIdTraits> {
    static constexpr std::size_t page_size = 2048;
};

struct IEntityComponent {};

template<std::derived_from<IEntityComponent> Type>
struct entt::component_traits<Type, EntityId> {
    using element_type = Type;
    using entity_type = EntityId;
    static constexpr bool in_place_delete = true;
    static constexpr std::size_t page_size = 128 * !std::is_empty_v<Type>;
};

template<typename Type>
struct entt::storage_type<Type, EntityId> {
    using type = basic_storage<Type, EntityId>;
};

struct ActorUniqueIDComponent : IEntityComponent {
    int64_t mActorUniqueID;
};

struct AbilitiesComponent : IEntityComponent {
    char _data[1];
};

struct EntityRegistry : std::enable_shared_from_this<EntityRegistry> {
    std::string name;
    entt::basic_registry<EntityId> registry;
    uint32_t id;
};

struct EntityContext {
    EntityRegistry& registry;
    entt::basic_registry<EntityId>& enttRegistry;
    EntityId entity;

    template<std::derived_from<IEntityComponent> T>
    [[nodiscard]] T* tryGetComponent() {
        return this->enttRegistry.try_get<T>(this->entity);
    }
};

class Actor {
public:
    [[nodiscard]] auto& getEntity(this auto& self) {
        return *reinterpret_cast<EntityContext*>(reinterpret_cast<std::uintptr_t>(&self) + 0x8);;
    }
};
