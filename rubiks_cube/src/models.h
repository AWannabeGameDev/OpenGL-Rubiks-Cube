#ifndef MODELS_H
#define MODELS_H

#include <glm/glm.hpp>

namespace models
{
	namespace cube
	{
		inline constexpr glm::vec3 positions[24] =
		{
			// face +x
			{0.5f, 0.5f, 0.5f}, // I quadrant
			{0.5f, -0.5f, 0.5f}, // II quadrant
			{0.5f, -0.5f, -0.5f}, // III quadrant
			{0.5f, 0.5f, -0.5f}, // IV quadrant

			// face -x
			{-0.5f, 0.5f, 0.5f},
			{-0.5f, -0.5f, 0.5f},
			{-0.5f, -0.5f, -0.5f},
			{-0.5f, 0.5f, -0.5f},

			// face +y
			{0.5f, 0.5f, 0.5f},
			{-0.5f, 0.5f, 0.5f},
			{-0.5f, 0.5f, -0.5f},
			{0.5f, 0.5f, -0.5f},

			// face -y
			{0.5f, -0.5f, 0.5f},
			{-0.5f, -0.5f, 0.5f},
			{-0.5f, -0.5f, -0.5f},
			{0.5f, -0.5f, -0.5f},

			// face +z
			{0.5f, 0.5f, 0.5f},
			{-0.5f, 0.5f, 0.5f},
			{-0.5f, -0.5f, 0.5f},
			{0.5f, -0.5f, 0.5f},

			// face -z
			{0.5f, 0.5f, -0.5f},
			{-0.5f, 0.5f, -0.5f},
			{-0.5f, -0.5f, -0.5f},
			{0.5f, -0.5f, -0.5f},
		};

		inline constexpr float CUBE_SIDE_LENGTH = 1.0f;

		inline constexpr unsigned int indices[36] =
		{
			// upper-left triangle                 // lower-right triangle
			0 + (4 * 0), 1 + (4 * 0), 2 + (4 * 0), 2 + (4 * 0), 3 + (4 * 0), 0 + (4 * 0),
			0 + (4 * 1), 1 + (4 * 1), 2 + (4 * 1), 2 + (4 * 1), 3 + (4 * 1), 0 + (4 * 1),
			0 + (4 * 2), 1 + (4 * 2), 2 + (4 * 2), 2 + (4 * 2), 3 + (4 * 2), 0 + (4 * 2),
			0 + (4 * 3), 1 + (4 * 3), 2 + (4 * 3), 2 + (4 * 3), 3 + (4 * 3), 0 + (4 * 3),
			0 + (4 * 4), 1 + (4 * 4), 2 + (4 * 4), 2 + (4 * 4), 3 + (4 * 4), 0 + (4 * 4),
			0 + (4 * 5), 1 + (4 * 5), 2 + (4 * 5), 2 + (4 * 5), 3 + (4 * 5), 0 + (4 * 5),
		};
	}
}

#endif