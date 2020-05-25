#include <vector>
#include <fstream>
#include <math.h>

#pragma once

using namespace std;

template<typename T> class Vector3D
{

public:
    Vector3D() {};
    Vector3D(const T& val0, const T& val1, const T& val2) 
    {
        data[0] = val0; data[1] = val1; data[2] = val2;
    }
    Vector3D(const Vector3D<T>& other) 
    {
        for (size_t i = 0; i < 3; i++) data[i] = other[i];
    }

    Vector3D<T>& operator= (const Vector3D<T>& other) 
    {
        for (size_t i = 0; i < 3; i++) data[i] = other[i];
        return *this;
    }

    T& operator[](int i) 
    {
        return data[i];
    }

    const T& operator[](int i) const
    {
        return data[i];
    }

    Vector3D<T> operator+ (const Vector3D<T>& other) 
    {
        Vector3D<T> res;
        for (size_t i = 0; i < 3; i++) res[i] = data[i] + other[i];
        return res;
    }

    Vector3D& operator+=(const Vector3D<T>& other) 
    {
        for (size_t i = 0; i < 3; i++) data[i] += other[i];
        return *this;
    }

    Vector3D<T> operator- (const Vector3D<T>& other)
    {
        Vector3D<T> res;
        for (size_t i = 0; i < 3; i++) res[i] = data[i] - other[i];
        return res;
    }

    Vector3D& operator-=(const Vector3D<T>& other)
    {
        for (size_t i = 0; i < 3; i++) data[i] -= other[i];
        return *this;
    }

private:
    T data[3];
};

class SimpleMesh
{
public:
    std::vector<Vector3D<float>> vertices;         // point coordinates
    std::vector<Vector3D<float>> normals;          // point normals
    std::vector<Vector3D<int>> faces;              // triangle faces
    std::vector<Vector3D<unsigned char>> colors;   // point colors

    bool writePly(const std::string& filename)
    {
        const std::vector<Vector3D<float>>& v = vertices;
        FILE* fp = fopen(filename.c_str(), "wb");
        if (!fp) return false;
        bool have_color = (colors.size() == vertices.size());
        bool have_normals = (normals.size() == vertices.size());
        fprintf(fp, "ply\nformat binary_little_endian 1.0\ncomment file created by arc3d!\n");
        fprintf(fp, "element vertex %d\n", v.size());
        fprintf(fp, "property float x\nproperty float y\nproperty float z\n");
        if (have_color)fprintf(fp, "property uchar red\nproperty uchar green\nproperty uchar blue\n");
        if (have_normals) fprintf(fp, "property float nx\nproperty float ny\nproperty float nz\n");
        if (!faces.empty()) {
            fprintf(fp, "element face %d\n", faces.size());
            fprintf(fp, "property list int int vertex_indices\n");
        }
        fprintf(fp, "end_header\n");
        fflush(fp);
        //v
        for (int i = 0; i < v.size(); ++i) {
            fwrite(&v[i], sizeof(Vector3D<float>), 1, fp);
            if (have_color) fwrite(&colors[i], sizeof(Vector3D<unsigned char>), 1, fp);
            if (have_normals) fwrite(&normals[i], sizeof(Vector3D<float>), 1, fp);
        }
        //f
        for (int i = 0; i < faces.size(); ++i) {
            int temp_face[] = { 3, faces[i][0], faces[i][1], faces[i][2] };
            fwrite(temp_face, sizeof(int), 4, fp);
        }
        fclose(fp);
        return true;
    }
};

void generateRecBars(
    SimpleMesh& mesh, 
    Vector3D<float>& center,
    float radius, 
    float height, 
    float width, 
    int num_bars, 
    Vector3D<unsigned char> default_color = Vector3D<unsigned char>(255, 0, 0))
{
    float half_height = height / 2.f, half_width = width / 2.f;
    Vector3D<float> points[4] = {
        Vector3D<float>(radius, -half_width, -half_height),
        Vector3D<float>(radius, half_width, -half_height),
        Vector3D<float>(radius, half_width, half_height),
        Vector3D<float>(radius, -half_width, half_height)
    };

    mesh.vertices.clear();
    mesh.faces.clear();
    float delta_theta = M_PI * 2.f / float(num_bars);
    for (size_t k = 0; k < num_bars; k++) {
        float theta = k * delta_theta;
        float cos = cosf(theta);
        float sin = sinf(theta);
        for (size_t i = 0; i < 4; i++) {
            Vector3D<float> p;
            p[0] = cos * points[i][0] - sin * points[i][1];
            p[1] = sin * points[i][0] + cos * points[i][1];
            p[2] = points[i][2];
            mesh.vertices.push_back(center + p);
            mesh.colors.push_back(default_color);
        }
        mesh.faces.push_back(Vector3D<int>(4 * k, 4 * k + 1, 4 * k + 2));
        mesh.faces.push_back(Vector3D<int>(4 * k + 2, 4 * k + 3, 4 * k));
    }
}

void reloadMeshToGL(){};

void drawMesh(){};

int test_main()
{
	// in preview, you can get object's position/size to set bar_center, bar_height, bar_radius, bar_width
	
	bool start_scanning = true;
	bool stop_scanning = true;
	
	if(start_scanning){
		// step1, generate rectangle bars, the bars is located at world coordinate
		
		SimpleMesh mesh_bars;
		Vector3D<float> bar_center(0.f, 0.f, 0.05f);
        float bar_radius = 0.12f, bar_height = 0.02f, bar_width = 0.01f;
		int num_bars = 16;
        generateRecBars(mesh_bars, bar_center, bar_radius, bar_height, bar_width, num_bars);
		mesh_bars.writePly("/sdcard/rect.ply");

		return 0;
	}
	
	/*vector<bool> bar_visited(num_bars, false);
	bool mesh_need_update = false;
	while(!stop_scanning)
	{
		// for kth frame, you will get the pose, which is a 4x4 matrix
		float pose[16] = {0};
		float theta = atan2f(pose[7], pose[3]);
        while (theta < std::numeric_limits<float>::epsilon()) theta += M_PI * 2.f; // clamp theta to [0~2pi]
        while (theta > M_PI * 2.f) theta -= M_PI * 2.f;
		
		float delta_theta = M_PI * 2.f / num_bars_;
        int bar_id = int(theta / delta_theta + 0.5f) % num_bars_;
        if (!bar_visited_[bar_id]) { 
            for (size_t k = 0; k < 4; k++) mesh_bars.colors[4 * bar_id + k] = Vector3D<unsigned char>(0,255,0);
            bar_visited_[bar_id] = true;
            mesh_need_update = true;
        }
		
		// then draw the mesh_bars on screen
		{
			if(mesh_need_update_) { // when mesh_bars color is updated, you should rolad the mesh_bars to your mesh render
				reloadMeshToGL(); // you can only reload mesh's vertex color for time efficiency
				mesh_need_update = false;
			}
			
			drawMesh();
		}
		
		// 
		
	}*/
    return 0;
}