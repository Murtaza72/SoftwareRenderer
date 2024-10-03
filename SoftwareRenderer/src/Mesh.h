#pragma once

#include <vector>
#include <fstream>
#include <sstream>

#include "Triangle.h"

class Mesh
{
public:

	bool LoadObject(std::string filename, bool hasTexture = false)
	{
		std::ifstream f(filename);

		if (!f.is_open())
			return false;

		std::vector<Vec3> verts;
		std::vector<TexCoord> texs;

		while (!f.eof())
		{
			char line[128];
			f.getline(line, 128);

			std::stringstream s;

			s << line;

			char junk;

			if (line[0] == 'v')
			{
				if (line[1] == 't')
				{
					TexCoord tc;
					s >> junk >> junk >> tc.u >> tc.v;
					// A little hack for the spyro texture
					tc.u = 1.0f - tc.u;
					tc.v = 1.0f - tc.v;
					texs.push_back(tc);
				}
				else
				{
					Vec3 v;
					s >> junk >> v.x >> v.y >> v.z;
					verts.push_back(v);
				}
			}

			if (!hasTexture)
			{
				if (line[0] == 'f')
				{
					int f[3]; TexCoord dummy{ 0.0f, 0.0f, 0.0f };
					s >> junk >> f[0] >> f[1] >> f[2];
					Triangle t = { verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1], dummy ,dummy, dummy, Colors::Magenta };
					m_Tris.push_back(t);
				}
			}
			else
			{
				if (line[0] == 'f')
				{
					s >> junk;

					std::string tokens[6];
					int tokenCount = -1;

					while (!s.eof())
					{
						char c = s.get();
						if (c == ' ' || c == '/')
							tokenCount++;
						else
							tokens[tokenCount].append(1, c);
					}

					tokens[tokenCount].pop_back();

					m_Tris.push_back({ verts[std::stoi(tokens[0]) - 1], verts[std::stoi(tokens[2]) - 1], verts[std::stoi(tokens[4]) - 1],
						texs[std::stoi(tokens[1]) - 1], texs[std::stoi(tokens[3]) - 1], texs[std::stoi(tokens[5]) - 1] });

				}
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
		m_Tris.reserve(12);

		//original javid cube
		// clockwise winding
		Add({ Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 1.0f, 0.0f), TexCoord{0.0f, 1.0f, 1.0f}, TexCoord{0.0f, 0.0f, 1.0f}, TexCoord{1.0f, 0.0f, 1.0f} });
		Add({ Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), TexCoord{0.0f, 1.0f, 1.0f}, TexCoord{1.0f, 0.0f, 1.0f}, TexCoord{1.0f, 1.0f, 1.0f} });
		Add({ Vec3(1.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), TexCoord{0.0f, 1.0f, 1.0f}, TexCoord{0.0f, 0.0f, 1.0f}, TexCoord{1.0f, 0.0f, 1.0f} });
		Add({ Vec3(1.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Vec3(1.0f, 0.0f, 1.0f), TexCoord{0.0f, 1.0f, 1.0f}, TexCoord{1.0f, 0.0f, 1.0f}, TexCoord{1.0f, 1.0f, 1.0f} });
		Add({ Vec3(1.0f, 0.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f), Vec3(0.0f, 1.0f, 1.0f), TexCoord{0.0f, 1.0f, 1.0f}, TexCoord{0.0f, 0.0f, 1.0f}, TexCoord{1.0f, 0.0f, 1.0f} });
		Add({ Vec3(1.0f, 0.0f, 1.0f), Vec3(0.0f, 1.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), TexCoord{0.0f, 1.0f, 1.0f}, TexCoord{1.0f, 0.0f, 1.0f}, TexCoord{1.0f, 1.0f, 1.0f} });
		Add({ Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 1.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f), TexCoord{0.0f, 1.0f, 1.0f}, TexCoord{0.0f, 0.0f, 1.0f}, TexCoord{1.0f, 0.0f, 1.0f} });
		Add({ Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), TexCoord{0.0f, 1.0f, 1.0f}, TexCoord{1.0f, 0.0f, 1.0f}, TexCoord{1.0f, 1.0f, 1.0f} });
		Add({ Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f), TexCoord{0.0f, 1.0f, 1.0f}, TexCoord{0.0f, 0.0f, 1.0f}, TexCoord{1.0f, 0.0f, 1.0f} });
		Add({ Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f), Vec3(1.0f, 1.0f, 0.0f), TexCoord{0.0f, 1.0f, 1.0f}, TexCoord{1.0f, 0.0f, 1.0f}, TexCoord{1.0f, 1.0f, 1.0f} });
		Add({ Vec3(1.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f, 0.0f), TexCoord{0.0f, 1.0f, 1.0f}, TexCoord{0.0f, 0.0f, 1.0f}, TexCoord{1.0f, 0.0f, 1.0f} });
		Add({ Vec3(1.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), TexCoord{0.0f, 1.0f, 1.0f}, TexCoord{1.0f, 0.0f, 1.0f}, TexCoord{1.0f, 1.0f, 1.0f} });
	}

private:
	std::vector<Triangle> m_Tris;
};