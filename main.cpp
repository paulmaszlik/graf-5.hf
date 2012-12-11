//=============================================================================================
// Szamitogepes grafika hazi feladat keret. Ervenyes 2012-tol.
// A //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sorokon beluli reszben celszeru garazdalkodni, mert a tobbit ugyis toroljuk.
// A beadott program csak ebben a fajlban lehet, a fajl 1 byte-os ASCII karaktereket tartalmazhat.
// Tilos:
// - mast "beincludolni", illetve mas konyvtarat hasznalni
// - faljmuveleteket vegezni (printf is fajlmuvelet!)
// - new operatort hivni az onInitialization függvényt kivéve, a lefoglalt adat korrekt felszabadítása nélkül
// - felesleges programsorokat a beadott programban hagyni
// - tovabbi kommenteket a beadott programba irni a forrasmegjelolest kommentjeit kiveve
// ---------------------------------------------------------------------------------------------
// A feladatot ANSI C++ nyelvu forditoprogrammal ellenorizzuk, a Visual Studio-hoz kepesti elteresekrol
// es a leggyakoribb hibakrol (pl. ideiglenes objektumot nem lehet referencia tipusnak ertekul adni)
// a hazibeado portal ad egy osszefoglalot.
// ---------------------------------------------------------------------------------------------
// A feladatmegoldasokban csak olyan gl/glu/glut fuggvenyek hasznalhatok, amelyek
// 1. Az oran a feladatkiadasig elhangzottak ES (logikai AND muvelet)
// 2. Az alabbi listaban szerepelnek:
// Rendering pass: glBegin, glVertex[2|3]f, glColor3f, glNormal3f, glTexCoord2f, glEnd, glDrawPixels
// Transzformaciok: glViewport, glMatrixMode, glLoadIdentity, glMultMatrixf, gluOrtho2D,
// glTranslatef, glRotatef, glScalef, gluLookAt, gluPerspective, glPushMatrix, glPopMatrix,
// Illuminacio: glMaterialfv, glMaterialfv, glMaterialf, glLightfv
// Texturazas: glGenTextures, glBindTexture, glTexParameteri, glTexImage2D, glTexEnvi,
// Pipeline vezerles: glShadeModel, glEnable/Disable a kovetkezokre:
// GL_LIGHTING, GL_NORMALIZE, GL_DEPTH_TEST, GL_CULL_FACE, GL_TEXTURE_2D, GL_BLEND, GL_LIGHT[0..7]
//
// NYILATKOZAT
// ---------------------------------------------------------------------------------------------
// Nev    : Maszlik Pal Zoltan
// Neptun : JHDPDR
// ---------------------------------------------------------------------------------------------
// ezennel kijelentem, hogy a feladatot magam keszitettem, es ha barmilyen segitseget igenybe vettem vagy
// mas szellemi termeket felhasznaltam, akkor a forrast es az atvett reszt kommentekben egyertelmuen jeloltem.
// A forrasmegjeloles kotelme vonatkozik az eloadas foliakat es a targy oktatoi, illetve a
// grafhazi doktor tanacsait kiveve barmilyen csatornan (szoban, irasban, Interneten, stb.) erkezo minden egyeb
// informaciora (keplet, program, algoritmus, stb.). Kijelentem, hogy a forrasmegjelolessel atvett reszeket is ertem,
// azok helyessegere matematikai bizonyitast tudok adni. Tisztaban vagyok azzal, hogy az atvett reszek nem szamitanak
// a sajat kontribucioba, igy a feladat elfogadasarol a tobbi resz mennyisege es minosege alapjan szuletik dontes.
// Tudomasul veszem, hogy a forrasmegjeloles kotelmenek megsertese eseten a hazifeladatra adhato pontokat
// negativ elojellel szamoljak el es ezzel parhuzamosan eljaras is indul velem szemben.
//=============================================================================================

#include <math.h>
#include <stdlib.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
// MsWindows-on ez is kell
#include <windows.h>
#endif // Win32 platform

#include <GL/gl.h>
#include <GL/glu.h>
// A GLUT-ot le kell tolteni: http://www.opengl.org/resources/libraries/glut/
#include <GL/glut.h>


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Innentol modosithatod...

#define _USE_MATH_DEFINES
long time;

float fv(float x, float y) {
	return sin(x)+cos(y);
}

float degreeToRadian(float degree) {
	return degree*(M_PI/180.0);
}

float radianToDegree(float radian) {
	return radian*(180.0/M_PI);
}

float sajatMin(float a, float b) {
	if (a>=b) return b;
	return a;
}

