

#define MENU_ROW_HEIGHT 15
#define LCD_ROWS 8

struct screen_t
{
	char menuPos;
	char menuScreen;
	char markerPos;
	char menuStartAt;
	//char statusA;
	//char statusB;
};


//extern int menuPos, menuScreen, markerPos, menuStartAt;

void showMenu(char **item,int size,int MENU_LENGTH);
void checkmenu();



