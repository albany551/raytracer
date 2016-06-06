
#include <gfx/mesh.hpp>
#include <core/common.hpp>
#include <core/filestream.hpp>
#include <core/filename.hpp>
#include <core/parser.hpp>
#include <math/vector2.hpp>

using namespace core;
using namespace gfx;

Mesh::Mesh() :
	boundmin(vector3f(math::FLOATMAX, math::FLOATMAX, math::FLOATMAX)),
	boundmax(vector3f(-math::FLOATMAX, -math::FLOATMAX, -math::FLOATMAX))
{
}

Mesh::~Mesh()
{

}

bool Mesh::LoadResource()
{
	FileStream file(GetName(), Stream::READ);
	if(!file.IsOpen())
		return false;

	String ext = GetFilenameEXT(GetName());
	ext.ToLower();

	// launch codec
	if(ext=="obj" || ext=="smf")
		return LoadOBJ(file);
	else if(ext=="nvx2")
		return LoadNVX2(file);

	return false;
}


int Mesh::GetVertexCount()
{
	return vertexcount;
}

float* Mesh::GetVertexDataPtr()
{
	return vertices.GetArray();
}

int Mesh::GetIndexCount()
{
	return indices.GetSize();
}

uint32* Mesh::GetIndexDataPtr()
{
	return indices.GetArray();
}


static int readIndex(const char* token)
{
	const char* slash = strchr(token,'/');
	if(slash) slash = 0;
	return atoi(token)-1;
}

// ========================================
// wavefront .obj loader
// ========================================

bool Mesh::LoadOBJ(Stream& stream)
{
	Array<vector3f> positions;
	Array<vector3f> normals;
	Array<vector2f> texcoords;

	// load the data into a buffer
	char* buffer = 0;
	int size = 0;
	size = stream.GetSize();
	buffer = new char[size];
	stream.Read(buffer,size);

	// now parse in the data
	Parser parser(buffer, size);
	while(parser.Advance())
	{
		if(strcmp(parser.GetToken(),"v")==0)
		{
			float x, y, z;
			parser.Advance(); x = static_cast<float>(Parser::Atof(parser.GetToken()));
			parser.Advance(); y = static_cast<float>(Parser::Atof(parser.GetToken()));
			parser.Advance(); z = static_cast<float>(Parser::Atof(parser.GetToken()));
			positions.PushBack(vector3f(x, y, z));
			if(x < boundmin.x) boundmin.x = x;
			if(y < boundmin.y) boundmin.y = y;
			if(z < boundmin.z) boundmin.z = z;
			if(x > boundmax.x) boundmax.x = x;
			if(y > boundmax.y) boundmax.y = y;
			if(z > boundmax.z) boundmax.z = z;
		}
		else if(strcmp(parser.GetToken(),"vn")==0)
		{
			float x, y, z;
			parser.Advance(); x = static_cast<float>(Parser::Atof(parser.GetToken()));
			parser.Advance(); y = static_cast<float>(Parser::Atof(parser.GetToken()));
			parser.Advance(); z = static_cast<float>(Parser::Atof(parser.GetToken()));
			normals.PushBack(vector3f(x, y, z));
		}
		else if(strcmp(parser.GetToken(),"vt")==0)
		{
			float u, v;
			parser.Advance(); u = static_cast<float>(Parser::Atof(parser.GetToken()));
			parser.Advance(); v = static_cast<float>(Parser::Atof(parser.GetToken()));
			texcoords.PushBack(vector2f(u, v));
		}
		else if(strcmp(parser.GetToken(),"f")==0)
		{
			parser.Advance(); indices.PushBack(readIndex(parser.GetToken()));
			parser.Advance(); indices.PushBack(readIndex(parser.GetToken()));
			parser.Advance(); indices.PushBack(readIndex(parser.GetToken()));
		}
	}
	delete[] buffer;
	
	if(normals.GetSize() == 0)
	{
		// calculate vertex normals
		normals.SetSize(positions.GetSize());
		memset(normals.GetArray(), 0, normals.GetSize() * sizeof(vector3f));

		for(int i = 0; i < indices.GetSize(); i += 3 )
		{
			// get the three vertices that comprise the face
			int i0 = indices[i+0];
			int i1 = indices[i+1];
			int i2 = indices[i+2];
			vector3f p0 = positions[i0];
			vector3f p1 = positions[i1];
			vector3f p2 = positions[i2];

			// calculate the face normal
			vector3f v0 = p1 - p0;
			vector3f v1 = p2 - p0;
			vector3f normal = Normalize(CrossProduct(v0,v1));

			// now store the face normal for each of the vertices that made up the face
			normals[i0] += normal;
			normals[i1] += normal;
			normals[i2] += normal;
		}

		// now normalize all the normals (this does the "averaging" implicitly)
		for(int i = 0; i < normals.GetSize(); i++)
			normals[i].Normalize();
	}

	// determine vertex format
	int components = 0;
	if(positions.GetSize() > 0)	
	{
		components |= VertexFormat::COORD;
	}
	if(normals.GetSize() > 0)
	{
		CORE_ASSERT(positions.GetSize() == normals.GetSize());
		components |= VertexFormat::NORMAL;
	}
	if(texcoords.GetSize() > 0)	
	{
		CORE_ASSERT(positions.GetSize() == texcoords.GetSize());
		components |= VertexFormat::UV0;
	}
	vertexformat = VertexFormat(components);

	// copy all vertex components into single array
	vertexcount = 0;
	for(int i=0; i<positions.GetSize(); i++)
	{
		vertexcount++;
		if(vertexformat.Contains(VertexFormat::COORD))
		{
			vertices.PushBack(positions[i].x);
			vertices.PushBack(positions[i].y);
			vertices.PushBack(positions[i].z);
		}
		if(vertexformat.Contains(VertexFormat::NORMAL))
		{
			vertices.PushBack(normals[i].x);
			vertices.PushBack(normals[i].y);
			vertices.PushBack(normals[i].z);
		}
		if(vertexformat.Contains(VertexFormat::UV0))
		{
			vertices.PushBack(texcoords[i].u);
			vertices.PushBack(texcoords[i].v);
		}
	}

	return true;
}

