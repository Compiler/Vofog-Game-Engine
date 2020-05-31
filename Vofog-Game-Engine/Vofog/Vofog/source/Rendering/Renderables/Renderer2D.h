#include <Vofog.h>
#include <Rendering/Renderables/Renderable.h>

namespace Vofog{


	class VOFOG_API Renderer2D{

	protected:
		unsigned int m_renderableCount = 0;

	public:

		virtual void submit(std::shared_ptr < Vofog::Renderable>) = 0;
		virtual void flush() = 0;

		virtual ~Renderer2D(){}


	};

}