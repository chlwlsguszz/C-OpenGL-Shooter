#include <GL/glut.h>
#include <math.h>
#include <vector>
using namespace std;

int MX = 300;
int MY = 300;
float time = 0;
float x = 0, y = 0; // 군인의 좌표(전역변수)

int tankFire = 0; // timer 함수와 연동 , 일정 시간마다 발사

int soldier_hp = 30; // 군인 체력
int soldierHitFlag = 0; // 한대만 맞기 위함

int tank_hp = 1000; // 탱크 체력

float lineZ = 0; // 도로 라인 좌표

// 색상을 주기 위한 열거형 변수
enum model {
	model_asphalt, model_soldier, model_tank, model_line, model_bullet
};

class vec3 {
public:
	float m[3];
	vec3(float x = 0, float y = 0, float z = 0) {
		m[0] = x; m[1] = y; m[2] = z;
	}
	vec3 operator+(vec3 x) {
		return vec3(m[0] + x.m[0],
			m[1] + x.m[1],
			m[2] + x.m[2]);
	}
	vec3 operator-(vec3 x) {
		return vec3(m[0] - x.m[0],
			m[1] - x.m[1],
			m[2] - x.m[2]);
	}

	vec3 operator*(float x) {
		return vec3(m[0] * x,
			m[1] * x,
			m[2] * x);
	}
	float Length() {
		return sqrtf(m[0] * m[0] + m[1] * m[1] + m[2] * m[2]);
	}
};
class Bullet {
public:
	vec3 v;
	vec3 pos;
};
vector<Bullet> bullets;
Bullet tankShell;

//마우스 시점변환
void MyMouseMove(GLint X, GLint Y) {
	MX = X;
	MY = Y;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(MX / 300.0, MY / 300.0, 1.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);
	glutPostRedisplay();
}

void MySpecial(int key, int X, int Y) { //x,y는 군인의 현재 위치
	if (key == GLUT_KEY_LEFT) {
		x -= 0.5f;
	}
	if (key == GLUT_KEY_RIGHT) {
		x += 0.5f;
	}
	glutPostRedisplay();
}

void MyKeyboard(unsigned char KeyPressed, int X, int Y) {
	if (KeyPressed == ' ') { //spacebar 
		Bullet b;
		b.pos.m[0] = x; // 군인의 좌표 x, y
		b.pos.m[1] = 5.5;
		b.pos.m[2] = y + 9.5;
		b.v.m[0] = 0;
		b.v.m[1] = 0;
		b.v.m[2] = -5.0;
		bullets.push_back(b);
		glutPostRedisplay();
	}
}

// 타이머 함수
// 탱크와 도로 라인을 움직이고, 탱크 포탄 조절
void MyTimer(int v) {
	time += 30.0;
	lineZ += 30;
	if (lineZ >= 10000)
		lineZ = 0;
	tankFire += 1;
	if (tankFire >= 300) {
		tankFire = 0;
		soldierHitFlag = 0;
		//(sin(time * 0.0001) * 7, 1.0, -10);
		tankShell.pos.m[0] = sin(time * 0.0001) * 7; // 탱크 좌표
		tankShell.pos.m[1] = 6.0;
		tankShell.pos.m[2] = -2;
		tankShell.v.m[0] = 0;
		tankShell.v.m[1] = 0;
		tankShell.v.m[2] = 3.0;
		glutPostRedisplay();
	}
	glutPostRedisplay();
	glutTimerFunc(10, MyTimer, 0);
}

void MyReshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-15.0, 15.0, -15.0, 15.0, -100.0, 100.0); // 15가 좋은 view
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(MX / 300.0, MY / 300.0, 1.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);  //시점변환 
}

