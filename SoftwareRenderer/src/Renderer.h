#pragma once

#include "Camera.h"
#include "Color.h"
#include "Mat4x4.h"
#include "Mesh.h"
#include "Texture.h"
#include "Triangle.h"
#include "Vec2.h"
#include "Vec3.h"

enum RENDER_FLAGS
{
	RENDER_WIRE = 0x01,
	RENDER_FLAT = 0x02,
	RENDER_TEXTURED = 0x04,
};

struct Light
{
	Vec3 dir = Vec3(0.0f, 0.0f, -1.0f);
	Color color = Colors::Magenta;
};

class Renderer
{
public:
	Renderer(int width, int height);
	~Renderer();

	void SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane);
	void SetTexture(Texture& tex);
	void SetTransform(Mat4x4& world);
	void SetLightSource(const Light& light);
	void SetCamera(Vec3& position, Vec3& target, Vec3& up);

	void Render(Mesh& mesh, Camera& camera, int flags = RENDER_FLAT | RENDER_WIRE);

	int BackfaceCulling(Triangle& tri, Camera& cam, Vec3& normal);

	void ClearColor(Color color);
	void DrawColor(Color color);

	void ClearDepth();
	bool DepthTest(Triangle& tri, float alpha, float beta, float gamma, int x, int y);

	void DrawTriangle(Triangle& tri, Color color);
	void DrawTriangle(float p1X, float p1Y, float p2X, float p2Y, float p3X, float p3Y, Color color);
	void FillTriangle(Vec2 p1, Vec2 p2, Vec2 p3, Color color);
	void FillTriangleOptimized(Triangle& tri, Color color);
	void FillTriangleTextured(Triangle& tri, Texture tex);

	void DrawLine(float x1, float y1, float x2, float y2, Color color);

	int ClipAgainstPlane(Vec3 point, Vec3 plane, Triangle& in, Triangle& outTri1, Triangle& outTri2);
	void ClipAgainstScreen(std::list<Triangle>& queue, Triangle& tri);

	void Present();

	SDL_Window* GetWindow() { return m_Window; }

private:
	void DrawPixel(float x, float y, Color c);
	void DrawPixel(float x, float y);

	void BresenhamNaive(int x1, int y1, int x2, int y2);
	void BresenhamHorizontal(float x1, float y1, float x2, float y2, Color color);
	void BresenhamVertical(float x1, float y1, float x2, float y2, Color color);

private:
	int GetWindowWidth();
	int GetWindowHeight();

private:
	SDL_Renderer* m_Renderer;
	SDL_Window* m_Window;

	int m_Width, m_Height;
	SDL_Rect m_ScreenRect;

	SDL_Texture* m_ColorTex;
	uint32_t* m_ColorBuffer = nullptr;
	float* m_DepthBuffer = nullptr;

	Texture m_Texture;

	Mat4x4 m_WorldMat, m_ProjectionMat, m_ViewMat;
	float m_NearPlane = 0.1f;
	Light m_Light;
};