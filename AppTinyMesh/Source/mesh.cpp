#include "mesh.h"

/*!
\class Mesh mesh.h

\brief Core triangle mesh class.
*/



/*!
\brief Initialize the mesh to empty.
*/
Mesh::Mesh()
{
}

/*!
\brief Initialize the mesh from a list of vertices and a list of triangles.

Indices must have a size multiple of three (three for triangle vertices and three for triangle normals).

\param vertices List of geometry vertices.
\param indices List of indices wich represent the geometry triangles.
*/
Mesh::Mesh(const std::vector<Vector>& vertices, const std::vector<int>& indices) :vertices(vertices), varray(indices)
{
  normals.resize(vertices.size(), Vector::Z);
}

/*!
\brief Create the mesh.

\param vertices Array of vertices.
\param normals Array of normals.
\param va, na Array of vertex and normal indexes.
*/
Mesh::Mesh(const std::vector<Vector>& vertices, const std::vector<Vector>& normals, const std::vector<int>& va, const std::vector<int>& na) :vertices(vertices), normals(normals), varray(va), narray(na)
{
}

/*!
\brief Reserve memory for arrays.
\param nv,nn,nvi,nvn Number of vertices, normals, vertex indexes and vertex normals.
*/
void Mesh::Reserve(int nv, int nn, int nvi, int nvn)
{
  vertices.reserve(nv);
  normals.reserve(nn);
  varray.reserve(nvi);
  narray.reserve(nvn);
}

/*!
\brief Empty
*/
Mesh::~Mesh()
{
}

/*!
\brief Smooth the normals of the mesh.

This function weights the normals of the faces by their corresponding area.
\sa Triangle::AreaNormal()
*/
void Mesh::SmoothNormals()
{
  // Initialize 
  normals.resize(vertices.size(), Vector::Null);

  narray = varray;

  // Accumulate normals
  for (int i = 0; i < varray.size(); i += 3)
  {
    Vector tn = Triangle(vertices[varray.at(i)], vertices[varray.at(i + 1)], vertices[varray.at(i + 2)]).AreaNormal();
    normals[narray[i + 0]] += tn;
    normals[narray[i + 1]] += tn;
    normals[narray[i + 2]] += tn;
  }

  // Normalize 
  for (int i = 0; i < normals.size(); i++)
  {
    Normalize(normals[i]);
  }
}

/*!
\brief Add a smooth triangle to the geometry.
\param a, b, c Index of the vertices.
\param na, nb, nc Index of the normals.
*/
void Mesh::AddSmoothTriangle(int a, int na, int b, int nb, int c, int nc)
{
  varray.push_back(a);
  narray.push_back(na);
  varray.push_back(b);
  narray.push_back(nb);
  varray.push_back(c);
  narray.push_back(nc);
}

/*!
\brief Add a triangle to the geometry.
\param a, b, c Index of the vertices.
\param n Index of the normal.
*/
void Mesh::AddTriangle(int a, int b, int c, int n)
{
  varray.push_back(a);
  narray.push_back(n);
  varray.push_back(b);
  narray.push_back(n);
  varray.push_back(c);
  narray.push_back(n);
}

/*!
\brief Add a smmoth quadrangle to the geometry.

Creates two smooth triangles abc and acd.

\param a, b, c, d  Index of the vertices.
\param na, nb, nc, nd Index of the normal for all vertices.
*/
void Mesh::AddSmoothQuadrangle(int a, int na, int b, int nb, int c, int nc, int d, int nd)
{
  // First triangle
  AddSmoothTriangle(a, na, b, nb, c, nc);

  // Second triangle
  AddSmoothTriangle(a, na, c, nc, d, nd);
}

/*!
\brief Add a quadrangle to the geometry.

\param a, b, c, d  Index of the vertices and normals.
*/
void Mesh::AddQuadrangle(int a, int b, int c, int d)
{
  AddSmoothQuadrangle(a, a, b, b, c, c, d, d);
}

/*!
\brief Compute the bounding box of the object.
*/
Box Mesh::GetBox() const
{
  if (vertices.size() == 0)
  {
    return Box::Null;
  }
  return Box(vertices);
}