class Vector {
//===============================================================
public:
    float x, y, z;   // a Descartes koordináták
	Vector( ) { x = y = z = 0.0; }
	Vector( float x0, float y0, float z0, float w0 = 1.0 ) { x = x0; y = y0; z = z0; }

	Vector operator+( const Vector& v ) { // két vektor összege
		float X = x + v.x, Y = y + v.y, Z = z + v.z;
		return Vector(X, Y, Z);
	}
	Vector operator-( const Vector& v ) {
		float X = x - v.x, Y = y - v.y, Z = z - v.z;
		return Vector(X, Y, Z);
	}
	Vector operator*( float f ) {         // vektor és szám szorzata
		return Vector( x * f, y * f, z * f );
	}
	float operator*( const Vector& v ) {  // két vektor skaláris szorzata
		return (x * v.x + y * v.y + z * v.z);
	}
	Vector operator%( const Vector& v ) { // két vektor vektoriális szorzata
		float X = y * v.z - z * v.y, Y = z * v.x - x * v.z, Z = x * v.y - y * v.x;
		return Vector(X, Y, Z);
	}
	float Length( ) {                     // vektor abszolút értéke
		return (float)sqrt( x * x + y * y + z * z );
	}
	void operator+=( const Vector& v ) {  // vektor összeadás
		x += v.x; y += v.y; z += v.z;
	}
	void operator-=( const Vector& v ) {  // vektor különbség
		x -= v.x; y -= v.y; z -= v.z;
	}
	void operator*=( float f ) {		  // vektor és szám szorzata
		x *= f; y *= f; z *= f;
	}
	Vector operator/( float f ) {		  // vektor osztva egy számmal
		return Vector( x/f, y/f, z/f );
	}
	void Normalize( ) {					  // vektor normalizálása
		float l = Length( );
		if ( l < 0.000001f) { x = 1; y = 0; z = 0; }
		else { x /= l; y /= l; z /= l; }
	}
	Vector UnitVector( ) {				  // egy vektorral párhuzamos egységvektor
		Vector r = * this;
		r.Normalize();
		return r;
	}
	Vector Rotate( Vector& axis, float angle ) {	// vektor forgatása egy tengely körül
		Vector iv = this -> UnitVector();
		Vector jv = axis.UnitVector() % this -> UnitVector();
		float radian = angle * M_PI/180;
		return (iv * cos(radian) +  jv * sin(radian));
	}

	float * GetArray() { return &x; }

	float& X() { return x; }
	float& Y() { return y; }
	float& Z() { return z; }
};

struct Color {
   float r, g, b;

   Color( ) {
	r = g = b = 0;
   }
   Color(float r0, float g0, float b0) {
	r = r0; g = g0; b = b0;
   }
   Color operator*(float a) {
	return Color(r * a, g * a, b * a);
   }
   Color operator*(const Color& c) {
	return Color(r * c.r, g * c.g, b * c.b);
   }
   Color operator+(const Color& c) {
 	return Color(r + c.r, g + c.g, b + c.b);
   }
};

//=============================================================
// kvaternió osztály FORRÁS: Háromdimenziós grafika könyv CD melléklet - ez kiegészítve
//=============================================================
class Quaternion {
public:
	float s;	// elforgadási szog felének koszinusza
	Vector d;	// elforgatási tengely
	Quaternion( ) { s = 0; d.x=0, d.y=0, d.z=1; }
	Quaternion( float s0, float x0, float y0, float z0 ) { s = s0; d.x=x0, d.y=y0, d.z=z0; }
	Quaternion( float s0, Vector d0 ) { s = s0; d.x=d0.x, d.y=d0.y, d.z=d0.z; }

	Quaternion operator+(Quaternion q) {	// összeadás
		return Quaternion( s + q.s, d + q.d );
	}

	Quaternion operator*(float f) {		// skalárral szorzás
		return Quaternion( s * f, d * f );
	}

	float operator*(Quaternion q) {		// skaláris szorzás
		return (s * s + d * d);
	}

	void Normalize( ) {						// egység hosszú kvaterniót csinál
		float length = (*this) * (*this);
		(*this) = (*this) * (1/sqrt(length));
	}

	Quaternion operator%(Quaternion q) { // kvaternió szorzás
		return Quaternion( s * q.s - d * q.d, q.d * s + d * q.s + d % q.d );
	}

	Quaternion invertQuaternion() { // q^-1 = [s,-w]/|q|^2
		Quaternion szamlalo = Quaternion(s,d*-1);
		Quaternion ret = szamlalo * (1 / pow(sqrt(s*s + d.x * d.x + d.y * d.y + d.z * d.z),2) );
		return ret;
	}

	float GetRotationAngle( ) {				// forgatási szög
		float cosa2 = s;
		float sina2 = d.Length();
		float angRad = atan2( sina2, cosa2 ) * 2;
		return angRad * 180 / M_PI;
	}