// 모델 이름을 주면 색상 변경
void InitLight(model modelname) {
	GLfloat light_specular[] = { 1.0, 0.8, 1.0, 1.0 };
	GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat light_position[] = { -3, 6, 3.0, 0.0 };
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);

	GLfloat asphalt_mat_diffuse[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat asphalt_mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat asphalt_mat_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat asphalt_mat_shininess[] = { 15.0 };

	GLfloat soldier_mat_diffuse[] = { 0.1, 0.8, 0.1, 1.0 };
	GLfloat soldier_mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat soldier_mat_ambient[] = { 0.1, 0.8, 0.1, 1.0 };
	GLfloat soldier_mat_shininess[] = { 15.0 };

	GLfloat tank_mat_diffuse[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat tank_mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat tank_mat_ambient[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat tank_mat_shininess[] = { 15.0 };

	GLfloat line_mat_diffuse[] = { 1, 1, 1, 1.0 };
	GLfloat line_mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat line_mat_ambient[] = { 1, 1, 1, 1.0 };
	GLfloat line_mat_shininess[] = { 15.0 };

	GLfloat bullet_mat_diffuse[] = { 0.5, 0.5, 0, 1.0 };
	GLfloat bullet_mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat bullet_mat_ambient[] = { 1.0, 1.0, 0, 1.0 };
	GLfloat bullet_mat_shininess[] = { 15.0 };

	// 물체의 이름으로 색상을 지정하기
	switch (modelname) {
	case model_asphalt:
		glMaterialfv(GL_FRONT, GL_DIFFUSE, asphalt_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, asphalt_mat_specular);
		glMaterialfv(GL_FRONT, GL_AMBIENT, asphalt_mat_ambient);
		glMaterialfv(GL_FRONT, GL_SHININESS, asphalt_mat_shininess);
		break;
	case model_soldier:
		glMaterialfv(GL_FRONT, GL_DIFFUSE, soldier_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, soldier_mat_specular);
		glMaterialfv(GL_FRONT, GL_AMBIENT, soldier_mat_ambient);
		glMaterialfv(GL_FRONT, GL_SHININESS, soldier_mat_shininess);
		break;
	case model_tank:
		glMaterialfv(GL_FRONT, GL_DIFFUSE, tank_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, tank_mat_specular);
		glMaterialfv(GL_FRONT, GL_AMBIENT, tank_mat_ambient);
		glMaterialfv(GL_FRONT, GL_SHININESS, tank_mat_shininess);
		break;
	case model_line:
		glMaterialfv(GL_FRONT, GL_DIFFUSE, line_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, line_mat_specular);
		glMaterialfv(GL_FRONT, GL_AMBIENT, line_mat_ambient);
		glMaterialfv(GL_FRONT, GL_SHININESS, line_mat_shininess);
		break;
	case model_bullet:
		glMaterialfv(GL_FRONT, GL_DIFFUSE, bullet_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, bullet_mat_specular);
		glMaterialfv(GL_FRONT, GL_AMBIENT, bullet_mat_ambient);
		glMaterialfv(GL_FRONT, GL_SHININESS, bullet_mat_shininess);
		break;
	}

	glEnable(GL_NORMALIZE); // 조명 계산할 때, 법선 벡터를 다시 계산해라
}

bool tankHitCheck() { // 총알과 tank의 충돌이면 true리턴
	vec3 tank(sin(time * 0.0001) * 7, 1.0, -10);
	int size = bullets.size();
	for (int i = 0; i < size; i++) {
		vec3 diff = tank - bullets[i].pos;
		if (diff.Length() < 5) {
			bullets.erase(bullets.begin() + i);
			return true;
		}
	}
	return false;
}

// 군인이 맞았는지 확인
bool soldierHitCheck() {
	vec3 soldier(x, 7, 12);
	vec3 diff = soldier - tankShell.pos;
	if (diff.Length() < 1.5) {
		return true;
	}
	return false;
}

// 전차 포탄 갱신
void UpdateTankShell() {
	float tankShellTime = 0.1;
	tankShell.pos = tankShell.pos + tankShell.v * tankShellTime;
}

// 전차 포탄 그림
void DrawTankShell() {
	UpdateTankShell(); 
	InitLight(model_asphalt);
	glPushMatrix();
	glTranslatef(tankShell.pos.m[0], tankShell.pos.m[1], tankShell.pos.m[2]);
	glutSolidSphere(0.5, 20, 20);
	glPopMatrix();
}

// 총알의 위치를 갱신
void UpdateBullets() {
	float bulletTime = 0.1;

	int size = bullets.size();
	for (int i = 0; i < size; i++) {

		bullets[i].pos = bullets[i].pos + bullets[i].v * bulletTime;
	}
	for (int i = 0; i < bullets.size(); ) {
		if (bullets[i].pos.m[2] > 17 || bullets[i].pos.m[2] < -17) 
			bullets.erase(bullets.begin() + i);
		else
			i++;
	}
}

// 총알을 그림
void DrawBullets() {
	UpdateBullets(); 
	int size = bullets.size();
	InitLight(model_bullet);
	for (int i = 0; i < size; i++) {
		glPushMatrix();
		glTranslatef(bullets[i].pos.m[0], bullets[i].pos.m[1], bullets[i].pos.m[2]);
		glScalef(0.1, 0.1, 2);
		glutSolidCube(1);
		glutSolidSphere(0.1, 20, 20);
		glPopMatrix();
	}
}

void DrawTank() {
	glPushMatrix();

	InitLight(model_tank);
	glTranslatef(0, 8, 0); // 탱크 바닥 위치

	glPushMatrix();
	glScalef(5, 2.5, 8);
	glutSolidCube(3); // 탱크 아랫부분
	glPopMatrix();

	glTranslatef(0, 5, 0);
	glPushMatrix(); // 탱크 윗부분
	glScalef(3, 3, 4);
	glutSolidCube(3);
	glPopMatrix();
	

	//포신
	InitLight(model_asphalt);
	glPushMatrix();
	glTranslatef(0, 2, 7);
	glRotatef(180, 0, 1, 0);
	glPushMatrix();
		glScalef(1, 1, 10);
		glutSolidCube(2);
	glPopMatrix();
	glTranslatef(0, 0, -10);
	glPushMatrix();
		glScalef(1.2, 1.2, 1.6);
		glutSolidCube(2);
	glPopMatrix();
	glPopMatrix();

	// 오른쪽 바퀴
	for (int i = 0; i < 5; i++) {
		glPushMatrix();
		glTranslatef(5, -9, 7-i*4);
		glScalef(0.7, 1, 1);
		glRotatef(time*0.1, 1.0, 0, 0);
		glutWireSphere(3, 30, 30);
		glPopMatrix();
	}

	// 왼쪽 바퀴
	for (int i = 0; i < 5; i++) {
		glPushMatrix();
		glTranslatef(-5, -9, 7 - i * 4);
		glScalef(0.7, 1, 1);
		glRotatef(time*0.1, 1.0, 0, 0);
		glutWireSphere(3, 30, 30);
		glPopMatrix();
	}
	glPopMatrix();
}

void DrawSoldier() {
	glPushMatrix();
	InitLight(model_soldier);

	glTranslatef(0, 9, 0); // 군인 바닥 위치
	
	// 몸통
	glPushMatrix();
		glScalef(2, 5, 2);
		glutSolidCube(1);
	glPopMatrix();

	//오른쪽
	glPushMatrix(); // 팔  
		glTranslatef(1, 0, 0);
		glRotatef(-50 + sin(time * 0.005) * 5, 1, 0, 0);
		glTranslatef(0, 2.5, 0);
		glPushMatrix();
			glScalef(0.1, 1, 0.1);
			glutSolidCube(3);
		glPopMatrix();
	// 팔목
		glTranslatef(0, 1.5, 0);
		glRotatef(15, 0, 0, 1);
		glTranslatef(0, 1.5, 0);
		glPushMatrix();
			glScalef(0.1, 1, 0.1);
			glutSolidCube(3);
		glPopMatrix();

		//손
		glTranslatef(0, 1.8, 0);
		glPushMatrix();
			glScalef(1, 1, 1);
			glutSolidCube(0.5);
		glPopMatrix();
		// 총, 오른손에 붙어있다
		InitLight(model_asphalt);

		glRotatef(-15, 0, 0, 1);
		glPushMatrix();
			glRotatef(45, 1, 0, 0);
			glScalef(1, 2, 1);
			glutSolidCube(0.5);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0, 0.7, -0.3);
			glRotatef(-45, 1, 0, 0);
			glScalef(1, 3, 1);
			glutSolidCube(0.5);
		glPopMatrix();
		InitLight(model_soldier);
	glPopMatrix();

	//왼쪽
	glPushMatrix(); // 팔  
	glTranslatef(-1, 0, 0);
	glRotatef(-50 + sin(time * 0.005) * 5, 1, 0, 0);
	glTranslatef(0, 2.5, 0);
	glPushMatrix();
	glScalef(0.1, 1, 0.1);
	glutSolidCube(3);
	glPopMatrix();
	// 팔목
	glTranslatef(0, 1.5, 0);
	glRotatef(-15, 0, 0, 1);
	glTranslatef(0, 1.5, 0);
	glPushMatrix();
	glScalef(0.1, 1, 0.1);
	glutSolidCube(3);
	glPopMatrix();
	//손
	glTranslatef(0, 1.8, 0);
	glPushMatrix();
	glScalef(1, 1, 1);
	glutSolidCube(0.5);
	glPopMatrix();
	glPopMatrix();

	//오른쪽 다리
	glPushMatrix();
		glTranslatef(0.5, -2, 0); 
		glRotatef(20 + sin(time * 0.005) * 45, 1, 0, 0);
		glTranslatef(0, -1, 0);
		glPushMatrix();
			glScalef(0.3, 1, 0.3);
			glutSolidCube(3);
		glPopMatrix();
		glPushMatrix();
		//종아리
			glTranslatef(0, -2, 1);
			glRotatef(sin(time * 0.005) * 15 - 60, 1, 0, 0);
			glPushMatrix();
				glScalef(0.3, 1, 0.3);
				glutSolidCube(3);
			glPopMatrix();

			//발
			glTranslatef(0, -1.7, -1);
			glPushMatrix();
				glScalef(0.3, 0.1, 0.5);
				glutSolidCube(4);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();

	//왼쪽 다리
	glPushMatrix();
	glTranslatef(-0.5, -2, 0);
	glRotatef(20 + -sin(time * 0.005) * 45, 1, 0, 0);
	glTranslatef(0, -1, 0);
	glPushMatrix();
	glScalef(0.3, 1, 0.3);
	glutSolidCube(3);
	glPopMatrix();
	glPushMatrix();
	//종아리
	glTranslatef(0, -2, 1);
	glRotatef(sin(time * 0.005) * 15 - 60, 1, 0, 0);
	glPushMatrix();
	glScalef(0.3, 1, 0.3);
	glutSolidCube(3);
	glPopMatrix();

	//발
	glTranslatef(0, -1.7, -1);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.5);
	glutSolidCube(4);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	// 머리
	glRotatef(sin(time * 0.002) * 5, 1, 0, 0);
	glTranslatef(0, 5, 0);
	glutSolidSphere(2, 20, 20); 
	glPopMatrix();
}

