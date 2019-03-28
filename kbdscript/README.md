1. 能操作鼠标位置，能操作所有按键，能操作鼠标点击

2. 语言定义 xxx.skdb

Skbd 1 {

    Key('ABCDEFG', Shift, 100);

    keyDown(Shift'ABCDEFG');
    Sleep(100);
    KeyUp(Shift'ABCDEFG');

    Key('C');

    KeyDown('C');
    Sleep(10);
    KeyUp('C')

    KeyDown('ASEC', 100);

    KeyDown('ASEC');
    Sleep(100);

    KeyDown('S', 100);

    LButton(100, 100, 100);

    LButtonDown(100 ,100);
    Sleep(100)
    LButtonUp();

    RButton(_X,_Y,100);

    RButtonDown(_X,_Y);
    Sleep(100);
    RButtonUp();
}

register(A);
bindStop(1);
bindStart(2);


3. compile

F1 = KeyDown
F2 = Sleep
F3 = KeyUp
F4 = LButtonDown
F5 = LButtonUp
F6 = RButtonDown
F7 = RButtonUp


Header Version

//系统信息表
//表长度
int 
int //开始ascii码
int //暂停ascii码
//动作表个数
int
//动作表
int //动作ascii
int //动作总长度
//动作表
F1 int
F2 int
F3 int
F4 int int
F2 int
F5
F6 int int
F2 int 
F7
SB actionLength

