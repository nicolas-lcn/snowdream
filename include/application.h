#ifndef APPLICATION_H
#define APPLICATION_H

#include "window.h"

namespace Vengine
{
	class App
	{
	public:
		static constexpr uint32_t WIDTH = 800;
		static constexpr uint32_t HEIGHT = 600;

	public:
		App(): window(Viewer::Window(WIDTH, HEIGHT, "Snow Dream")){};
		~App(){};

		void run();


	private:
		
		void initVulkan(){};

		void mainLoop();

	private:

		Viewer::Window window;

	

	};
}

#endif //APPLICATION_H