/*!
\brief Creates an axis aligned box.

The object has 8 vertices, 6 normals and 12 triangles.
\param box The box.
*/
Mesh::Mesh(const Box& box)
{
  // Vertices
  vertices.resize(8);

  for (int i = 0; i < 8; i++)
  {
    vertices[i] = box.Vertex(i);
  }

  // Normals
  normals.push_back(Vector(-1, 0, 0));
  normals.push_back(Vector(1, 0, 0));
  normals.push_back(Vector(0, -1, 0));
  normals.push_back(Vector(0, 1, 0));
  normals.push_back(Vector(0, 0, -1));
  normals.push_back(Vector(0, 0, 1));

  // Reserve space for the triangle array
  varray.reserve(12 * 3);
  narray.reserve(12 * 3);

  AddTriangle(0, 2, 1, 4);
  AddTriangle(1, 2, 3, 4);

  AddTriangle(4, 5, 6, 5);
  AddTriangle(5, 7, 6, 5);

  AddTriangle(0, 4, 2, 0);
  AddTriangle(4, 6, 2, 0);

  AddTriangle(1, 3, 5, 1);
  AddTriangle(3, 7, 5, 1);

  AddTriangle(0, 1, 5, 2);
  AddTriangle(0, 5, 4, 2);

  AddTriangle(3, 2, 7, 3);
  AddTriangle(6, 7, 2, 3);
}

Mesh::Mesh(const Disk& disk, int div)
{
    float alpha;
    float step = 2.0 * M_PI / (div);
    double r = disk.getRadius();
    Vector c = disk.getCenter();

    normals.push_back(c);
    vertices.push_back(c);
    for (int i = 0; i <= div+1; ++i) {
        alpha = i * step;
        vertices.push_back(Vector(cos(alpha) * r + c[0], c[1], sin(alpha) * r + c[2]));
        normals.push_back(Vector(cos(alpha) * r, 0, sin(alpha) * r));
        AddTriangle(0, i, i + 1 , i);
    }
}

Mesh::Mesh(const Sphere& sphere, int div)
{
    double radius = sphere.getRadius();
    vertices.push_back(Vector(0, radius, 0));
    normals.push_back(Vector(0, 1, 0));

    for (int i = 0; i < div; i++)
    {
        float theta = ((i * M_PI) / div);
        for (int j = 0; j < div; j++)
        {
            float phi = j * ((2 * M_PI) / div);
            float x = std::sin(theta) * std::cos(phi);
            float y = std::cos(theta);
            float z = std::sin(theta) * std::sin(phi);
            vertices.push_back(Vector(x * radius, y * radius, z * radius));
            normals.push_back(Vector(x * radius, y * radius, z * radius));
        }
    }

    vertices.push_back(Vector(0, -radius, 0));
    normals.push_back(Vector(0, -1, 0));

    //Haut de la sphere
    for (int i = 0; i < div; ++i)
    {
        AddTriangle(0, i + 1, i + 2, i);
    }
    AddTriangle(0, div, 1, 0);
    // entre les deux 
    for (int j = 0; j < div - 1; j++)
    {
        for (int i = 0; i < div - 1; i++)
        {
            int i0 = 1 + i + j * div;
            int i1 = 1 + i + (j + 1) * div;
            int i2 = 1 + (i + 1) + (j + 1) * div;
            int i3 = 1 + (i + 1) + j * div;
            AddTriangle(i0, i1, i2, i1);
            AddTriangle(i0, i2, i3, i2);
        }
        int i0 = div + j * div;
        int i1 = div + (j + 1) * div;
        int i2 = 1 + (j + 1) * div;
        int i3 = 1 + j * div;
        AddTriangle(i0, i1, i2, i1);
        AddTriangle(i0, i2, i3, i2);
    }
    //Bas de la sphere
    for (int i = 0; i < div; ++i)
    {
        int i0 = vertices.size() - 1 - div + i + 1;
        int i1 = vertices.size() - 1 - div + i;
        AddTriangle(vertices.size() - 1, i0, i1, i0);
    }

    AddTriangle(vertices.size() - 1, vertices.size() - 1 - div, vertices.size() - 2, vertices.size() - 1 - div);
}

