// Copyright (C) 2002-2009 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "CWaterSurfaceSceneNode.h"
#include "ISceneManager.h"
#include "IMeshManipulator.h"
#include "IMeshCache.h"
#include "S3DVertex.h"
#include "SMesh.h"
#include "os.h"

namespace irr
{
namespace scene
{

//! constructor
CWaterSurfaceSceneNode::CWaterSurfaceSceneNode(f32 waveHeight, f32 waveSpeed, f32 waveLength, 
		IMesh* mesh, ISceneNode* parent, ISceneManager* mgr, s32 id,
		const core::vector3df& position, const core::vector3df& rotation,
		const core::vector3df& scale)
	: CMeshSceneNode(mesh, parent, mgr, id, position, rotation, scale),
	WaveLength(waveLength), WaveSpeed(waveSpeed), WaveHeight(waveHeight),
	OriginalMesh(0)
{
	#ifdef _DEBUG
	setDebugName("CWaterSurfaceSceneNode");
	#endif

	// create copy of the mesh
	if (!mesh)
		return;

	// Mesh is set in CMeshSceneNode constructor, now it is moved to OriginalMesh
	IMesh* clone = SceneManager->getMeshManipulator()->createMeshCopy(mesh);
	OriginalMesh = Mesh;
	Mesh = clone;
}


//! destructor
CWaterSurfaceSceneNode::~CWaterSurfaceSceneNode()
{
	// Mesh is dropped in CMeshSceneNode destructor
	if (OriginalMesh)
		OriginalMesh->drop();
}


//! frame
void CWaterSurfaceSceneNode::OnRegisterSceneNode()
{
	CMeshSceneNode::OnRegisterSceneNode();
}


void CWaterSurfaceSceneNode::OnAnimate(u32 timeMs)
{
	if (Mesh && IsVisible)
	{
		const u32 meshBufferCount = Mesh->getMeshBufferCount();
		const f32 time = timeMs / WaveSpeed;

		for (u32 b=0; b<meshBufferCount; ++b)
		{
			const u32 vtxCnt = Mesh->getMeshBuffer(b)->getVertexCount();

			for (u32 i=0; i<vtxCnt; ++i)
				addWave(Mesh->getMeshBuffer(b)->getPosition(i),
					OriginalMesh->getMeshBuffer(b)->getPosition(i),
					time);
		}// end for all mesh buffers

		SceneManager->getMeshManipulator()->recalculateNormals(Mesh);
	}
	CMeshSceneNode::OnAnimate(timeMs);
}


void CWaterSurfaceSceneNode::setMesh(IMesh* mesh)
{
	CMeshSceneNode::setMesh(mesh);
	if (!mesh)
		return;
	if (OriginalMesh)
		OriginalMesh->drop();
	IMesh* clone = SceneManager->getMeshManipulator()->createMeshCopy(mesh);
	OriginalMesh = mesh;
	Mesh = clone;
}


//! Writes attributes of the scene node.
void CWaterSurfaceSceneNode::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
{
	out->addFloat("WaveLength", WaveLength);
	out->addFloat("WaveSpeed",  WaveSpeed);
	out->addFloat("WaveHeight", WaveHeight);
	
	CMeshSceneNode::serializeAttributes(out, options);
	// serialize original mesh
	out->setAttribute("Mesh", SceneManager->getMeshCache()->getMeshFilename(OriginalMesh));
}


//! Reads attributes of the scene node.
void CWaterSurfaceSceneNode::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
{
	WaveLength = in->getAttributeAsFloat("WaveLength");
	WaveSpeed  = in->getAttributeAsFloat("WaveSpeed");
	WaveHeight = in->getAttributeAsFloat("WaveHeight");
	
	if (Mesh)
	{
		Mesh->drop();
		Mesh = OriginalMesh;
		OriginalMesh = 0;
	}
	// deserialize original mesh
	CMeshSceneNode::deserializeAttributes(in, options);

	if (Mesh)
	{
		IMesh* clone = SceneManager->getMeshManipulator()->createMeshCopy(Mesh);
		OriginalMesh = Mesh;
		Mesh = clone;
	}
}


void CWaterSurfaceSceneNode::addWave(core::vector3df& dest, const core::vector3df &source, f32 time) const
{
	dest.Y = source.Y +
	(sinf(((source.X/WaveLength) + time)) * WaveHeight) +
	(cosf(((source.Z/WaveLength) + time)) * WaveHeight);
}

} // end namespace scene
} // end namespace irr

