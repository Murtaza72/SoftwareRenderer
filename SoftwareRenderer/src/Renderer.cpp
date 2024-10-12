#include "Renderer.h"

#include <iostream>
#include <algorithm>

#include <omp.h>

Renderer::Renderer(int width, int height)
	: m_Width(width), m_Height(height)
{
	SDL_Init(SDL_INIT_VIDEO);

	m_Window = SDL_CreateWindow(
		"Software Renderer",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		m_Width, m_Height, SDL_WINDOW_SHOWN);

	m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_SOFTWARE);

	m_ScreenRect = { 0,0,m_Width,m_Height };

	// s->format->format = SDL_PIXELFORMAT_XRGB8888
	m_ColorTex = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, m_ScreenRect.w, m_ScreenRect.h);

	m_ColorBuffer = new uint32_t[m_Width * m_Height];

	m_DepthBuffer = new float[m_Width * m_Height];
}

Renderer::~Renderer()
{
	SDL_DestroyRenderer(m_Renderer);
	SDL_DestroyWindow(m_Window);
	SDL_Quit();
}

void Renderer::Render(Mesh& mesh, Camera& cam, int flags)
{
	Mat4x4 worldViewMat = m_WorldMat * m_ViewMat;

	std::vector<Triangle> triangles = mesh.GetTriangles();
	int meshCount = triangles.size();

	#pragma omp parallel for 
	for (int i = 0; i < meshCount; i++)
	{
		Triangle tri = triangles[i];
		tri = TransformTriangle(tri, worldViewMat);

		Vec3 normal;
		if (BackfaceCulling(tri, cam, normal)) continue;

		// Check how much the triangle normal is facing the light dir 
		// and shade it appropriately
		m_Light.dir = m_Light.dir.GetNormalized();
		float dp = Dot(normal, m_Light.dir);
		dp = std::max(0.1f, dp);
		Color color = m_Light.color * dp;
		tri.color = color;

		// Clipping
		int clippedCount = 0;
		Triangle clippedTri[2];
		clippedCount = ClipAgainstPlane({ 0.0f, 0.0f, m_NearPlane }, { 0.0f, 0.0f, 1.0f }, tri, clippedTri[0], clippedTri[1]);

		for (int n = 0; n < clippedCount; n++)
		{
			tri = TransformTriangle(clippedTri[n], m_ProjectionMat);
			tri.color = clippedTri[n].color;

			// invert x/y axis
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

			tri.p[0] *= Vec3(0.5f * m_Width, 0.5f * m_Height, 1.0f);
			tri.p[1] *= Vec3(0.5f * m_Width, 0.5f * m_Height, 1.0f);
			tri.p[2] *= Vec3(0.5f * m_Width, 0.5f * m_Height, 1.0f);

			std::list<Triangle> triList;
			ClipAgainstScreen(triList, tri);

			// Draw the transformed, viewed, clipped, projected, sorted, clipped triangles
			for (auto& tri : triList)
			{
				if (flags & RENDER_WIRE)
				{
					DrawTriangle(tri, Colors::Red);
				}

				if (flags & RENDER_FLAT)
				{
					FillTriangleOptimized(tri, tri.color);
				}

				if (flags & RENDER_TEXTURED)
				{
					FillTriangleTextured(tri, m_Texture);
				}
			}
		}
	}
}

void Renderer::SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane)
{
	m_ProjectionMat = Mat4x4::Projection(fov, aspectRatio, nearPlane, farPlane);
	m_NearPlane = nearPlane;
}
void Renderer::SetCamera(Vec3& position, Vec3& target, Vec3& up)
{
	m_ViewMat = Mat4x4::LookAt(position, target, up);
	m_ViewMat = Mat4x4::QuickInverse(m_ViewMat);
}

void Renderer::SetTexture(Texture& tex)
{
	m_Texture = tex;
}

void Renderer::SetTransform(Mat4x4& world)
{
	m_WorldMat = world;
}

void Renderer::SetLightSource(const Light& light)
{
	m_Light.dir = light.dir;
	m_Light.color = light.color;
}

int Renderer::BackfaceCulling(Triangle& tri, Camera& cam, Vec3& normal)
{
	// Calculate two vectors on triangles and their cross product
	Vec3 line1 = tri.p[1] - tri.p[0];
	Vec3 line2 = tri.p[2] - tri.p[0];
	normal = Cross(line1, line2);
	normal = normal.GetNormalized();

	// Cull triangle if its a back face
	if (Dot(normal, { tri.p[0] - cam.position }) > 0.0f)
		return 1;

	return 0;
}

