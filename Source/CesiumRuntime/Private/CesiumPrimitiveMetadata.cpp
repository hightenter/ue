// Copyright 2020-2021 CesiumGS, Inc. and Contributors

#include "CesiumPrimitiveMetadata.h"
#include "CesiumGltf/ExtensionMeshPrimitiveExtStructuralMetadata.h"
#include "CesiumGltf/Model.h"

FCesiumPrimitiveMetadata::FCesiumPrimitiveMetadata(
    const CesiumGltf::Model& InModel,
    const CesiumGltf::MeshPrimitive& Primitive,
    const CesiumGltf::ExtensionMeshPrimitiveExtStructuralMetadata& Metadata)
    : _propertyTextureIndices(), _propertyAttributeIndices() {
  this->_propertyTextureIndices.Reserve(Metadata.propertyTextures.size());
  for (const int64 propertyTextureIndex : Metadata.propertyTextures) {
    this->_propertyTextureIndices.Emplace(
        static_cast<int64>(propertyTextureIndex));
  }

  this->_propertyAttributeIndices.Reserve(Metadata.propertyAttributes.size());
  for (const int64 propertyAttributeIndex : Metadata.propertyAttributes) {
    this->_propertyAttributeIndices.Emplace(
        static_cast<int64>(propertyAttributeIndex));
  }
}

const TArray<int64>&
UCesiumPrimitiveMetadataBlueprintLibrary::GetPropertyTextureIndices(
    UPARAM(ref) const FCesiumPrimitiveMetadata& PrimitiveMetadata) {
  return PrimitiveMetadata._propertyTextureIndices;
}

const TArray<int64>&
UCesiumPrimitiveMetadataBlueprintLibrary::GetPropertyAttributeIndices(
    UPARAM(ref) const FCesiumPrimitiveMetadata& PrimitiveMetadata) {
  return PrimitiveMetadata._propertyAttributeIndices;
}