Mesh::Mesh(const Cylindre& cylindre, int div) {
    float alpha;
    float step = 2.0 * M_PI / (div);
    double r = cylindre.getRadius();
    double h = cylindre.getHeight();

    normals.push_back(Vector(0, h, 0));
    vertices.push_back(Vector(0, h, 0));

    for (int i = 1; i <= div*2; ++i) {
        alpha = i * step;
        vertices.push_back(Vector(cos(alpha) * r, -h, sin(alpha) * r));
        normals.push_back(Vector(cos(alpha) * r, 0, sin(alpha) * r));
        vertices.push_back(Vector(cos(alpha) * r, h, sin(alpha) * r));
        normals.push_back(Vector(cos(alpha) * r, 0, sin(alpha) * r));
    }

    vertices.push_back(Vector(0, -h, 0));
    normals.push_back(Vector(0, h, 0));

    for (int i = 1; i <= div*2; i++) {
        AddTriangle(i, i+1, i+2, i+1);
    }

    for (int i = 2; i <= div * 2; i += 2) {
        AddTriangle(0, i, i + 2, i);
    }
    
    for (int i = 1; i <= div * 2; i += 2) {
        AddTriangle(vertices.size() - 1, i, i + 2, i);
    }



}


Mesh::Mesh(const Tore& tore, int divR, int divT)
{

    for (float i = 0; i < divR; i++) {
        for (float j = 0; j < divT; j++) {
            float u = j / divT * M_PI * 2.0;
            float v = i / divR * M_PI * 2.0;
            float x = (tore.getRadius() + tore.getThickness() * std::cos(v)) * std::cos(u);
            float y = (tore.getRadius() + tore.getThickness() * std::cos(v)) * std::sin(u);
            float z = tore.getThickness() * std::sin(v);
            vertices.push_back(Vector(x, y, z));
            normals.push_back(Vector(x, y, z));
        }
    }

    for (int i = 0; i < divR; i++) {
        int i_next = (i + 1) % divR;
        for (int j = 0; j < divT; j++) {
            int j_next = (j + 1) % divT;
            int i0 = i * divT + j;
            int i1 = i * divT + j_next;
            int i2 = i_next * divT + j_next;
            int i3 = i_next * divT + j;
            AddQuadrangle(i0, i1, i2, i3);
        }
    }
}

Mesh::Mesh(const Capsule& capsule, int div)
{
    vertices.resize(1);
    float alpha;
    float step = 2.0 * M_PI / (div);
    double r = capsule.getRadius();
    double h = capsule.getHeight();

    for (int i = 1; i <= div * 2; ++i) {
        alpha = i * step;
        vertices.push_back(Vector(cos(alpha) * r, -h, sin(alpha) * r));
        normals.push_back(Vector(cos(alpha) * r, 0, sin(alpha) * r));
        vertices.push_back(Vector(cos(alpha) * r, h, sin(alpha) * r));
        normals.push_back(Vector(cos(alpha) * r, 0, sin(alpha) * r));
    }

    for (int i = 1; i <= div * 2; i++) {
        AddTriangle(i, i + 1, i + 2, i + 1);
    }

    int ii = vertices.size();

    int n_slices = div;
    int n_stacks = div;

    for (int i = 0; i < n_stacks; i++)
    {
        float theta = ((i * M_PI) / n_stacks);
        for (int j = 0; j < n_slices; j++)
        {
            float phi = j * ((2 * M_PI) / n_slices);
            float x = std::sin(theta) * std::cos(phi);
            float y = std::cos(theta);
            float z = std::sin(theta) * std::sin(phi);
            vertices.push_back(Vector(x * r, y * r, z * r));
            normals.push_back(Vector(x * r, y * r, z * r));
        }
    }

    for (int j = 0; j < n_stacks - 1; j++)
    {
        for (int i = 0; i < n_slices - 1; i++)
        {
            int i0 = 1 + i + j * n_slices;
            int i1 = 1 + i + (j + 1) * n_slices;
            int i2 = 1 + (i + 1) + (j + 1) * n_slices;
            int i3 = 1 + (i + 1) + j * n_slices;
            AddTriangle(i0 + ii, i1 + ii, i2 + ii, i1 + ii);
            AddTriangle(i0 + ii, i2 + ii, i3 + ii, i2 + ii);
        }
        int i0 = n_slices + j * n_slices;
        int i1 = n_slices + (j + 1) * n_slices;
        int i2 = 1 + (j + 1) * n_slices;
        int i3 = 1 + j * n_slices;
        AddTriangle(i0 + ii, i1 + ii, i2 + ii, i1 + ii);
        AddTriangle(i0 + ii, i2 + ii, i3 + ii, i2 + ii);
    }
}

