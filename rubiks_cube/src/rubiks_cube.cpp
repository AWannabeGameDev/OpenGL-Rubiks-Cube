#include "rubiks_cube.h"
#include "models.h"

void RubiksCube::rotateFaceVoxIndices(VoxIndexRotation rotation)
{
	const Index* face = FACES_POS_INDEX[selectedFace];

	switch(rotation)
	{
		case CLOCKWISE :
		{
			std::swap(posToVoxIndex[face[2]], posToVoxIndex[face[0]]);
			std::swap(posToVoxIndex[face[0]], posToVoxIndex[face[6]]);
			std::swap(posToVoxIndex[face[6]], posToVoxIndex[face[8]]);

			std::swap(posToVoxIndex[face[1]], posToVoxIndex[face[3]]);
			std::swap(posToVoxIndex[face[3]], posToVoxIndex[face[7]]);
			std::swap(posToVoxIndex[face[7]], posToVoxIndex[face[5]]);

			break;
		}

		case COUNTER_CLOCKWISE :
		{
			std::swap(posToVoxIndex[face[2]], posToVoxIndex[face[8]]);
			std::swap(posToVoxIndex[face[8]], posToVoxIndex[face[6]]);
			std::swap(posToVoxIndex[face[6]], posToVoxIndex[face[0]]);

			std::swap(posToVoxIndex[face[1]], posToVoxIndex[face[5]]);
			std::swap(posToVoxIndex[face[5]], posToVoxIndex[face[7]]);
			std::swap(posToVoxIndex[face[7]], posToVoxIndex[face[3]]);

			break;
		}

		case DOUBLE :
		{
			std::swap(posToVoxIndex[face[0]], posToVoxIndex[face[8]]);
			std::swap(posToVoxIndex[face[1]], posToVoxIndex[face[7]]);
			std::swap(posToVoxIndex[face[2]], posToVoxIndex[face[6]]);
			std::swap(posToVoxIndex[face[3]], posToVoxIndex[face[5]]);

			break;
		}
	}
}

float RubiksCube::getPrincipleAngle(float radians) const
{
	float inRange = fmodf(radians, glm::radians(360.0f));
	float mag = fabsf(inRange);

	if(mag > glm::radians(180.0f))
		return copysignf(glm::radians(360.0f) - mag, -inRange);
	else
		return inRange;
}

RubiksCube::RubiksCube() :
	colors{}, voxTransforms{}, posToVoxIndex{}, 
	snapped{true}, faceRotationRads{0.0f}, selectedFace{FACE_POS_X}, globalTransform{}
{
	std::copy(models::cube::positions, models::cube::positions + VOX_NUM_VERTS, vertexPositions);
	std::copy(models::cube::indices, models::cube::indices + VOX_NUM_INDICES, vertexIndices);

	for(int face = 0; face < NUM_FACES; face++)
	{
		for(int voxel = 0; voxel < CUBE_NUM_VOXES_PER_FACE; voxel++)
		{
			Index posIndex = FACES_POS_INDEX[face][voxel];
			Index voxIndex = posIndex + Index{1, 1, 1};

			posToVoxIndex[posIndex] = voxIndex;

			glm::vec3 position = posIndex;
			position *= VOX_SPACING;
			voxTransformsIndexed[voxIndex.x][voxIndex.y][voxIndex.z].position = position;

			for(size_t vertex = 0; vertex < VOX_NUM_VERTS_PER_FACE; vertex++)
				voxFaceColorsIndexed[voxIndex.x][voxIndex.y][voxIndex.z][face][vertex] = FACE_COLORS[face];
		}
	}

	Index posIndex{0, 0, 0};
	Index voxIndex{1, 1, 1};
	posToVoxIndex[posIndex] = voxIndex;
}

const glm::vec4* RubiksCube::getColors() const
{
	return colors;
}

const glm::vec3* RubiksCube::getVoxelVertexPositions() const
{
	return vertexPositions;
}

const unsigned int* RubiksCube::getVoxelIndices() const
{
	return vertexIndices;
}

bool RubiksCube::isSnapped() const
{
	return snapped;
}

void RubiksCube::snapFace()
{
	float rotatedDegs = glm::degrees(getPrincipleAngle(faceRotationRads));
	float rotatedDegsMag = fabsf(rotatedDegs);

	float remainingRotationRads;

	if((rotatedDegsMag <= 180.0f) && (rotatedDegsMag >= 135.0f))
	{
		rotateFaceVoxIndices(DOUBLE);
		remainingRotationRads = glm::radians(180.0f - rotatedDegs);
	}
	else if((rotatedDegs >= 45.0f) && (rotatedDegs <= 135.0f))
	{
		rotateFaceVoxIndices(COUNTER_CLOCKWISE);
		remainingRotationRads = glm::radians(90.0f - rotatedDegs);
	}
	else if((rotatedDegs <= -45.0f) && (rotatedDegs >= -135.0f))
	{
		rotateFaceVoxIndices(CLOCKWISE);
		remainingRotationRads = glm::radians(-90.0f - rotatedDegs);
	}
	else
	{
		remainingRotationRads = glm::radians(-rotatedDegs);
	}

	for(const Index& posIndex : FACES_POS_INDEX[selectedFace])
	{
		const Index& voxIndex = posToVoxIndex[posIndex];
		Transform& voxTransform = voxTransformsIndexed[voxIndex.x][voxIndex.y][voxIndex.z];

		voxTransform.position = VOX_SPACING * glm::vec3{posIndex};
		voxTransform.rotation = glm::angleAxis(remainingRotationRads, FACES_ROTATION_AXIS[selectedFace]) 
								* voxTransform.rotation;
	}

	faceRotationRads = 0.0f;
}

void RubiksCube::rotateFace(float radians)
{
	glm::quat rotation = glm::angleAxis(radians, FACES_ROTATION_AXIS[selectedFace]);
	faceRotationRads += radians;

	for(const Index& posIndex : FACES_POS_INDEX[selectedFace])
	{
		const Index& voxIndex = posToVoxIndex.at(posIndex);
		Transform& voxTransform = voxTransformsIndexed[voxIndex.x][voxIndex.y][voxIndex.z];

		voxTransform.rotation = rotation * voxTransform.rotation;
		voxTransform.position = rotation * voxTransform.position;
	}

	snapped = false;
}

const Transform* RubiksCube::getTransforms() const
{
	return voxTransforms;
}