	Vector& GetAxis( ) { return d; }		// forgatási tengely
};

class Material {
public:
	float kd[4],ks[4],ka[4],s; // diffuz,speku,ambiens,shiniess
	Material() {
			for (int i=0;i<4;i++) {
				kd[i] = 0; ks[i] = 0; ka[i] = 0;
			}
			s = 0;
		}

	void setDiffuse(float d0, float d1, float d2, float d3) {
		kd[0] = d0; kd[1] = d1; kd[2] = d2; kd[3] = d3;
	}

	void setAmbient(float a0, float a1, float a2, float a3) {
		ka[0] = a0; ka[1] = a1; ka[2] = a2; ka[3] = a3;
	}

	void setSpecular(float s0, float s1, float s2, float s3) {
		ks[0] = s0; ks[1] = s1; ks[2] = s2; ks[3] = s3;
	}

	void setShiniess(float s) {
		(*this).s = s;
	}

	void useMaterial() {
		glMaterialfv( GL_FRONT, GL_AMBIENT, ka);
		glMaterialfv( GL_FRONT, GL_DIFFUSE, kd);
		glMaterialfv( GL_FRONT, GL_SPECULAR, ks);
		glMaterialf( GL_FRONT, GL_SHININESS, s);
	}
};

const int screenWidth = 600;	// alkalmazás ablak felbontása
const int screenHeight = 600;
Material uveg;
Material khaki;
Material fem;
Material pirosnyil;
Material haz;
Material talaj;
Vector kvatIrany(0,0,0);
Quaternion kvat(1,kvatIrany);
float gAcc = 0.0981;

void createTeglatest(float hossz, float szelesseg, float melyseg) {
	glBegin(GL_QUAD_STRIP);
			glNormal3f(0,0,-1);
			glVertex3f(-szelesseg, 0, -melyseg);
			glVertex3f( szelesseg, 0, -melyseg);
			glVertex3f(-szelesseg, hossz, -melyseg);
			glVertex3f( szelesseg, hossz, -melyseg);
			glNormal3f(0,1,0);
			glVertex3f(-szelesseg, hossz, melyseg);
			glVertex3f( szelesseg, hossz, melyseg);
			glNormal3f(0,0,1);
			glVertex3f(-szelesseg, 0, melyseg);
			glVertex3f( szelesseg, 0, melyseg);
			glNormal3f(0,-1,0);
			glVertex3f(-szelesseg, 0, -melyseg);
			glVertex3f( szelesseg, 0, -melyseg);
		glEnd();
		glBegin(GL_QUADS);
			glNormal3f(1,0,0);
			glVertex3f(szelesseg, 0, -melyseg);
			glVertex3f(szelesseg, 0,  melyseg);
			glVertex3f(szelesseg, hossz,  melyseg);
			glVertex3f(szelesseg, hossz, -melyseg);
			glNormal3f(-1,0,0);
			glVertex3f(-szelesseg, 0, -melyseg);
			glVertex3f(-szelesseg, 0,  melyseg);
			glVertex3f(-szelesseg, hossz,  melyseg);
			glVertex3f(-szelesseg, hossz, -melyseg);
		glEnd();
}

void createHengerTest(int felbontas, float hossz, float sugar) 	{
			glBegin(GL_TRIANGLE_STRIP);
			for(int i = 0; i <= felbontas; i++) {
			  float alpha = (2 * M_PI * (i%felbontas)) / felbontas;
			  float x = sugar * sin(alpha);
			  float z = sugar * cos(alpha);
			  glNormal3f(sin(alpha), 0, cos(alpha));
			  glVertex3f(x, 0, z);
			  glVertex3f(x, hossz, z);
			}
			glEnd();

			glNormal3f(0,-1,0);
			glBegin(GL_TRIANGLE_FAN);
			  glVertex3f(0, 0, 0);
			  for(int i = 0; i <= felbontas; i++) {
				float alpha = (2 * M_PI * (i%felbontas)) / felbontas;
				glVertex3f(sugar*sin(alpha), 0, sugar*cos(alpha));
			  }
			glEnd();

			glNormal3f(0,1,0);
			glBegin(GL_TRIANGLE_FAN);
			  glVertex3f(0, hossz, 0);
			  for(int i = 0; i <= felbontas; i++) {
				float alpha = (2 * M_PI * (i%felbontas)) / felbontas;
				glVertex3f(sugar*sin(alpha), hossz, sugar*cos(alpha));
			  }
			glEnd();
}

