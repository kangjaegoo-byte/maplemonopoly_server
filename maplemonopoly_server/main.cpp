#include "pch.h"
#include "App.h"
int main() 
{
	App::GetInstance()->Init();

	while (App::GetInstance()->Update()) {};

	App::GetInstance()->DeleteInstance();

	return 0;
}