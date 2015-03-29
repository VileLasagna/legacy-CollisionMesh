#include "Terrain.h"
#include "Tri.h"

bool Terrain::LoadHeightmap(const std::string& filename)
{
    return m_bitmap.Load(filename);
}

void Terrain::MakeMesh(CollisionMesh* pMesh, int SIZE, float M)
{
    int s = SIZE / 2;
    for (int i = -s; i < s; i++)
    {
        for (int j = -s; j < s; j++)
        {
            Tri tri;
            // NB note order so normal faces in the dir we expect (up)
            tri.m_verts[0] = Vec3f((i + 1) * M, GetY(i + 1 + s, j + 1 + s), (j + 1)* M);
            tri.m_verts[1] = Vec3f((i + 1) * M, GetY(i + 1 + s, j + s), j * M);
            tri.m_verts[2] = Vec3f(i * M, GetY(i + s, j + s), j * M);
            pMesh->AddTri(tri);

            std::swap(tri.m_verts[0], tri.m_verts[2]);
            tri.m_verts[1] = Vec3f(i * M, GetY(i + s, j + 1 + s), (j + 1) * M);
            pMesh->AddTri(tri);
        }
    }
}

void Terrain::MakeTerrain(CollisionMesh::Tris* pTris, int SIZE, float M)
{
    int s = SIZE / 2;
    for (int i = -s; i < s; i++)
    {
        for (int j = -s; j < s; j++)
        {
            Tri tri;
            // NB note order so normal faces in the dir we expect (up)
            tri.m_verts[0] = Vec3f((i + 1) * M, GetY(i + 1 + s, j + 1 + s), (j + 1)* M);
            tri.m_verts[1] = Vec3f((i + 1) * M, GetY(i + 1 + s, j + s), j * M);
            tri.m_verts[2] = Vec3f(i * M, GetY(i + s, j + s), j * M);
            //tri.MakePerpNormal();
            pTris->push_back(tri);

            std::swap(tri.m_verts[0], tri.m_verts[2]);
            tri.m_verts[1] = Vec3f(i * M, GetY(i + s, j + 1 + s), (j + 1) * M);
            //tri.MakePerpNormal();
            pTris->push_back(tri);
        }
    }
}

float Terrain::GetY(int x, int y)
{
    int r = m_bitmap.GetColourAtPoint(x, y);
    int maxColorVal = 1 << 24;
    float f = (float)(r) / (float)(maxColorVal) * 50.0f ;
    return f;
}


