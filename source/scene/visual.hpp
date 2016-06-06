
#ifndef SCENE_VISUAL_HPP
#define SCENE_VISUAL_HPP

#include <scene/transformnode.hpp>
#include <math/box.hpp>
#include <math/vector4.hpp>
#include <gfx/shader.hpp>
#include <gfx/texture.hpp>

namespace gfx
{
	class Context;
}

namespace scene
{
	using gfx::Context;
	using gfx::Shader;
	using gfx::Texture;

    class Visual : public TransformNode
    {
        public:

		Visual();

		virtual void	Render(Context* context, class Camera* camera, int priority = 0) {}

		// ====================================
        // material properties
		// ====================================

		void	SetShader(Shader* fx);
		void	SetTexture(Shader::Param param, Texture* texture);
		void	SetDiffuse(const vector4f& color) { diffuse = color; }
		void	SetSpecular(const vector4f& color) { specular = color; }
		void	SetReflective(float value) { reflective = value; }
		void	SetRefractive(float value) { refractive = value; }
		void	SetRefractiveIndex(float value) { refractiveindex = value; }



		Shader*			GetShader();
		Texture*		GetTexture(Shader::Param param);
		const vector4f&	GetDiffuse() const { return diffuse; }
		const vector4f&	GetSpecular() const { return specular; }
		float			GetReflective() const { return reflective; }
		float			GetRefractive() const { return refractive; }
		float			GetRefractiveIndex() const { return refractive; }

		// ====================================
        // bound box
		// ====================================

		// set local boundbox (OBB)
		void			SetLocalBox(const box3f& box);

        // get local boundbox
		const box3f&		GetLocalBox() const { return boundbox; }

        // get global axis-aligned bounding box
		// fixme: must contain all child boxes!
		box3f			GetWorldBox();

		private:

		box3f	boundbox;

		vector4f	diffuse;
		vector4f	specular;
		float		reflective;
		float		refractive;
		float		refractiveindex;

		Ref<Shader>		shader;
		Ref<Texture>	diffmap0;
    };
}

#endif
