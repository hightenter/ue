// Copyright 2020-2023 CesiumGS, Inc. and Contributors

#include "CesiumPropertyTableProperty.h"
#include "CesiumGltf/PropertyTypeTraits.h"
#include "CesiumMetadataConversions.h"
#include <utility>

using namespace CesiumGltf;

namespace {
/**
 * Callback on a std::any, assuming that it contains a PropertyTablePropertyView
 * of the specified type. If the type does not match, the callback is performed
 * on an invalid PropertyTablePropertyView instead.
 *
 * @param property The std::any containing the property.
 * @param callback The callback function.
 *
 * @tparam TProperty The property type.
 * @tparam Normalized Whether the PropertyTablePropertyView is normalized.
 * @tparam TResult The type of the output from the callback function.
 * @tparam Callback The callback function type.
 */
template <
    typename TProperty,
    bool Normalized,
    typename TResult,
    typename Callback>
inline TResult singlePropertyTablePropertyCallback(
    const std::any& property,
    Callback&& callback) noexcept {
  const PropertyTablePropertyView<TProperty, Normalized>* pProperty =
      std::any_cast<PropertyTablePropertyView<TProperty, Normalized>>(
          &property);
  if (pProperty) {
    return callback(*pProperty);
  }

  return callback(PropertyTablePropertyView<uint8_t>());
}

/**
 * Callback on a std::any, assuming that it contains a PropertyTablePropertyView
 * on a scalar type. If the valueType does not have a valid component type, the
 * callback is performed on an invalid PropertyTablePropertyView instead.
 *
 * @param property The std::any containing the property.
 * @param valueType The FCesiumMetadataValueType of the property.
 * @param callback The callback function.
 *
 * @tparam Normalized Whether the PropertyTablePropertyView is normalized.
 * @tparam TResult The type of the output from the callback function.
 * @tparam Callback The callback function type.
 */
template <bool Normalized, typename TResult, typename Callback>
TResult scalarPropertyTablePropertyCallback(
    const std::any& property,
    const FCesiumMetadataValueType& valueType,
    Callback&& callback) noexcept {
  switch (valueType.ComponentType) {
  case ECesiumMetadataComponentType::Int8:
    return singlePropertyTablePropertyCallback<
        int8_t,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Uint8:
    return singlePropertyTablePropertyCallback<
        uint8_t,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Int16:
    return singlePropertyTablePropertyCallback<
        int16_t,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Uint16:
    return singlePropertyTablePropertyCallback<
        uint16_t,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Int32:
    return singlePropertyTablePropertyCallback<
        int32_t,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Uint32:
    return singlePropertyTablePropertyCallback<
        uint32_t,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Int64:
    return singlePropertyTablePropertyCallback<
        int64_t,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Uint64:
    return singlePropertyTablePropertyCallback<
        uint64_t,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Float32:
    return singlePropertyTablePropertyCallback<float, false, TResult, Callback>(
        property,
        std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Float64:
    return singlePropertyTablePropertyCallback<
        double,
        false,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  default:
    return callback(PropertyTablePropertyView<uint8_t>());
  }
}

/**
 * Callback on a std::any, assuming that it contains a PropertyTablePropertyView
 * on a scalar array type. If the valueType does not have a valid component
 * type, the callback is performed on an invalid PropertyTablePropertyView
 * instead.
 *
 * @param property The std::any containing the property.
 * @param valueType The FCesiumMetadataValueType of the property.
 * @param callback The callback function.
 *
 * @tparam Normalized Whether the PropertyTablePropertyView is normalized.
 * @tparam TResult The type of the output from the callback function.
 * @tparam Callback The callback function type.
 */
template <bool Normalized, typename TResult, typename Callback>
TResult scalarArrayPropertyTablePropertyCallback(
    const std::any& property,
    const FCesiumMetadataValueType& valueType,
    Callback&& callback) noexcept {
  switch (valueType.ComponentType) {
  case ECesiumMetadataComponentType::Int8:
    return singlePropertyTablePropertyCallback<
        PropertyArrayView<int8_t>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Uint8:
    return singlePropertyTablePropertyCallback<
        PropertyArrayView<uint8_t>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Int16:
    return singlePropertyTablePropertyCallback<
        PropertyArrayView<int16_t>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Uint16:
    return singlePropertyTablePropertyCallback<
        PropertyArrayView<uint16_t>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Int32:
    return singlePropertyTablePropertyCallback<
        PropertyArrayView<int32_t>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Uint32:
    return singlePropertyTablePropertyCallback<
        PropertyArrayView<uint32_t>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Int64:
    return singlePropertyTablePropertyCallback<
        PropertyArrayView<int64_t>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Uint64:
    return singlePropertyTablePropertyCallback<
        PropertyArrayView<uint64_t>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Float32:
    return singlePropertyTablePropertyCallback<
        PropertyArrayView<float>,
        false,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Float64:
    return singlePropertyTablePropertyCallback<
        PropertyArrayView<double>,
        false,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  default:
    return callback(PropertyTablePropertyView<uint8_t>());
  }
}

/**
 * Callback on a std::any, assuming that it contains a PropertyTablePropertyView
 * on a glm::vecN type. If the valueType does not have a valid component type,
 * the callback is performed on an invalid PropertyTablePropertyView instead.
 *
 * @param property The std::any containing the property.
 * @param valueType The FCesiumMetadataValueType of the property.
 * @param callback The callback function.
 *
 * @tparam N The dimensions of the glm::vecN
 * @tparam Normalized Whether the PropertyTablePropertyView is normalized.
 * @tparam TResult The type of the output from the callback function.
 * @tparam Callback The callback function type.
 */
template <glm::length_t N, bool Normalized, typename TResult, typename Callback>
TResult vecNPropertyTablePropertyCallback(
    const std::any& property,
    const FCesiumMetadataValueType& valueType,
    Callback&& callback) noexcept {
  switch (valueType.ComponentType) {
  case ECesiumMetadataComponentType::Int8:
    return singlePropertyTablePropertyCallback<
        glm::vec<N, int8_t>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Uint8:
    return singlePropertyTablePropertyCallback<
        glm::vec<N, uint8_t>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Int16:
    return singlePropertyTablePropertyCallback<
        glm::vec<N, int16_t>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Uint16:
    return singlePropertyTablePropertyCallback<
        glm::vec<N, uint16_t>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Int32:
    return singlePropertyTablePropertyCallback<
        glm::vec<N, int32_t>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Uint32:
    return singlePropertyTablePropertyCallback<
        glm::vec<N, uint32_t>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Int64:
    return singlePropertyTablePropertyCallback<
        glm::vec<N, int64_t>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Uint64:
    return singlePropertyTablePropertyCallback<
        glm::vec<N, uint64_t>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Float32:
    return singlePropertyTablePropertyCallback<
        glm::vec<N, float>,
        false,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Float64:
    return singlePropertyTablePropertyCallback<
        glm::vec<N, double>,
        false,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  default:
    return callback(PropertyTablePropertyView<uint8_t>());
  }
}

/**
 * Callback on a std::any, assuming that it contains a PropertyTablePropertyView
 * on a glm::vecN type. If the valueType does not have a valid component type,
 * the callback is performed on an invalid PropertyTablePropertyView instead.
 *
 * @param property The std::any containing the property.
 * @param valueType The FCesiumMetadataValueType of the property.
 * @param callback The callback function.
 *
 * @tparam Normalized Whether the PropertyTablePropertyView is normalized.
 * @tparam TResult The type of the output from the callback function.
 * @tparam Callback The callback function type.
 */
template <bool Normalized, typename TResult, typename Callback>
TResult vecNPropertyTablePropertyCallback(
    const std::any& property,
    const FCesiumMetadataValueType& valueType,
    Callback&& callback) noexcept {
  if (valueType.Type == ECesiumMetadataType::Vec2) {
    return vecNPropertyTablePropertyCallback<2, Normalized, TResult, Callback>(
        property,
        valueType,
        std::forward<Callback>(callback));
  }

  if (valueType.Type == ECesiumMetadataType::Vec3) {
    return vecNPropertyTablePropertyCallback<3, Normalized, TResult, Callback>(
        property,
        valueType,
        std::forward<Callback>(callback));
  }

  if (valueType.Type == ECesiumMetadataType::Vec4) {
    return vecNPropertyTablePropertyCallback<4, Normalized, TResult, Callback>(
        property,
        valueType,
        std::forward<Callback>(callback));
  }

  return callback(PropertyTablePropertyView<uint8_t>());
}

/**
 * Callback on a std::any, assuming that it contains a PropertyTablePropertyView
 * on a glm::vecN array type. If the valueType does not have a valid component
 * type, the callback is performed on an invalid PropertyTablePropertyView
 * instead.
 *
 * @param property The std::any containing the property.
 * @param valueType The FCesiumMetadataValueType of the property.
 * @param callback The callback function.
 *
 * @tparam N The dimensions of the glm::vecN
 * @tparam Normalized Whether the PropertyTablePropertyView is normalized.
 * @tparam TResult The type of the output from the callback function.
 * @tparam Callback The callback function type.
 */
template <glm::length_t N, bool Normalized, typename TResult, typename Callback>
TResult vecNArrayPropertyTablePropertyCallback(
    const std::any& property,
    const FCesiumMetadataValueType& valueType,
    Callback&& callback) noexcept {
  switch (valueType.ComponentType) {
  case ECesiumMetadataComponentType::Int8:
    return singlePropertyTablePropertyCallback<
        PropertyArrayView<glm::vec<N, int8_t>>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Uint8:
    return singlePropertyTablePropertyCallback<
        PropertyArrayView<glm::vec<N, uint8_t>>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Int16:
    return singlePropertyTablePropertyCallback<
        PropertyArrayView<glm::vec<N, int16_t>>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Uint16:
    return singlePropertyTablePropertyCallback<
        PropertyArrayView<glm::vec<N, uint16_t>>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Int32:
    return singlePropertyTablePropertyCallback<
        PropertyArrayView<glm::vec<N, int32_t>>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Uint32:
    return singlePropertyTablePropertyCallback<
        PropertyArrayView<glm::vec<N, uint32_t>>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Int64:
    return singlePropertyTablePropertyCallback<
        PropertyArrayView<glm::vec<N, int64_t>>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Uint64:
    return singlePropertyTablePropertyCallback<
        PropertyArrayView<glm::vec<N, uint64_t>>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Float32:
    return singlePropertyTablePropertyCallback<
        PropertyArrayView<glm::vec<N, float>>,
        false,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Float64:
    return singlePropertyTablePropertyCallback<
        PropertyArrayView<glm::vec<N, double>>,
        false,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  default:
    return callback(PropertyTablePropertyView<uint8_t>());
  }
}

/**
 * Callback on a std::any, assuming that it contains a PropertyTablePropertyView
 * on a glm::vecN array type. If the valueType does not have a valid component
 * type, the callback is performed on an invalid PropertyTablePropertyView
 * instead.
 *
 * @param property The std::any containing the property.
 * @param valueType The FCesiumMetadataValueType of the property.
 * @param callback The callback function.
 *
 * @tparam Normalized Whether the PropertyTablePropertyView is normalized.
 * @tparam TResult The type of the output from the callback function.
 * @tparam Callback The callback function type.
 */
template <bool Normalized, typename TResult, typename Callback>
TResult vecNArrayPropertyTablePropertyCallback(
    const std::any& property,
    const FCesiumMetadataValueType& valueType,
    Callback&& callback) noexcept {
  if (valueType.Type == ECesiumMetadataType::Vec2) {
    return vecNArrayPropertyTablePropertyCallback<
        2,
        Normalized,
        TResult,
        Callback>(property, valueType, std::forward<Callback>(callback));
  }

  if (valueType.Type == ECesiumMetadataType::Vec3) {
    return vecNArrayPropertyTablePropertyCallback<
        3,
        Normalized,
        TResult,
        Callback>(property, valueType, std::forward<Callback>(callback));
  }

  if (valueType.Type == ECesiumMetadataType::Vec4) {
    return vecNArrayPropertyTablePropertyCallback<
        4,
        Normalized,
        TResult,
        Callback>(property, valueType, std::forward<Callback>(callback));
  }

  return callback(PropertyTablePropertyView<uint8_t>());
}

/**
 * Callback on a std::any, assuming that it contains a PropertyTablePropertyView
 * on a glm::matN type. If the valueType does not have a valid component type,
 * the callback is performed on an invalid PropertyTablePropertyView instead.
 *
 * @param property The std::any containing the property.
 * @param valueType The FCesiumMetadataValueType of the property.
 * @param callback The callback function.
 *
 * @tparam N The dimensions of the glm::matN
 * @tparam TNormalized Whether the PropertyTablePropertyView is normalized.
 * @tparam TResult The type of the output from the callback function.
 * @tparam Callback The callback function type.
 */
template <glm::length_t N, bool Normalized, typename TResult, typename Callback>
TResult matNPropertyTablePropertyCallback(
    const std::any& property,
    const FCesiumMetadataValueType& valueType,
    Callback&& callback) noexcept {
  switch (valueType.ComponentType) {
  case ECesiumMetadataComponentType::Int8:
    return singlePropertyTablePropertyCallback<
        glm::mat<N, N, int8_t>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Uint8:
    return singlePropertyTablePropertyCallback<
        glm::mat<N, N, uint8_t>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Int16:
    return singlePropertyTablePropertyCallback<
        glm::mat<N, N, int16_t>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Uint16:
    return singlePropertyTablePropertyCallback<
        glm::mat<N, N, uint16_t>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Int32:
    return singlePropertyTablePropertyCallback<
        glm::mat<N, N, int32_t>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Uint32:
    return singlePropertyTablePropertyCallback<
        glm::mat<N, N, uint32_t>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Int64:
    return singlePropertyTablePropertyCallback<
        glm::mat<N, N, int64_t>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Uint64:
    return singlePropertyTablePropertyCallback<
        glm::mat<N, N, uint64_t>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Float32:
    return singlePropertyTablePropertyCallback<
        glm::mat<N, N, float>,
        false,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Float64:
    return singlePropertyTablePropertyCallback<
        glm::mat<N, N, double>,
        false,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  default:
    return callback(PropertyTablePropertyView<uint8_t>());
  }
}

/**
 * Callback on a std::any, assuming that it contains a PropertyTablePropertyView
 * on a glm::matN type. If the valueType does not have a valid component type,
 * the callback is performed on an invalid PropertyTablePropertyView instead.
 *
 * @param property The std::any containing the property.
 * @param valueType The FCesiumMetadataValueType of the property.
 * @param callback The callback function.
 *
 * @tparam Normalized Whether the PropertyTablePropertyView is normalized.
 * @tparam TResult The type of the output from the callback function.
 * @tparam Callback The callback function type.
 */
template <bool Normalized, typename TResult, typename Callback>
TResult matNPropertyTablePropertyCallback(
    const std::any& property,
    const FCesiumMetadataValueType& valueType,
    Callback&& callback) noexcept {
  if (valueType.Type == ECesiumMetadataType::Mat2) {
    return matNPropertyTablePropertyCallback<2, Normalized, TResult, Callback>(
        property,
        valueType,
        std::forward<Callback>(callback));
  }

  if (valueType.Type == ECesiumMetadataType::Mat3) {
    return matNPropertyTablePropertyCallback<3, Normalized, TResult, Callback>(
        property,
        valueType,
        std::forward<Callback>(callback));
  }

  if (valueType.Type == ECesiumMetadataType::Mat4) {
    return matNPropertyTablePropertyCallback<4, Normalized, TResult, Callback>(
        property,
        valueType,
        std::forward<Callback>(callback));
  }

  return callback(PropertyTablePropertyView<uint8>());
}

/**
 * Callback on a std::any, assuming that it contains a PropertyTablePropertyView
 * on a glm::matN array type. If the valueType does not have a valid component
 * type, the callback is performed on an invalid PropertyTablePropertyView
 * instead.
 *
 * @param property The std::any containing the property.
 * @param valueType The FCesiumMetadataValueType of the property.
 * @param callback The callback function.
 *
 * @tparam N The dimensions of the glm::matN
 * @tparam TNormalized Whether the PropertyTablePropertyView is normalized.
 * @tparam TResult The type of the output from the callback function.
 * @tparam Callback The callback function type.
 */
template <glm::length_t N, bool Normalized, typename TResult, typename Callback>
TResult matNArrayPropertyTablePropertyCallback(
    const std::any& property,
    const FCesiumMetadataValueType& valueType,
    Callback&& callback) noexcept {
  switch (valueType.ComponentType) {
  case ECesiumMetadataComponentType::Int8:
    return singlePropertyTablePropertyCallback<
        PropertyArrayView<glm::mat<N, N, int8_t>>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Uint8:
    return singlePropertyTablePropertyCallback<
        PropertyArrayView<glm::mat<N, N, uint8_t>>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Int16:
    return singlePropertyTablePropertyCallback<
        PropertyArrayView<glm::mat<N, N, int16_t>>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Uint16:
    return singlePropertyTablePropertyCallback<
        PropertyArrayView<glm::mat<N, N, uint16_t>>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Int32:
    return singlePropertyTablePropertyCallback<
        PropertyArrayView<glm::mat<N, N, int32_t>>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Uint32:
    return singlePropertyTablePropertyCallback<
        PropertyArrayView<glm::mat<N, N, uint32_t>>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Int64:
    return singlePropertyTablePropertyCallback<
        PropertyArrayView<glm::mat<N, N, int64_t>>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Uint64:
    return singlePropertyTablePropertyCallback<
        PropertyArrayView<glm::mat<N, N, uint64_t>>,
        Normalized,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Float32:
    return singlePropertyTablePropertyCallback<
        PropertyArrayView<glm::mat<N, N, float>>,
        false,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataComponentType::Float64:
    return singlePropertyTablePropertyCallback<
        PropertyArrayView<glm::mat<N, N, double>>,
        false,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  default:
    return callback(PropertyTablePropertyView<uint8_t>());
  }
}

/**
 * Callback on a std::any, assuming that it contains a PropertyTablePropertyView
 * on a glm::matN array type. If the valueType does not have a valid component
 * type, the callback is performed on an invalid PropertyTablePropertyView
 * instead.
 *
 * @param property The std::any containing the property.
 * @param valueType The FCesiumMetadataValueType of the property.
 * @param callback The callback function.
 *
 * @tparam Normalized Whether the PropertyTablePropertyView is normalized.
 * @tparam TResult The type of the output from the callback function.
 * @tparam Callback The callback function type.
 */
template <bool Normalized, typename TResult, typename Callback>
TResult matNArrayPropertyTablePropertyCallback(
    const std::any& property,
    const FCesiumMetadataValueType& valueType,
    Callback&& callback) noexcept {
  if (valueType.Type == ECesiumMetadataType::Mat2) {
    return matNArrayPropertyTablePropertyCallback<
        2,
        Normalized,
        TResult,
        Callback>(property, valueType, std::forward<Callback>(callback));
  }

  if (valueType.Type == ECesiumMetadataType::Mat3) {
    return matNArrayPropertyTablePropertyCallback<
        3,
        Normalized,
        TResult,
        Callback>(property, valueType, std::forward<Callback>(callback));
  }

  if (valueType.Type == ECesiumMetadataType::Mat4) {
    return matNArrayPropertyTablePropertyCallback<
        4,
        Normalized,
        TResult,
        Callback>(property, valueType, std::forward<Callback>(callback));
  }

  return callback(PropertyTablePropertyView<uint8_t>());
}

template <bool Normalized, typename TResult, typename Callback>
TResult arrayPropertyTablePropertyCallback(
    const std::any& property,
    const FCesiumMetadataValueType& valueType,
    Callback&& callback) noexcept {
  switch (valueType.Type) {
  case ECesiumMetadataType::Scalar:
    return scalarArrayPropertyTablePropertyCallback<
        Normalized,
        TResult,
        Callback>(property, valueType, std::forward<Callback>(callback));
  case ECesiumMetadataType::Vec2:
  case ECesiumMetadataType::Vec3:
  case ECesiumMetadataType::Vec4:
    return vecNArrayPropertyTablePropertyCallback<
        Normalized,
        TResult,
        Callback>(property, valueType, std::forward<Callback>(callback));
  case ECesiumMetadataType::Mat2:
  case ECesiumMetadataType::Mat3:
  case ECesiumMetadataType::Mat4:
    return matNArrayPropertyTablePropertyCallback<
        Normalized,
        TResult,
        Callback>(property, valueType, std::forward<Callback>(callback));
  case ECesiumMetadataType::Boolean:
    return singlePropertyTablePropertyCallback<
        PropertyArrayView<bool>,
        false,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  case ECesiumMetadataType::String:
    return singlePropertyTablePropertyCallback<
        PropertyArrayView<std::string_view>,
        false,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  default:
    return callback(PropertyTablePropertyView<uint8_t>());
  }
}

template <typename TResult, typename Callback>
TResult propertyTablePropertyCallback(
    const std::any& property,
    const FCesiumMetadataValueType& valueType,
    bool normalized,
    Callback&& callback) noexcept {
  if (valueType.bIsArray) {
    return normalized
               ? arrayPropertyTablePropertyCallback<true, TResult, Callback>(
                     property,
                     valueType,
                     std::forward<Callback>(callback))
               : arrayPropertyTablePropertyCallback<false, TResult, Callback>(
                     property,
                     valueType,
                     std::forward<Callback>(callback));
  }

  switch (valueType.Type) {
  case ECesiumMetadataType::Scalar:
    return normalized
               ? scalarPropertyTablePropertyCallback<true, TResult, Callback>(
                     property,
                     valueType,
                     std::forward<Callback>(callback))
               : scalarPropertyTablePropertyCallback<false, TResult, Callback>(
                     property,
                     valueType,
                     std::forward<Callback>(callback));
  case ECesiumMetadataType::Vec2:
  case ECesiumMetadataType::Vec3:
  case ECesiumMetadataType::Vec4:
    return normalized
               ? vecNPropertyTablePropertyCallback<true, TResult, Callback>(
                     property,
                     valueType,
                     std::forward<Callback>(callback))
               : vecNPropertyTablePropertyCallback<false, TResult, Callback>(
                     property,
                     valueType,
                     std::forward<Callback>(callback));
  case ECesiumMetadataType::Mat2:
  case ECesiumMetadataType::Mat3:
  case ECesiumMetadataType::Mat4:
    return normalized
               ? matNPropertyTablePropertyCallback<true, TResult, Callback>(
                     property,
                     valueType,
                     std::forward<Callback>(callback))
               : matNPropertyTablePropertyCallback<false, TResult, Callback>(
                     property,
                     valueType,
                     std::forward<Callback>(callback));
  case ECesiumMetadataType::Boolean:
    return singlePropertyTablePropertyCallback<bool, false, TResult, Callback>(
        property,
        std::forward<Callback>(callback));
  case ECesiumMetadataType::String:
    return singlePropertyTablePropertyCallback<
        std::string_view,
        false,
        TResult,
        Callback>(property, std::forward<Callback>(callback));
  default:
    return callback(PropertyTablePropertyView<uint8_t>());
  }
}

} // namespace

ECesiumPropertyTablePropertyStatus
UCesiumPropertyTablePropertyBlueprintLibrary::GetPropertyTablePropertyStatus(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property) {
  return Property._status;
}

ECesiumMetadataBlueprintType
UCesiumPropertyTablePropertyBlueprintLibrary::GetBlueprintType(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property) {
  return CesiumMetadataValueTypeToBlueprintType(Property._valueType);
}

ECesiumMetadataBlueprintType
UCesiumPropertyTablePropertyBlueprintLibrary::GetArrayElementBlueprintType(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property) {
  if (!Property._valueType.bIsArray) {
    return ECesiumMetadataBlueprintType::None;
  }

  FCesiumMetadataValueType valueType(Property._valueType);
  valueType.bIsArray = false;

  return CesiumMetadataValueTypeToBlueprintType(valueType);
}

FCesiumMetadataValueType
UCesiumPropertyTablePropertyBlueprintLibrary::GetValueType(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property) {
  return Property._valueType;
}

int64 UCesiumPropertyTablePropertyBlueprintLibrary::GetPropertySize(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property) {
  return propertyTablePropertyCallback<int64>(
      Property._property,
      Property._valueType,
      Property._normalized,
      [](const auto& view) noexcept -> int64 { return view.size(); });
}

int64 UCesiumPropertyTablePropertyBlueprintLibrary::GetArraySize(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property) {
  return propertyTablePropertyCallback<int64>(
      Property._property,
      Property._valueType,
      Property._normalized,
      [](const auto& view) noexcept -> int64 { return view.arrayCount(); });
}

namespace {

template <typename TTo> struct Convertifier {
  int64 FeatureID;
  const TTo& DefaultValue;

  template <typename TFrom, bool Normalized = false>
  TTo operator()(
      const PropertyTablePropertyView<TFrom, Normalized>& v) noexcept {
    // size() returns zero if the view is invalid.
    if (FeatureID < 0 || FeatureID >= v.size()) {
      return DefaultValue;
    }
    auto maybeValue = v.get(FeatureID);
    if (maybeValue) {
      return CesiumMetadataConversions<
          TTo,
          std::remove_reference_t<std::remove_cv_t<decltype((*maybeValue))>>>::
          convert(*maybeValue, DefaultValue);
    }
    return DefaultValue;
  }
};

} // namespace

bool UCesiumPropertyTablePropertyBlueprintLibrary::GetBoolean(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property,
    int64 FeatureID,
    bool DefaultValue) {
  return propertyTablePropertyCallback<bool>(
      Property._property,
      Property._valueType,
      Property._normalized,
      Convertifier<bool>{FeatureID, DefaultValue});
}

uint8 UCesiumPropertyTablePropertyBlueprintLibrary::GetByte(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property,
    int64 FeatureID,
    uint8 DefaultValue) {
  return propertyTablePropertyCallback<uint8_t>(
      Property._property,
      Property._valueType,
      Property._normalized,
      Convertifier<uint8>{FeatureID, DefaultValue});
}

int32 UCesiumPropertyTablePropertyBlueprintLibrary::GetInteger(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property,
    int64 FeatureID,
    int32 DefaultValue) {
  return propertyTablePropertyCallback<int32>(
      Property._property,
      Property._valueType,
      Property._normalized,
      Convertifier<int32>{FeatureID, DefaultValue});
}

int64 UCesiumPropertyTablePropertyBlueprintLibrary::GetInteger64(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property,
    int64 FeatureID,
    int64 DefaultValue) {
  return propertyTablePropertyCallback<int64>(
      Property._property,
      Property._valueType,
      Property._normalized,
      Convertifier<int64>{FeatureID, DefaultValue});
}

float UCesiumPropertyTablePropertyBlueprintLibrary::GetFloat(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property,
    int64 FeatureID,
    float DefaultValue) {
  return propertyTablePropertyCallback<float>(
      Property._property,
      Property._valueType,
      Property._normalized,
      Convertifier<float>{FeatureID, DefaultValue});
}

double UCesiumPropertyTablePropertyBlueprintLibrary::GetFloat64(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property,
    int64 FeatureID,
    double DefaultValue) {
  return propertyTablePropertyCallback<double>(
      Property._property,
      Property._valueType,
      Property._normalized,
      Convertifier<double>{FeatureID, DefaultValue});
}

FIntPoint UCesiumPropertyTablePropertyBlueprintLibrary::GetIntPoint(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property,
    int64 FeatureID,
    const FIntPoint& DefaultValue) {
  return propertyTablePropertyCallback<FIntPoint>(
      Property._property,
      Property._valueType,
      Property._normalized,
      Convertifier<FIntPoint>{FeatureID, DefaultValue});
}

FVector2D UCesiumPropertyTablePropertyBlueprintLibrary::GetVector2D(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property,
    int64 FeatureID,
    const FVector2D& DefaultValue) {
  return propertyTablePropertyCallback<FVector2D>(
      Property._property,
      Property._valueType,
      Property._normalized,
      Convertifier<FVector2D>{FeatureID, DefaultValue});
}

FIntVector UCesiumPropertyTablePropertyBlueprintLibrary::GetIntVector(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property,
    int64 FeatureID,
    const FIntVector& DefaultValue) {
  return propertyTablePropertyCallback<FIntVector>(
      Property._property,
      Property._valueType,
      Property._normalized,
      Convertifier<FIntVector>{FeatureID, DefaultValue});
}

FVector3f UCesiumPropertyTablePropertyBlueprintLibrary::GetVector3f(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property,
    int64 FeatureID,
    const FVector3f& DefaultValue) {
  return propertyTablePropertyCallback<FVector3f>(
      Property._property,
      Property._valueType,
      Property._normalized,
      Convertifier<FVector3f>{FeatureID, DefaultValue});
}

FVector UCesiumPropertyTablePropertyBlueprintLibrary::GetVector(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property,
    int64 FeatureID,
    const FVector& DefaultValue) {
  return propertyTablePropertyCallback<FVector>(
      Property._property,
      Property._valueType,
      Property._normalized,
      Convertifier<FVector>{FeatureID, DefaultValue});
}

FVector4 UCesiumPropertyTablePropertyBlueprintLibrary::GetVector4(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property,
    int64 FeatureID,
    const FVector4& DefaultValue) {
  return propertyTablePropertyCallback<FVector4>(
      Property._property,
      Property._valueType,
      Property._normalized,
      Convertifier<FVector4>{FeatureID, DefaultValue});
}

FMatrix UCesiumPropertyTablePropertyBlueprintLibrary::GetMatrix(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property,
    int64 FeatureID,
    const FMatrix& DefaultValue) {
  return propertyTablePropertyCallback<FMatrix>(
      Property._property,
      Property._valueType,
      Property._normalized,
      Convertifier<FMatrix>{FeatureID, DefaultValue});
}

FString UCesiumPropertyTablePropertyBlueprintLibrary::GetString(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property,
    int64 FeatureID,
    const FString& DefaultValue) {
  return propertyTablePropertyCallback<FString>(
      Property._property,
      Property._valueType,
      Property._normalized,
      Convertifier<FString>{FeatureID, DefaultValue});
}

FCesiumPropertyArray UCesiumPropertyTablePropertyBlueprintLibrary::GetArray(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property,
    int64 FeatureID) {
  return propertyTablePropertyCallback<FCesiumPropertyArray>(
      Property._property,
      Property._valueType,
      Property._normalized,
      [FeatureID](const auto& v) -> FCesiumPropertyArray {
        // size() returns zero if the view is invalid.
        if (FeatureID < 0 || FeatureID >= v.size()) {
          return FCesiumPropertyArray();
        }
        auto maybeValue = v.get(FeatureID);
        if (maybeValue) {
          auto value = *maybeValue;
          if constexpr (CesiumGltf::IsMetadataArray<decltype(value)>::value) {
            return FCesiumPropertyArray(std::move(value));
          }
        }
        return FCesiumPropertyArray();
      });
}

FCesiumMetadataValue UCesiumPropertyTablePropertyBlueprintLibrary::GetValue(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property,
    int64 FeatureID) {
  return propertyTablePropertyCallback<FCesiumMetadataValue>(
      Property._property,
      Property._valueType,
      Property._normalized,
      [FeatureID](const auto& view) -> FCesiumMetadataValue {
        // size() returns zero if the view is invalid.
        if (FeatureID >= 0 && FeatureID < view.size()) {
          return FCesiumMetadataValue(view.get(FeatureID));
        }
        return FCesiumMetadataValue();
      });
}

FCesiumMetadataValue UCesiumPropertyTablePropertyBlueprintLibrary::GetRawValue(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property,
    int64 FeatureID) {
  return propertyTablePropertyCallback<FCesiumMetadataValue>(
      Property._property,
      Property._valueType,
      Property._normalized,
      [FeatureID](const auto& view) -> FCesiumMetadataValue {
        // Return an empty value if the property is empty.
        if (view.status() ==
            PropertyTablePropertyViewStatus::EmptyPropertyWithDefault) {
          return FCesiumMetadataValue();
        }

        // size() returns zero if the view is invalid.
        if (FeatureID >= 0 && FeatureID < view.size()) {
          return FCesiumMetadataValue(view.getRaw(FeatureID));
        }

        return FCesiumMetadataValue();
      });
}

bool UCesiumPropertyTablePropertyBlueprintLibrary::IsNormalized(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property) {
  return Property._normalized;
}

FCesiumMetadataValue UCesiumPropertyTablePropertyBlueprintLibrary::GetOffset(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property) {
  return propertyTablePropertyCallback<FCesiumMetadataValue>(
      Property._property,
      Property._valueType,
      Property._normalized,
      [](const auto& view) -> FCesiumMetadataValue {
        // Returns an empty value if no offset is specified.
        return FCesiumMetadataValue(view.offset());
      });
}

FCesiumMetadataValue UCesiumPropertyTablePropertyBlueprintLibrary::GetScale(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property) {
  return propertyTablePropertyCallback<FCesiumMetadataValue>(
      Property._property,
      Property._valueType,
      Property._normalized,
      [](const auto& view) -> FCesiumMetadataValue {
        // Returns an empty value if no scale is specified.
        return FCesiumMetadataValue(view.scale());
      });
}

FCesiumMetadataValue
UCesiumPropertyTablePropertyBlueprintLibrary::GetMinimumValue(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property) {
  return propertyTablePropertyCallback<FCesiumMetadataValue>(
      Property._property,
      Property._valueType,
      Property._normalized,
      [](const auto& view) -> FCesiumMetadataValue {
        // Returns an empty value if no min is specified.
        return FCesiumMetadataValue(view.min());
      });
}

FCesiumMetadataValue
UCesiumPropertyTablePropertyBlueprintLibrary::GetMaximumValue(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property) {
  return propertyTablePropertyCallback<FCesiumMetadataValue>(
      Property._property,
      Property._valueType,
      Property._normalized,
      [](const auto& view) -> FCesiumMetadataValue {
        // Returns an empty value if no max is specified.
        return FCesiumMetadataValue(view.max());
      });
}

FCesiumMetadataValue
UCesiumPropertyTablePropertyBlueprintLibrary::GetNoDataValue(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property) {
  return propertyTablePropertyCallback<FCesiumMetadataValue>(
      Property._property,
      Property._valueType,
      Property._normalized,
      [](const auto& view) -> FCesiumMetadataValue {
        // Returns an empty value if no "no data" value is specified.
        return FCesiumMetadataValue(view.noData());
      });
}

FCesiumMetadataValue
UCesiumPropertyTablePropertyBlueprintLibrary::GetDefaultValue(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property) {
  return propertyTablePropertyCallback<FCesiumMetadataValue>(
      Property._property,
      Property._valueType,
      Property._normalized,
      [](const auto& view) -> FCesiumMetadataValue {
        // Returns an empty value if no default value is specified.
        return FCesiumMetadataValue(view.defaultValue());
      });
}

PRAGMA_DISABLE_DEPRECATION_WARNINGS

ECesiumMetadataBlueprintType
UCesiumPropertyTablePropertyBlueprintLibrary::GetBlueprintComponentType(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property) {
  return UCesiumPropertyTablePropertyBlueprintLibrary::
      GetArrayElementBlueprintType(Property);
}

ECesiumMetadataTrueType_DEPRECATED
UCesiumPropertyTablePropertyBlueprintLibrary::GetTrueType(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property) {
  return CesiumMetadataValueTypeToTrueType(Property._valueType);
}

ECesiumMetadataTrueType_DEPRECATED
UCesiumPropertyTablePropertyBlueprintLibrary::GetTrueComponentType(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property) {
  FCesiumMetadataValueType type = Property._valueType;
  type.bIsArray = false;
  return CesiumMetadataValueTypeToTrueType(type);
}

int64 UCesiumPropertyTablePropertyBlueprintLibrary::GetNumberOfFeatures(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property) {
  return UCesiumPropertyTablePropertyBlueprintLibrary::GetPropertySize(
      Property);
}

int64 UCesiumPropertyTablePropertyBlueprintLibrary::GetComponentCount(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property) {
  return UCesiumPropertyTablePropertyBlueprintLibrary::GetArraySize(Property);
}

FCesiumMetadataValue
UCesiumPropertyTablePropertyBlueprintLibrary::GetGenericValue(
    UPARAM(ref) const FCesiumPropertyTableProperty& Property,
    int64 FeatureID) {
  return UCesiumPropertyTablePropertyBlueprintLibrary::GetValue(
      Property,
      FeatureID);
}

PRAGMA_ENABLE_DEPRECATION_WARNINGS
