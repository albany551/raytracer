
#ifndef GFX_OPENGL_MESH_HPP
#define GFX_OPENGL_MESH_HPP

#include <gfx/mesh.hpp>

namespace gfx
{
	class GlMesh : public Mesh
    {
        public:

		void	Render(int group);
    
    };

} // namespace gfx

#endif
