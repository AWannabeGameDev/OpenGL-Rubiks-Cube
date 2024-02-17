#ifndef RUBIKS_CUBE_H
#define RUBIKS_CUBE_H

#include <glm/glm.hpp>

#include "transform.h"

#include <unordered_map>

template<>
struct std::hash<glm::vec<3, int>>
{
	size_t operator()(const glm::vec<3, int>& vec) const noexcept
	{
		std::hash<int> intHash;
		size_t ret = intHash(vec.x);
		ret ^= intHash(vec.y) + 0x9e3779b9 + (ret << 6) + (ret >> 2);
		ret ^= intHash(vec.z) + 0x9e3779b9 + (ret << 6) + (ret >> 2);
		return ret;
	}
};

class RubiksCube
{
public :
	typedef glm::vec<3, int> Index;

	static constexpr int
		CUBE_NUM_VOXES = 27,
		CUBE_NUM_VOXES_PER_FACE = 9,
		VOX_NUM_VERTS = 24,
		VOX_NUM_INDICES = 36,
		NUM_FACES = 6,
		VOX_NUM_VERTS_PER_FACE = 4;

	enum Face
	{
		FACE_POS_X,
		FACE_NEG_X,
		FACE_POS_Y,
		FACE_NEG_Y,
		FACE_POS_Z,
		FACE_NEG_Z
	};

private :
	enum VoxIndexRotation
	{
		CLOCKWISE,
		COUNTER_CLOCKWISE,
		DOUBLE
	};

	static constexpr Index FACES_POS_INDEX[NUM_FACES][CUBE_NUM_VOXES_PER_FACE]
	{
		// face +x
		{{1, 1, 1}, {1, 1, 0}, {1, 1, -1},
		{1, 0, 1}, {1, 0, 0}, {1, 0, -1},
		{1, -1, 1}, {1, -1, 0}, {1, -1, -1}},

		// face -x
		{{-1, 1, -1}, {-1, 1, 0}, {-1, 1, 1},
		{-1, 0, -1}, {-1, 0, 0}, {-1, 0, 1},
		{-1, -1, -1}, {-1, -1, 0}, {-1, -1, 1}},

		// face +y
		{{-1, 1, -1}, {0, 1, -1}, {1, 1, -1},
		{-1, 1, 0}, {0, 1, 0}, {1, 1, 0},
		{-1, 1, 1}, {0, 1, 1}, {1, 1, 1}},

		// face -y
		{{1, -1, -1}, {0, -1, -1}, {-1, -1, -1},
		{1, -1, 0}, {0, -1, 0}, {-1, -1, 0},
		{1, -1, 1}, {0, -1, 1}, {-1, -1, 1}},

		// face +z
		{{-1, 1, 1}, {0, 1, 1}, {1, 1, 1},
		{-1, 0, 1}, {0, 0, 1}, {1, 0, 1},
		{-1, -1, 1}, {0, -1, 1}, {1, -1, 1}},

		// face -z
		{{1, 1, -1}, {0, 1, -1}, {-1, 1, -1},
		{1, 0, -1}, {0, 0, -1}, {-1, 0, -1},
		{1, -1, -1}, {0, -1, -1}, {-1, -1, -1}}
	};

	static constexpr glm::vec4 FACE_COLORS[NUM_FACES]
	{
		{0.0f, 0.0f, 1.0f, 1.0f}, // blue
		{0.0f, 1.0f, 0.0f, 1.0f}, // green
		{1.0f, 1.0f, 1.0f, 1.0f}, // white
		{1.0f, 1.0f, 0.0f, 1.0f}, // yellow
		{1.0f, 0.0f, 0.0f, 1.0f}, // red
		{1.0f, 0.65, 0.0f, 1.0f} // orange
	};

	static constexpr glm::vec3 FACES_ROTATION_AXIS[NUM_FACES]
	{
		{1.0f, 0.0f, 0.0f},
		{-1.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, -1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, -1.0f}
	};

	static constexpr float VOX_SPACING = 1.3f;

	glm::vec3 vertexPositions[VOX_NUM_VERTS];
	unsigned int vertexIndices[VOX_NUM_INDICES];

	union
	{
		glm::vec4 colors[CUBE_NUM_VOXES * VOX_NUM_VERTS];

		glm::vec4 voxColors[CUBE_NUM_VOXES][VOX_NUM_VERTS];
		glm::vec4 voxColorsIndexed[3][3][3][VOX_NUM_VERTS];

		glm::vec4 voxFaceColors[CUBE_NUM_VOXES][NUM_FACES][VOX_NUM_VERTS_PER_FACE];
		glm::vec4 voxFaceColorsIndexed[3][3][3][NUM_FACES][VOX_NUM_VERTS_PER_FACE];
	};

	union
	{
		Transform voxTransforms[CUBE_NUM_VOXES];
		Transform voxTransformsIndexed[3][3][3];
	};

	std::unordered_map<Index, Index> posToVoxIndex;

	bool snapped;
	float faceRotationRads;

	void rotateFaceVoxIndices(VoxIndexRotation rotation);
	float getPrincipleAngle(float radians) const;

public :
	Face selectedFace;
	Transform globalTransform;

	RubiksCube();

	const glm::vec4* getColors() const;
	const Transform* getTransforms() const;
	const glm::vec3* getVoxelVertexPositions() const;
	const unsigned int* getVoxelIndices() const;

	bool isSnapped() const;
	void snapFace();
	void rotateFace(float radians);
};

#endif