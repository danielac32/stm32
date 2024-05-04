



void init_buttons();
int get_a8();
int get_joystick1();
int get_joystick2();
int get_joystick3();
int get_joystick4();

#define key_up() get_joystick2()
#define key_down() get_joystick1()
#define key_left() get_joystick3()//derecha
#define key_right() get_joystick4() //izquierda

