
#include <gfx/opengl/glmesh.hpp>

#include <gfx/opengl/gl.hpp>

using namespace gfx;

void GlMesh::Render(int group)
{
	/*
	int stride = vertexformat.GetByteWidth();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glVertexPointer(3, GL_FLOAT, stride, vertices.GetArray() + vertexformat.GetOffset(VertexFormat::COORD));
	glNormalPointer(GL_FLOAT, stride, vertices.GetArray() + vertexformat.GetOffset(VertexFormat::NORMAL));
	glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, indices.GetArray());
	*/

	
	uint32* indices = GetIndexDataPtr();
	float* vertices = GetVertexDataPtr() + GetVertexFormat().GetOffset(VertexFormat::COORD);
	float* normals = GetVertexDataPtr() + GetVertexFormat().GetOffset(VertexFormat::NORMAL);
	float* texcoords = GetVertexDataPtr() + GetVertexFormat().GetOffset(VertexFormat::UV0);
	int stride = GetVertexFormat().GetWidth();

	glBegin(GL_TRIANGLES);
	for(int i = 0; i+3<=GetIndexCount(); i+=3)
	{
		uint32 index0 = *(indices + i + 0);
		uint32 index1 = *(indices + i + 1);
		uint32 index2 = *(indices + i + 2);

		vector3f v0(vertices[index0 * stride + 0], vertices[index0 * stride + 1], vertices[index0 * stride + 2]);
		vector3f v1(vertices[index1 * stride + 0], vertices[index1 * stride + 1], vertices[index1 * stride + 2]);
		vector3f v2(vertices[index2 * stride + 0], vertices[index2 * stride + 1], vertices[index2 * stride + 2]);

		glVertex3f(v0.x, v0.y, v0.z);
		glVertex3f(v1.x, v1.y, v1.z);
		glVertex3f(v2.x, v2.y, v2.z);

	}
	glEnd();
	

}


