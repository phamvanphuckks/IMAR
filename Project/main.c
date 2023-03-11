#include "app.h"

void main(void)
{
	app_init();

	while(1)
	{
		app_process_action();
	}
}