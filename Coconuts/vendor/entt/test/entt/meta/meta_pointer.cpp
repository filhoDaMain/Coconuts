#include <memory>
#include <type_traits>
#include <gtest/gtest.h>
#include <entt/core/hashed_string.hpp>
#include <entt/meta/meta.hpp>
#include <entt/meta/pointer.hpp>
#include <entt/meta/resolve.hpp>

template<typename Type>
struct wrapped_shared_ptr {
    wrapped_shared_ptr(Type init): ptr{new Type {init}} {}

    Type & deref() const { return *ptr; }

private:
    std::shared_ptr<Type> ptr;
};

template<typename Type>
struct adl_wrapped_shared_ptr: wrapped_shared_ptr<Type> {};

template<typename Type>
struct spec_wrapped_shared_ptr: wrapped_shared_ptr<Type> {};

template<typename Type>
struct entt::is_meta_pointer_like<adl_wrapped_shared_ptr<Type>>: std::true_type {};

template<typename Type>
struct entt::is_meta_pointer_like<spec_wrapped_shared_ptr<Type>>: std::true_type {};

template<typename Type>
Type & dereference_meta_pointer_like(const adl_wrapped_shared_ptr<Type> &ptr) {
    return ptr.deref();
}

template<typename Type>
struct entt::adl_meta_pointer_like<spec_wrapped_shared_ptr<Type>> {
    static decltype(auto) dereference(const spec_wrapped_shared_ptr<Type> &ptr) {
        return ptr.deref();
    }
};

struct not_copyable_t {
    not_copyable_t() = default;
    not_copyable_t(const not_copyable_t &) = delete;
    not_copyable_t(not_copyable_t &&) = default;
    not_copyable_t & operator=(const not_copyable_t &) = delete;
    not_copyable_t & operator=(not_copyable_t &&) = default;
};

TEST(MetaPointerLike, DereferenceOperatorInvalidType) {
    int value = 0;
    entt::meta_any any{value};

    ASSERT_FALSE(any.type().is_pointer());
    ASSERT_FALSE(any.type().is_pointer_like());
    ASSERT_EQ(any.type(), entt::resolve<int>());

    auto deref = *any;

    ASSERT_FALSE(deref);
}

TEST(MetaPointerLike, DereferenceOperatorConstType) {
    const int value = 42;
    entt::meta_any any{&value};

    ASSERT_TRUE(any.type().is_pointer());
    ASSERT_TRUE(any.type().is_pointer_like());
    ASSERT_EQ(any.type(), entt::resolve<const int *>());

    auto deref = *any;

    ASSERT_TRUE(deref);
    ASSERT_FALSE(deref.type().is_pointer());
    ASSERT_FALSE(deref.type().is_pointer_like());
    ASSERT_EQ(deref.type(), entt::resolve<int>());

    ASSERT_EQ(deref.try_cast<int>(), nullptr);
    ASSERT_EQ(deref.try_cast<const int>(), &value);
    ASSERT_DEATH(deref.cast<int &>() = 0, ".*");
    ASSERT_EQ(deref.cast<const int &>(), 42);
}

TEST(MetaPointerLike, DereferenceOperatorConstAny) {
    auto test = [](const entt::meta_any any) {
        auto deref = *any;

        ASSERT_TRUE(deref);
        ASSERT_FALSE(deref.type().is_pointer());
        ASSERT_FALSE(deref.type().is_pointer_like());
        ASSERT_EQ(deref.type(), entt::resolve<int>());

        ASSERT_EQ(deref.try_cast<int>(), nullptr);
        ASSERT_NE(deref.try_cast<const int>(), nullptr);
        ASSERT_DEATH(deref.cast<int &>() = 0, ".*");
        ASSERT_EQ(deref.cast<const int &>(), 42);
    };

    int value = 42;

    test(&value);
    test(&std::as_const(value));
}

TEST(MetaPointerLike, DereferenceOperatorRawPointer) {
    int value = 0;
    entt::meta_any any{&value};

    ASSERT_TRUE(any.type().is_pointer());
    ASSERT_TRUE(any.type().is_pointer_like());
    ASSERT_EQ(any.type(), entt::resolve<int *>());

    auto deref = *any;

    ASSERT_TRUE(deref);
    ASSERT_FALSE(deref.type().is_pointer());
    ASSERT_FALSE(deref.type().is_pointer_like());
    ASSERT_EQ(deref.type(), entt::resolve<int>());

    deref.cast<int &>() = 42;

    ASSERT_EQ(*any.cast<int *>(), 42);
    ASSERT_EQ(value, 42);
}

