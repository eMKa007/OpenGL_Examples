
#include "libs.h"

int main()
{
	glfwInit();

	//CREATE WINDOW BEFORE GLEW INIT!
	
	glewExperimental = GL_TRUE;

	if(glewInit() != GLEW_OK)
	{
		glfwTerminate();
		std::cout << "Bad hat harry.." << "\n";
	}
	else
		std::cout << "Good.." << "\n";


	system("pause");
	return 0;
}