Vec3 IntersectPlane(const Vec3& planePoint, Vec3& planeNormal, const Vec3& start, const Vec3& end, float& interpolant)
{
	planeNormal = planeNormal.GetNormalized();
	float planeD = -Dot(planeNormal, planePoint);
	float ad = Dot(start, planeNormal);
	float bd = Dot(end, planeNormal);
	interpolant = (-planeD - ad) / (bd - ad);
	Vec3 line = end - start;
	Vec3 lineIntersect = line * interpolant;

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
	TexCoord* insideTex[3];	int insideTexCount = 0;
	TexCoord* outsideTex[3]; int outsideTexCount = 0;

	float d0 = dist(in.p[0]);
	float d1 = dist(in.p[1]);
	float d2 = dist(in.p[2]);

	if (d0 >= 0)
	{
		insidePoint[insidePointCount++] = &in.p[0];
		insideTex[insideTexCount++] = &in.tc[0];
	}
	else
	{
		outsidePoint[outsidePointCount++] = &in.p[0];
		outsideTex[outsideTexCount++] = &in.tc[0];
	}

	if (d1 >= 0)
	{
		insidePoint[insidePointCount++] = &in.p[1];
		insideTex[insideTexCount++] = &in.tc[1];
	}
	else
	{
		outsidePoint[outsidePointCount++] = &in.p[1];
		outsideTex[outsideTexCount++] = &in.tc[1];
	}

	if (d2 >= 0)
	{
		insidePoint[insidePointCount++] = &in.p[2];
		insideTex[insideTexCount++] = &in.tc[2];
	}
	else
	{
		outsidePoint[outsidePointCount++] = &in.p[2];
		outsideTex[outsideTexCount++] = &in.tc[2];
	}

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
		outTri1.color = in.color;

		outTri1.p[0] = *insidePoint[0];
		outTri1.tc[0] = *insideTex[0];

		// but the two new points are at the locations where the
		// original sides of the triangle intersect with the plane
		float interpolant;

		// calculate intersection points
		outTri1.p[1] = IntersectPlane(planePoint, planeNormal, *insidePoint[0], *outsidePoint[0], interpolant);
		outTri1.tc[1].u = interpolant * (outsideTex[0]->u - insideTex[0]->u) + insideTex[0]->u;
		outTri1.tc[1].v = interpolant * (outsideTex[0]->v - insideTex[0]->v) + insideTex[0]->v;
		outTri1.tc[1].w = interpolant * (outsideTex[0]->w - insideTex[0]->w) + insideTex[0]->w;

		outTri1.p[2] = IntersectPlane(planePoint, planeNormal, *insidePoint[0], *outsidePoint[1], interpolant);
		outTri1.tc[2].u = interpolant * (outsideTex[1]->u - insideTex[0]->u) + insideTex[0]->u;
		outTri1.tc[2].v = interpolant * (outsideTex[1]->v - insideTex[0]->v) + insideTex[0]->v;
		outTri1.tc[2].w = interpolant * (outsideTex[1]->w - insideTex[0]->w) + insideTex[0]->w;

		return 1;
	}

	if (outsidePointCount == 1 && insidePointCount == 2)
	{
		outTri1 = in;
		outTri1.color = in.color;

		outTri2 = in;
		outTri2.color = in.color;

		float interpolant;

		// first tri is made of two inside points and one intersection point
		outTri1.p[0] = *insidePoint[0];
		outTri1.p[1] = *insidePoint[1];
		outTri1.tc[0] = *insideTex[0];
		outTri1.tc[1] = *insideTex[1];

		outTri1.p[2] = IntersectPlane(planePoint, planeNormal, *insidePoint[0], *outsidePoint[0], interpolant);
		outTri1.tc[2].u = interpolant * (outsideTex[0]->u - insideTex[0]->u) + insideTex[0]->u;
		outTri1.tc[2].v = interpolant * (outsideTex[0]->v - insideTex[0]->v) + insideTex[0]->v;
		outTri1.tc[2].w = interpolant * (outsideTex[0]->w - insideTex[0]->w) + insideTex[0]->w;

		// second tri is made of two intersection points and one inside point
		outTri2.p[0] = *insidePoint[1];
		outTri2.p[1] = outTri1.p[2];
		outTri2.tc[0] = *insideTex[1];
		outTri2.tc[1] = outTri1.tc[2];

		outTri2.p[2] = IntersectPlane(planePoint, planeNormal, *insidePoint[1], *outsidePoint[0], interpolant);
		outTri2.tc[2].u = interpolant * (outsideTex[0]->u - insideTex[1]->u) + insideTex[1]->u;
		outTri2.tc[2].v = interpolant * (outsideTex[0]->v - insideTex[1]->v) + insideTex[1]->v;
		outTri2.tc[2].w = interpolant * (outsideTex[0]->w - insideTex[1]->w) + insideTex[1]->w;

		return 2;
	}

	return -1;	// debug only
}

