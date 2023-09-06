#include "qte.h"
#
#include "implicits.h"
#include "ui_interface.h"
#include "../tore.h"
#include "../capsule.h"

MainWindow::MainWindow() : QMainWindow(), uiw(new Ui::Assets)
{
	// Chargement de l'interface
    uiw->setupUi(this);

	// Chargement du GLWidget
	meshWidget = new MeshWidget;
	QGridLayout* GLlayout = new QGridLayout;
	GLlayout->addWidget(meshWidget, 0, 0);
	GLlayout->setContentsMargins(0, 0, 0, 0);
    uiw->widget_GL->setLayout(GLlayout);

	// Creation des connect
	CreateActions();

	meshWidget->SetCamera(Camera(Vector(10, 0, 0), Vector(0.0, 0.0, 0.0)));
}

MainWindow::~MainWindow()
{
	delete meshWidget;
}

void MainWindow::CreateActions()
{
	// Buttons
    connect(uiw->boxMesh, SIGNAL(clicked()), this, SLOT(BoxMeshExample()));
    connect(uiw->sphereImplicit, SIGNAL(clicked()), this, SLOT(SphereImplicitExample()));
	connect(uiw->sphereMesh, SIGNAL(clicked()), this, SLOT(SphereMeshExample()));
	connect(uiw->disqueMesh, SIGNAL(clicked()), this, SLOT(DisqueMeshExample()));
	connect(uiw->cylindreMesh, SIGNAL(clicked()), this, SLOT(CylindreMeshExample()));
	connect(uiw->toreMesh, SIGNAL(clicked()), this, SLOT(ToreMeshExample()));
	connect(uiw->capsuleMesh, SIGNAL(clicked()), this, SLOT(CapsuleMeshExample()));
	connect(uiw->terrainMesh, SIGNAL(clicked()), this, SLOT(TerrainMeshExample()));
	connect(uiw->sceneMesh, SIGNAL(clicked()), this, SLOT(SceneExample()));



    connect(uiw->resetcameraButton, SIGNAL(clicked()), this, SLOT(ResetCamera()));
    connect(uiw->wireframe, SIGNAL(clicked()), this, SLOT(UpdateMaterial()));
    connect(uiw->radioShadingButton_1, SIGNAL(clicked()), this, SLOT(UpdateMaterial()));
    connect(uiw->radioShadingButton_2, SIGNAL(clicked()), this, SLOT(UpdateMaterial()));

	// Widget edition
	connect(meshWidget, SIGNAL(_signalEditSceneLeft(const Ray&)), this, SLOT(editingSceneLeft(const Ray&)));
	connect(meshWidget, SIGNAL(_signalEditSceneRight(const Ray&)), this, SLOT(editingSceneRight(const Ray&)));
}

void MainWindow::editingSceneLeft(const Ray&)
{
}

void MainWindow::editingSceneRight(const Ray&)
{
}

void MainWindow::BoxMeshExample()
{
	Mesh boxMesh = Mesh(Box(1.0));
	std::vector<Color> cols;
	cols.resize(boxMesh.Vertexes());
    for (size_t i = 0; i < cols.size(); i++)
		cols[i] = Color(0.8, 0.8, 0.8);

	meshColor = MeshColor(boxMesh, cols, boxMesh.VertexIndexes());
	UpdateGeometry();
}

void MainWindow::SphereImplicitExample()
{
  AnalyticScalarField implicit;

  Mesh implicitMesh;
  implicit.Polygonize(31, implicitMesh, Box(2.0));

  std::vector<Color> cols;
  cols.resize(implicitMesh.Vertexes());
  for (size_t i = 0; i < cols.size(); i++)
    cols[i] = Color(0.8, 0.8, 0.8);

  meshColor = MeshColor(implicitMesh, cols, implicitMesh.VertexIndexes());
  UpdateGeometry();
}

