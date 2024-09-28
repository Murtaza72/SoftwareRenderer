#include "Renderer.h"

#include <iostream>
#include <algorithm>
#include <list>

#include "Triangle.h"
#include "Camera.h"
#include "Mesh.h"

Renderer::~Renderer()
{
	SDL_DestroyRenderer(m_Renderer);
	SDL_DestroyWindow(m_Window);
	SDL_Quit();
}

void Renderer::Render(Camera& camera)
{
	JavidDemo(camera);
}

void Renderer::JavidDemo(Camera& cam)
{
	Mesh cubeMesh; // clockwise winding
	//cubeMesh.tris.push_back({ Vec3{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f} });
	//cubeMesh.tris.push_back({ Vec3{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f} });
	//cubeMesh.tris.push_back({ Vec3{1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f} });
	//cubeMesh.tris.push_back({ Vec3{1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f} });
	//cubeMesh.tris.push_back({ Vec3{1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f} });
	//cubeMesh.tris.push_back({ Vec3{1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f} });
	//cubeMesh.tris.push_back({ Vec3{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f} });
	//cubeMesh.tris.push_back({ Vec3{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f} });
	//cubeMesh.tris.push_back({ Vec3{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f} });
	//cubeMesh.tris.push_back({ Vec3{0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f} });
	//cubeMesh.tris.push_back({ Vec3{1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f} });
	//cubeMesh.tris.push_back({ Vec3{1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f} });

	if (!cubeMesh.LoadObject("./assets/mountains.obj"))
		std::cout << "Could not load the obj file!!!" << std::endl;

	float theta = 0;

	float nearPlane = 0.1f;
	float farPlane = 1000.0f;
	float fov = 90.0f;
	float aspectRatio = (float)GetWindowWidth() / GetWindowHeight();

	Mat4x4 projectionMat = Mat4x4::Projection(nearPlane, farPlane, aspectRatio, fov);

	theta += SDL_GetTicks() / 2000.0f;

	Mat4x4 rotationMatX = Mat4x4::RotationX(0);
	Mat4x4 rotationMatY = Mat4x4::RotationY(0);
	Mat4x4 rotationMatZ = Mat4x4::RotationZ(0);
	Mat4x4 translateMat = Mat4x4::Translation(0.0f, 0.0f, 3.0f);

	Mat4x4 worldMat = rotationMatX * rotationMatY * rotationMatZ * translateMat;

	//cam.lookDir = { 0.0f,0.0f,1.0f };
	Vec3 up(0.0f, 1.0f, 0.0f);
	Vec3 target(0.0f, 0.0f, 1.0f);

	Mat4x4 camRotation = Mat4x4::Identity();
	//camRotation = camRotation * Mat4x4::RotationX(cam.roll);
	camRotation = camRotation * Mat4x4::RotationY(cam.yaw);
	//camRotation = camRotation * Mat4x4::RotationZ(cam.pitch);
	cam.lookDir = MultiplyMatrixVector(target, camRotation);
	target = cam.position + cam.lookDir;

	cam.mat = Mat4x4::LookAt(cam.position, target, up);
	cam.mat = Mat4x4::QuickInverse(cam.mat);

	std::vector<Triangle> trisToRaster;

	for (Triangle tri : cubeMesh.GetTriangles())
	{
		tri = TransformTriangle(tri, cam.mat);

		tri = TransformTriangle(tri, worldMat);

		// translate
		tri = TransformTriangle(tri, translateMat);

		Vec3 line1 = tri.p[1] - tri.p[0];
		Vec3 line2 = tri.p[2] - tri.p[0];
		Vec3 normal = Cross(line1, line2);
		normal = normal.GetNormalized();

		// proceed if not back face culled
		if (Dot(normal, { tri.p[0] - cam.position }) < 0.0f)
		{
			Vec3 LigthDir = Vec3(0.0f, 1.0f, 0.0f);
			LigthDir = LigthDir.GetNormalized();

			float dp = Dot(normal, LigthDir);
			dp = std::max(0.2f, dp);

			Color color = Colors::White * dp;
			tri.color = color;

			// clipping
			int clippedCount = 0;
			Triangle clippedTri[2];
			clippedCount = ClipAgainstPlane({ 0.0f, 0.0f, nearPlane }, { 0.0f, 0.0f, 1.0f }, tri, clippedTri[0], clippedTri[1]);

			for (int n = 0; n < clippedCount; n++)
			{
				//tri = TransformTriangle(tri, projectionMat);
				tri = TransformTriangle(clippedTri[n], projectionMat);
				tri.color = clippedTri[n].color;

				//	 invert x/y axis
				tri.p[0].x *= -1.0f;
				tri.p[1].x *= -1.0f;
				tri.p[2].x *= -1.0f;
				tri.p[0].y *= -1.0f;
				tri.p[1].y *= -1.0f;
				tri.p[2].y *= -1.0f;

				// translate to middle of the screen
				tri.p[0] += Vec3(1.0f, 1.0f, 0.0f);
				tri.p[1] += Vec3(1.0f, 1.0f, 0.0f);
				tri.p[2] += Vec3(1.0f, 1.0f, 0.0f);

				tri.p[0].x *= 0.5f * (float)GetWindowWidth();
				tri.p[0].y *= 0.5f * (float)GetWindowHeight();
				tri.p[1].x *= 0.5f * (float)GetWindowWidth();
				tri.p[1].y *= 0.5f * (float)GetWindowHeight();
				tri.p[2].x *= 0.5f * (float)GetWindowWidth();
				tri.p[2].y *= 0.5f * (float)GetWindowHeight();

				trisToRaster.push_back(tri);
			}
		}
	}

	sort(trisToRaster.begin(), trisToRaster.end(), [](Triangle& t1, Triangle& t2)
		{
			float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z);
			float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z);
			return z1 > z2;
		});


	//Clipping and Draw loop
	for (const Triangle& tri : trisToRaster)
	{
		// Clip triangles against all four screen edges, this could yield
		// a bunch of triangles, so create a queue that we traverse to 
		// ensure we only test new triangles generated
		Triangle clipped[2];
		std::list<Triangle> queue;

		queue.push_back(tri);
		int nNewTriangles = 1;

		for (int p = 0; p < 4; p++)
		{
			int nTrisToAdd = 0;
			while (nNewTriangles > 0)
			{
				Triangle test = queue.front();
				queue.pop_front();
				nNewTriangles--;

				// Clip it against a plane. We only need to test each 
				// subsequent plane, against subsequent new triangles
				// as all triangles after a plane clip are guaranteed
				// to lie on the inside of the plane.
				switch (p)
				{
				case 0:
					nTrisToAdd = ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]);
					break;
				case 1:
					nTrisToAdd = ClipAgainstPlane({ 0.0f, (float)GetWindowHeight() - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]);
					break;
				case 2:
					nTrisToAdd = ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]);
					break;
				case 3:
					nTrisToAdd = ClipAgainstPlane({ (float)GetWindowWidth() - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]);
					break;
				}

				// add any newly generated tris to the end of queue
				for (int w = 0; w < nTrisToAdd; w++)
					queue.push_back(clipped[w]);
			}

			nNewTriangles = queue.size();
		}

		// Draw the transformed, viewed, clipped, projected, sorted, clipped triangles
		for (auto& tri : queue)
		{
			FillTriangle({ tri.p[0].x, tri.p[0].y }, { tri.p[1].x, tri.p[1].y }, { tri.p[2].x, tri.p[2].y }, tri.color);
			DrawTriangle(tri.p[0].x, tri.p[0].y, tri.p[1].x, tri.p[1].y, tri.p[2].x, tri.p[2].y, Colors::Red);
		}
	}
}