void createKupTest(int felbontas, float hossz, float sugar) 	{

			glBegin(GL_TRIANGLE_STRIP);
			for(int i = 0; i <= felbontas; i++) {
			  float alpha = (2 * M_PI * (i%felbontas)) / felbontas;
			  float x = sugar * sin(alpha);
			  float z = sugar * cos(alpha);
			  glNormal3f(sin(alpha), 0, cos(alpha));
			  glVertex3f(0, 0, 0);
			  glVertex3f(x, hossz, z);
			}
			glEnd();

			glNormal3f(0,-1,0);
			glBegin(GL_TRIANGLE_FAN);
			  glVertex3f(0, hossz, 0);
			  for(int i = 0; i <= felbontas; i++) {
				float alpha = (2 * M_PI * (i%felbontas)) / felbontas;
				glVertex3f(sugar*sin(alpha), hossz, sugar*cos(alpha));
			  }
			glEnd();
}

void createGombTest(int felbontas, float sugar) {
    float tStep = (M_PI) / (float)felbontas;
	float sStep = (M_PI) / (float)felbontas;
	for(float t = -M_PI/2; t <= (M_PI/2); t += tStep)
	{
			glBegin(GL_TRIANGLE_STRIP);
			for(float s = -M_PI; s <= M_PI; s += sStep)
			{
			    glNormal3f(sugar * cos(t) * cos(s), sugar * cos(t) * sin(s), sugar * sin(t));
				glVertex3f(sugar * cos(t) * cos(s), sugar * cos(t) * sin(s), sugar * sin(t));
				glVertex3f(sugar * cos(t+tStep) * cos(s), sugar * cos(t+tStep) * sin(s), sugar * sin(t+tStep));
			}
			glEnd();
	}
}

void createHeliBody(float fA, float fB, float fC) {
	float tStep = (M_PI) / (float)50;
	float sStep = (M_PI) / (float)50;
	for(float t = -M_PI/2; t <= (M_PI/2); t += tStep)
	{
			glBegin(GL_TRIANGLE_STRIP);
			for(float s = -M_PI; s <= M_PI; s += sStep)
			{   if (s <= 0.1) {
				glNormal3f(fA * cos(t) * cos(s), fB * cos(t) * sin(s), fC * sin(t));
				glVertex3f(fA * cos(t) * cos(s), fB * cos(t) * sin(s), fC * sin(t));
				glVertex3f(fA * cos(t+tStep) * cos(s), fB * cos(t+tStep) * sin(s), fC * sin(t+tStep));
				}
			}
			glEnd();
	}

	for(float t = -M_PI/2; t <= (M_PI/2)+.0001; t += tStep)
	{
		if (t >= 0) {
			glBegin(GL_TRIANGLE_STRIP);
			for(float s = -M_PI; s <= M_PI+.0001; s += sStep)
			{   if (s >= -0.1) {
				glNormal3f(fA * cos(t) * cos(s), fB * cos(t) * sin(s), fC * sin(t));
				glVertex3f(fA * cos(t) * cos(s), fB * cos(t) * sin(s), fC * sin(t));
				glVertex3f(fA * cos(t+tStep) * cos(s), fB * cos(t+tStep) * sin(s), fC * sin(t+tStep));
				}
			}
			glEnd();
		}
	}

//üvegszín
uveg.useMaterial();

//üveg rajzolás
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for(float t = -M_PI/2; t <= (M_PI/2)+.0001; t += tStep)
	{
		if (t <= 0) {
			glBegin(GL_TRIANGLE_STRIP);
			for(float s = -M_PI; s <= M_PI+.0001; s += sStep)
			{   if (s >= -0.1) {
				glNormal3f(fA * cos(t) * cos(s), fB * cos(t) * sin(s), fC * sin(t));
				glVertex3f(fA * cos(t) * cos(s), fB * cos(t) * sin(s), fC * sin(t));
				glVertex3f(fA * cos(t+tStep) * cos(s), fB * cos(t+tStep) * sin(s), fC * sin(t+tStep));
				}
			}
			glEnd();
		}
	}
	glDisable(GL_BLEND);
}

class Bomb {
public:
    Vector startPos;
    Vector offsetPos;
    Vector pos;
    Vector v0;
    Vector v;
    float shootTime;
    float gravityTime;
    float shootAngle;
    float yMax;
    bool active;


    Bomb() {
        active = false;
    }

    void SetPos(float x, float y, float z) {
        pos.x = x; pos.y = y; pos.z = z;
    }
};

class Helicopter {
public:
	Vector pos;
	int rotorDegree;
	float speed;

	Helicopter(float x, float y, float z) {
		setPosition(x,y,z);
		rotorDegree = 0;
		speed = 0.3;
	}