TEST(MetaPointerLike, DereferenceOperatorSmartPointer) {
    auto value = std::make_shared<int>(0);
    entt::meta_any any{value};

    ASSERT_FALSE(any.type().is_pointer());
    ASSERT_TRUE(any.type().is_pointer_like());
    ASSERT_EQ(any.type(), entt::resolve<std::shared_ptr<int>>());

    auto deref = *any;

    ASSERT_TRUE(deref);
    ASSERT_FALSE(deref.type().is_pointer());
    ASSERT_FALSE(deref.type().is_pointer_like());
    ASSERT_EQ(deref.type(), entt::resolve<int>());

    deref.cast<int &>() = 42;

    ASSERT_EQ(*any.cast<std::shared_ptr<int>>(), 42);
    ASSERT_EQ(*value, 42);
}

TEST(MetaPointerLike, PointerToConstMoveOnlyType) {
    const not_copyable_t instance;
    entt::meta_any any{&instance};
    auto deref = *any;

    ASSERT_TRUE(any);
    ASSERT_TRUE(deref);

    ASSERT_DEATH(deref.cast<not_copyable_t &>() = {}, ".*");
    ASSERT_EQ(deref.try_cast<not_copyable_t>(), nullptr);
    ASSERT_NE(deref.try_cast<const not_copyable_t>(), nullptr);
}

TEST(MetaPointerLike, AsRef) {
    int value = 0;
    int * ptr = &value;
    entt::meta_any any{std::ref(ptr)};

    ASSERT_TRUE(any.type().is_pointer());
    ASSERT_TRUE(any.type().is_pointer_like());
    ASSERT_EQ(any.type(), entt::resolve<int *>());

    auto deref = *any;

    ASSERT_TRUE(deref);
    ASSERT_FALSE(deref.type().is_pointer());
    ASSERT_FALSE(deref.type().is_pointer_like());
    ASSERT_EQ(deref.type(), entt::resolve<int>());

    deref.cast<int &>() = 42;

    ASSERT_EQ(*any.cast<int *>(), 42);
    ASSERT_EQ(value, 42);
}

TEST(MetaPointerLike, AsConstRef) {
    int value = 42;
    int * ptr = &value;
    entt::meta_any any{std::cref(ptr)};

    ASSERT_TRUE(any.type().is_pointer());
    ASSERT_TRUE(any.type().is_pointer_like());
    ASSERT_EQ(any.type(), entt::resolve<int *>());

    auto deref = *any;

    ASSERT_TRUE(deref);
    ASSERT_FALSE(deref.type().is_pointer());
    ASSERT_FALSE(deref.type().is_pointer_like());
    ASSERT_EQ(deref.type(), entt::resolve<int>());

    deref.cast<int &>() = 42;

    ASSERT_EQ(*any.cast<int *>(), 42);
    ASSERT_EQ(value, 42);
}

TEST(MetaPointerLike, DereferenceMetaPointerLikeOverload) {
    auto test = [](entt::meta_any any) {
        ASSERT_FALSE(any.type().is_pointer());
        ASSERT_TRUE(any.type().is_pointer_like());

        auto deref = *any;

        ASSERT_TRUE(deref);
        ASSERT_FALSE(deref.type().is_pointer());
        ASSERT_FALSE(deref.type().is_pointer_like());
        ASSERT_EQ(deref.type(), entt::resolve<int>());

        ASSERT_EQ(deref.cast<int &>(), 42);
        ASSERT_EQ(deref.cast<const int &>(), 42);
    };

    test(adl_wrapped_shared_ptr<int>{42});
    test(spec_wrapped_shared_ptr<int>{42});
}

TEST(MetaPointerLike, DereferenceMetaPointerToConstLikeOverload) {
    auto test = [](entt::meta_any any) {
        ASSERT_FALSE(any.type().is_pointer());
        ASSERT_TRUE(any.type().is_pointer_like());

        auto deref = *any;

        ASSERT_TRUE(deref);
        ASSERT_FALSE(deref.type().is_pointer());
        ASSERT_FALSE(deref.type().is_pointer_like());
        ASSERT_EQ(deref.type(), entt::resolve<int>());

        ASSERT_DEATH(deref.cast<int &>() = 42, ".*");
        ASSERT_EQ(deref.cast<const int &>(), 42);
    };

    test(adl_wrapped_shared_ptr<const int>{42});
    test(spec_wrapped_shared_ptr<const int>{42});
}