void Renderer::ClipAgainstScreen(std::list<Triangle>& queue, Triangle& tri)
{
	// Clip triangles against all four screen edges, this could yield
	// a bunch of triangles, so create a queue that we traverse to 
	// ensure we only test new triangles generated

	Triangle clipped[2];

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
				nTrisToAdd = ClipAgainstPlane({ 0.0f, (float)m_Height - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]);
				break;
			case 2:
				nTrisToAdd = ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]);
				break;
			case 3:
				nTrisToAdd = ClipAgainstPlane({ (float)m_Width , 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]);
				break;
			}

			// add any newly generated tris to the end of queue
			for (int w = 0; w < nTrisToAdd; w++)
				queue.push_back(clipped[w]);
		}

		nNewTriangles = queue.size();
	}
}

void Renderer::DrawPixel(float x, float y, Color color)
{
	int dummy;
	SDL_LockTexture(m_ColorTex, &m_ScreenRect, (void**)&m_ColorBuffer, &dummy);

	uint32_t c = color.GetPackedColor();
	m_ColorBuffer[(int)y * m_ScreenRect.w + (int)x] = c;

	SDL_UnlockTexture(m_ColorTex);
}

void Renderer::DrawPixel(float x, float y)
{
	DrawPixel(x, y, Colors::White);
}

void Renderer::ClearColor(Color color)
{
	int dummy;
	SDL_LockTexture(m_ColorTex, &m_ScreenRect, (void**)&m_ColorBuffer, &dummy);

	int totalPixels = m_ScreenRect.w * m_ScreenRect.h;
	for (int i = 0; i < totalPixels; ++i)
	{
		m_ColorBuffer[i] = color.GetPackedColor();
	}

	SDL_UnlockTexture(m_ColorTex);
}

void Renderer::DrawColor(Color color)
{
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, 255);
	SDL_RenderClear(m_Renderer);
}

void Renderer::ClearDepth()
{
	std::memset(m_DepthBuffer, 0.0f, (sizeof(float) * m_Width * m_Height));
}

void Renderer::DrawTriangle(Triangle& tri, Color color)
{
	DrawTriangle(tri.p[0].x, tri.p[0].y, tri.p[1].x, tri.p[1].y, tri.p[2].x, tri.p[2].y, Colors::Red);
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
}

float EdgeCross(Vec2 a, Vec2 b, Vec2 c)
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
				DrawPixel(x, y, color);
			}
		}
	}
}

void Renderer::FillTriangleOptimized(Triangle& tri, Color color)
{
	// crude observation suggests 2x speedup 
	// proper profiling required from original

	// Todo: tile-based rendering using 4x4 and 8x8 pixels together for parallelization

	Vec3 p1 = tri.p[0];
	Vec3 p2 = tri.p[1];
	Vec3 p3 = tri.p[2];

	float area = EdgeCross(p1, p2, p3);
	// If the triangle is clockwise, swap two vertices to make it counterclockwise
	if (area < 0)
	{
		std::swap(p2, p3);
		area = -area;
	}

	float invArea = 1.0f / area;

	// Calculate bounding box around the tri
	int xMin, xMax, yMin, yMax;
	tri.GetBoundingBox(xMin, xMax, yMin, yMax, m_Width, m_Height);

	// Determine if either top or left edge
	float bias1 = IsTopLeftEdge(p1, p2) ? 0 : -0.001f;
	float bias2 = IsTopLeftEdge(p2, p3) ? 0 : -0.001f;
	float bias3 = IsTopLeftEdge(p3, p1) ? 0 : -0.001f;

	float deltaW1Col = p2.y - p3.y, deltaW1Row = p3.x - p2.x;
	float deltaW2Col = p3.y - p1.y, deltaW2Row = p1.x - p3.x;
	float deltaW3Col = p1.y - p2.y, deltaW3Row = p2.x - p1.x;

	Vec2 p = { (float)xMin + 0.5f , (float)yMin + 0.5f };

	float w1Row = EdgeCross(p2, p3, p) + bias1;
	float w2Row = EdgeCross(p3, p1, p) + bias2;
	float w3Row = EdgeCross(p1, p2, p) + bias3;

	for (int y = yMin; y <= yMax; y++)
	{
		float w2 = w2Row;
		float w1 = w1Row;
		float w3 = w3Row;

		for (int x = xMin; x <= xMax; x++)
		{
			bool isInside = w1 >= 0 && w2 >= 0 && w3 >= 0;

			if (isInside)
			{
				float z = 1.0f / (((w1 * invArea) * p1.z) + ((w2 * invArea) * p2.z) + ((w3 * invArea) * p3.z));

				float d = m_DepthBuffer[y * m_Width + x];
				if (z > d)
				{
					m_DepthBuffer[y * m_Width + x] = z;
					DrawPixel(x, y, color);
				}
			}

			w1 += deltaW1Col;
			w2 += deltaW2Col;
			w3 += deltaW3Col;
		}

		w1Row += deltaW1Row;
		w2Row += deltaW2Row;
		w3Row += deltaW3Row;
	}
}

