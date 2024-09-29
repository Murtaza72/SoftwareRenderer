#pragma once

#include <vector>
#include <fstream>
#include <sstream>

#include "Triangle.h"

class Mesh
{
public:
	bool LoadObject(std::string filename)
	{
		std::ifstream f(filename);

		if (!f.is_open())
			return false;

		std::vector<Vec3> verts;

		while (!f.eof())
		{
			char line[128];
			f.getline(line, 128);

			std::stringstream s;

			s << line;

			char junk;

			if (line[0] == 'v')
			{
				Vec3 v;
				s >> junk >> v.x >> v.y >> v.z;
				verts.push_back(v);
			}

			if (line[0] == 'f')
			{
				int f[3];
				s >> junk >> f[0] >> f[1] >> f[2];
				Triangle t = { verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1], Colors::Magenta };
				m_Tris.push_back(t);
			}

		}

		return true;
	}

	void Add(Triangle tri)
	{
		m_Tris.push_back(tri);
	}

	std::vector<Triangle> GetTriangles()
	{
		return m_Tris;
	}

	void LoadCube()
	{
		m_Tris.clear();

		//original javid cube
		// clockwise winding
		Add({ Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 1.0f, 0.0f) });
		Add({ Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f) });
		Add({ Vec3(1.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f) });
		Add({ Vec3(1.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Vec3(1.0f, 0.0f, 1.0f) });
		Add({ Vec3(1.0f, 0.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f), Vec3(0.0f, 1.0f, 1.0f) });
		Add({ Vec3(1.0f, 0.0f, 1.0f), Vec3(0.0f, 1.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f) });
		Add({ Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 1.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f) });
		Add({ Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f) });
		Add({ Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f) });
		Add({ Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Vec3(1.0f, 1.0f, 0.0f) });
		Add({ Vec3(1.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f, 0.0f) });
		Add({ Vec3(1.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f) });
	}

private:
	std::vector<Triangle> m_Tris;
};