Vec3 IntersectPlane(const Vec3& planePoint, Vec3& planeNormal, const Vec3& start, const Vec3& end)
{
	planeNormal = planeNormal.GetNormalized();
	float planeD = -Dot(planeNormal, planePoint);
	float ad = Dot(start, planeNormal);
	float bd = Dot(end, planeNormal);
	float t = (-planeD - ad) / (bd - ad);
	Vec3 line = end - start;
	Vec3 lineIntersect = line * t;

	return start + lineIntersect;
}

int Renderer::ClipAgainstPlane(Vec3 planePoint, Vec3 planeNormal, Triangle& in, Triangle& outTri1, Triangle& outTri2)
{
	planeNormal.GetNormalized();

	auto dist = [&](Vec3& p)
		{
			return Dot(planeNormal, p) - Dot(planeNormal, planePoint);
		};

	// positive sign if inside
	Vec3* insidePoint[3];	int insidePointCount = 0;
	Vec3* outsidePoint[3];	int outsidePointCount = 0;

	float d0 = dist(in.p[0]);
	float d1 = dist(in.p[1]);
	float d2 = dist(in.p[2]);

	if (d0 >= 0) { insidePoint[insidePointCount++] = &in.p[0]; }
	else { outsidePoint[outsidePointCount++] = &in.p[0]; }

	if (d1 >= 0) { insidePoint[insidePointCount++] = &in.p[1]; }
	else { outsidePoint[outsidePointCount++] = &in.p[1]; }

	if (d2 >= 0) { insidePoint[insidePointCount++] = &in.p[2]; }
	else { outsidePoint[outsidePointCount++] = &in.p[2]; }

	if (insidePointCount == 0)
	{
		// all points are outside
		// no triangle is returned

		return 0;
	}

	if (insidePointCount == 3)
	{
		// all points are inside
		// return original triangle

		outTri1 = in;
		outTri1.color = in.color;

		return 1;
	}

	if (outsidePointCount == 2 && insidePointCount == 1)
	{
		// plane intersects on two points
		// original tri becomes small tri

		outTri1 = in;
		outTri1.color = Colors::Red;//in.color;

		outTri1.p[0] = *insidePoint[0];

		// calculate intersection points
		outTri1.p[1] = IntersectPlane(planePoint, planeNormal, *insidePoint[0], *outsidePoint[0]);
		outTri1.p[2] = IntersectPlane(planePoint, planeNormal, *insidePoint[0], *outsidePoint[1]);

		return 1;
	}

	if (outsidePointCount == 1 && insidePointCount == 2)
	{
		outTri1 = in;
		outTri1.color = Colors::Blue;//in.color;

		outTri2 = in;
		outTri2.color = Colors::Green;//in.color;

		// first tri is made of two inside points and one intersection point
		outTri1.p[0] = *insidePoint[0];
		outTri1.p[1] = *insidePoint[1];
		outTri1.p[2] = IntersectPlane(planePoint, planeNormal, *insidePoint[0], *outsidePoint[0]);

		// second tri is made of two intersection points and one inside point
		outTri2.p[0] = *insidePoint[1];
		outTri2.p[1] = outTri1.p[2];
		outTri2.p[2] = IntersectPlane(planePoint, planeNormal, *insidePoint[1], *outsidePoint[0]);

		return 2;
	}
}

