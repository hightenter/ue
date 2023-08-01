// Copyright 2020-2021 CesiumGS, Inc. and Contributors

#pragma once

#include "CesiumEncodedMetadataComponent.h"
#include "CesiumGltf/Mesh.h"
#include "CesiumGltf/MeshPrimitive.h"
#include "CesiumGltf/Model.h"
#include "CesiumGltf/Node.h"
#include "LoadGltfResult.h"

// TODO: internal documentation
namespace CreateGltfOptions {
struct CreateModelOptions {
  /**
  * A pointer to the glTF model.
  */
  CesiumGltf::Model* pModel = nullptr;
  const FCesiumPrimitiveFeaturesDescription* pPrimitiveFeaturesDescription =
      nullptr;
  const FCesiumModelMetadataDescription* pEncodedMetadataDescription = nullptr;
  bool alwaysIncludeTangents = false;
  bool createPhysicsMeshes = true;
  bool ignoreKhrMaterialsUnlit = false;
};

struct CreateNodeOptions {
  const CreateModelOptions* pModelOptions = nullptr;
  const LoadGltfResult::LoadModelResult* pHalfConstructedModelResult = nullptr;
  const CesiumGltf::Node* pNode = nullptr;
};

struct CreateMeshOptions {
  const CreateNodeOptions* pNodeOptions = nullptr;
  const LoadGltfResult::LoadNodeResult* pHalfConstructedNodeResult = nullptr;
  const CesiumGltf::Mesh* pMesh = nullptr;
};

struct CreatePrimitiveOptions {
  const CreateMeshOptions* pMeshOptions = nullptr;
  const LoadGltfResult::LoadMeshResult* pHalfConstructedMeshResult = nullptr;
  const CesiumGltf::MeshPrimitive* pPrimitive = nullptr;
};
} // namespace CreateGltfOptions
