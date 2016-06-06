
#ifndef GFX_MESH_HPP
#define GFX_MESH_HPP

#include <core/resource.hpp>
#include <core/array.hpp>
#include <core/stream.hpp>
#include <math/vector3.hpp>
#include <gfx/vertexformat.hpp>

namespace gfx
{
	using core::Array;
	using core::Resource;
	using core::Stream;

	class Mesh : public Resource
    {
        public:

		Mesh();
		~Mesh();

		virtual void	Render(int group) = 0;

		bool			LoadResource();

		int				GetVertexCount();
		float*			GetVertexDataPtr();
		int				GetIndexCount();
		uint32*			GetIndexDataPtr();

		const vector3f&	GetBoundMin() const { return boundmin; }
		const vector3f&	GetBoundMax() const { return boundmax; }

		VertexFormat	GetVertexFormat() const { return vertexformat; }

		// put vertex elements into single array
		// static InterlaceArrays(result vertices, const coords, const normals)

		// calculate normals
		// static CalculateNormals(const indices, const vertices, (result) normals)
	
		protected:

		bool			LoadOBJ(Stream& stream);
		bool			LoadNVX2(Stream& stream);

		Array<uint32>	indices;
		Array<float>	vertices;
		VertexFormat	vertexformat;
		int				vertexcount;

		vector3f		boundmin;
		vector3f		boundmax;
    
    };

} // namespace gfx

#endif