bool Mesh::LoadNVX2(Stream& stream)
{
	// read magic
	int magic = stream.ReadLittleEndian<int>();
	if (magic != 'NVX2') return false;

	// read header info
	int primitivecount		= stream.ReadLittleEndian<int>();
	vertexcount				= stream.ReadLittleEndian<int>();
	int vertexwidth			= stream.ReadLittleEndian<int>();
	int indexcount			= stream.ReadLittleEndian<int>() * 3;
	int numedges			= stream.ReadLittleEndian<int>();
	int vertexcomponents	= stream.ReadLittleEndian<int>();

	vertexformat			= VertexFormat(vertexcomponents);

	// width sanity check
	CORE_ASSERT(vertexwidth == vertexformat.GetWidth());

	// allocate storage
	//this->primitives = new Primitive[this->primitiveCount];
	vertices.SetSize(vertexcount * vertexformat.GetWidth());
	indices.SetSize(indexcount);

	// read in primitive info
	for (int i = 0; i < primitivecount; i++)
	{
		/* this->primitives[i].vertexStart = */stream.ReadLittleEndian<int>();
		/* this->primitives[i].vertexCount = */stream.ReadLittleEndian<int>();
		/* this->primitives[i].indexStart = */stream.ReadLittleEndian<int>();
		/* this->primitives[i].indexCount  = */stream.ReadLittleEndian<int>() * 3; // given in triangles 
		stream.ReadLittleEndian<int>();
		stream.ReadLittleEndian<int>();
	}

	// read triangle data
	for(int i=0; i<vertexcount * vertexformat.GetWidth(); i++)
		vertices[i] = stream.ReadLittleEndian<float>();
	for(int i=0; i<indexcount; i++)
		indices[i] = (uint32)stream.ReadLittleEndian<uint16>();

	// get bounding box
	int coordoffset = vertexformat.GetOffset(VertexFormat::COORD);
	for(int i = 0; i <vertexcount; i++)
	{
		float x = vertices[i * vertexformat.GetWidth() + coordoffset + 0];
		float y = vertices[i * vertexformat.GetWidth() + coordoffset + 1];
		float z = vertices[i * vertexformat.GetWidth() + coordoffset + 2];
		if(x < boundmin.x) boundmin.x = x;
		if(y < boundmin.y) boundmin.y = y;
		if(z < boundmin.z) boundmin.z = z;
		if(x > boundmax.x) boundmax.x = x;
		if(y > boundmax.y) boundmax.y = y;
		if(z > boundmax.z) boundmax.z = z;		
	}

	return true;
}