Mesh::Mesh(HeightField hf) {
    for (int i = 0; i <= hf.getM(); i++) {
        for (int j = 0; j <= hf.getN(); j++) {
            vertices.push_back(Vector(i, j, hf.getHeight(i,j)));
            normals.push_back(Vector(hf.getNormal(i,j)));
        }
    }

    int step = 0;
    for (int i = 0; i < hf.getM(); i+=1) {
        for (int j = 0; j < hf.getN(); j += 1) {
            AddQuadrangle(step, step + hf.getN() + 1,step + hf.getN() + 2, step + 1);
            step++;
        }
        step++;
    }    

}



/*!
\brief Scale the mesh.
\param s Scaling factor.
*/
void Mesh::Scale(double s)
{
    for (int i = 0; i < vertices.size(); i++)
    {
        vertices[i] *= s;
    }

    if (s < 0.0)
    {
        for (int i = 0; i < normals.size(); i++)
        {
            normals[i] = -normals[i];
        }
    }
}

void Mesh::Translation(float x, float y, float z) {
    for (int i = 0; i < vertices.size(); i++)
    {
        vertices[i] += Vector(x,y,z);
    }
}

void Mesh::SphereWarp(int h) {
    for (int i = 0; i < vertices.size(); i++)
    {
        if (Norm(Vector(h)) - Norm(vertices[i]) <= 0) {
            vertices[i] *= Vector(h);
        }
    }
}

void Mesh::modifyHeight(int x, int y, int h) {
    for (int i = 0; i < vertices.size(); ++i) {
        if (vertices[i][0] == x && vertices[i][1] == y) {
            vertices[i][2] = h;
            break;
        }
    }
}

void Mesh::Terrassement(int x, int y, int w, int h, int d) {
    for (int i = 0; i < vertices.size(); ++i) {
        if (vertices[i][0] == x && vertices[i][1] == y) {
            for (int ii = x-d; ii <x+d; ++ii) {
                for (int jj = y-d; jj < y+d; ++jj) {
                    modifyHeight(ii, jj, h);
                }
            }
            break;
        }
    }
}

void Mesh::RotaionX(double deg) {
    double rad = Math::DegreeToRadian(deg);
    Matrix m;
    m.tab[1][1] = cos(rad);
    m.tab[2][2] = cos(rad);

    m.tab[1][2] = sin(rad);
    m.tab[2][1] = -sin(rad);

    for (int i = 0; i < vertices.size(); i++)
    {
        vertices[i] = m * vertices[i];
    }
    for (int i = 0; i < normals.size(); i++)
    {
        normals[i] = m * normals[i];
    }
}
void Mesh::RotaionY(double deg) {
    double rad = Math::DegreeToRadian(deg);
    Matrix m;
    m.tab[0][0] = cos(rad);
    m.tab[2][2] = cos(rad);

    m.tab[2][0] = sin(rad);
    m.tab[0][2] = -sin(rad);

    for (int i = 0; i < vertices.size(); i++)
    {
        vertices[i] = m * vertices[i];
    }
    for (int i = 0; i < normals.size(); i++)
    {
        normals[i] = m * normals[i];
    }
}
void Mesh::RotaionZ(double deg) {
    double rad = Math::DegreeToRadian(deg);
    Matrix m;
    m.tab[0][0] = cos(rad);
    m.tab[1][1] = cos(rad);

    m.tab[0][1] = sin(rad);
    m.tab[1][0] = -sin(rad);

    for (int i = 0; i < vertices.size(); i++)
    {
        vertices[i] = m * vertices[i];
    }
    for (int i = 0; i < normals.size(); i++)
    {
        normals[i] = m * normals[i];
    }
}

