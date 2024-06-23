#include <stdio.h>
#include <math.h>
#include <random>
#include <iostream>
#include <glut.h>
#include <string>

double playerX = 150, playerY = 150,
enemyX = 150, enemyY = 250,
enemyBulletX, enemyBulletY,
playerBulletX = 3000, playerBulletY = 3000;

int lives = 3, enemyHealth = 5, X, Y;
int powerUpTimer;
bool shooting, enemyShooting, enemyOnRight = false, powerup, availablepowerup, playerbullet = true, hit, captured, enemyhit, gameover;

// draws rectangles using the (x,y) of the bottom left vertex, width (w) and height (h)
void drawRect(int x, int y, int w, int h) {
	glBegin(GL_POLYGON);
	glVertex2f(x, y);
	glVertex2f(x + w, y);
	glVertex2f(x + w, y + h);
	glVertex2f(x, y + h);
	glEnd();
}
// draws a circle using OpenGL's gluDisk, given (x,y) of its center and tis radius
void drawCircle(int x, int y, float r) {
	glPushMatrix();
	glTranslatef(x, y, 0);
	GLUquadric* quadObj = gluNewQuadric();
	gluDisk(quadObj, 0, r, 50, 50);
	glPopMatrix();
}

void shoot(int) {
	if (playerBulletY < 400)
	{
		playerBulletY += 5;
		if ((playerBulletX >= enemyX - 10 && playerBulletX <= enemyX + 10)
			&& playerBulletY + 10 == enemyY && hit == false)
		{
			enemyHealth--;
			hit = true;
		}
	}
	else
	{
		playerBulletX = playerX;
		playerBulletY = playerY + 10;
		shooting = false;
		return;
	}
	glutPostRedisplay();

	glutTimerFunc(1000, shoot, 0);
}
void generatePowerup(int) {
	//a powerup should be generated every 10 seconds.
	availablepowerup = true;
	if (availablepowerup) {
		X = 10 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 200));
		Y = 10 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 200));
	}


	if (!availablepowerup) {
		X = 1000;
		Y = 1000;
	}

	glutPostRedisplay();
	glutTimerFunc(10000, generatePowerup, 0);
}

void enemyshootTimer(int) {
	enemyBulletY -= 5;
	if ((enemyBulletX >= playerX - 10 && enemyBulletX <= playerX + 10)
		&& enemyBulletY - 10 <= playerY && enemyhit == false && !powerup)
	{
		lives--;
		enemyhit = true;
	}

	if (enemyBulletY == -50)
		enemyShooting = false;

	glutPostRedisplay();
	glutTimerFunc(250, enemyshootTimer, 0);
}

void enemyShoot(int) {
	//the enemy should shoot a bullet once every three seconds or so
	enemyBulletX = enemyX;
	enemyBulletY = enemyY;
	enemyhit = false;

	glutTimerFunc(0, enemyshootTimer, 0);

	enemyShooting = true;

	glutPostRedisplay();

	glutTimerFunc(4000, enemyShoot, 0);
}

void poweruptimer(int) {
	if (!captured && powerup) {
		powerup = false;
	}
	else if (captured && !powerup) {
		powerup = true;
		captured = false;
	}
	glutTimerFunc(4000, poweruptimer, 0);

	glutPostRedisplay();
}

void gameOverDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	//		glColor4f(1.0f, 1.0f, 1.0f, 1.0f); //black
	glColor3f(1.0, 0.0, 0.0);
	glRasterPos2f(150, 150); //define position on the screen
	char* string = "game over";
	while (*string) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *string++);
	}
	glPopMatrix();
	glFlush();

}

