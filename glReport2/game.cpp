#include <GL/glut.h>
#include <math.h>
#include <vector>
using namespace std;

int MX = 300;
int MY = 300;
float time = 0;
float x = 0, y = 0; // ������ ��ǥ(��������)

int tankFire = 0; // timer �Լ��� ���� , ���� �ð����� �߻�

int soldier_hp = 30; // ���� ü��
int soldierHitFlag = 0; // �Ѵ븸 �±� ����

int tank_hp = 1000; // ��ũ ü��

float lineZ = 0; // ���� ���� ��ǥ

// ������ �ֱ� ���� ������ ����
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

//���콺 ������ȯ
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

void MySpecial(int key, int X, int Y) { //x,y�� ������ ���� ��ġ
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
		b.pos.m[0] = x; // ������ ��ǥ x, y
		b.pos.m[1] = 5.5;
		b.pos.m[2] = y + 9.5;
		b.v.m[0] = 0;
		b.v.m[1] = 0;
		b.v.m[2] = -5.0;
		bullets.push_back(b);
		glutPostRedisplay();
	}
}

// Ÿ�̸� �Լ�
// ��ũ�� ���� ������ �����̰�, ��ũ ��ź ����
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
		tankShell.pos.m[0] = sin(time * 0.0001) * 7; // ��ũ ��ǥ
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
	glOrtho(-15.0, 15.0, -15.0, 15.0, -100.0, 100.0); // 15�� ���� view
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(MX / 300.0, MY / 300.0, 1.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);  //������ȯ 
}

// �� �̸��� �ָ� ���� ����
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

	// ��ü�� �̸����� ������ �����ϱ�
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

	glEnable(GL_NORMALIZE); // ���� ����� ��, ���� ���͸� �ٽ� ����ض�
}

