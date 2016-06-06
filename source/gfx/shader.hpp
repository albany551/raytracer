
#ifndef GFX_SHADER_HPP
#define GFX_SHADER_HPP

#include <core/resource.hpp>
#include <math/matrix44.hpp>

namespace gfx
{
	using core::Resource;
	using core::String;

	class Shader : public Resource
	{
		public:

/*

	new nshapenode static_0_0
		sel static_0_0
		.setlocalbox -0.855658 6.673505 0.793874 4.281816 7.170653 4.482170
		.settexture "DiffMap0" "textures:buildings/burgtextur.dds"
		.settexture "BumpMap0" "textures:buildings/burg_bump.dds"
		.settexture "DiffMap1" "textures:lightmaps/examples_lightmapping_Pool1.dds"
		.setvector "MatAmbient" 0.000000 0.000000 0.000000 1.000000
		.setvector "MatDiffuse" 1.000000 1.000000 1.000000 1.000000
		.setvector "MatSpecular" 0.160580 0.160580 0.160580 1.000000
		.setfloat "MatSpecularPower" 32.000000
		.setfloat "Intensity0" 2.000000
		.setint "AlphaRef" 100
		.setint "CullMode" 2
		.setshader "colr" "shaders:default_lightmapped.fx"
		.setmesh "meshes:examples/lightmapping_s_0.nvx2"
		.setgroupindex 0
		.setmeshusage "WriteOnce|"

*/

		enum Param
		{
			VIEW,
			PROJECTION,
			DIFFMAP0,
			COUNT			// Number of different dynamic parameters
		};


		void SetVertexShader(const char* source);
		void SetFragmentShader(const char* source);

		virtual int Begin() = 0;
		virtual void End() = 0;

		virtual void SetParam(Param p, bool val) = 0;
		virtual void SetParam(Param p, int val) = 0;
		virtual void SetParam(Param p, float val) = 0;
		virtual void SetParam(Param p, const vector3f& val) = 0;
		virtual void SetParam(Param p, const matrix44f& val) = 0;

		virtual void SetParam(const char* name, bool val) = 0;
		virtual void SetParam(const char* name, int val) = 0;
		virtual void SetParam(const char* name, float val) = 0;
		virtual void SetParam(const char* name, const vector3f& val) = 0;
		virtual void SetParam(const char* name, const matrix44f& val) = 0;

		protected:

		String sourcevs;
		String sourcefs;

	};

} // namespace gfx

#endif