	void setPosition(float x, float y, float z) {
		pos.x = x; pos.y = y; pos.z = z;
	}

	void draw() {

	glPushMatrix();
		glTranslatef(pos.x,pos.y,pos.z);
		glRotatef(kvat.GetRotationAngle(), kvat.d.x, kvat.d.y, kvat.d.z);
		// helikopter rajz
		fem.useMaterial();
		// fõrotor
		glPushMatrix();
			glTranslatef(0.0,0.35,0.0);
			createHengerTest(40,0.1,0.04);
			glRotatef(90,1,0,0); // rotor elforgatása egy irányba
			glRotatef(rotorDegree,0,0,1);
			createTeglatest(1.2,0.05,0.02);
			glRotatef(180,0,0,1);
			createTeglatest(1.2,0.05,0.02);
		glPopMatrix();

		// hátsókisrotor
		glPushMatrix();
			glRotatef(90,0,1,0); // rotor elforgatása egy irányba
			glTranslatef(-1.6,0.0,-0.07);
			glPushMatrix();
				glRotatef(90,1,0,0);
				createHengerTest(40,0.06,0.02);
			glPopMatrix();
			glRotatef(rotorDegree,0,0,1);
			createTeglatest(0.2,0.02,0.01);
			glRotatef(120,0,0,1);
			createTeglatest(0.2,0.02,0.01);
			glRotatef(120,0,0,1);
			createTeglatest(0.2,0.02,0.01);
		glPopMatrix();

		// talp jobb
		glPushMatrix();
				glRotatef(20,0,0,1);
				glTranslatef(0.2,-1.35,-0.25);
				glTranslatef(0,0.9,0);
				createHengerTest(40,0.2,0.02);
				glTranslatef(0,0,0.5);
				createHengerTest(40,0.2,0.02);
				glRotatef(90,1,0,0);
				glTranslatef(0,-0.75,0);
				createHengerTest(40,1.0,0.025);
		glPopMatrix();
		// talp bal
		glPushMatrix();
				glRotatef(-20,0,0,1);
				glTranslatef(-0.2,-1.35,-0.25);
				glTranslatef(0,0.9,0);
				createHengerTest(40,0.2,0.02);
				glTranslatef(0,0,0.5);
				createHengerTest(40,0.2,0.02);
				glRotatef(90,1,0,0);
				glTranslatef(0,-0.75,0);
				createHengerTest(40,1.0,0.025);
		glPopMatrix();

		khaki.useMaterial();
		glPushMatrix();
			glRotatef(90,1,0,0);
			glTranslatef(0.0,0.55,0.0);
			createHengerTest(40,1.1,0.05);
		glPopMatrix();

		glPushMatrix();
			glRotatef(90,0,1,0);
			glTranslatef(-1.54,-0.05,0);
			glRotatef(20,0,0,1);
			createTeglatest(0.3,0.1,0.01);
		glPopMatrix();

		createHeliBody(0.4,0.3,0.6);
	glPopMatrix();
	}
};

class Camera {
public:
    Vector pos;
    Vector speed;
    Vector camHeliDistance;

    Camera() {
        setPos(0,7.5,4.5);
    }
    void setPos(float x, float y, float z) {
        pos.x = x, pos.y = y, pos.z = z;
    }
};