void Renderer::DrawPixel(float x, float y, Color color)
{
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, 255);
	SDL_RenderDrawPointF(m_Renderer, x, y);
}

void Renderer::DrawPixel(float x, float y)
{
	DrawPixel(x, y, Colors::White);
}

void Renderer::ClearColor(Color color)
{
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, 0);
	SDL_RenderClear(m_Renderer);
}

void Renderer::DrawColor(Color color)
{
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, 255);
	SDL_RenderClear(m_Renderer);
}

void Renderer::DrawTriangle(float p1X, float p1Y, float p2X, float p2Y, float p3X, float p3Y, Color color)
{
	DrawLine(p1X, p1Y, p2X, p2Y, color);
	DrawLine(p2X, p2Y, p3X, p3Y, color);
	DrawLine(p3X, p3Y, p1X, p1Y, color);
}

void Renderer::DrawLine(float x1, float y1, float x2, float y2, Color color)
{
	if (abs(x2 - x1) > abs(y2 - y1))
		BresenhamHorizontal(x1, y1, x2, y2, color);
	else
		BresenhamVertical(x1, y1, x2, y2, color);

	//SDL_RenderPresent(m_Renderer);
	//SDL_Delay(10);
}

int EdgeCross(Vec2 a, Vec2 b, Vec2 c)
{
	Vec2 ab = { b.x - a.x, b.y - a.y };
	Vec2 ac = { c.x - a.x, c.y - a.y };

	return Cross(ab, ac);
}

bool IsTopLeftEdge(Vec2 start, Vec2 end)
{
	Vec2 edge = { end.x - start.x,end.y - start.y };

	bool isTop = edge.y == 0 && edge.x > 0;
	bool isLeft = edge.y < 0;

	return isTop || isLeft;
}

