
#include "Precompiled.h"
#include "TriangleRasterizer.h"

TriangleRasterizer::TriangleRasterizer()
{
	RecalcBounds();
}

TriangleRasterizer::~TriangleRasterizer()
{
	if (ContourBuffer != nullptr)
	{
		delete[] ContourBuffer;
		ContourBuffer = nullptr;
	}
}

TriangleRasterizer::TriangleRasterizer(VertexData InVertex0, VertexData InVertex1, VertexData InVertex2)
{
	VertexBuffer[0] = InVertex0;
	VertexBuffer[1] = InVertex1;
	VertexBuffer[2] = InVertex2;

	bHasColor = true;
	bHasUV = true;
		
	RecalcBounds();
}

void TriangleRasterizer::RecalcBounds()
{
	// screen bounds
	Vector2 sbbMin2D = Vector2(INFINITY, INFINITY);
	Vector2 sbbMax2D = Vector2(-INFINITY, -INFINITY);

	for (int i = 0; i < 3; i++)
	{
		if (VertexBuffer[i].Position.X < sbbMin2D.X) sbbMin2D.X = VertexBuffer[i].Position.X;
		if (VertexBuffer[i].Position.Y < sbbMin2D.Y) sbbMin2D.Y = VertexBuffer[i].Position.Y;
		if (VertexBuffer[i].Position.X > sbbMax2D.X) sbbMax2D.X = VertexBuffer[i].Position.X;
		if (VertexBuffer[i].Position.Y > sbbMax2D.Y) sbbMax2D.Y = VertexBuffer[i].Position.Y;
	}

	UVector = VertexBuffer[1].Position - VertexBuffer[0].Position;
	VVector = VertexBuffer[2].Position - VertexBuffer[0].Position;
	DotUU = UVector.Dot(UVector);
	DotUV = UVector.Dot(VVector);
	DotVV = VVector.Dot(VVector);
	InvDenom = 1.f / (DotUU * DotVV - DotUV * DotUV);

	BottomLeft = ScreenPoint(sbbMin2D.X, sbbMin2D.Y);
	TopRight = ScreenPoint(sbbMax2D.X, sbbMax2D.Y);

	if (ContourBuffer != nullptr)
	{
		delete[] ContourBuffer;
	}

	ContourBufferSize = TopRight.Y - BottomLeft.Y + 1;
	// 0번째 버퍼는 최대값 보관
	ContourBuffer = new int[ContourBufferSize * 2];

	for (int i = 0; i < ContourBufferSize; i++)
	{
		ContourBuffer[i * 2] = Math::IntMin;
	}

	// 1번째 버퍼는 최소값 보관
	for (int i = 0; i < ContourBufferSize; i++)
	{
		ContourBuffer[i * 2 + 1] = Math::IntMax;
	}

	// Generate ContourBuffer with 3 edges for Rasterization
	GenerateContourBuffer(ScreenPoint(VertexBuffer[0].Position), ScreenPoint(VertexBuffer[1].Position));
	GenerateContourBuffer(ScreenPoint(VertexBuffer[0].Position), ScreenPoint(VertexBuffer[2].Position));
	GenerateContourBuffer(ScreenPoint(VertexBuffer[1].Position), ScreenPoint(VertexBuffer[2].Position));

}

void TriangleRasterizer::GenerateContourBuffer(const ScreenPoint& InStartPos, const ScreenPoint& InEndPos)
{
	int dx = Math::Abs(InEndPos.X - InStartPos.X);
	int sx = (InStartPos.X < InEndPos.X) ? 1 : -1;
	int dy = -Math::Abs(InEndPos.Y - InStartPos.Y);
	int sy = (InStartPos.Y < InEndPos.Y) ? 1 : -1;
	int err = dx + dy;

	int currentX = InStartPos.X;
	int currentY = InStartPos.Y;

	while (true)
	{
		int currentYIndex = currentY - BottomLeft.Y;
		CheckAndSetContourValue(currentX, currentYIndex);

		int e2 = err * 2;
		if (e2 >= dy)
		{
			if (currentX == InEndPos.X)
			{
				break;
			}

			err += dy;
			currentX += sx;
			CheckAndSetContourValue(currentX, currentYIndex);
		}

		if (e2 <= dx)
		{
			if (currentY == InEndPos.Y)
			{
				break;
			}

			err += dx;
			currentY += sy;
		}

	}
}

void TriangleRasterizer::CheckAndSetContourValue(int currentX, int currentYIndex)
{
	if (currentX > ContourBuffer[currentYIndex * 2])
	{
		Vector2 testVector = ScreenPoint(currentX, currentYIndex).ToVector2();
		Vector2 st = GetBaryCentricCoord(testVector);
		if ((st.X != 0.f) && (st.Y != 0.f) && ((st.X + st.Y) != 1.f))
		{
			ContourBuffer[currentYIndex * 2] = currentX;
		}
	}

	if (currentX < ContourBuffer[currentYIndex * 2 + 1])
	{
		ContourBuffer[currentYIndex * 2 + 1] = currentX;
	}
}