void Mesh::Merge(Mesh &m) {
    for (int i = 0; i < m.varray.size(); i++)
    {
        varray.push_back(vertices.size() + m.varray[i]);
    }
    for (int i = 0; i < m.narray.size(); i++)
    {
        narray.push_back(normals.size() + m.narray[i]);
    }
    for (int i = 0; i < m.vertices.size(); i++)
    {
        vertices.push_back(m.vertices[i]);
    }
    for (int i = 0; i < m.normals.size(); i++)
    {
        normals.push_back(m.normals[i]);
    }

}

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QRegularExpression>
#include <QtCore/qstring.h>

/*!
\brief Import a mesh from an .obj file.
\param filename File name.
*/
void Mesh::Load(const QString& filename)
{
  vertices.clear();
  normals.clear();
  varray.clear();
  narray.clear();

  QFile data(filename);

  if (!data.open(QFile::ReadOnly))
    return;
  QTextStream in(&data);

  // Set of regular expressions : Vertex, Normal, Triangle
  QRegularExpression rexv("v\\s*([-|+|\\s]\\d*\\.\\d+)\\s*([-|+|\\s]\\d*\\.\\d+)\\s*([-|+|\\s]\\d*\\.\\d+)");
  QRegularExpression rexn("vn\\s*([-|+|\\s]\\d*\\.\\d+)\\s*([-|+|\\s]\\d*\\.\\d+)\\s*([-|+|\\s]\\d*\\.\\d+)");
  QRegularExpression rext("f\\s*(\\d*)/\\d*/(\\d*)\\s*(\\d*)/\\d*/(\\d*)\\s*(\\d*)/\\d*/(\\d*)");
  while (!in.atEnd())
  {
    QString line = in.readLine();
    QRegularExpressionMatch match = rexv.match(line);
    QRegularExpressionMatch matchN = rexn.match(line);
    QRegularExpressionMatch matchT = rext.match(line);
    if (match.hasMatch())//rexv.indexIn(line, 0) > -1)
    {
      Vector q = Vector(match.captured(1).toDouble(), match.captured(2).toDouble(), match.captured(3).toDouble()); vertices.push_back(q);
    }
    else if (matchN.hasMatch())//rexn.indexIn(line, 0) > -1)
    {
      Vector q = Vector(matchN.captured(1).toDouble(), matchN.captured(2).toDouble(), matchN.captured(3).toDouble());  normals.push_back(q);
    }
    else if (matchT.hasMatch())//rext.indexIn(line, 0) > -1)
    {
      varray.push_back(matchT.captured(1).toInt() - 1);
      varray.push_back(matchT.captured(3).toInt() - 1);
      varray.push_back(matchT.captured(5).toInt() - 1);
      narray.push_back(matchT.captured(2).toInt() - 1);
      narray.push_back(matchT.captured(4).toInt() - 1);
      narray.push_back(matchT.captured(6).toInt() - 1);
    }
  }
  data.close();
}

/*!
\brief Save the mesh in .obj format, with vertices and normals.
\param url Filename.
\param meshName %Mesh name in .obj file.
*/
void Mesh::SaveObj(const QString& url, const QString& meshName) const
{
  QFile data(url);
  if (!data.open(QFile::WriteOnly))
    return;
  QTextStream out(&data);
  out << "g " << meshName << Qt::endl;
  for (int i = 0; i < vertices.size(); i++)
    out << "v " << vertices.at(i)[0] << " " << vertices.at(i)[1] << " " << vertices.at(i)[2] << QString('\n');
  for (int i = 0; i < normals.size(); i++)
    out << "vn " << normals.at(i)[0] << " " << normals.at(i)[1] << " " << normals.at(i)[2] << QString('\n');
  for (int i = 0; i < varray.size(); i += 3)
  {
    out << "f " << varray.at(i) + 1 << "//" << narray.at(i) + 1 << " "
      << varray.at(i + 1) + 1 << "//" << narray.at(i + 1) + 1 << " "
      << varray.at(i + 2) + 1 << "//" << narray.at(i + 2) + 1 << " "
      << "\n";
  }
  out.flush();
  data.close();
}

