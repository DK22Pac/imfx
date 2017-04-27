#include "ImVerts.h"

RwIm2DVertex ImVerts::m_Buffer[4];

void ImVerts::AddVertex(int id, float x, float y, float z, float rhw, unsigned char r, unsigned char g, unsigned char b, unsigned char a, float u, float v) {
	m_Buffer[id].x = x;
	m_Buffer[id].y = y;
	m_Buffer[id].z = z;
	m_Buffer[id].rhw = rhw;
	m_Buffer[id].emissiveColor = RWRGBALONG(r, g, b, a);
	m_Buffer[id].u = u;
	m_Buffer[id].v = v;
}