void MainWindow::DisqueMeshExample()
{
	Mesh diskMesh = Mesh(Disk(Vector(0,1,0), 2), 50);

	std::vector<Color> cols;
	cols.resize(diskMesh.Vertexes());
	for (size_t i = 0; i < cols.size(); i++)
		cols[i] = Color(0.8, 0.8, 0.8);

	meshColor = MeshColor(diskMesh, cols, diskMesh.VertexIndexes());
	UpdateGeometry();
}

void MainWindow::SphereMeshExample()
{
	Mesh sphereMesh = Mesh(Sphere(2),50);
	//sphereMesh.SphereWarp(2);
	std::vector<Color> cols;
	cols.resize(sphereMesh.Vertexes());

	for (size_t i = 0; i < cols.size(); i++) {
		cols[i] = Color(0.8, 0.8, 0.8);
	}
	meshColor = MeshColor(sphereMesh, cols, sphereMesh.VertexIndexes());
	UpdateGeometry();
}

void MainWindow::CylindreMeshExample()
{
	Mesh cylindreMesh = Mesh(Cylindre(1, 3), 50);
	
	std::vector<Color> cols;
	cols.resize(cylindreMesh.Vertexes());
	for (size_t i = 0; i < cols.size(); i++) {
		cols[i] = Color(0.8, 0.8, 0.8);
	}
	meshColor = MeshColor(cylindreMesh, cols, cylindreMesh.VertexIndexes());
	UpdateGeometry();
}

void MainWindow::ToreMeshExample()
{
	Mesh toreMesh = Mesh(Tore(1, 0.5), 10, 10);
	std::vector<Color> cols;
	cols.resize(toreMesh.Vertexes());
	for (size_t i = 0; i < cols.size(); i++) {
		cols[i] = Color(double(i) / 6.0, fmod(double(i) * 39.478378, 1.0), 0.0);
	}
	meshColor = MeshColor(toreMesh, cols, toreMesh.VertexIndexes());
	UpdateGeometry();
}

void MainWindow::CapsuleMeshExample()
{
	Mesh capsuleMesh = Mesh(Cylindre(1, 1.5), 50);
	Mesh sphereMesh = Mesh(Sphere(1), 50);
	sphereMesh.Translation(0, 1.5, 0);
	Mesh sphereMesh2 = Mesh(Sphere(1), 50);
	sphereMesh2.Translation(0, -1.5, 0);
	capsuleMesh.Merge(sphereMesh);
	capsuleMesh.Merge(sphereMesh2);

	std::vector<Color> cols;
	cols.resize(capsuleMesh.Vertexes());
	for (size_t i = 0; i < cols.size(); i++) {

		cols[i] = Color(0.8, 0.8, 0.8);
	}
	meshColor = MeshColor(capsuleMesh, cols, capsuleMesh.VertexIndexes());
	UpdateGeometry();
}




bool castRay(const Vector& ro, const Vector& rd, float& resT, HeightField hf)
{
	const float stepValue = 0.05f;
	const float minStep = 0.001f;
	const float maxStep = 100.0f;
	float lh = 0.0f;
	float ly = 0.0f;
	for (float step = minStep; step < maxStep; step += stepValue)
	{
		Vector d = Vector(rd[0] - (ro[0] * step), rd[1] - (ro[1] * step), rd[2] - (ro[2] * step));
		Vector p = ro + d;

		const float h = hf.getHeight(p[0], p[1]);
		if (p[2] < h && h != -1)
		{
			resT = step - stepValue + stepValue * (lh - ly) / (p[2] - ly - h + lh);
			return true;
		}
		lh = h;
		ly = p[2];
	}
	return false;
}