void Renderer::FillTriangle(Vec2 p1, Vec2 p2, Vec2 p3, Color color)
{
	// Todo: Add barycentric interpolation for textures
	// Todo: sub-pixel precision to reduce jittering

	float area = EdgeCross(p1, p2, p3);
	// If the triangle is clockwise, swap two vertices to make it counterclockwise
	if (area < 0)
	{
		std::swap(p2, p3);
	}

	// Calculate bounding box around the tri
	int xMin = std::min(std::min(p1.x, p2.x), p3.x);
	int xMax = std::max(std::max(p1.x, p2.x), p3.x);
	int yMin = std::min(std::min(p1.y, p2.y), p3.y);
	int yMax = std::max(std::max(p1.y, p2.y), p3.y);

	// Determine if either top or left edge
	int bias1 = IsTopLeftEdge(p1, p2) ? 0 : -1;
	int bias2 = IsTopLeftEdge(p2, p3) ? 0 : -1;
	int bias3 = IsTopLeftEdge(p3, p1) ? 0 : -1;

	for (int y = yMin; y <= yMax; y++)
	{
		for (int x = xMin; x <= xMax; x++)
		{
			Vec2 p = { (float)x, (float)y };

			int w1 = EdgeCross(p1, p2, p) + bias1;
			int w2 = EdgeCross(p2, p3, p) + bias2;
			int w3 = EdgeCross(p3, p1, p) + bias3;

			bool isInside = w1 >= 0 && w2 >= 0 && w3 >= 0;

			if (isInside)
			{
				//__debugbreak();
				DrawPixel(x, y, color);
			}
		}
	}
}

void Renderer::BresenhamNaive(int x1, int y1, int x2, int y2)
{
	// Works for slope < 1 only

	// invert y-axis
	y1 = GetWindowHeight() - y1;
	y2 = GetWindowHeight() - y2;

	int x = x1;
	int y = y1;
	int dx = x2 - x1;
	int dy = y2 - y1;
	float m = (float)dy / dx;
	int p = 2 * dy - dx;

	if (m > 1.0)
	{
		std::cout << "Slope greater than 1" << std::endl;
		return;
	}

	while (x <= x2)
	{
		DrawPixel(x++, y);

		if (p < 0)
		{
			p += 2 * dy;
		}
		else
		{
			p += 2 * dy - 2 * dx;
			y++;
		}
	}
}

void Renderer::BresenhamHorizontal(float x1, float y1, float x2, float y2, Color color)
{
	// Swap em
	if (x1 > x2)
	{
		float temp = x1;
		x1 = x2;
		x2 = temp;

		temp = y1;
		y1 = y2;
		y2 = temp;
	}

	float x = x1;
	float y = y1;
	float dx = x2 - x1;
	float dy = y2 - y1;

	float p = 2 * dy - dx;

	float dir = dy < 0 ? -1 : 1;
	dy *= dir;

	while (x <= x2)
	{
		DrawPixel(x++, y, color);

		if (p < 0)
		{
			p += 2 * dy;
		}
		else
		{
			p += 2 * (dy - dx);
			y += dir;
		}
	}
}

void Renderer::BresenhamVertical(float x1, float y1, float x2, float y2, Color color)
{
	// Swap em
	if (y1 > y2)
	{
		float temp = x1;
		x1 = x2;
		x2 = temp;

		temp = y1;
		y1 = y2;
		y2 = temp;
	}

	float x = x1;
	float y = y1;
	float dx = x2 - x1;
	float dy = y2 - y1;

	float p = 2 * dx - dy;

	float dir = dx < 0 ? -1 : 1;
	dx *= dir;

	while (y <= y2)
	{
		DrawPixel(x, y++, color);

		if (p < 0)
		{
			p += 2 * dx;
		}
		else
		{
			p += 2 * (dx - dy);
			x += dir;
		}
	}
}

int Renderer::GetWindowWidth()
{
	int width;
	SDL_GetWindowSize(m_Window, &width, nullptr);
	return width;
}

int Renderer::GetWindowHeight()
{
	int height;
	SDL_GetWindowSize(m_Window, nullptr, &height);
	return height;
}