void Renderer::FillTriangleTextured(Triangle& tri, Texture tex)
{
	Vec3 p1 = tri.p[0];
	Vec3 p2 = tri.p[1];
	Vec3 p3 = tri.p[2];
	TexCoord t1 = tri.tc[0];
	TexCoord t2 = tri.tc[1];
	TexCoord t3 = tri.tc[2];

	float area = EdgeCross(p1, p2, p3);
	if (area < 0)
	{
		std::swap(p2, p3);
		std::swap(t2, t3);
		area = -area;
	}

	float invArea = 1.0f / area;

	// Calculate bounding box around the tri
	int xMin, xMax, yMin, yMax;
	tri.GetBoundingBox(xMin, xMax, yMin, yMax, m_Width, m_Height);

	// Determine if either top or left edge
	float bias1 = IsTopLeftEdge(p1, p2) ? 0 : -0.001f;
	float bias2 = IsTopLeftEdge(p2, p3) ? 0 : -0.001f;
	float bias3 = IsTopLeftEdge(p3, p1) ? 0 : -0.001f;

	float deltaW1Col = p2.y - p3.y, deltaW1Row = p3.x - p2.x;
	float deltaW2Col = p3.y - p1.y, deltaW2Row = p1.x - p3.x;
	float deltaW3Col = p1.y - p2.y, deltaW3Row = p2.x - p1.x;

	Vec2 p = { (float)xMin + 0.5f , (float)yMin + 0.5f };

	float w1Row = EdgeCross(p2, p3, p) + bias1;
	float w2Row = EdgeCross(p3, p1, p) + bias2;
	float w3Row = EdgeCross(p1, p2, p) + bias3;

	for (int y = yMin; y <= yMax; y++)
	{
		float w1 = w1Row;
		float w2 = w2Row;
		float w3 = w3Row;

		for (int x = xMin; x <= xMax; x++)
		{
			bool isInside = w1 >= 0.0f && w2 >= 0.0f && w3 >= 0.0f;

			if (isInside)
			{
				float alpha = w1 * invArea;
				float beta = w2 * invArea;
				float gamma = w3 * invArea;

				#if 1
				float u = (alpha * t1.u) + (beta * t2.u) + (gamma * t3.u);
				float v = (alpha * t1.v) + (beta * t2.v) + (gamma * t3.v);

				#else
				float w = (alpha * (1.0f / p1.w)) + (beta * (1.0f / p2.w)) + (gamma * (1.0f / p3.w));
				float u = (alpha * (t1.u / t1.w)) + (beta * (t2.u / t2.w)) + (gamma * (t3.u / t3.w));
				float v = (alpha * (t1.v / t1.w)) + (beta * (t2.v / t2.w)) + (gamma * (t3.v / t3.w));
				u /= w;
				v /= w;
				#endif

				float z = 1.0f / ((alpha * p1.z) + (beta * p2.z) + (gamma * p3.z));

				if (z > m_DepthBuffer[y * m_Width + x])
				{
					m_DepthBuffer[y * m_Width + x] = z;

					u = std::clamp(u, 0.0f, 1.0f);
					v = std::clamp(v, 0.0f, 1.0f);
					Color color = tex.GetRGB(u, v);

					DrawPixel(x, y, color);
				}
			}

			w1 += deltaW1Col;
			w2 += deltaW2Col;
			w3 += deltaW3Col;
		}

		w1Row += deltaW1Row;
		w2Row += deltaW2Row;
		w3Row += deltaW3Row;
	}
}

void Renderer::BresenhamNaive(int x1, int y1, int x2, int y2)
{
	// Works for slope < 1 only

	// invert y-axis
	y1 = m_Height - y1;
	y2 = m_Height - y2;

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
	return m_Width;
}

int Renderer::GetWindowHeight()
{
	return m_Height;
}

void Renderer::Present()
{
	SDL_RenderCopy(m_Renderer, m_ColorTex, &m_ScreenRect, &m_ScreenRect);
	SDL_RenderPresent(m_Renderer);
}