void MainWindow::TerrainMeshExample()
{

	QImage terrainH("../Data/terrain.png");
	HeightField hf(196, 196, terrainH, 20);
	Mesh terrainMesh = Mesh(hf);
	meshWidget->SetCamera(Camera(Vector(-59, -112, 52), Vector(16, -3, 20), Vector(0.13, 0.19, 0.97)));

	//terrainMesh.Terrassement(50, 50, 100, 3, 5);
	std::vector<Color> cols;
	cols.resize(terrainMesh.Vertexes());
	
	for (size_t i = 0; i < cols.size(); i++) {
		Ray ray(Vector(-59, -112, 52), terrainMesh.Vertex(i));
		float t;
		double r, g, b;
		//r = hf.getHeight(terrainMesh.Vertex(i)[0], terrainMesh.Vertex(i)[1]) / 50;
		r,g,b = 1.0;
		Color c(hf.getHeight(terrainMesh.Vertex(i)[0], terrainMesh.Vertex(i)[1]) / 50);
		if (terrainMesh.Normal(i)[2] > 0.8) {
			//g = 0.9;
		}

		if (castRay(ray.Origin(), ray.Direction(), t, hf)) {
			float value = (t - 1) * 10000;
			if (value > 6) {
				c = Color(1.0);
			}
			else if (value > 4 && value < 6) {
				c = Color(150, 75, 0);
			}
			else if (value > 2 && value < 4) {
				c = Color(88, 86, 84);
			}
			else if (value < 2) {
				c = Color(0.0, 0.6, 0.1);
			}

			else{
				c = Color(0.0);
			}


		}
		b = 0;
		cols[i] = c;
	}
	
	meshColor = MeshColor(terrainMesh, cols, terrainMesh.VertexIndexes());
	UpdateGeometry();
}

void MainWindow::SceneExample()
{
	Mesh sceneMesh = Mesh();

	Mesh capsuleMesh = Mesh(Cylindre(1, 1.5), 50);
	Mesh sphereMesh = Mesh(Sphere(1), 50);
	sphereMesh.Translation(0, 1.5, 0);
	Mesh sphereMesh2 = Mesh(Sphere(1), 50);
	sphereMesh2.Translation(0, -1.5, 0);
	capsuleMesh.Merge(sphereMesh);
	capsuleMesh.Merge(sphereMesh2);

	Mesh toreMesh = Mesh(Tore(1.5, 0.5), 50, 50);
	toreMesh.RotaionX(90);


	Mesh diskMesh = Mesh(Disk(Vector(0, 2.78, 0), 2), 50);
	diskMesh.Scale(0.9);
	Mesh diskMesh2 = Mesh(Disk(Vector(0, -2.78, 0), 2), 50);
	diskMesh2.Scale(0.9);

	Mesh boxMesh = Mesh(Box(0.2));
	boxMesh.Translation(-1, -1, 0);
	Mesh boxMesh2 = Mesh(Box(0.2));
	boxMesh2.Translation(-1, 1, 0);

	sceneMesh.Merge(capsuleMesh);
	sceneMesh.Merge(toreMesh);
	sceneMesh.Merge(diskMesh);
	sceneMesh.Merge(diskMesh2);
	sceneMesh.Merge(boxMesh);
	sceneMesh.Merge(boxMesh2);

	std::vector<Color> cols;
	cols.resize(sceneMesh.Vertexes());
	for (size_t i = 0; i < cols.size(); i++) {
		cols[i] = Color(0.1, 0.8, 1.0);
	}
	meshColor = MeshColor(sceneMesh, cols, sceneMesh.VertexIndexes());
	UpdateGeometry();
}



void MainWindow::UpdateGeometry()
{
	meshWidget->ClearAll();
	meshWidget->AddMesh("BoxMesh", meshColor);

    uiw->lineEdit->setText(QString::number(meshColor.Vertexes()));
    uiw->lineEdit_2->setText(QString::number(meshColor.Triangles()));

	UpdateMaterial();
}

void MainWindow::UpdateMaterial()
{
    meshWidget->UseWireframeGlobal(uiw->wireframe->isChecked());

    if (uiw->radioShadingButton_1->isChecked())
		meshWidget->SetMaterialGlobal(MeshMaterial::Normal);
	else
		meshWidget->SetMaterialGlobal(MeshMaterial::Color);
}

void MainWindow::ResetCamera()
{
	meshWidget->SetCamera(Camera(Vector(-10.0), Vector(0.0)));
}