void Display() {
	glClear(GL_COLOR_BUFFER_BIT);

	if (lives == 0 || enemyHealth == 0) {
		gameover = true;
	}
	if (gameover)
	{
		glutDisplayFunc(gameOverDisplay);
	}

	//our player should be green colored
	//in case we used a powerup, it will be yellow colored

	if (availablepowerup &&
		(playerX >= X - 25 && playerX <= X + 25) && (playerY >= Y - 10 && playerY <= Y + 10)) {
		availablepowerup = false;
		captured = true;
		poweruptimer(0);
	}
	else if (availablepowerup &&
		(playerX >= X - 25 && playerX <= X + 25) && !(playerY >= Y - 10 && playerY <= Y + 10)) {
		availablepowerup = false;
	}

	if (!powerup)
		glColor3f(0.0f, 1.0f, 0.0f);
	if (powerup)
		glColor3f(0.0f, 1.0f, 1.0f);

	drawRect(playerX - 15, playerY, 30, 10);
	drawCircle(playerX, playerY, 10);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	glVertex3f(playerX, playerY, 0.0f);
	glEnd();

	//the enemy should be red colored, at the top of the screen and continuously moving
	glColor3f(1.0f, 0.0f, 0.0f);
	drawRect(enemyX - 15, enemyY, 30, 10);
	drawCircle(enemyX, enemyY, 10);
	glColor3f(0.0f, 1.0f, 1.0f);
	glBegin(GL_POINTS);
	glVertex3f(enemyX, enemyY, 0.0f);
	glEnd();

	//the health of the enemy shown as a rectangle where the length is = to the health
	drawRect(75, 13, enemyHealth * 5, 10);
	glColor3f(1.0, 0.0, 0.0);
	glRasterPos2f(20, 15); //define position on the screen
	char* string = "Enemy's health";
	while (*string) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *string++);
	}

	//print the number of player lives
	glColor3f(1.0, 0.0, 0.0);
	glRasterPos2f(20, 285); //define position on the screen
	char* livesString = "";
	switch (lives) {
	case 3: livesString = "remaining lives: 3"; break;
	case 2: livesString = "remaining lives: 2"; break;
	case 1: livesString = "remaining lives: 1"; break;
	}

	while (*livesString) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *livesString++);
	}



	if (shooting) { //display the shot if the spacebar is clicked
		glColor3f(1.0f, 1.0f, 1.0f);
		drawRect(playerBulletX, playerBulletY, 1, 15);
		glPushMatrix();
		shoot(0);
		glPopMatrix();
	}

	if (enemyShooting) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 0.0f);
		drawRect(enemyBulletX, enemyBulletY - 10, 1, -20);
		glPopMatrix();
	}
	if (availablepowerup) {
		glPushMatrix();
		glColor3f(0.0f, 0.50f, 0.50f);

		drawRect(X, Y, 20, 20);
		glPopMatrix();
		//availablepowerup = false;
	}


	glFlush();
}

void enemyMove() {
	//the enemy should continuously move from right to left and vice versa
	switch (enemyOnRight) {
	case false:
		enemyX++;
		for (int i = 0; i < 8000000; i++);
		if (enemyX == 280) enemyOnRight = true;
		break;
	case true:
		enemyX--;
		for (int i = 0; i < 8000000; i++);
		if (enemyX == 20) enemyOnRight = false;
		break;
	}
	glutPostRedisplay();
}

void keyboard(int key, int x, int y) {
	{
		if (key == GLUT_KEY_LEFT)
			playerX -= 5;
		else if (key == GLUT_KEY_RIGHT)
			playerX += 5;
		else if (key == GLUT_KEY_DOWN)
			playerY -= 5;
		else if (key == GLUT_KEY_UP)
			playerY += 5;

	}
	// ask OpenGL to recall the display function to reflect the changes on the window
	glutPostRedisplay();

}

void key(unsigned char key, int x, int y) {
	//using the spacebar to shoot
	switch (key) {
	case ' ':
		hit = false;
		shooting = true;
		playerBulletX = playerX;
		playerBulletY = playerY + 10;
		break;
	}
	glutPostRedisplay();
}


void main(int argc, char** argr) {
	glutInit(&argc, argr);


	glutInitWindowSize(700, 700);

	glutCreateWindow("game");
	glutDisplayFunc(Display);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
	gluOrtho2D(0.0, 300, 0.0, 300);
	glutIdleFunc(enemyMove);
	glutSpecialFunc(keyboard);
	glutKeyboardFunc(key);
	glutTimerFunc(3000, enemyShoot, 0);
	glutTimerFunc(10000, generatePowerup, 0);


	glutMainLoop();
}
