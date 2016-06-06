
#ifndef SCENE_SHAPENODE_HPP
#define SCENE_SHAPENODE_HPP

#include <scene/visual.hpp>
#include <gfx/mesh.hpp>

namespace gfx
{
	class Context;
}

namespace scene
{
	using gfx::Context;
	using gfx::Mesh;

	class MeshNode : public Visual
    {
        public:

		MeshNode();

		void		Render(Context* context, Camera* camera, int priority = 0);

		void		SetMesh(Mesh* m);
		void		SetGroup(int index);

		Mesh*		GetMesh() { return mesh; }

		protected:

		Ref<Mesh>	mesh;
		int			group;
            
    };
    
} // namespace scene

#endif