void createTalaj() {
	// talaj
glPushMatrix();
talaj.useMaterial();
float image2[256][256][3];

for(int Y = 0; Y < 256; Y++) {
		for(int X = 0; X < 256; X++) {
			float heightmapcolor;
			heightmapcolor = (fv((float)X/(5*M_PI),(float)Y/(5*M_PI)) + 2.0) / 4.0;
			//image2[(Y*255 + X)] = Color(0.3,heightmapcolor,0.05);
			image2[X][Y][0] = 0.3;
			image2[X][Y][1] = heightmapcolor;
			image2[X][Y][2] = 0.05;
		}
}

glEnable(GL_TEXTURE_2D);
unsigned int texids;
glGenTextures(1, &texids);

glBindTexture( GL_TEXTURE_2D, texids );
int level = 0, border = 1, width = 256, height = 256;

glTexImage2D(GL_TEXTURE_2D, level, GL_RGB, width, height, border, GL_RGB, GL_FLOAT, image2);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


//tesszelált talaj
glNormal3f(0,1,0);
glBegin(GL_QUADS);
		float d = 0.2;
		 for (float x = -50.0f; x < 50.0f; x += 2.5) {
        for (float z = -50.0f; z < 50.0f; z += 2.5) {
            glNormal3f(0,1,0); glTexCoord2f(x,z); glVertex3f(x,-1,-z);
            glNormal3f(0,1,0); glTexCoord2f(x+2.5,z); glVertex3f(x+2.5,-1,-z);
            glNormal3f(0,1,0); glTexCoord2f(x+2.5,z+2.5); glVertex3f(x+2.5,-1,-(z+2.5));
            glNormal3f(0,1,0); glTexCoord2f(x,z+2.5); glVertex3f(x,-1,-(z+2.5));
        }
    }
glEnd();

glDisable(GL_TEXTURE_2D);

haz.useMaterial();
//épületek
	glTranslatef(0.0,-0.999,0.0);
	createTeglatest(.1,.1,.1);
	glTranslatef(-1,0,-5);
	createTeglatest(.2,.2,.1);
	glTranslatef(2,0,2);
	createTeglatest(.2,.1,.1);
	glTranslatef(-1,0,0);
	createTeglatest(.1,.1,.2);
	glTranslatef(-2,0,-1.5);
	createTeglatest(.2,.1,.1);
	glTranslatef(-2,0,5);
	createTeglatest(.1,.2,.5);
	glTranslatef(6,0,2);
	createTeglatest(.2,.1,.1);
	glTranslatef(-1,0,1);
	createTeglatest(.5,.1,.3);
	glTranslatef(1,0,-1);
	createTeglatest(.1,.1,.1);
	glTranslatef(-3,0,-1);
	createTeglatest(.3,.2,.1);
	glTranslatef(2,0,-2);
	createTeglatest(.7,.1,.1);
	glTranslatef(-1,0,1);
	createTeglatest(.1,.1,.2);
glPopMatrix();
}

void setupLights() {
	GLfloat ambient[4] = {0.0f, 0.0f, 0.1f, 1.0f};
	GLfloat diffuse[4] = {0.0f, 0.0f, 0.1f, 1.0f};
	GLfloat position0[4] = {20, 4, 0, 1.0f};

	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position0);
	glEnable(GL_LIGHT0);

	GLfloat ambientSun[4] = {0.4f, 0.4f, 0.4f, 1.0f};
	GLfloat diffuseSun[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat positionSun[4] = {20.0, 5.0, 0.0, 0.0f};

	 glLightfv(GL_LIGHT1, GL_AMBIENT, ambientSun);
	 glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseSun);
	 glLightfv(GL_LIGHT1, GL_POSITION, positionSun);
	 glEnable(GL_LIGHT1);
}

Camera cam;
float gumihossz = 7;
Helicopter heli1(0,3.0,0.6);
const int noOfBombs = 5;
Bomb bomb[noOfBombs];
int actualBomb = 0;

float maradek_ido = 0;
float mintavetel_ms = 50;
float nyulas,ero;
Vector tavolsag; Vector huzo_gyorsulas;

void SimulateWorld(float tstart, float tend) {
   for(float ts = tstart; ts < tend; ts += mintavetel_ms) {

      float te = sajatMin(tend, ts + mintavetel_ms);

	  maradek_ido += (te-ts);

	  for(;maradek_ido >= mintavetel_ms;) {
		  heli1.rotorDegree += 40;
		  Quaternion e = kvat % Quaternion(0, Vector(0, 0, heli1.speed)) % kvat.invertQuaternion();
		  heli1.pos = heli1.pos + e.d*(-heli1.speed);
		  if(heli1.pos.y < -0.3) {
            Vector kvatReset(0,0,0);
            kvat.s = 1;
            kvat.d = kvatReset;
            heli1.setPosition(0,3,0.6);
            cam.setPos(0,7.5,4.5);
		  }

          // forrás: Grafika levlista - tomasz987 levele alapján (1-2 alap fizika képlet) módosítva
          // RÚGÓ
            tavolsag = heli1.pos - cam.pos;
            nyulas = tavolsag.Length() - gumihossz;
            if(nyulas < 0) nyulas=0;
            ero=nyulas*0.5;
            Vector irany = tavolsag;
            irany.Normalize();
            huzo_gyorsulas = (irany*ero) / 2;
            cam.speed += huzo_gyorsulas;
          //LÉGELLENÁLLÁS
          Vector seged;
          seged=cam.speed;
          seged.Normalize();
          if (cam.speed.Length() > 0.1) {
                cam.speed -= (seged*0.05) ;
          }
          //felhajtoero
          Vector felhajto = Vector(0,0.001,0);
          cam.speed += felhajto;

          cam.pos += cam.speed;

          for(int i=0;i<noOfBombs;i++) {
              if (bomb[i].active) {
                      bomb[i].offsetPos.x = bomb[i].v0.x*(time-bomb[i].shootTime)/100;
                      bomb[i].pos.x = bomb[i].startPos.x + bomb[i].offsetPos.x;

                      bomb[i].offsetPos.z = bomb[i].v0.z*(time-bomb[i].shootTime)/100;
                      bomb[i].pos.z = bomb[i].startPos.z + bomb[i].offsetPos.z;
                      bomb[i].offsetPos.y = bomb[i].v0.y*(time-bomb[i].gravityTime)/100  - (gAcc/2)*pow((time-bomb[i].gravityTime)/100,2);
                      bomb[i].pos.y = bomb[i].startPos.y + bomb[i].offsetPos.y;

                      bomb[i].v = bomb[i].v0;
                      bomb[i].v.y = bomb[i].v.y - gAcc*(time-bomb[i].gravityTime)/100;
                      if (bomb[i].pos.y < 0) {
                        Vector temp(bomb[i].v.x,0,bomb[i].v.z);
                        float szog = acos(temp.Length()/bomb[i].v.Length()); bomb[i].shootAngle = szog; //beesesi szog
                        bomb[i].v0.y = sqrt(2*bomb[i].yMax*0.9*gAcc);
                        bomb[i].yMax = bomb[i].pos.y + ( pow(bomb[i].v0.y,2)/(2*gAcc) );
                        bomb[i].startPos.y = 0.001;
                        bomb[i].gravityTime = time;
                      }
              }
		  }
		  maradek_ido = maradek_ido - mintavetel_ms;
		  glutPostRedisplay();
	  }
   }
}

// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization( ) {
	glViewport(0, 0, screenWidth, screenHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0,1.0,0.01,200.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);

	//Materialok beállítása
	uveg.setAmbient(0.0,0.0,0,0.0);
	uveg.setDiffuse(0,0,1,0.5);
	uveg.setSpecular(1,1,1,1);
	uveg.setShiniess(100);

	pirosnyil.setAmbient(1.0,0,0,1);
	pirosnyil.setDiffuse(1.0,0,0,1);
	pirosnyil.setSpecular(1.0,0,0,1);
	pirosnyil.setShiniess(10);

	khaki.setAmbient(0.7, 0.6, 0.4, 1.0);
	khaki.setDiffuse(0.8, 0.6, 0.4, 1.0);
	khaki.setSpecular(0.8, 0.6, 0.4, 1.0);
	khaki.setShiniess(500);

	fem.setAmbient(0.3, 0.3, 0.3, 1.0);
	fem.setDiffuse(0.3, 0.3, 0.3, 1.0);
	fem.setSpecular(1, 1, 1, 1.0);
	fem.setShiniess(500);

	talaj.setAmbient(0.3, 0.8, 0.4, 1.0);
	talaj.setDiffuse(0.8, 0.8, 0.8, 1.0);
	talaj.setSpecular(0.8, 0.8, 0.8, 1.0);
	talaj.setShiniess(5);

	haz.setAmbient(0.6, 0.2, 0.4, 1.0);
	haz.setDiffuse(0.8, 0.8, 0.8, 1.0);
	haz.setSpecular(0.8, 0.8, 0.8, 1.0);
	haz.setShiniess(5);

	actualBomb = 0;
	}

// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay( ) {
	glClearColor(0.7f, 0.8f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles

	glPushMatrix();
	gluLookAt( cam.pos.x, cam.pos.y, cam.pos.z, heli1.pos.x, heli1.pos.y, heli1.pos.z, 0.0, 1.0, 0.0 );

	setupLights();
	createTalaj();
	heli1.draw();

    pirosnyil.useMaterial();
    for(int i=0;i<noOfBombs;i++) {
        if (bomb[i].active) {
        glPushMatrix();
            glTranslatef(bomb[i].pos.x,bomb[i].pos.y,bomb[i].pos.z);
            createGombTest(20,0.5);
        glPopMatrix();
        }
    }

glPopMatrix();

glutSwapBuffers();     				// Buffercsere: rajzolas vege
}
// Billentyuzet esemenyeket lekezelo fuggveny
void onKeyboard(unsigned char key, int x, int y) {
    if (key == 'R') {
		Quaternion e = kvat % Quaternion(0, Vector(0, 0, 1)) % kvat.invertQuaternion();
		float degInRad = degreeToRadian(20);
		Quaternion q = Quaternion(cos(degInRad/2.0), e.d * sin(degInRad/2.0));
		kvat = q % kvat;
	}
	if (key  == 'E') {
		Quaternion e = kvat % Quaternion(0, Vector(0, 0, 1)) % kvat.invertQuaternion();
		float degInRad = degreeToRadian(-20);
		Quaternion q = Quaternion(cos(degInRad/2.0), e.d * sin(degInRad/2.0));
		kvat = q % kvat;
	}
	if (key == 'Y') {
		Quaternion e = kvat % Quaternion(0, Vector(0, 1, 0)) % kvat.invertQuaternion();
		float degInRad = degreeToRadian(20);
		Quaternion q = Quaternion(cos(degInRad/2.0), e.d * sin(degInRad/2.0));
		kvat = q % kvat;
	}
	if (key  == 'X') {
		Quaternion e = kvat % Quaternion(0, Vector(0, 1, 0)) % kvat.invertQuaternion();
		float degInRad = degreeToRadian(-20);
		Quaternion q = Quaternion(cos(degInRad/2.0), e.d * sin(degInRad/2.0));
		kvat = q % kvat;
	}
	if (key == 'P') {
		Quaternion e = kvat % Quaternion(0, Vector(1, 0, 0)) % kvat.invertQuaternion();
		float degInRad = degreeToRadian(20);
		Quaternion q = Quaternion(cos(degInRad/2.0), e.d * sin(degInRad/2.0));
		kvat = q % kvat;
	}
	if (key  == 'O') {
		Quaternion e = kvat % Quaternion(0, Vector(1, 0, 0)) % kvat.invertQuaternion();
		float degInRad = degreeToRadian(-20);
		Quaternion q = Quaternion(cos(degInRad/2.0), e.d * sin(degInRad/2.0));
		kvat = q % kvat;
	}
	if (key  == 32) {
            if (actualBomb == noOfBombs) { actualBomb = 0; }
            bomb[actualBomb].active = true;
            bomb[actualBomb].shootTime = time;
            bomb[actualBomb].gravityTime = time;
            Quaternion e = kvat % Quaternion(0, Vector(0, 0, 1)) % kvat.invertQuaternion();
		    bomb[actualBomb].v0 = e.d*(-0.7);
            bomb[actualBomb].startPos = heli1.pos - e.d;
		    Vector temp(bomb[actualBomb].v0.x,0,bomb[actualBomb].v0.z);
		    float szog = acos(temp.Length()/bomb[actualBomb].v0.Length());
		    bomb[actualBomb].shootAngle = szog;
		    bomb[actualBomb].yMax = heli1.pos.y + ( pow(bomb[actualBomb].v0.y,2)/(2*gAcc) );
            actualBomb++;
	}
	if (key == 27) {
        exit(0);
    }

	kvatIrany = kvat.GetAxis();
	kvatIrany.Normalize();
	kvatIrany = (Vector(0, 1, 0) + kvatIrany)*0.5;
	if(fabs(kvatIrany.x)<0.0001)kvatIrany.x=0;
    if(fabs(kvatIrany.y)<0.0001)kvatIrany.y=0;
    if(fabs(kvatIrany.z)<0.0001)kvatIrany.z=0;
	glutPostRedisplay( );

}

// Eger esemenyeket lekezelo fuggveny
void onMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT && state == GLUT_DOWN)   // A GLUT_LEFT_BUTTON / GLUT_RIGHT_BUTTON illetve GLUT_DOWN / GLUT_UP
		glutPostRedisplay( ); 						 // Ilyenkor rajzold ujra a kepet
}

// `Idle' esemenykezelo, jelzi, hogy az ido telik, az Idle esemenyek frekvenciajara csak a 0 a garantalt minimalis ertek
void onIdle( ) {
     long old_time = time;
     time = glutGet(GLUT_ELAPSED_TIME);		// program inditasa ota eltelt ido
	 SimulateWorld(old_time, time);
}

// ...Idaig modosithatod
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// A C++ program belepesi pontja, a main fuggvenyt mar nem szabad bantani
int main(int argc, char **argv) {
    glutInit(&argc, argv); 				// GLUT inicializalasa
    glutInitWindowSize(600, 600);			// Alkalmazas ablak kezdeti merete 600x600 pixel
    glutInitWindowPosition(100, 100);			// Az elozo alkalmazas ablakhoz kepest hol tunik fel
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);	// 8 bites R,G,B,A + dupla buffer + melyseg buffer

    glutCreateWindow("Grafika hazi feladat");		// Alkalmazas ablak megszuletik es megjelenik a kepernyon

    glMatrixMode(GL_MODELVIEW);				// A MODELVIEW transzformaciot egysegmatrixra inicializaljuk
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);			// A PROJECTION transzformaciot egysegmatrixra inicializaljuk
    glLoadIdentity();

    onInitialization();					// Az altalad irt inicializalast lefuttatjuk

    glutDisplayFunc(onDisplay);				// Esemenykezelok regisztralasa
    glutMouseFunc(onMouse);
    glutIdleFunc(onIdle);
    glutKeyboardFunc(onKeyboard);

    glutMainLoop();					// Esemenykezelo hurok

    return 0;
}