bool tankHitCheck() { // �Ѿ˰� tank�� �浹�̸� true����
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

// ������ �¾Ҵ��� Ȯ��
bool soldierHitCheck() {
	vec3 soldier(x, 7, 12);
	vec3 diff = soldier - tankShell.pos;
	if (diff.Length() < 1.5) {
		return true;
	}
	return false;
}

// ���� ��ź ����
void UpdateTankShell() {
	float tankShellTime = 0.1;
	tankShell.pos = tankShell.pos + tankShell.v * tankShellTime;
}

// ���� ��ź �׸�
void DrawTankShell() {
	UpdateTankShell(); 
	InitLight(model_asphalt);
	glPushMatrix();
	glTranslatef(tankShell.pos.m[0], tankShell.pos.m[1], tankShell.pos.m[2]);
	glutSolidSphere(0.5, 20, 20);
	glPopMatrix();
}

// �Ѿ��� ��ġ�� ����
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

// �Ѿ��� �׸�
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
	glTranslatef(0, 8, 0); // ��ũ �ٴ� ��ġ

	glPushMatrix();
	glScalef(5, 2.5, 8);
	glutSolidCube(3); // ��ũ �Ʒ��κ�
	glPopMatrix();

	glTranslatef(0, 5, 0);
	glPushMatrix(); // ��ũ ���κ�
	glScalef(3, 3, 4);
	glutSolidCube(3);
	glPopMatrix();
	

	//����
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

	// ������ ����
	for (int i = 0; i < 5; i++) {
		glPushMatrix();
		glTranslatef(5, -9, 7-i*4);
		glScalef(0.7, 1, 1);
		glRotatef(time*0.1, 1.0, 0, 0);
		glutWireSphere(3, 30, 30);
		glPopMatrix();
	}

	// ���� ����
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

	glTranslatef(0, 9, 0); // ���� �ٴ� ��ġ
	
	// ����
	glPushMatrix();
		glScalef(2, 5, 2);
		glutSolidCube(1);
	glPopMatrix();

	//������
	glPushMatrix(); // ��  
		glTranslatef(1, 0, 0);
		glRotatef(-50 + sin(time * 0.005) * 5, 1, 0, 0);
		glTranslatef(0, 2.5, 0);
		glPushMatrix();
			glScalef(0.1, 1, 0.1);
			glutSolidCube(3);
		glPopMatrix();
	// �ȸ�
		glTranslatef(0, 1.5, 0);
		glRotatef(15, 0, 0, 1);
		glTranslatef(0, 1.5, 0);
		glPushMatrix();
			glScalef(0.1, 1, 0.1);
			glutSolidCube(3);
		glPopMatrix();

		//��
		glTranslatef(0, 1.8, 0);
		glPushMatrix();
			glScalef(1, 1, 1);
			glutSolidCube(0.5);
		glPopMatrix();
		// ��, �����տ� �پ��ִ�
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

	//����
	glPushMatrix(); // ��  
	glTranslatef(-1, 0, 0);
	glRotatef(-50 + sin(time * 0.005) * 5, 1, 0, 0);
	glTranslatef(0, 2.5, 0);
	glPushMatrix();
	glScalef(0.1, 1, 0.1);
	glutSolidCube(3);
	glPopMatrix();
	// �ȸ�
	glTranslatef(0, 1.5, 0);
	glRotatef(-15, 0, 0, 1);
	glTranslatef(0, 1.5, 0);
	glPushMatrix();
	glScalef(0.1, 1, 0.1);
	glutSolidCube(3);
	glPopMatrix();
	//��
	glTranslatef(0, 1.8, 0);
	glPushMatrix();
	glScalef(1, 1, 1);
	glutSolidCube(0.5);
	glPopMatrix();
	glPopMatrix();

	//������ �ٸ�
	glPushMatrix();
		glTranslatef(0.5, -2, 0); 
		glRotatef(20 + sin(time * 0.005) * 45, 1, 0, 0);
		glTranslatef(0, -1, 0);
		glPushMatrix();
			glScalef(0.3, 1, 0.3);
			glutSolidCube(3);
		glPopMatrix();
		glPushMatrix();
		//���Ƹ�
			glTranslatef(0, -2, 1);
			glRotatef(sin(time * 0.005) * 15 - 60, 1, 0, 0);
			glPushMatrix();
				glScalef(0.3, 1, 0.3);
				glutSolidCube(3);
			glPopMatrix();

			//��
			glTranslatef(0, -1.7, -1);
			glPushMatrix();
				glScalef(0.3, 0.1, 0.5);
				glutSolidCube(4);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();

	//���� �ٸ�
	glPushMatrix();
	glTranslatef(-0.5, -2, 0);
	glRotatef(20 + -sin(time * 0.005) * 45, 1, 0, 0);
	glTranslatef(0, -1, 0);
	glPushMatrix();
	glScalef(0.3, 1, 0.3);
	glutSolidCube(3);
	glPopMatrix();
	glPushMatrix();
	//���Ƹ�
	glTranslatef(0, -2, 1);
	glRotatef(sin(time * 0.005) * 15 - 60, 1, 0, 0);
	glPushMatrix();
	glScalef(0.3, 1, 0.3);
	glutSolidCube(3);
	glPopMatrix();

	//��
	glTranslatef(0, -1.7, -1);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.5);
	glutSolidCube(4);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	// �Ӹ�
	glRotatef(sin(time * 0.002) * 5, 1, 0, 0);
	glTranslatef(0, 5, 0);
	glutSolidSphere(2, 20, 20); 
	glPopMatrix();
}

void drawAsphalt() {
	InitLight(model_asphalt);
	glPushMatrix();
	glScalef(20, 0.5, 70); // x-z���� ���� ��
	glutSolidCube(1.0);
	glPopMatrix();

	//�� �׸��� �ڷ� ����
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
	// ���� (�ƽ���Ʈ)
	drawAsphalt();

	// ����
	glPushMatrix();
	glTranslatef(0, 0, 12); // �⺻ ��ġ
	glTranslatef(x, 0, y); // Ű���� ����
	glScalef(0.4, 0.4, 0.4);
	DrawSoldier();
	glPopMatrix();

	// ��ũ
	glPushMatrix();
	glTranslatef(0, 0, -10); // �⺻ ��ġ
	glTranslatef(sin(time * 0.0001) * 7, 0, 0); // �ڵ� �¿� �̵�
	glScalef(0.4, 0.4, 0.4);
	DrawTank();
	glPopMatrix();
	
	// �Ѿ� �׸���
	DrawBullets();
	DrawTankShell();
	glFlush();

	bool tankHit = tankHitCheck();
	if (tankHit) {
		printf("hit!\n");
		tank_hp -= 50; // 20�� ���߸� �ı�
		if (tank_hp <= 0)
			exit(1);
	}
	bool soldierHit = soldierHitCheck();
	if (soldierHit && soldierHitFlag != 1) {
		printf("DAMAGED!\n");
		soldierHitFlag = 1; // ��ź�� �ѹ��� �±� ���� 
		soldier_hp -= 10; // 3�� ������ ����
		if (soldier_hp <= 0)
			exit(1);
	}
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);               //GLUT ������ �Լ�
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