void drawAsphalt() {
	InitLight(model_asphalt);
	glPushMatrix();
	glScalef(20, 0.5, 70); // x-z방향 넓은 판
	glutSolidCube(1.0);
	glPopMatrix();

	//선 그리고 뒤로 보냄
	InitLight(model_line);
	for (int i = 0; i < 30; i++) {
		glPushMatrix();
		glTranslatef(0, 0, -55);
		glScalef(1, 0.5, 3);
		glTranslatef(0, 0.1, i*2+lineZ*0.001);
		glutSolidCube(1.0);
		glPopMatrix();
	}
}

void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 도로 (아스팔트)
	drawAsphalt();

	// 군인
	glPushMatrix();
	glTranslatef(0, 0, 12); // 기본 위치
	glTranslatef(x, 0, y); // 키보드 조작
	glScalef(0.4, 0.4, 0.4);
	DrawSoldier();
	glPopMatrix();

	// 탱크
	glPushMatrix();
	glTranslatef(0, 0, -10); // 기본 위치
	glTranslatef(sin(time * 0.0001) * 7, 0, 0); // 자동 좌우 이동
	glScalef(0.4, 0.4, 0.4);
	DrawTank();
	glPopMatrix();
	
	// 총알 그리기
	DrawBullets();
	DrawTankShell();
	glFlush();

	bool tankHit = tankHitCheck();
	if (tankHit) {
		printf("hit!\n");
		tank_hp -= 50; // 20대 맞추면 파괴
		if (tank_hp <= 0)
			exit(1);
	}
	bool soldierHit = soldierHitCheck();
	if (soldierHit && soldierHitFlag != 1) {
		printf("DAMAGED!\n");
		soldierHitFlag = 1; // 포탄에 한번만 맞기 위함 
		soldier_hp -= 10; // 3대 맞으면 실패
		if (soldier_hp <= 0)
			exit(1);
	}
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);               //GLUT 윈도우 함수
	glutInitWindowSize(700, 700);
	glutCreateWindow("OpenGL Drawing Example");
	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(MyReshape);
	glutTimerFunc(10,MyTimer,0);
	glutSpecialFunc(MySpecial);
	glutMotionFunc(MyMouseMove);
	glutKeyboardFunc(MyKeyboard);
	glClearColor(0.4, 0.4, 0.4, 1);
	//InitLight(model_asphalt);
	tankShell.pos.m[1] = -1;
	glutMainLoop();

	return 0;
}